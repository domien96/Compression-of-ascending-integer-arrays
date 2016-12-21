#include "../common/Bitstream.h"
#include <stdio.h>
#include <string.h>
#include "Huffman.h"
#include <time.h>
//#define TEST

void main1(void) {
	char* in = "Resources\\invoer\\test1.txt", *out = "Resources\\uitvoer\\test1-out.txt";
	encode(in, out);
}

int main(int argc, char** argv) {
#ifdef _DEBUG
	printf("Debug Mode: Assertions (may cause crash) and Verbose\n");
#endif
#ifdef TEST
	int x = 0;
	if (x)
		main1();
	else
		testmain();
#else
	if (argc != 4) {
		printf("Usage: %s [-d|-c] [inputname] [outputname]",argv[0]);
		return 1;
	}	

	char *c = "-c";
	bool encodeMode = strcmp(argv[1],c)==0;
	const char* file_in = argv[2], *file_out = argv[3];
	//clock_t begin = clock();
	int code=0;
	if (encodeMode)
		code=encode(file_in, file_out);
	else
		code=decode(file_in, file_out);
	//clock_t end = clock();
	//double cpu_time = (double)(end - begin) / CLOCKS_PER_SEC;
	switch (code) {
	case 0:
		printf("OK\n");
		return 0;
	case 1:
		printf("Cannot open input or output file.\n");
		break;
	case 2:
		if (encodeMode)
			printf("Frequency table or Huffman tree cannot be initialized.Maybe inputfile is empty(all frequencies are 0) or is corrupt.\n");
		else
			printf("Huffman tree cannot be reconstructed. Maybe inputfile is corrupt.\n");
		break;
	default:
		printf("Unrecognized returning code.\n");
	}
	return code;
#endif
}