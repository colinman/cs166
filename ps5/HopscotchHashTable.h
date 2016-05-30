#ifndef HopscotchHashTable_Included
#define HopscotchHashTable_Included

#include "Hashes.h"

class HopscotchHashTable {
public:
  /**
   * Constructs a new Hopescotch hash table with the specified number of buckets,
   * using hash functions drawn from the indicated family of hash functions.
   * Because our testing harness attempts to exercise a number of different
   * load factors, you should not change the number of buckets once the hash
   * table has initially be created.
   *
   * You can choose a hash function out of the family of hash functions by
   * declaring a variable of type HashFunction and assigning it the value
   * family->get(). For example:
   *
   *    HashFunction h;
   *    h = family->get();
   */
  HopscotchHashTable(size_t numBuckets, std::shared_ptr<HashFamily> family);
  
  /**
   * Cleans up all memory allocated by this hash table.
   */
  ~HopscotchHashTable();
  
  /**
   * Inserts the specified element into this hash table. If the element already
   * exists, this operation is a no-op.
   */
  void insert(int key);
  
  /**
   * Returns whether the specified key is contained in this hash tasble.
   */
  bool contains(int key) const;
  
  /**
   * Removes the specified element from this hash table. If the element is not
   * present in the hash table, this operation is a no-op.
   *
   * You should implement this operation using tombstone deletion - replace the
   * key to remove with a special "tombstone" value indicating that something
   * that was stored here has since been removed.
   */
  void remove(int key);
  
private:
 	struct Neighborhood *table;
	HashFunction h;
	size_t numBuckets; 

 	void rehash();
	int containsHelper(int data) const;
 
/* Fun with C++: these next two lines disable implicitly-generated copy
   * functions that would otherwise cause weird errors if you tried to
   * implicitly copy an object of this type. You don't need to touch these
   * lines.
   */
  HopscotchHashTable(HopscotchHashTable const &) = delete;
  void operator=(HopscotchHashTable const &) = delete;
};

#endif
