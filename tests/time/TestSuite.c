#pragma warning(disable:4996)

#include "TestSuite.h"
#include <assert.h>

long long get_file_size(FILE* fp) {
	fseek(fp, 0L, SEEK_END);
	return ftell(fp);
}
void test() {
	int number_of_files = 16;

	//Files to compress
	char *files[16];
	files[0] = "..\\Resources\\invoer\\TestFiles\\small_consecutive";
	files[1] = "..\\Resources\\invoer\\TestFiles\\medium_consecutive";
	files[2] = "..\\Resources\\invoer\\TestFiles\\large_consecutive";
	files[3] = "..\\Resources\\invoer\\TestFiles\\small_big_jumps";
	files[4] = "..\\Resources\\invoer\\TestFiles\\medium_big_jumps";
	files[5] = "..\\Resources\\invoer\\TestFiles\\big_big_jumps";
	files[6] = "..\\Resources\\invoer\\TestFiles\\small_small_jumps";
	files[7] = "..\\Resources\\invoer\\TestFiles\\medium_small_jumps";
	files[8] = "..\\Resources\\invoer\\TestFiles\\big_small_jumps";
	files[9] = "..\\Resources\\invoer\\TestFiles\\small_random";
	files[10] = "..\\Resources\\invoer\\TestFiles\\medium_random";
	files[11] = "..\\Resources\\invoer\\TestFiles\\big_random";
	files[12] = "..\\Resources\\invoer\\TestFiles\\small_numbers";
	files[13] = "..\\Resources\\invoer\\TestFiles\\large_numbers";
	files[14] = "..\\Resources\\invoer\\medium.txt";
	files[15] = "..\\Resources\\invoer\\small.txt";

	//Results from compression
	char *output[16];
	output[0] = "..\\Resources\\invoer\\TestFiles\\small_consecutive_o";
	output[1] = "..\\Resources\\invoer\\TestFiles\\medium_consecutive_o";
	output[2] = "..\\Resources\\invoer\\TestFiles\\large_consecutive_o";
	output[3] = "..\\Resources\\invoer\\TestFiles\\small_big_jumps_o";
	output[4] = "..\\Resources\\invoer\\TestFiles\\medium_big_jumps_o";
	output[5] = "..\\Resources\\invoer\\TestFiles\\big_big_jumps_o";
	output[6] = "..\\Resources\\invoer\\TestFiles\\small_small_jumps_o";
	output[7] = "..\\Resources\\invoer\\TestFiles\\medium_small_jumps_o";
	output[8] = "..\\Resources\\invoer\\TestFiles\\big_small_jumps_o";
	output[9] = "..\\Resources\\invoer\\TestFiles\\small_random_o";
	output[10] = "..\\Resources\\invoer\\TestFiles\\medium_random_o";
	output[11] = "..\\Resources\\invoer\\TestFiles\\big_random_o";
	output[12] = "..\\Resources\\invoer\\TestFiles\\small_numbers_o";
	output[13] = "..\\Resources\\invoer\\TestFiles\\large_numbers_o";
	files[14] = "..\\Resources\\invoer\\medium.cmp";
	files[15] = "..\\Resources\\invoer\\small.cmp";

	//Results from decompression
	char *decomp[16];
	decomp[0] = "..\\Resources\\invoer\\TestFiles\\small_consecutive_d";
	decomp[1] = "..\\Resources\\invoer\\TestFiles\\medium_consecutive_d";
	decomp[2] = "..\\Resources\\invoer\\TestFiles\\large_consecutive_d";
	decomp[3] = "..\\Resources\\invoer\\TestFiles\\small_big_jumps_d";
	decomp[4] = "..\\Resources\\invoer\\TestFiles\\medium_big_jumps_d";
	decomp[5] = "..\\Resources\\invoer\\TestFiles\\big_big_jumps_d";
	decomp[6] = "..\\Resources\\invoer\\TestFiles\\small_small_jumps_d";
	decomp[7] = "..\\Resources\\invoer\\TestFiles\\medium_small_jumps_d";
	decomp[8] = "..\\Resources\\invoer\\TestFiles\\big_small_jumps_d";
	decomp[9] = "..\\Resources\\invoer\\TestFiles\\small_random_d";
	decomp[10] = "..\\Resources\\invoer\\TestFiles\\medium_random_d";
	decomp[11] = "..\\Resources\\invoer\\TestFiles\\big_random_d";
	decomp[12] = "..\\Resources\\invoer\\TestFiles\\small_numbers_d";
	decomp[13] = "..\\Resources\\invoer\\TestFiles\\large_numbers_d";
	files[14] = "..\\Resources\\invoer\\medium-dec.txt";
	files[15] = "..\\Resources\\invoer\\small-dec.txt";

	double compr_times[14][10];
	double decompr_times[14][10];

	//Compress each file 10 times and count the time the cpu needs
	for (int i = 0; i < number_of_files; i++) {
		for (int j = 0; j < 10; j++) {
			clock_t begin = clock();
			
			encodeSpecifiek(files[i],output[i]);
			
			
			clock_t end = clock();
			double cpu_time = (double)(end - begin) / CLOCKS_PER_SEC;
			compr_times[i][j] = cpu_time;
			FILE* in = fopen(files[i], "rb");
			FILE* out = fopen(output[i], "rb");
			assert(in != NULL && out != NULL);
			if (j == 0) {
				long long original_size = get_file_size(in);
				long long compressed_size = get_file_size(out);
				printf("Compression-factor for file%d: %lf\n", i, (double)compressed_size / original_size);
			}
			fclose(in);
			fclose(out);
		}
	}

	//Print the average cpu_time for each file for compression
	for (int i = 0; i < number_of_files; i++) {
		double total = compr_times[i][0];
		for (int j = 1; j < 10; j++) {
			total += compr_times[i][j];
		}
		printf("File %d average compression time: %lf\n", i, total / 10.0);
	}

	//Decompress each file 10 times and count the time the cpu needs
	for (int i = 0; i < number_of_files; i++) {
		for (int j = 0; j < 10; j++) {
			clock_t begin = clock();
			decode(output[i], decomp[i]);
			clock_t end = clock();
			double cpu_time = (double)(end - begin) / CLOCKS_PER_SEC;
			decompr_times[i][j] = cpu_time;
		}
		printf("Finished file%d\n", i);
	}

	//Print the average cpu_time for each file for decompression
	for (int i = 0; i < number_of_files; i++) {
		double total = decompr_times[i][0];
		for (int j = 1; j < 10; j++) {
			total += decompr_times[i][j];
		}
		printf("File %d average decompression time: %lf\n", i, total / 10.0);
	}
}
