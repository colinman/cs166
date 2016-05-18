#include "LinearProbingHashTable.h"

struct Node {
	Node () {
		isEmpty = true;
		isTombstone = false;
	}	

	int data;
	bool isEmpty;
	bool isTombstone;
};

LinearProbingHashTable::LinearProbingHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
	table = new Node [numBuckets]();
	h = family->get();
	this->numBuckets = numBuckets;
}

LinearProbingHashTable::~LinearProbingHashTable() {
	delete []table;
}

void LinearProbingHashTable::insert(int data) {
	size_t index = h(data) % numBuckets;
	while (!table[index].isEmpty){
		if (!table[index].isTombstone && table[index].data == data)
			return;
		index = (index + 1) % numBuckets; 
	}
	
	table[index].data = data;
	table[index].isEmpty = false;
	table[index].isTombstone = false;
}


int LinearProbingHashTable::indexOfData(int data) const {
	int index = h(data) % numBuckets;
	while (true) {
		if (!table[index].isTombstone){
			if (table[index].isEmpty){
				 break;
			}
			else{
				if (table[index].data == data) return index;
			}	
		} 
		index = (index + 1) % numBuckets;	
	}
	return -1;	
}

bool LinearProbingHashTable::contains(int data) const {
	return indexOfData(data) >= 0;
}

void LinearProbingHashTable::remove(int data) {
	int index_to_remove = indexOfData(data); 
	if (index_to_remove >= 0){
		table[index_to_remove].isTombstone = true;
		table[index_to_remove].isEmpty = true;
	}	
}

