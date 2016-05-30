#include <iostream>
#include <chrono>
#include <tuple>
#include <random>
#include <thread>
#include <atomic>
#include "Hopscotch_wrapper.h"

using namespace std;

Hopscotch *table;

void benchmarkConcurrentInsert(atomic<int>* numProcessed, int target, chrono::high_resolution_clock::duration* testsTotal, mutex* totalMutex) {

  chrono::high_resolution_clock::duration total = chrono::high_resolution_clock::duration::zero();
  while (*numProcessed <= target) {
    atomic_fetch_add(numProcessed, 1);
    int value = rand() % 10000;
    bool lookup = rand() % 2;
    auto before = chrono::high_resolution_clock::now();
    if (!lookup)
      table->add(&value, NULL);
    else
      table->contains(&value);
    auto after = chrono::high_resolution_clock::now();
    total += after - before;
  }

  totalMutex->lock();
  *testsTotal = *testsTotal + total;
  totalMutex->unlock();
}

chrono::high_resolution_clock::duration benchmarkInsert(int numElements, int numThreads) {

  vector<thread> threads;

  atomic<int> numProcessed = ATOMIC_VAR_INIT(0);
  mutex totalMutex;
  chrono::high_resolution_clock::duration total = chrono::high_resolution_clock::duration::zero();;

  for (int i = 0; i < numThreads; i++) {
    threads.push_back(thread(benchmarkConcurrentInsert, &numProcessed, numElements, &total, &totalMutex));
  }

  for (auto &t : threads) t.join();

  return total;
}

int main() {
  int numThreads = 10;

  table = new Hopscotch();

  chrono::high_resolution_clock::duration time = benchmarkInsert(1000000, numThreads);
  long long nanoseconds = chrono::duration_cast<std::chrono::nanoseconds>(time).count();
  cout << "Total Insert Time: " << nanoseconds << " ns" << endl;

  return 0;
}
