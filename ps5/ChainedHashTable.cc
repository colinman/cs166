#include "ChainedHashTable.h"

ChainedHashTable::ChainedHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
	table = new Node *[numBuckets]();
	h = family->get();	
	this->numBuckets = numBuckets;
}

static void deleteChain(Node *curr) {
	if (curr == NULL) return;

	deleteChain (curr->next);
	delete curr;		
}

ChainedHashTable::~ChainedHashTable() {
	for (size_t i = 0; i < numBuckets; i++){
		deleteChain(table[i]);
	}	
	delete []table;
}

static bool chainContains(Node *curr, int data) {
	if (curr == NULL) return false;
	if (curr->data == data) return true;
	return chainContains(curr->next, data);	
}

void ChainedHashTable::insert(int data) {
	size_t hash_index = h(data) % numBuckets;
	if (chainContains(table[hash_index], data)) return;

	Node *prev_front = table[hash_index];
	Node *entry = new Node(data);
	table[hash_index] = entry;
	entry->next = prev_front;	
}


bool ChainedHashTable::contains(int data) const {
	return chainContains(table[h(data) % numBuckets], data);  
}

static void removeFromChain(Node *curr, int data) {
	if (curr->next == NULL) return;
	
	if (curr->next->data == data){
		Node *to_delete = curr->next;
		curr->next = curr->next->next;
		delete to_delete;
		return;
	}
	removeFromChain(curr->next, data);	
}

void ChainedHashTable::remove(int data) {
	size_t hash_index = h(data) % numBuckets;
	if (table[hash_index] != NULL) {
		if (table[hash_index]->data == data){
			Node *to_delete = table[hash_index];
			table[hash_index] = table[hash_index]->next;
			delete to_delete;
		}else {
			removeFromChain(table[hash_index], data);	
		}
	} 
}









