
#include <stdio.h>
#include <time.h> 
#include <string.h>

#include "sha256.h"

const int iterations = 10000000;
const int passes = 5;


void sha256_consume() {
	BYTE text[] = {"@friedkiwi's quick and dirty CPU benchmark"};
	BYTE buf[SHA256_BLOCK_SIZE];
	SHA256_CTX ctx;
	int i = 0;

	for (i = 0; i < iterations; i++)
	{
		sha256_init(&ctx);
		sha256_update(&ctx, text, strlen(text));
		sha256_final(&ctx, buf);
	}
}

double pass() {
	struct timespec start, stop;

	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	sha256_consume();
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &stop);

	double result = (stop.tv_sec - start.tv_sec) * 1e6 + (stop.tv_nsec - start.tv_nsec) / 1e3;    // in microseconds
	
	return (result / 1000);
}


void bench() {
	printf("\nRunning %d passes...\n", passes);
	double min = 9999999999999;
	double max = 0;
	double total = 0;
	int i = 0;

	for (i = 0; i < passes; i++)
	{
		double pass_res = pass();
		if (min > pass_res) min = pass_res;
		if (max < pass_res) max = pass_res;
		total += pass_res;

		printf("Pass %d: %f\n", i, pass_res);
	}

	printf("pass:%d min: %f max: %f avg: %f\n", passes, min, max, (total/passes));
}

void print_comparison_results() {
	printf("Results for comparison:\n");
	printf("(less is better)\n");
	printf("* MacBookPro13,2  - 2.9GHz i5 - pass:5 min: 11723.954000 max: 12397.721000 avg: 12036.680800\n");
	printf("* Power7 AIX LPAR             - pass:5 min: 37090.000000 max: 37200.000000 avg: 37126.000000\n");
}

int main() {
	printf("Simple SHA-256 based CPU benchmark\n");
	printf("Written by @friedkiwi with code from github.com/B-Con/crypto-algorithms\n\n");

	print_comparison_results();

	bench();
	return 0;
}
