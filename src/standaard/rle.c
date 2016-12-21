#include "rle.h"
#include "../common/Services.h"

void rle_encode(huffman_node * node, Bitstream * out)
{
	writeBits(out, 8, chrtobool(node->symbol));
	bool * arr = ulltobool(node->frequencies);
	writeBits(out, sizeof(unsigned long long) * 8, arr);
	free(arr);
}

void rle_decode(Bitstream * in, FILE * out)
{
	bool symbol[8];
	readBits(in, 8, (bool*)symbol);
	bool* freq = malloc(sizeof(bool) * 64);
	readBits(in, sizeof(unsigned long long) * 8, freq);
	char c = booltochr(symbol);
	for (int i = 0; i < booltoull(freq);i++)
		fwrite(&c, 1, 1, out);
	free(freq);
}
