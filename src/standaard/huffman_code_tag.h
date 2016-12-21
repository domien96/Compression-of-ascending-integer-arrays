#ifndef CODE_TAG
#define CODE_TAG
/*
Class and methods for the code_tags of each character.
*/

#include <stdlib.h>
#include "huffman_tree.h"
#include "huffman_node.h"
#include <stdbool.h>

#define MAX_SYMBOLS 256

typedef struct {
	//length of the code == length of bool array
	int code_length;
	// array with same code length, representing the bits
	// LEFT 0 and RIGHT 1
	bool* codeword;
} huffman_code_tag;

typedef huffman_code_tag* charToCodeMap[MAX_SYMBOLS];

/*
Returns array of length MAX_SYMBOLS, use ascii translation of char as index.
*/
charToCodeMap* init_charToCodeMap();

void free_charToCodeMap(charToCodeMap* m);

void fill_charToCodeMap(charToCodeMap* m, huffman_tree* tree);
#endif