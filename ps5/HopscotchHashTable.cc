#include "HopscotchHashTable.h"
#include <string.h>

#define H 32

struct Neighborhood {
	int data;
	int bitmap; 
};

HopscotchHashTable::HopscotchHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family) {
	table = new Neighborhood[numBuckets];
	h = family->get();
	this->numBuckets = numBuckets;
	
	for (size_t i = 0; i < numBuckets; i++){
		table[i].data = -1;
		table[i].bitmap = 0;
	}
}

HopscotchHashTable::~HopscotchHashTable() {
	//printf("DELTEED EVERYTHING\n");
	delete []table;
}

static int calcDiff (int index, int nextIndex, size_t numBuckets) {
	if (nextIndex < index)
		return numBuckets - ((int) nextIndex - index);
	return nextIndex - index;	
}

void HopscotchHashTable::rehash() {
	//printf("rehash triggered\n");
	// Doubles the size of the hash table and rehashes everything 
	size_t prevNumBuckets = numBuckets;
	numBuckets *= 2;

	Neighborhood *oldTable = table;
	table = new Neighborhood[numBuckets];
	for (size_t i = 0; i < numBuckets; i++){
		table[i].data = -1;
		table[i].bitmap = 0;
	}

	for (size_t i = 0; i < prevNumBuckets; i++){
		if (oldTable[i].data != -1)
			insert(oldTable[i].data);	
	}

	delete [] oldTable;
}

void HopscotchHashTable::insert(int data) {
	//printf("insert %d\n", data);
	if (contains(data)) return;

	int index = h(data) % numBuckets;
	//printf("original bucket %d\n", index);
	int emptyIndex = index;
	// Linear probing
	size_t count = 0; 
	for (; count < numBuckets; count ++) {
		if (table[emptyIndex].data == -1)
			break; 	
		
		emptyIndex = (emptyIndex + 1) % numBuckets;
	}

	if (count == numBuckets){
		rehash();
		insert(data);
		return;
	}

	// Insert the element immediately if you can
	int offSet = calcDiff(index, emptyIndex, numBuckets);
	if (offSet < H) {
		//printf("Injected in index: %d\n", emptyIndex); 
		table[emptyIndex].data = data;
		table[index].bitmap |= (1 << offSet);  
		//printf("Bitmap %d\n", table[index].bitmap);
	}else {
//		printf("OH GOD\n");
		// Hopescotch backwards until you succeed
		while (offSet >= H) {
			// offsetEarlier: earlier neighbor to earlier spot
			// offsetEmpty: earlier neighbor to empty spot
			int offsetEmpty = H - 1;
			int earlierNeighbor;
			int offsetEarlier;
			for (; offsetEmpty >= 0; offsetEmpty --){
				earlierNeighbor = emptyIndex - offsetEmpty;
				if (earlierNeighbor < 0) 
					earlierNeighbor += numBuckets;	

				offsetEarlier = ffs(table[earlierNeighbor].bitmap) - 1;
				if (offsetEarlier >= 0 && offsetEarlier < offsetEmpty) 
					break; 
			}

			if (offsetEmpty < 0){ // rehash 
				rehash();
				insert(data);
				return;
			}
	
			// Move entry up
			int earlier = (earlierNeighbor + offsetEarlier) % numBuckets;	
			table[emptyIndex].data = table[earlier].data;

			// Update bitmap
			table[earlierNeighbor].bitmap &= ~(1 << offsetEarlier); 
			table[earlierNeighbor].bitmap |= (1 << offsetEmpty);
		
			// Move empty index down	
			emptyIndex = earlier; 
			offSet -= (offsetEmpty - offsetEarlier); 
		}	
		
		// Insert element	
		table[(index + offSet) % numBuckets].data = data;
		table[index].bitmap |= (1 << offSet);
	}
}

// RETURNS OFFSET
int HopscotchHashTable::containsHelper(int data) const {
	int index = h(data) % numBuckets;
	int bitmap = table[index].bitmap;
	int fbs = ffs(bitmap); // 1 bigger than it should be 
	while (fbs != 0) {
		int offSet = fbs - 1;
		if (table[(index + offSet) % numBuckets].data == data)
			return offSet;
		bitmap &= ~(1 << offSet);
		fbs = ffs(bitmap);
	}
	return -1;	
}

bool HopscotchHashTable::contains(int data) const {
//	printf("contains %d ? %d\n", data, containsHelper(data) != -1);
	return containsHelper(data) != -1;	
}


void HopscotchHashTable::remove(int data) {
	//printf("remove %d\n", data);
	int offSet = containsHelper(data);
	if (offSet != -1) {
		int index = h(data) % numBuckets;
		table[(index + offSet) % numBuckets].data = -1;
		table[index].bitmap &= ~(1 << offSet);  
	}	
}


