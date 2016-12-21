#ifndef TREE
#define TREE

#include "huffman_node.h"

typedef struct {
	huffman_node* root;
} huffman_tree;

/*
Characters with Frequency 0 will NOT be added to the tree.
Returns NULL if initialization not possible or no characters with a frequency of at least 1.
*/
huffman_tree* initTree_ByFrequencies(unsigned long long* freqTable);

huffman_tree* initTree_byRoot(huffman_node* root);

void freeTree(huffman_tree*);

/*
ITERATOR
*/
typedef struct {
	huffman_node* cur;
} huffman_tree_iterator;

huffman_tree_iterator* init_iterator(huffman_tree* tree);

void free_iterator(huffman_tree_iterator* i);

huffman_node* iter_next(huffman_tree_iterator* iter);
#endif

