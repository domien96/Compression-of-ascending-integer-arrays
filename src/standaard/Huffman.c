#include <stdlib.h>
#include "huffman_tree.h"
#include "huffman_node.h"
#include "../common/Bitstream.h"
#include "../common/Services.h"
#include "huffman_code_tag.h"
#include "FrequencyCounter.h"
#include "Huffman.h"
#include "rle.h"

#ifdef _DEBUG
#include <assert.h>
#endif

// in bits, maximum 8! Don't change.
// FORMAT HEADER: 1bit to determine if RLE or not (1 if RLE), 3bits to determine garbage bits.
// !! if RLE, then garbage header bits will not be written.
#define HEADER_SIZE 4

/***********************
		ENCODE
***********************/

/*
Writes tree.
*/
void huff_writeTree(huffman_tree* tree,Bitstream* output) {
	//Depth first iteration over the tree
	huffman_tree_iterator* iter = init_iterator(tree);
	huffman_node* current = iter_next(iter);
	if (current == NULL)
		return;
	current = iter_next(iter); // we tellen de bogen, niet de topen. dus wortel overslaan als node.
	//TODO : tree met 1 node speciaal geval
	bool T = true, F = false;
	while (current != NULL) {
		if (current->isLeaf) {
			writeBits(output, 1, &T);
			bool* boolarr = chrtobool(current->symbol);
			writeBits(output, 8, boolarr);
			free(boolarr);
		}
		else {
			writeBits(output, 1, &F);
		}
		current = iter_next(iter);
	}
	free_iterator(iter);
	// ending bit : ZEER BELANGRIJK voor correcte DEcodering
	writeBits(output, 1, &T);
}

/*
Writes data
*/
void huff_writeData(huffman_tree* tree, FILE* input, Bitstream* output) {
	// Quick char to codeword mapping
	charToCodeMap* map = init_charToCodeMap();
	fill_charToCodeMap(map, tree);
	//
	huffman_code_tag* tag;
	int c = fgetc(input);
	
	while (!feof(input) && !(c==EOF)) {
		tag = (*map)[c];
		writeBits(output, tag->code_length, tag->codeword);
		c = fgetc(input);
	}
	// free all stuff
	free_charToCodeMap(map);
}

int encode(const char* infilename, const char* outfilename) {
	// Open streams
	Bitstream *out = initBitstream(outfilename, "wb");
	if (out==NULL)
		return 1;
	FILE* in = fopen(infilename, "rb");
	if (in==NULL) {
		closeBitstream(out);
		return 1;
	}

	// Generate frequencies (use input)
	unsigned long long *freqs = init_freq(in); // may contain 0's frequencies
	if (freqs==NULL) {
		fclose(in);
		closeBitstream(out);
		return 2;
	}
	// Create tree
	huffman_tree* tree = initTree_ByFrequencies(freqs);
	if (tree==NULL) {
		fclose(in);
		closeBitstream(out);
		return 2;
	}
	free_freq(freqs);

	if (tree->root->isLeaf) {
		// Only 1 charachter in file, this cannot be encoded by huffman efficiently. We use RLE instead.
		bool t = true;
		writeBits(out, 1, &t);
		rle_encode(tree->root,out);
		fclose(in);
		closeBitstream(out);
	}
	else {
		// HUFFMAN
		// Save space for header, wil be filled in later when we have all header-info. (in output)
		bool* placeholder = calloc(HEADER_SIZE, sizeof(bool));
		writeBits(out, HEADER_SIZE, placeholder);
		free(placeholder);
		// Write huffmann tree (in output)
		huff_writeTree(tree, out);
		// Write prefixes (prefixes in output, characters from input)
		rewind(in); // we counted frequencies already, so reset cursor.**
		huff_writeData(tree, in, out);

		// Fill in header space now we have all the information we need. (in output)
		// Header contains amount of garbage bits at the last byte, which flush will tell us.
		char garbage_size = flushBits(out);
		closeBitstream(out);

		// reading other bits of first byte to prevent overwriting with garbage-header-bits.
		FILE *outf = fopen(outfilename, "r+");
		if (!outf) {
			fclose(in);
			return 1;
		}
		char firstByte;
		fread(&firstByte, 1, 1, outf);
		rewind(outf);

		// manipulate this stuff in first byte.
		garbage_size = garbage_size << (8 - HEADER_SIZE);
		firstByte = firstByte | garbage_size; // FORM : 0GGG DDDD , G(arbage) D(ata)
		fwrite(&firstByte, 1, 1, outf);

		// Closing the streams is our job...
		fclose(in);
		fclose(outf);
	}
	return 0;
}

/***********************
		DECODE
***********************/

