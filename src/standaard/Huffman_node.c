#include "huffman_node.h"
#include <stdlib.h>

huffman_node* initNode(bool isLeaf, char symbol) {
	huffman_node* node = malloc(sizeof(huffman_node));
	node->isLeaf = isLeaf;
	node->frequencies = 0;
	node->left = NULL;
	node->right = NULL;
	if(isLeaf)
		node->symbol = symbol;
	node->parent = NULL;
	return node;
}

void freeNode(huffman_node * node)
{
	free(node);
}

bool addChild(huffman_node * parent, huffman_node * child, bool addAsLeftChild)
{
	if (addAsLeftChild && !parent->left) {
		parent->left = child;
	}
	else if (!parent->right)
		parent->right = child;
	else
		return false;
	child->parent = parent;
	return true;
}

/*
Merges both, resulting in a new (common) parent node being the root of a new tree.
*/
huffman_node* merge(huffman_node *one, huffman_node *two) {
	huffman_node* merged = initNode(false, 0);
	addChild(merged, one, true);
	addChild(merged, two, false);
	merged->frequencies = one->frequencies + two->frequencies;
	return merged;
}

int compareNodes(huffman_node * a, huffman_node * b)
{
	if (a->frequencies > b->frequencies)
		return 1;
	else if (a->frequencies < b->frequencies)
		return -1;
	else
		return 0;
}
