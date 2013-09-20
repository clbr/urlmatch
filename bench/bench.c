#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <lrtypes.h>
#include "urlmatch.h"

const u32 urls = 1000 * 1000;
const u32 rules = 1500;

static void gen() {

}

static void simple() {

}

static void opti() {

}

int main() {

	srand(time(NULL));

	printf("Generating %u urls and %u rules.\n", urls, rules);
	gen();

	struct timeval start, end;
	u32 ms;

	printf("Starting testing.\n");
	gettimeofday(&start, NULL);
	simple();
	gettimeofday(&end, NULL);

	ms = end.tv_sec - start.tv_sec;
	ms += (end.tv_usec - start.tv_usec) * 1000;
	printf("Simple backend took %u ms, or %u checks per millisecond.\n",
		ms, urls / ms);

	gettimeofday(&start, NULL);
	opti();
	gettimeofday(&end, NULL);

	ms = end.tv_sec - start.tv_sec;
	ms += (end.tv_usec - start.tv_usec) * 1000;
	printf("Simple backend took %u ms, or %u checks per millisecond.\n",
		ms, urls / ms);

	return 0;
}
