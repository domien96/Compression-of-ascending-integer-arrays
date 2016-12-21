#ifndef BITSTREAM
#define BITSTREAM
#include <stdio.h>
#include <stdbool.h>

/********************
Writes or reads data with bit precision.
*********************/
typedef struct {
	FILE* file;
	unsigned char buffer; // buffer of 8 bits
	int cursor; //1->8,  amount of bits that -can be read from-/-are al ready Written- in the buffer.
} Bitstream;

/*
Return NULL if initialization not succesfull or File-pointer is NULL.
*/
Bitstream* initBitstream(const char* filename, const char* mode);

/*
Flushes buffer and closes stream.
*/
void closeBitstream(Bitstream* bs);

/*write
returns bits written,
parameters: true->1 , false->0
CAUTION: bits may not effictivly been written after this function, they may still be in buffer.
Close the stream to flush all.
*/
int writeBits(Bitstream* bs, int amount, bool* codeword);

/*
true ->1
false ->0
Returns amount of bits read, this can be at most the value of the "amount"-argument.
*/
int readBits(Bitstream* bs, int amount, bool* buffer);

/*
Fills in >>>remaining bits<<< (bits after the cursors location) from buffer with garbage and flushes it.
Also all previous written bits (before the bits in the current buffer) will be flushed.
So this function guarantees all bits are persistent, but caution for the garbage bits: 
If amount of written bits isn't a multiple of 8 garbage will be added until amount is multiple of 8.

returns amount of garbage bits
*/
int flushBits(Bitstream*);

/*
Bits still in Buffer will be flushed at end.
Then the stream will be rewinded to the beginning of the file.
Returns amount of garbage bits added before effectively rewinding.
*/
int rewindBitstream(Bitstream*);
#endif