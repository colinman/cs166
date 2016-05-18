#include "SplayTree.h"

struct node {
	int key;
	struct node *left;
	struct node *right;
};

/* Start is inclusive, end is exclusive. */
static struct node *buildDegenerateTree(const std::vector<double>& weights, int index){
	if (index >= weights.size()) return NULL;

	struct node *new_node = new struct node();
	new_node->key = index;
	new_node->left = NULL;
	new_node->right = buildDegenerateTree(weights, index + 1);
	return new_node;
}
/**
 * Given a list of the future access probabilities of the elements 0, 1, 2,
 * ..., weights.size() - 1, constructs a new splay tree holding those
 * elements.
 *
 * Because splay trees rearrange their elements in response to queries, you
 * can safely ignore the assigned probabilities here and just build a BST
 * storing the elements 0, 1, 2, ..., weights.size() - 1 however you'd like.
 */
SplayTree::SplayTree(const std::vector<double>& weights) {
	/* Splay trees can start with an arbitrary configuration, right?*/
	/*std::vector<double> temp = std::vector<double> (7);
	
	t = buildDegenerateTree(temp, 0);

	test_flag = true;
	contains(0);
	contains(6);
	contains(4);
	contains(10);
	contains(50);
	test_flag = false;*/
	
	t = buildDegenerateTree(weights, 0);
}

static void freeTree(struct node *curr) {
	if (curr == NULL) return;

	freeTree(curr->left);
	freeTree(curr->right);
	delete curr;
}

/**
 * Frees all memory used by this tree.
 */
SplayTree::~SplayTree() {
	freeTree(t);
}

void SplayTree::link_left() {
	if (l != NULL) l->right = t;
	else root_L = t; 

	l = t;
	t = t->right;
}

void SplayTree::link_right() {
	if (r != NULL) r->left = t;
	else root_R = t;

	r = t;
	t = t->left;
}

void SplayTree::rotate_left() {
	struct node *t_right = t->right;	
	struct node *t_right_left = t->right->left;
	t->right->left = t;	
	t->right = t_right_left;
	t = t_right;
}

void SplayTree::rotate_right() {
	struct node *t_left = t->left;
	struct node *t_left_right = t->left->right;
	t->left->right = t;
	t->left = t_left_right;
	t = t_left;
}

bool SplayTree::assemble() {
	/* If we didn't find anything  */
/*	if (t == NULL) {
		if (root_L == NULL) {
			t = root_R;
			return;
		}
		else {
			t = l;
			if (prev_l != NULL){
				prev_l->right = l->left;
				l = prev_l;
				while (l->right != NULL) l = l->right;	
			}
			t->left = NULL;
			t->right = NULL;	
		}
	}*/
	if (t == NULL){
		if (root_L == NULL) t = root_R;
		else t = root_L;
		return false;
	}

	if (t != NULL) {
		if (l != NULL){
			l->right = t->left;	
			t->left = root_L; 
		}

		if (r != NULL) {
			r->left = t->right;
			t->right = root_R;
		}
	}
	return true;
}
/*
static void print_nodes(struct node *curr, int max) {
	if (max <= 0) return;

	if (curr == NULL) return;
	printf("(");
	print_nodes(curr->left, max - 1);
	printf("%d", curr->key);
	print_nodes(curr->right, max - 1);
	printf(")");
}

static void inorder_print(struct node *curr, int max) {
	print_nodes(curr, max);
	printf("\n");	
}*/

/**
 * Determines whether the specified key is present in the splay tree. Your
 * implementation should use only O(1) memory. We recommend looking up the
 * top-down splaying approach described in Sleator and Tarjan's paper,
 * tracing through it, and coding it up.
 */
bool SplayTree::contains(int key) {
	if (!test_flag){};

	root_L = NULL;
	root_R = NULL;
	l = NULL;
	r = NULL;

	while (t != NULL && t->key != key) {
		if (key < t->key) {
			if (t->left != NULL && key < t->left->key){
				rotate_right();
			}
			link_right();	
		}else {
			if (t->right != NULL && key > t->right->key){
				rotate_left();
			}
			link_left();
		}
	}
	/* Assemble! */
	return assemble();
/*
	printf("\n Contains %d: %d\n", key, t != NULL);
	inorder_print(t, 7);
*/
}

