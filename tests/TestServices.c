#include <stdbool.h>
#include <assert.h>
#include "../src/standaard/FrequencyCounter.h"
#include "../src/standaard/huffman_node.h"
#include "../src/standaard/huffman_tree.h"
#include <stdlib.h>
#include "../src/standaard/huffman_code_tag.h"
#include "../src/common/Services.h"

bool compareBoolArrays(bool* a, bool* b, int length) {
	for (int i = 0; i < length; i++) {
		if (a[i] != b[i])
			return false;
	}
	return true;
}

void cmpFiles(const char* fname_one, const char* fname_two) {
	unsigned char c1, c2;
	FILE *fone = fopen(fname_one, "r"), *ftwo = fopen(fname_two, "r");
	assert(fone && ftwo);
	int readone=fread(&c1,1,1,fone), readtwo=fread(&c2,1,1,ftwo);
	int index = 0;
	while (readone && readtwo) {
		if (c1 != c2) {
			printf("%i %c %c\n", index, c1, c2);
			assert( false);
		}
		index++;
		readone = fread(&c1, 1, 1, fone);
		readtwo = fread(&c2, 1, 1, ftwo);
	}
	if (readone != readtwo) {
		printf("%i %i %i\n", index, readone, readtwo);
		assert(readone == readtwo);
	}
}

void test_chrtobool() {
	const char e = 'e', f = 'f', two = '2';
	bool be[8] = { false,true,true,false,false,true,false,true },
		bf[8] = { false,true,true,false,false,true,true,false },
		btwo[8] = { false,false,true,true,false,false,true,false };
	bool* res = chrtobool(e);
	assert(compareBoolArrays(res, &be[0],8));
	free(res);
	res = chrtobool(f);
	assert(compareBoolArrays(res, &bf[0],8));
	free(res);
	res = chrtobool(two);
	assert(compareBoolArrays(res, &btwo[0],8));
	free(res);
}

void test_strtobool() {
	const char* a = "101110011";
	bool ba[9] = { true,false,true,true,true,false,false,true,true };
	bool* res = strtobool(a, 9);
	assert(compareBoolArrays(res, &ba[0], 9));
	free(res);
}

void test_Countfreq() {
	char* f = "Resources/invoer/test1.txt";
	FILE* fp = fopen(f, "r");
	assert(fp);
	unsigned long long* freqs = init_freq(fp);
	// expected frequencies a->3 b->1 c->1
	assert(freqs[97] == 3);
	assert(freqs[98] == 1);
	assert(freqs[99] == 1);
	free_freq(freqs);
	fclose(fp);
}

void test_decode1() {
	return;
	char* in = "Resources\\invoer\\test1.compressed", *out = "Resources\\uitvoer\\test1-decmp.txt";
	assert(decode(in, out)==0);
}

void test_2() {
	// For decoding and readData(), only first buffer will have the data and EOF (gabrage = 0)
	char* ein = "Resources\\invoer\\test2.txt", *eout = "Resources\\uitvoer\\test2.cmp";
	assert(encode(ein, eout) == 0);
	char* dout = "Resources\\uitvoer\\test2.dec";
	assert(decode(eout, dout) == 0);
	(cmpFiles(ein, dout));
}

void test_3() {
	// For decoding @ readData, secondbuffer contains EOF, but garbage is in both buffers (garbage=6)
	char* ein = "Resources\\invoer\\test3.txt", *eout = "Resources\\uitvoer\\test3.cmp";
	assert(encode(ein, eout) == 0);
	char* dout = "Resources\\uitvoer\\test3.dec";
	assert(decode(eout, dout) == 0);
	(cmpFiles(ein, dout));
}

void test_4() {
	// For decoding @ readData, second buffer contains EOF, and garbage is in second buffer only. (gabage = 5)
	char* ein = "Resources\\invoer\\test4.txt", *eout = "Resources\\uitvoer\\test4.cmp";
	assert(encode(ein, eout) == 0);
	char* dout = "Resources\\uitvoer\\test4.dec";
	assert(decode(eout, dout) == 0);
	(cmpFiles(ein, dout));
}

void test_supermini() {
	char* ein = "Resources\\invoer\\supermini.txt", *eout = "Resources\\uitvoer\\supermini.cmp";
	assert(encode(ein, eout) == 0);
	char* dout = "Resources\\uitvoer\\supermini.dec";
	assert(decode(eout, dout) == 0);
	(cmpFiles(ein, dout));
}

void test_mini() {
	char* ein = "Resources\\invoer\\mini.txt", *eout = "Resources\\uitvoer\\mini.cmp";
	assert(encode(ein, eout) == 0);
	char* dout = "Resources\\uitvoer\\mini.dec";
	assert(decode(eout, dout) == 0);
	(cmpFiles(ein, dout));
}

