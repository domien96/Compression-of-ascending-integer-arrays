#include "huffman_code_tag.h"
#include "huffman_node.h"
#include <stdlib.h>
#include <string.h>

/*
Tag
*/
huffman_code_tag* init_code_tag() {
	huffman_code_tag* res = (huffman_code_tag*) malloc(sizeof(huffman_code_tag));
	res->code_length = 0;
	res->codeword = NULL;
	return res;
}

/*
DEEP COPY
*/
huffman_code_tag* copy_code_tag(huffman_code_tag* orig) {
	huffman_code_tag* copy = init_code_tag();
	copy->code_length = orig->code_length;
	copy->codeword = malloc(copy->code_length*sizeof(bool));
	memcpy(copy->codeword, orig->codeword, copy->code_length);
	return copy;
}

void free_code_tag(huffman_code_tag* c) {
	free(c->codeword);
	free(c);
}

/* 1->true ; 0->false */
void append_bit_to_code_tag(huffman_code_tag* c, bool bit) {
	c->code_length = c->code_length + 1;
	c->codeword = (bool*)realloc(c->codeword, c->code_length);
	c->codeword[c->code_length - 1] = bit;
}

/*
Map
*/

charToCodeMap* init_charToCodeMap() {
	charToCodeMap* map = (charToCodeMap*)calloc(1, sizeof(charToCodeMap));
	return map;
}

void free_charToCodeMap(charToCodeMap* m) {
	for (int i = 0; i < MAX_SYMBOLS; i++) {
		if ((*m)[i])
			free((*m)[i]);
	}
	free(m);
}

/*
currentroot: from where we are now in the tree.
prototype: built based on where we are now in the whole tree.
*/
void _recursivefill_charToCodeMap(charToCodeMap* m, huffman_node* currentroot, huffman_code_tag* prototype) {
	if (currentroot->isLeaf) {
		(*m)[currentroot->symbol] = copy_code_tag(prototype);
	}
	else {
		huffman_code_tag* prototypeLeft=copy_code_tag(prototype), *prototypeRight=copy_code_tag(prototype);
		append_bit_to_code_tag(prototypeLeft, false);
		_recursivefill_charToCodeMap(m, currentroot->left, prototypeLeft);
		append_bit_to_code_tag(prototypeRight, true);
		_recursivefill_charToCodeMap(m, currentroot->right, prototypeRight);
		free_code_tag(prototypeLeft);
		free_code_tag(prototypeRight);
	}
}

void fill_charToCodeMap(charToCodeMap* m, huffman_tree* tree) {
	huffman_node* cur = tree->root;
	if (cur == NULL)
		return;
	// dummy code_tag-object
	huffman_code_tag* dummy = init_code_tag();
	_recursivefill_charToCodeMap(m, cur, dummy);
	free_code_tag(dummy);
}