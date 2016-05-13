#include "WeightBalancedTree.h"
#include <cmath>

struct node {
	int key;
	struct node *left;
	struct node *right;
};
/*
static bool verifyWeights(const std::vector<double>& weights,
						int start, int end, double sum_weights){
	double sum = 0;
	for (int i = start; i <= end; i++) sum += weights[i];
	if (sum != sum_weights){
		printf("sum: %f sum_weights: %f\n", sum, sum_weights);
		return false;
	}
	return true; 

}
*/

/* Start and end are both inclusive now */
static struct node *buildWeightBalancedTree(const std::vector<double>& weights,
											int start, int end, double sum_weights){

	if (end < start) return NULL;

	int index_from_left = start;
	int index_from_right = end;
	double sum_from_left = 0;
	double sum_from_right = 0;
	int new_node_index;
	
	if (end == start) {
		new_node_index = start;
		/* No need to set sums because end < start in the next recursion  */
	}else {

		double diff_from_left = sum_weights;
		double diff_from_right = sum_weights;
		double new_diff;
		while (true){

			/* Evaluate the left pointer */	
			sum_from_left += weights[index_from_left];
			new_diff = std::abs((sum_weights - sum_from_left) - sum_from_left);
			if (diff_from_left < new_diff) {
				new_node_index = index_from_left - 1;
				sum_from_left -= weights[index_from_left] + weights[index_from_left - 1];
				sum_from_right = sum_weights - sum_from_left - weights[index_from_left - 1];
				break;				
			}	
			diff_from_left = new_diff; 
			index_from_left ++;
	
			/* Evaluate the right pointer */	
			sum_from_right += weights[index_from_right];
			new_diff = std::abs((sum_weights - sum_from_right) - sum_from_right); 
			if (diff_from_right < new_diff){
				new_node_index = index_from_right + 1;
				sum_from_right -= weights[index_from_right] + weights[index_from_right + 1];
				sum_from_left = sum_weights - sum_from_right - weights[index_from_right + 1];
				break;
			}
			diff_from_right = new_diff; 
			index_from_right --; 	
		}
	}
	
/*	bool bad = false;
	!verifyWeights(weights, start, new_node_index - 1, sum_from_left);
	if (bad){
		printf("left start %d end %d\n", start, end);
		printf("selected %d %f\n", new_node_index, weights[new_node_index]);	
	}

	bool otherbad = !verifyWeights(weights, new_node_index + 1, end, sum_from_right);
	if (otherbad){
		printf("right start %d end %d\n", start, end);
		printf("selected %d %f\n", new_node_index, weights[new_node_index]);	
	}
	if (bad || otherbad) return NULL;
*/

	struct node *new_node = new struct node();
	new_node->key = new_node_index;
	new_node->left = buildWeightBalancedTree(weights, start, new_node_index - 1, sum_from_left);
	new_node->right = buildWeightBalancedTree(weights, new_node_index + 1, end, sum_from_right);
	return new_node;
}

static double sumOfVector(const std::vector<double>& weights) {
	double sum = 0;
	for (int i = 0; i < weights.size(); i ++){
		sum += weights[i];
	}
	return sum;
}

/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new, optimally-balanced BST for the
 * elements 0, 1, 2, ..., weights.size() - 1.
 *
 * Notice that the keys are not given to you explicitly, but the assumption
 * is that the keys are 0, 1, 2, ..., weights.size() - 1 with weights
 * weights[0], weights[1], weights[2], ..., weights[weights.size() - 1].
 */
WeightBalancedTree::WeightBalancedTree(const std::vector<double>& weights) {
	root = buildWeightBalancedTree(weights, 0, weights.size() - 1, sumOfVector(weights));	
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
WeightBalancedTree::~WeightBalancedTree() {
	freeTree(root);
}


static bool searchForKey(struct node *curr, int search_key) {
	if (curr == NULL) return false;
	if (curr->key == search_key) return true;
	
	if (search_key < curr->key) return searchForKey (curr->left, search_key);
	else return searchForKey(curr->right, search_key);
}

/**
 * Determines whether the specified key is present in the weight-balanced
 * tree.
 */
bool WeightBalancedTree::contains(int key) const {
	return searchForKey(root, key);
}











