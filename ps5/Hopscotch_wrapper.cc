#include <iostream>
#include <chrono>
#include <tuple>
#include <random>
#include <atomic>
#include "Hopscotch_wrapper.h"

Hopscotch *table;

void benchmarkConcurrentInsert(std::atomic<int>* numProcessed, int target, std::chrono::high_resolution_clock::duration& testsTotal, std::mutex& totalMutex) {

  std::chrono::high_resolution_clock::duration total = std::chrono::high_resolution_clock::duration::zero();
  while (*numProcessed <= target) {
    std::atomic_fetch_add(numProcessed, 1);
    int value = rand() % 10000;
    auto before = std::chrono::high_resolution_clock::now();    
    table->add(&value, NULL);
    auto after = std::chrono::high_resolution_clock::now();    
    total += after - before;    
  }
  


  totalMutex.lock();
  testsTotal += total;
  totalMutex.unlock();
}

std::chrono::high_resolution_clock::duration benchmarkInsert(int numElements) {

  std::atomic<int> numProcessed = ATOMIC_VAR_INIT(0);
  std::mutex totalMutex;
  std::chrono::high_resolution_clock::duration total = std::chrono::high_resolution_clock::duration::zero();;

  benchmarkConcurrentInsert(&numProcessed, numElements, total, totalMutex);
  return total;
}

int main() {
  int numThreads = 1;

  int key = 3;
  table = new Hopscotch();
  table->add(&key, NULL);

  std::chrono::high_resolution_clock::duration time = benchmarkInsert(1000);
  int nanoseconds = std::chrono::duration_cast<std::chrono::nanoseconds>(time).count();
  cout << "Total Insert Time: " << nanoseconds << " ns" << endl;

  return 0;
}
