#include "SecondChoiceHashTable.h"

struct Node {
	Node (int input_data) {
		data = input_data;
		next = NULL;
	}

	int data;
	Node *next;
};

SecondChoiceHashTable::SecondChoiceHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
	table = new StartNode [numBuckets]();
	h1 = family->get();
	h2 = family->get();
	this->numBuckets = numBuckets;
}

static void deleteChain(Node *curr) {
	if (curr == NULL) return;

	deleteChain (curr->next);
	delete curr;
}

SecondChoiceHashTable::~SecondChoiceHashTable() {
	for (size_t i = 0; i < numBuckets; i++) {
		deleteChain(table[i].chain);
	}
	delete []table;	
}

static bool chainContains (Node *curr, int data) {
	if (curr == NULL) return false;
	if (curr->data == data) return true;
	return chainContains (curr->next, data);
}

void SecondChoiceHashTable::insert(int data) {
	size_t hash_index1 = h1(data) % numBuckets;
	size_t hash_index2 = h2(data) % numBuckets;
	if (chainContains(table[hash_index1].chain, data) ||
		chainContains(table[hash_index2].chain, data))
		return;	

	size_t hash_index = table[hash_index1].count < table[hash_index2].count ?
						hash_index1 : hash_index2;
	Node *prev_front = table[hash_index].chain;
	Node *entry = new Node(data);
	table[hash_index].chain = entry;
	entry->next = prev_front;
	table[hash_index].count ++;
}


bool SecondChoiceHashTable::contains(int data) const {
	size_t hash_index1 = h1(data) % numBuckets;
	size_t hash_index2 = h2(data) % numBuckets;
	
	return chainContains(table[hash_index1].chain, data) ||
			chainContains(table[hash_index2].chain, data);
} 

static bool removeFromChain(Node *curr, int data) {
	if (curr->next == NULL) return false;
	
	if (curr->next->data == data){
		Node *to_delete = curr->next;
		curr->next = curr->next->next;
		delete to_delete;
		return true;
	}
    return removeFromChain(curr->next, data);	
}

static bool removeChain(StartNode *table, size_t hash_index, int data) {
	if (table[hash_index].chain != NULL) {
		if (table[hash_index].chain->data == data) {
			Node *to_delete = table[hash_index].chain;
			table[hash_index].chain = table[hash_index].chain->next;
			delete to_delete;
			return true;
		}else {
			return removeFromChain(table[hash_index].chain, data);
		}
	}
	return false;
}

void SecondChoiceHashTable::remove(int data) {
	size_t hash_index1 = h1(data) % numBuckets;
	size_t hash_index2 = h2(data) % numBuckets;
	if (removeChain(table, hash_index1, data))
		table[hash_index1].count --;
	else if (removeChain(table, hash_index2, data))
		table[hash_index2].count --;
}






