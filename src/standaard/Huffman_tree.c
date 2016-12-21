#include "huffman_tree.h"
#include <stdlib.h>
#include <limits.h>

#ifdef _DEBUG
#include <assert.h>
#endif

/*
Sets indices of smallest 2 numbers to given parameters.
*/
void find2smallest(int* smallestIdx, int* secondsmallestIdx, huffman_node** nodes,int nodeslength) {
	unsigned long long curfreq=0;
	unsigned long long smallestFreq = LONG_MAX, secondsmallestFreq = LONG_MAX;
	for (int i = 0; i < nodeslength; i++) {
		if (!nodes[i])
			continue;
		curfreq = nodes[i]->frequencies;
		if (curfreq < secondsmallestFreq) {
			if (curfreq < smallestFreq) {
				*secondsmallestIdx = *smallestIdx;
				*smallestIdx = i;
				secondsmallestFreq = smallestFreq;
				smallestFreq = curfreq;
			}
			else {
				secondsmallestFreq = curfreq;
				*secondsmallestIdx = i;
			}
		}
	}
}

huffman_tree * initTree_ByFrequencies(unsigned long long * freqTable)
{
	huffman_node** nodes = malloc(256 * sizeof(huffman_node*)); // only frequency > 0 allowed
	if (!nodes)
		return NULL;
	// init single symbol roots
	
	// (canceled) enchancement #1 : we can already store the 2 smallest frequencies while doing enhancement #2.
	//int smallest, secondsmallest;
	
	//enhancement #2 : we store only the not 0 frequencies in the array
	int amountNonZero=0;
	for (int i = 0; i < 256; i++) {
		if (freqTable[i] > 0) {
			nodes[amountNonZero] = initNode(true, i);
			nodes[amountNonZero]->frequencies = freqTable[i];
			amountNonZero++;
		}
	}
	nodes = realloc(nodes, amountNonZero*sizeof(huffman_node));
	// Merge huffman-way
	int smallest=0, secondsmallest=0;
	for (int i = 0; i < amountNonZero - 1; i++) {
		find2smallest(&smallest, &secondsmallest,nodes,amountNonZero);
		nodes[smallest] = merge(nodes[smallest], nodes[secondsmallest]);
		nodes[secondsmallest] = NULL;
	}
	huffman_tree* tree = malloc(sizeof(huffman_tree));
	tree->root = nodes[smallest];
	free(nodes);
	return tree;
}

huffman_tree * initTree_byRoot(huffman_node * root)
{
	huffman_tree* tree = malloc(sizeof(huffman_tree));
	tree->root = root;
	return tree;
}

void _recursive_freeTree(huffman_node* node) {
	if (!node || node->isLeaf)
		freeNode(node);
	else {
		_recursive_freeTree(node->left);
		_recursive_freeTree(node->right);
	}
}

void freeTree(huffman_tree * tree)
{
	if (tree) {
		huffman_node* cur = tree->root;
		_recursive_freeTree(cur);
	}
	free(tree);
}

/*
ITERATOR
*/
huffman_tree_iterator * init_iterator(huffman_tree* tree)
{
	huffman_tree_iterator* i = malloc(sizeof(huffman_tree_iterator));
	i->cur = tree->root;
	return i;
}

void free_iterator(huffman_tree_iterator * i)
{
	if (i)
		free(i);
}

huffman_node* iter_next(huffman_tree_iterator* iter) {
	//todo speciaal geval 1 node
	huffman_node* cur = iter->cur,*parent, *toReturn = cur;
	if (cur == NULL)
		return NULL;
	// finding next, remember each node has 0 or 2 children.
	if (cur->isLeaf) {
			// cur is leaf
		while (true) {
			parent = cur->parent;
			if (parent == NULL) {
				// This can only happen if we came from right! See code below.
				iter->cur = NULL;
				break;
			}

			// is cur the left child?
			bool left = parent->left == cur;
			//
			if (!left) {
				//cur is right
				cur = parent; 
				continue; // repeat process, but on 1 level higher.
			}
			else {
				//cur is left => go to right
#ifdef _DEBUG
				assert(parent->right != NULL);
#endif
				iter->cur = parent->right;
				break;
			}
		}
	}
	else if (cur->left)
		iter->cur = cur->left;
	else 
		//(cur->right != NULL)
		iter->cur = cur->right;
	return toReturn;
}
