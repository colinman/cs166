#include "PerfectlyBalancedTree.h"


struct node {
	int key;
	struct node *left;
	struct node *right;
};

/* Start is inclusive, end is exclusive. */
static struct node *buildBalancedTree(const std::vector<double>& weights, int start, int end){
	if (end <= start) return NULL;
	
	int new_node_index = start + (end - start) / 2;

	struct node *new_node = new struct node(); 
	new_node->key = new_node_index;
	new_node->left = buildBalancedTree(weights, start, new_node_index);
	new_node->right = buildBalancedTree(weights, new_node_index + 1, end);
	return new_node;  
}

/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new perfectly-balanced tree holding
 * those elements.
 *
 * Because perfectly-balanced trees don't care about access probabilities,
 * you should ignore the actual weights provided here and just build a
 * perfectly-balanced tree holding the keys 0, 1, 2, ..., weights.size() - 1.
 */
PerfectlyBalancedTree::PerfectlyBalancedTree(const std::vector<double>& weights) {
	root = buildBalancedTree(weights, 0, weights.size());
}

static void freeTree (struct node *curr) {
	if (curr == NULL) return;

	freeTree(curr->left);
	freeTree(curr->right);
	delete curr;
}
/**
 * Frees all memory used by this tree.
 */
PerfectlyBalancedTree::~PerfectlyBalancedTree() {
	freeTree(root);
}

static bool searchForKey(struct node *curr, int search_key){
	if (curr == NULL) return false;
	if (curr->key == search_key) return true;
	
	if (search_key < curr->key) return searchForKey(curr->left, search_key);
	else return searchForKey(curr->right, search_key);
}


/**
 * Determines whether the specified key is present in the perfectly-balanced
 * tree.
 */
bool PerfectlyBalancedTree::contains(int key) const {
 	return searchForKey(root, key); 
}





