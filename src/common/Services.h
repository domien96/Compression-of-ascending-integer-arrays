#ifndef SERVICES
#define SERVICES

/*
SERVICE FUNCTIONS
*/

#include <stdbool.h>
#include <stdlib.h>

/* Convert string of 0's (false) and 1's (true) to array of bool 
Remember to free array after use.
Length is amount of chararachters (without null byte).
*/
bool* strtobool(const char* binarystring, int length);

/* Convert char to array of bool using it's integer representation.
Remember to free array after use.
*/
bool* chrtobool(const char chr);

/*
Unsigned long long to bool array
Remember to free array after use.
*/
bool* ulltobool(unsigned long long l);

/*
PRECONDITION: bool array must have (minimum) 8 entries.
*/
unsigned char booltochr(const bool* bits);

/*
PRECONDITION: bool array must have (minimum) 64 entries.
*/
unsigned long long booltoull(const bool* bits);
#endif