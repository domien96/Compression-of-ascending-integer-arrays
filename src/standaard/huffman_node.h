#ifndef NODE
#define NODE

#include <stdbool.h>

typedef struct _huffman_node {
	struct _huffman_node *left, *right, *parent;
	unsigned char symbol;
	bool isLeaf;
	unsigned long long frequencies;
} huffman_node;


/*
symbol is only relevant if isLeaf == true
*/
huffman_node* initNode(bool isLeaf, char symbol);

/*
Parents and children will NOT be freed.
*/
void freeNode(huffman_node* node);

/*
Will not do something if spot already taken by another node (and returning false).
return true if succesfull.
*/
bool addChild(huffman_node* parent, huffman_node* child, bool addAsLeftChild);

/*
Merges both, resulting in a new (common) parent node being the root of a new tree.
*/
huffman_node* merge(huffman_node *one, huffman_node *two);

/*
Compares two nodes based on their frequencies.
*/
int compareNodes(huffman_node*, huffman_node*);

#endif