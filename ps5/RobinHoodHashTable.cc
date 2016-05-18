#include "RobinHoodHashTable.h"

struct Node {
	int data; 
	size_t intended_bucket;
};

RobinHoodHashTable::RobinHoodHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
	table = new Node[numBuckets]();
	h = family->get();
	this->numBuckets = numBuckets;

	for (size_t i = 0; i < numBuckets; i++)
		table[i].data = -1; /* Set it to a sentinel value */
}

RobinHoodHashTable::~RobinHoodHashTable() {
	delete []table;
}

size_t RobinHoodHashTable::calcDiff (size_t index, size_t intended_bucket) const {
	if (index < intended_bucket)
		return numBuckets - ((int) index - intended_bucket);
	return index - intended_bucket;
}

bool RobinHoodHashTable::shouldReplace(size_t index, size_t intended_bucket) const{
	return calcDiff (index, table[index].intended_bucket) < calcDiff (index, intended_bucket);
}

void RobinHoodHashTable::insert(int data) {
	size_t hash_index = h(data) % numBuckets;
	size_t curr_intended_bucket = hash_index;
	int curr_data = data;

	while (table[hash_index].data >= 0){ 
		if (table[hash_index].data == curr_data) return;

		if (shouldReplace(hash_index, curr_intended_bucket)){
			int new_data = table[hash_index].data;
			size_t new_intended_bucket = table[hash_index].intended_bucket;		
			table[hash_index].data = curr_data;
			table[hash_index].intended_bucket = curr_intended_bucket;	
			curr_data = new_data;
			curr_intended_bucket = new_intended_bucket;
		}	
		hash_index = (hash_index + 1) % numBuckets;
	}

	table[hash_index].data = curr_data;
	table[hash_index].intended_bucket = curr_intended_bucket;	
}

int RobinHoodHashTable::containsWithStartIndex(size_t hash_index, int data) const {
	size_t intended_bucket = hash_index;	

	while (table[hash_index].data >= 0){ 
		if (shouldReplace(hash_index, intended_bucket))
			break;	
		if (table[hash_index].data == data)
			return hash_index;	
		hash_index = (hash_index + 1) % numBuckets;	
	}
	return -1; 
}

bool RobinHoodHashTable::contains(int data) const {
	bool doesContain = containsWithStartIndex(h(data) % numBuckets, data) >= 0;	
	return doesContain;
}

void RobinHoodHashTable::remove(int data) {
	int index = containsWithStartIndex(h(data) % numBuckets, data);	
	
	if (index >= 0) {
		while (true) {
			int next_index = (index + 1) % numBuckets;
			if (next_index == (int)table[next_index].intended_bucket){
				table[index].data = -1;
				break;
			}
			
			table[index] = table[next_index];
			if (table[next_index].data < 0)
				break;
			index = next_index;
		}
	}
}





