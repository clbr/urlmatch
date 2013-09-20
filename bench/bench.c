#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <lrtypes.h>
#include "urlmatch.h"

static const u32 urls = 1000 * 1000;
static const u32 rules = 1500;

static const char **ruling;
static const char **urling;

static char *genurl() {

}

static char *genrule() {

	const u32 len = (rand() % 10) + 5;
	char *buf = calloc(len + 1, 1);

	u32 i;
	for (i = 0; i < len; i++) {
		const u32 type = rand() % 3;

		switch (type) {
			case 0:
				buf[i] = 'a' + rand() % 26;
			break;
			case 1:
				buf[i] = '0' + rand() % 10;
			break;
			case 2:
				buf[i] = '*';
			break;
		}
	}

	return buf;
}

static void gen() {
	ruling = calloc(rules, sizeof(char *));
	urling = calloc(urls, sizeof(char *));

	u32 i;
	for (i = 0; i < rules; i++) {
		ruling[i] = genrule();
	}

	for (i = 0; i < urls; i++) {
		urling[i] = genurl();
	}
}

static void simple() {

	u32 i, j;
	for (i = 0; i < urls; i++) {
		for (j = 0; j < rules; j++) {
			if (url_simplematch(ruling[j], urling[i])) break;
		}
	}
}

static void opti_init() {

}

static void opti() {

}

int main() {

	srand(time(NULL));

	printf("Generating %u urls and %u rules.\n", urls, rules);
	gen();

	struct timeval start, end;
	u32 ms;

	printf("Starting testing.\n\n");
	gettimeofday(&start, NULL);
	simple();
	gettimeofday(&end, NULL);

	ms = end.tv_sec - start.tv_sec;
	ms += (end.tv_usec - start.tv_usec) / 1000;
	if (!ms) ms = 1;
	printf("Simple backend took %u ms, or %u checks per millisecond.\n",
		ms, urls / ms);



	gettimeofday(&start, NULL);
	opti_init();
	gettimeofday(&end, NULL);

	ms = end.tv_sec - start.tv_sec;
	ms += (end.tv_usec - start.tv_usec) / 1000;
	if (!ms) ms = 1;
	printf("Optimized init took %u ms\n",
		ms);



	gettimeofday(&start, NULL);
	opti();
	gettimeofday(&end, NULL);

	ms = end.tv_sec - start.tv_sec;
	ms += (end.tv_usec - start.tv_usec) / 1000;
	if (!ms) ms = 1;
	printf("Optimized backend took %u ms, or %u checks per millisecond.\n",
		ms, urls / ms);

	return 0;
}
