#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include "HuffmanSpecifiek.h"
#include "../standaard/Huffman.h"

#define tempFilename "@@@tijdelijk_bestand_voor_huffman_specifiek.tmp"

/* Gebruikt om eens te vergelijken. De decode als char wordt nog NIET ondersteund, want ik ben enkel geintresseerd in compressiefactor.
*  Ik weet toch dat het mogelijk is om te decoden als stringrepresentatie, zolang er een bv. komma meegeschreven wordt als scheidingsteken.
*  Verder heb ik geen behoefte aan code voor het decoden, want de andere optie presteert beter.
*/
#define WRITE_DIFF_AS_STRING

int encodeSpecifiek(const char* infilename, const char* outfilename) {
	// Open streams
	FILE* in = fopen(infilename, "rb");
	if (in == NULL)
		return 1;
	FILE *out = fopen(&(tempFilename[0]), "wb");
	if (out == NULL) {
		fclose(in);
		return 1;
	}

	char buffer[BUFSIZ];
	fread(buffer, sizeof(char), 1, in);
	size_t no_read = fread(buffer, sizeof(char), BUFSIZ, in);
	unsigned long long oldNum = 0, num = 0;
	while (no_read)
	{
		for (int i = 0; i < no_read; i++)  {
			if (isdigit(buffer[i]))
				num = num * 10 + buffer[i]-'0';
			else {
				unsigned long long diff = num - oldNum;
#ifdef WRITE_DIFF_AS_STRING
				char alsString[25];
				int amount = sprintf(&alsString[0], "%llu", diff);
				alsString[amount] = ',';
				fwrite(&alsString[0], sizeof(char), amount+1, out);
#else
				fwrite(&diff, sizeof(unsigned long long), 1, out);
#endif
				oldNum = num; num = 0;
			}
		}
		no_read = fread(buffer, sizeof(char), BUFSIZ, in);
	}
	// Close streams
	fclose(in);
	fclose(out);

	// Let do the general algorithm do the further work
	int returncode = encode(&(tempFilename[0]), outfilename);
	remove(&(tempFilename[0]));
	return returncode;
}


/**
arr[0] is meest rechtse cijfer.
*/
void convertToCharRepresentation(int* length, unsigned char *arr, unsigned long long a)
{
	*length = 0;
	while (a != 0) {
		int cijfer = a % 10;
		a /= 10;
		arr[*length] = '0' + cijfer;
		*length = *length + 1;
	}
}

void reverseOrderArray(int* length, unsigned char *a)
{
	unsigned char temp;
	for (int i = 0; i < *length / 2; i++) {
		temp = a[i];
		a[i] = a[*length - 1 - i];
		a[*length - 1 - i] = temp;
	}
}

int decodeSpecifiek(const char* infilename, const char* outfilename) {
	// Let the general algorithm decode the file with differences for us first.
	int returncode = decode(infilename, &(tempFilename[0]));
	if (returncode)
		return returncode;
	// Open streams
	FILE* in = fopen(&(tempFilename[0]), "rb");
	if (in == NULL)
		return 1;
	FILE *out = fopen(outfilename, "w");
	if (out == NULL) {
		fclose(in);
		return 1;
	}
	// Declare used variables
	unsigned long long currentNumber = 0;
	unsigned char voorstelling[22]; // ruim voldoende grootte
	int length = 0;
	unsigned long long prevnumber=0, number=0;
	// Write opening bracket
	fwrite(&"[",sizeof(char),1,out);
	// Read all differences
	size_t no_read;
#ifdef WRITE_DIFF_AS_STRING
	char buffer[BUFSIZ];
	no_read = fread(&buffer, sizeof(char), BUFSIZ, in);
	while (no_read)
	{
		for (int i = 0; i < no_read; i++) {
			if (isdigit(buffer[i]))
				currentNumber = currentNumber * 10 + buffer[i] - '0';
			else {
				number = currentNumber + prevnumber;
				// Write number
				convertToCharRepresentation(&length, &voorstelling[0], number);
				reverseOrderArray(&length, &voorstelling[0]);
				
				voorstelling[length++] = ',';
				fwrite(voorstelling, sizeof(char), length, out);

				length = 0;
				prevnumber = number; currentNumber = 0;
			}
		}
		no_read = fread(buffer, sizeof(char), BUFSIZ, in);
	}
	// Last comma is too much.
	fseek(out, -1, SEEK_CUR);
#else
	no_read = fread(&currentNumber, sizeof(long long), 1, in);
	while (no_read != 0)
	{
		// these numbers are differences, calculate original number
		number = currentNumber + prevnumber;

		// Write number
		convertToCharRepresentation(&length, &voorstelling[0], number);
		reverseOrderArray(&length, &voorstelling[0]);
		// Read next number
		no_read = fread(&currentNumber, sizeof(long long), 1, in);
		// Write seperating comma if not last number
		if (no_read != 0) {
			voorstelling[length] = ',';
			length++;
		}
		fwrite(voorstelling, sizeof(char), length, out);
		length = 0;
		prevnumber = number;
	}
#endif
	
	// Write closing bracket
	fwrite(&"]", sizeof(char), 1, out);
	// Close streams
	fclose(in);
	fclose(out);
	remove(&(tempFilename[0]));
	return 0;
}