void test_small() {
	char* ein = "Resources\\invoer\\small.txt", *eout = "Resources\\uitvoer\\small.cmp";
	assert(encode(ein, eout) == 0);
	char* dout = "Resources\\uitvoer\\small.dec";
	assert(decode(eout, dout) == 0);
	(cmpFiles(ein, dout));
}

void test_img1() {
	char* ein = "Resources\\invoer\\foto1.png", *eout = "Resources\\uitvoer\\foto1.cmp";
	assert(encode(ein, eout) == 0);
	char* dout = "Resources\\uitvoer\\foto1.dec";
	assert(decode(eout, dout) == 0);
	(cmpFiles(ein, dout));
}

void test_medium() {
	char* ein = "Resources\\invoer\\medium.txt", *eout = "Resources\\uitvoer\\medium.cmp";
	assert(encode(ein, eout) == 0);
	char* dout = "Resources\\uitvoer\\medium.dec";
	assert(decode(eout, dout) == 0);
	(cmpFiles(ein, dout));
}

void test_singleChar() {
	char* ein = "Resources\\invoer\\singleChar.txt", *eout = "Resources\\uitvoer\\singleChar.cmp";
	assert(encode(ein, eout) == 0);
	char* dout = "Resources\\uitvoer\\singleChar.dec";
	assert(decode(eout, dout) == 0);
	(cmpFiles(ein, dout));
}

void test_booltoull() {
	bool arr1[64] = { false, true, true, false, false, true, true, true, false, true, true, true, false, false, false, false, true, false, false, false, true, true, true, false, false, false, true, true, false, false, true, true, false, false, false, false, true, true, true, true, true, true, false, true, false, true, false, false, false, false, false, true, true, false, false, false, false, true, false, false, false, true, false, false };
	bool arr2[64] = { true, true, true, true, true, true, true, true, true, true, false, true, true, true, true, true, true, true, true, true, false, true, true, true, true, false, true, true, true, true, true, true, false, true, false, true, true, false, false, true, true, true, true, true, true, true, true, true, true, true, true, true, true, true, false, true, false, true, false, true, true, false, true, true };
	bool arr3[64] = { false, false, false, false, false, false, false, true, false, false, true, true, false, false, true, false, true, false, false, false, true, false, true, false, false, false, false, true, false, false, false, false, false, true, false, true, true, false, false, false, false, false, false, false, true, false, true, true, false, false, true, true, true, false, false, true, false, false, true, true, true, false, false, true };
	unsigned long long l = booltoull((bool*)arr1);
	assert(l == 7453613733258205252);
	l = booltoull((bool*)arr2);
	assert(l == 18437727800698862939);
	l = booltoull((bool*)arr3);
	assert(l == 86283145674701113);
}

int sign(int x) {
	return (x > 0) - (x < 0);
}

int nodecompareBySymbol(const void* one, const void* two) {
	return sign(((huffman_node*) one)->frequencies - ((huffman_node*) two)->frequencies);
}

int debug() {
	const char* infilename = "resources\\Screenshot.png";
	FILE* in = fopen(infilename, "rb");
	if (in == NULL) {
		return 1;
	}

	// Generate frequencies (use input)
	unsigned long long *freqs = init_freq(in); // may contain 0's frequencies
	if (freqs == NULL) {
		fclose(in);
		return 2;
	}
	// Create tree
	huffman_tree* tree = initTree_ByFrequencies(freqs);
	if (tree == NULL) {
		fclose(in);
		return 2;
	}
	free_freq(freqs);
	charToCodeMap* m = init_charToCodeMap();
	fill_charToCodeMap(m, tree);
	int j = 0;
	while (j < 256) {
		printf("eigenSymbol: %i Freqlen: %i\n", j, (*m)[j]->code_length);
		j++;
	}

	/*huffman_tree_iterator* iter = init_iterator(tree);
	huffman_node* cur= iter_next(iter);
	huffman_node** list = malloc(sizeof(huffman_node*) * 256);
	int i = 0;
	while (cur != NULL) {
		if (cur->isLeaf)
			list[i++] = cur;
		cur = iter_next(iter);
	}
	qsort(list, i, sizeof(huffman_node*), nodecompareBySymbol);
	int j = 0;
	while (j < i) {
		cur = list[j++];
		printf("Symbol: %i Freq: %i\n", cur->symbol, cur->frequencies);
	}
	free(list);
	free_iterator(iter);*/
	freeTree(tree);
	return 0;
}

int testmain() {
	//debug();
	//tests
	test_booltoull();
	test_chrtobool();
	test_strtobool();
	test_Countfreq();
	test_decode1();
	test_2();
	test_3();
	test_4();
	test_supermini();
	test_mini();
	test_small();
	test_img1();
	test_medium();
	test_singleChar();
	printf("Tests succeeded\n");
	return 0;
}