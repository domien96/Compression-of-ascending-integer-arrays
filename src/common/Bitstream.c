#include <stdlib.h>
#include <stdint.h>
#include "Bitstream.h"

Bitstream* initBitstream(const char* filename, const char* mode) {
	Bitstream* bs = (Bitstream*)malloc(sizeof(Bitstream));
	FILE* file = fopen(filename, mode);
	if (!bs || !file)
		return NULL;
	bs->file = file;
	bs->cursor = 0;
	bs->buffer = 0;
	return bs;
};

/*
Flushes buffer and closes stream.
*/
void closeBitstream(Bitstream* bs) {
	flushBits(bs);
	fclose(bs->file);
	free(bs);
};

/*
Writes the buffer and resets the cursor and the buffer to 0.
Returns amount of garbage bits added.
*/
int writeBuffer(Bitstream* bs) {
	int res = 8 - bs->cursor;
	fwrite(&bs->buffer, 1, 1, bs->file);
	bs->cursor = 0;
	bs->buffer = 0;
	return res;
}

/*write
returns bits written,
parameters: true->1 , false->0
CAUTION: bits may not effictivly been written after this function, they may still be in buffer.
Close the stream to flush all.
*/
int writeBits(Bitstream* bs, int amount, bool* codeword) {
	int i, shift;

	for (i = 0; i < amount; i++)
	{
		if (bs->cursor == 8) {
			writeBuffer(bs);
		}

		shift = 8 - bs->cursor - 1;
		if (codeword[i])
			bs->buffer |= (1 << shift);
		else
			bs->buffer &= ~(1 << shift);

		(bs->cursor)++;
	}
	return amount;
}

/*
true ->1
false ->0
return amount of btis read (this is maximum the given value/amount argument).
*/
int readBits(Bitstream* bs, int amount, bool* result) {
	uint8_t mask;
	bool curbit;
	int amountRead=0;
	for (int i = 0; i < amount; i++) {
		if (bs->cursor == 0) {
			//read next buffer
			bs->cursor = 7;
			amountRead = fread(&bs->buffer, 1, 1, bs->file);
			if (amountRead == 0) {
				bs->cursor = 0;
				return i;
			}
		}
		else
			bs->cursor--;
		
		mask = 1 << bs->cursor;
		curbit = mask & bs->buffer;
		result[i] = curbit;
	}
	return amount;
}

int flushBits(Bitstream* bs) {
	int res=0;
	if (!(bs->cursor == 0))
		//following function also resets cursor, which means everything after cursor is garbage.
		res = writeBuffer(bs);
	fflush(bs->file);
	return res;
}

int rewindBitstream(Bitstream* bs) {
	int garbage = flushBits(bs);
	rewind(bs->file);
	return garbage;
}