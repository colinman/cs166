#include "CuckooHashTable.h"
#include <math.h>

int *createInitialHashTable(size_t numBuckets) {
	int *table = new int[numBuckets];
	for (int i = 0; i < numBuckets; i++)
		table[i] = -1;
	return table; 
}

CuckooHashTable::CuckooHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
	this->family = family;
	this->numBuckets = numBuckets / 2;
	h1 = this->family->get();
	h2 = this->family->get();
	left_table = createInitialHashTable(this->numBuckets); 
	right_table = createInitialHashTable(this->numBuckets); 
	numElements = 0;
}

CuckooHashTable::~CuckooHashTable() {
	delete[] left_table;
	delete[] right_table;
}

int CuckooHashTable::insertData(int data, size_t num_evictions) {
	int *table = (num_evictions % 2 == 0) ? left_table : right_table;
	HashFunction h = (num_evictions % 2 == 0) ? h1 : h2;
	size_t hash_index = h(data) % numBuckets;
	int old_data = table[hash_index];
	table[hash_index] = data;
	return old_data; 
}

void CuckooHashTable::rehash() {
	h1 = family->get();
	h2 = family->get();
	int *old_right_table = right_table;
	int *old_left_table = left_table;
	right_table = createInitialHashTable(numBuckets);
	left_table = createInitialHashTable(numBuckets); 

	for (int i = 0; i < numBuckets; i++) {
		if (old_left_table[i] >= 0) insert(old_left_table[i]); 
		if (old_right_table[i] >= 0) insert(old_right_table[i]); 
	}
	
	delete[] old_right_table;
	delete[] old_left_table;	
}

void CuckooHashTable::insert(int data) {
	if (contains(data)) return;
	
	size_t num_evictions = 0;
	int curr_data = data;
	while(true) {
		curr_data = insertData(curr_data, num_evictions);
		if (curr_data == -1) break;
		num_evictions ++;

		if (num_evictions > 6 * log(numElements)){
			num_evictions = (num_evictions - 1) % 2;	
			rehash();
		}
	} 
	numElements ++;
}

bool CuckooHashTable::contains(int data) const {
	size_t left_index = h1(data) % numBuckets;
	size_t right_index = h2(data) % numBuckets;
	bool to_return = left_table[left_index] == data || 
			right_table[right_index] == data;
	return to_return;
}

void CuckooHashTable::remove(int data) {
	size_t left_index = h1(data) % numBuckets;
	size_t right_index = h2(data) % numBuckets;
	if (left_table[left_index] == data){
		left_table[left_index] = -1;
		numElements --;
	}
	else if (right_table[right_index] == data){
		right_table[right_index] = -1;
		numElements --;
	}
}

