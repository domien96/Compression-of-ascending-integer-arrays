#ifndef HUFFMAN
#define HUFFMAN
#include <stdio.h>

/*
Return values:
0 - OK
1 - Cannot open input or output file.
2 - Frequency table or Huffman tree cannot be initialized. Maybe inputfile is empty (all frequencies are 0) or is corrupt.
*/
int encode(const char* in, const char* out);

/*
Return values:
0 - OK
1 - Cannot open input or output file.
2 - Huffman tree cannot be reconstructed. Maybe inputfile is corrupt.
*/
int decode(const char* in, const char* out);
#endif