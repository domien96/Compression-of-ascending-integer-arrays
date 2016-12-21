#include "FrequencyCounter.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define BUFFERSIZE 100 // in bytes
#define minimum(a,b) (a<b?a:b)

/*
Runs and count throughtout the buffer
validlen is the length of the valid data in the buffer.
For example, if EOF is reached in between the buffer, then validlen should be set there.
*/
void countBuffer(unsigned long long* frequencies,unsigned char* buffer, size_t validlen) {
	int WARNED_ALREADY = 0; // To surpress multiple output of same warn
	unsigned char cur;
	for (int i = 0; i < minimum(BUFFERSIZE,validlen); i++) {
		cur = buffer[i];
		if (! WARNED_ALREADY && frequencies[cur] == LONG_MAX - 1) {
			WARNED_ALREADY = 1;
			printf("WARNING: Frequentie heeft de bovengrens bereikt. Mogelijks zal niet de meest optimale code opgesteld worden.\
				Gebruik kleinere files voor een crash te voorkomen.");
		}
		else
			frequencies[cur] = frequencies[cur] + 1;
	}
}

unsigned long long* init_freq(FILE* file) {
	unsigned long long* frequencies = calloc(MAX_SYMBOLS, sizeof(unsigned long long));
	if (!frequencies)
		return NULL;
	unsigned char buffer[BUFFERSIZE];
	size_t no_bytes_read;
	while ((no_bytes_read = fread(buffer, sizeof(char), BUFFERSIZE, file)) > 0) {
		//repeat
		countBuffer(frequencies, buffer, no_bytes_read);
	}
	return frequencies;
}

void free_freq(unsigned long long * f) {
	if (f)
		free(f);
}