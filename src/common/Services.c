/*
SERVICE FUNCTIONS IMPLEMENTATION
*/

#include "Services.h"

bool* strtobool(const char* binarystring, int length) {
	bool* res = (bool*)malloc(length*sizeof(bool));
	for (int i = 0; i<length; ++i) {
		res[i] = binarystring[i] == '1';
	}
	return res;
}

bool* chrtobool(const char chr) {
	bool* res = (bool*)malloc(8 * sizeof(bool));
	int mask = 0b10000000;
	for (int i = 0; i<8; ++i) {
		res[i] = (chr & mask) > 0;
		mask >>= 1;
	}
	return res;
}

bool * ulltobool(unsigned long long l)
{
	bool* res = (bool*)malloc(64 * sizeof(bool));
	unsigned long long mask = 0b1000000000000000000000000000000000000000000000000000000000000000;
	for (int i = 0; i<64; i++) {
		res[i] = (l & mask) > 0;
		mask >>= 1;
	}
	return res;
}

unsigned char booltochr(const bool* bits) {
	unsigned char res = 0;
	for (int i = 0; i<8; ++i) {
		if (bits[i])
			res = res | 1 << (7 - i);
	}
	return res;
}

unsigned long long booltoull(const bool * bits)
{
	unsigned long long res = 0;
	for (int i = 0; i <= 56; i += 8) {
		res += ((unsigned long long) booltochr(bits + i)) << (56-i);
	}
	return res;
}
