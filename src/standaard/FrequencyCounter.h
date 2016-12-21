#ifndef FREQUENCY_COUNTER
#define FREQUENCY_COUNTER

#include <stdio.h>

#define MAX_SYMBOLS 256

/*
Count frequencies per token, returns array of counts for each token.
ascii token index is used as array index.
array has size MAX_SYMBOLS (=> last index = MAX_SYMBOLS-1)
*/
unsigned long long* init_freq(FILE* file);

void free_freq(unsigned long long* frequencies);
#endif