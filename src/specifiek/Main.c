#include "../common/Bitstream.h"
#include <stdio.h>
#include <string.h>
#include "HuffmanSpecifiek.h"
#include <time.h>
// #define TEST

int main(int argc, char** argv) {
#ifdef _DEBUG
	printf("Debug Mode: Assertions (may cause crash) and Verbose\n");
#endif
#ifdef TEST
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
		code=encodeSpecifiek(file_in, file_out);
	else
		code=decodeSpecifiek(file_in, file_out);
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