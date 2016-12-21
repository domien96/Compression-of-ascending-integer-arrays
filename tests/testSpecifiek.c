#include <stdio.h>
#include "../src/specifiek/HuffmanSpecifiek.h"
#include <assert.h>

/*
Je kan de bestandsnamen vrij aanpassen en manueel de resultaten bekijken.
Met FC-commando in windows check je voor gelijkenis tussen origineel en decoded bestand.
*/

void testEncode(void) {
	char* input = "../Resources/invoerSpecifiek/1.txt", *output = "../Resources/uitvoerSpecifiek/1.txt";
	assert(0 == encodeSpecifiek(input, output));
	// Check manueel
}

void testEncode2(void) {
	char* input = "../Resources/invoer/TestFiles/large_numbers", *output = "../Resources/uitvoerSpecifiek/encode2.cmp";
	assert(0 == encodeSpecifiek(input, output));
	// Check manueel
}

void testDecode(void) {
	char* output = "../Resources/uitvoerSpecifiek/1-dec.txt", *input = "../Resources/uitvoerSpecifiek/1.txt";
	assert(0 == decodeSpecifiek(input, output));
	// Check manueel
}

void testDecode2(void) {
	char* output = "../Resources/uitvoerSpecifiek/encode2-dec.txt", *input = "../Resources/uitvoerSpecifiek/encode2.cmp";
	assert(0 == decodeSpecifiek(input, output));
	// Check manueel
}

void testFull(void) {
	// getest voor alle small, medium, large, en alle andere files van dit formaat in de map invoer en invoer/Testfiles.
	// Vergeleken met elkaar in Notepad ++ .
	char* input = "../Resources/invoer/TestFiles/large_numbers", *output = "../Resources/uitvoerSpecifiek/output";
	char* med= "../Resources/uitvoerSpecifiek/output.cmp";
	assert(0==encodeSpecifiek(input, med));
	assert(0==decodeSpecifiek(med, output));
}

int testmain(void) {
	testEncode2();
	testDecode2();
	//testFull();
	return 0;
}