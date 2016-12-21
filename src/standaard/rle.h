#ifndef RLE
#define RLE
#include "../standaard/huffman_node.h"
#include "../common/Bitstream.h"

/*
Encodes node by following format:
writes symbol (1 byte)
writes amount of occurences using the frequency field of the node (8 bytes)
*/
void rle_encode(huffman_node* node, Bitstream* out);


/*
Decodes using the encoding format.
Reads the next 9 bytes.
*/
void rle_decode(Bitstream* in, FILE* out);
#endif