huffman_tree* huff_readTree(Bitstream* in) { // TODO this code can be made cleaner.
	huffman_node* root = initNode(false, '\0'), *newnode;
	huffman_tree* tree = initTree_byRoot(root);
	huffman_node* cur = root;
	bool bit;
	bool chr[8];
	
	// DEBUG STUFF
	bool AlreadyPrintedWarning= false; // Used to avoid mwarning spams
	int receivedValue = 0; // used to store the return value in.
	//

	while (true) {
		receivedValue = readBits(in, 1, &bit);
		if (receivedValue != 1 && !AlreadyPrintedWarning) {
			AlreadyPrintedWarning = true;
			fprintf(stderr,"Assertion failed! Expected 1, but got %i \nMaybe your inputfile is corrupt or not suitable for this compression program.", receivedValue);
		}
		if (!bit) {
			// Internal node
			newnode = initNode(false, '\0');
			while (true)
			{
				if (cur->left == NULL) {
					addChild(cur, newnode, true);
					cur = newnode;
					break;
				}
				else if (cur->right == NULL) {
					addChild(cur, newnode, false);
					cur = newnode;
					break;
				}
				else {
					if (cur->parent)
						cur = cur->parent;
					else {
						freeTree(tree);
						return NULL;
					}
				}
			}
		}
		else {
			// Leaf node
			while (true)
			{
				if (cur->left == NULL) {
					receivedValue = readBits(in, 8, (bool*) chr);
					if (receivedValue != 8 && !AlreadyPrintedWarning) {
						AlreadyPrintedWarning = true;
						fprintf(stderr, "Assertion failed! Expected 8, but got %i \nMaybe your inputfile is corrupt or not suitable for this compression program.", receivedValue);
					}
					newnode = initNode(true, booltochr(chr));
					addChild(cur, newnode, true);
					break;
				}
				else if (cur->right == NULL) {
					receivedValue = readBits(in, 8, (bool*) chr);
					if (receivedValue != 8 && !AlreadyPrintedWarning) {
						AlreadyPrintedWarning = true;
						fprintf(stderr, "Assertion failed! Expected 8, but got %i \nMaybe your inputfile is corrupt or not suitable for this compression program.", receivedValue);
					}
					newnode = initNode(true, booltochr(chr));
					addChild(cur, newnode, false);
					break;
				}
				else {
					if (cur->parent)
						cur = cur->parent;
					else
						return tree;
				}
			}
		}
	}
}

/*
HELPFUNCTIONS huff_readData : do not call
*/
void _switch_buffers(bool** buffer1, bool** buffer2) {
	bool* temp = *buffer1;
	*buffer1 = *buffer2;
	*buffer2 = temp;
}

void _switch_limits(int* limit1, int* limit2) {
	int temp = *limit1;
	*limit1 = *limit2;
	*limit2 = temp; 
}

void huff_readData(huffman_tree* tree,Bitstream* in, FILE* out, int garbageSize) {
#ifdef _DEBUG
	assert(garbageSize < 8 && garbageSize >= 0);
#endif
	huffman_node* cur = tree->root;
	bool* buffer1 = malloc(sizeof(bool) * 8), *buffer2 = malloc(sizeof(bool) * 8); 
	int limit1 = 8, limit2 = 8;
	int cursor = 0; // index of next bit to be read

	bool* buffer;
	int limit;

	int gelezen = readBits(in, 8, buffer1);
	if (gelezen < 8) {
#ifdef _DEBUG
		assert(gelezen > garbageSize);
#endif
		limit1 = gelezen-garbageSize;
	}
	else {
		// There is still data left to fill in buffer 2...
		gelezen = readBits(in, 8, buffer2);
		if (gelezen < 8) {
			//We reached the EOF, so since we know this we lower the upper limit (=> var gelezen) even more.
			if (gelezen < garbageSize) {
				limit2 = 0; 
				limit1 = 8 - (garbageSize - gelezen); 
			}
			else { // garbage bits volledig in buffer 2
				limit2 = gelezen - garbageSize;
			}
		}
	}

	limit = limit1;
	buffer = buffer1;
	
#ifdef _DEBUG
	int charcounter = 0; // To jump to the nth character that will be written;
#endif
	while (!(limit<8 && limit==cursor))
	{
		if (cursor == 8) {
			gelezen = readBits(in, 8, buffer);
			if (gelezen>0 && gelezen < 8) {
				//We reached the EOF, so since we know this we lower the upper limit (=> var gelezen) even more.
				if (gelezen < garbageSize) {
					// De garbage bits bevinden zich volledig in de nieuwe buffer + een deel van de andere buffer.
					// => De zonet ingelezen buffer wordt volledig weggeggooid + een deel van de andere buffer ook.
					limit1 = 0; // -> for second buffer (because we will switch soon)
					limit2 = 8 - (garbageSize - gelezen); // for the first buffer after switch
				}
				else {
					// Alle garbage bits bevinden zich in de zonet ingelezen buffer.
					limit1 = gelezen - garbageSize; // next buffer
					//limit2 stays 8.
				}
			}
			_switch_buffers(&buffer1, &buffer2);
			buffer = buffer1;
			_switch_limits(&limit1, &limit2);
			limit = limit1;
			cursor = 0;
			continue;
		}
		bool bit = buffer[cursor];
		cursor++;
		if (bit) {
			cur = cur->right;
		}
		else {
			cur = cur->left;
		}
		if (cur->isLeaf) {
#ifdef _DEBUG
			charcounter++;
#endif
			fwrite(&cur->symbol, sizeof(char), 1, out);
			cur = tree->root;
		}
	}
	free(buffer);
}

int decode(const char* infilename, const char* outfilename) {
	// Open streams
	Bitstream *in = initBitstream(infilename, "rb");
	if (in == NULL)
		return 1;
	FILE* out = fopen(outfilename, "wb");
	if (out == NULL) {
		closeBitstream(in);
		return 1;
	}
	// read first header bit
	bool* headerBits = calloc(8,sizeof(bool));
	readBits(in, 1, headerBits);
	if (headerBits[0]) {
		rle_decode(in, out);
	}
	else {
		// Read garbage bits
		readBits(in, 3,headerBits);
		unsigned int garbageSize = booltochr(headerBits)>>5;//first 3 bits for garbagesize
		// Recreate tree
		huffman_tree* tree = huff_readTree(in);
		if (tree == NULL) {
			fclose(out);
			closeBitstream(in);
			return 2;
		}
	
		// Read prefixes (prefixes in input, characters to output)
		huff_readData(tree, in, out, garbageSize);
	}
	free(headerBits);

	// Closing the streams is our job...
	closeBitstream(in);
	fclose(out);
	return 0;
}