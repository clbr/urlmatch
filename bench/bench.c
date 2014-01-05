#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include <sys/types.h>
#include <regex.h>
#include "urlmatch.h"
#include "lrtypes.h"

static const u32 urls = 1000 * 1000;
static const u32 rules = 1500;

static const char **ruling;
static const char **urling;

static regex_t regex;

static char *genurl() {

	const u32 len = (rand() % 80) + 10;
	char *buf = calloc(len + 1, 1);

	u32 i;

	switch (rand() % 10) {
		case 0 ... 6:
			strcpy(buf, "http://");
		break;
		case 7 ... 8:
			strcpy(buf, "https://");
		break;
		case 9:
			strcpy(buf, "ftp://");
		break;
	}

	for (i = strlen(buf); i < len; i++) {
		const u32 type = rand() % 10;

		switch (type) {
			case 0 ... 6:
				buf[i] = 'a' + rand() % 26;
			break;
			case 7:
				buf[i] = '0' + rand() % 10;
			break;
			case 8 ... 9:
				buf[i] = ':' + rand() % 7;
			break;
		}
	}

	return buf;
}

static char *genrule() {

	const u32 len = (rand() % 10) + 5;
	char *buf = calloc(len + 1, 1);

	u32 i;
	for (i = 0; i < len; i++) {
		u32 type = rand() % 3;

		if (i && type == 2 && buf[i - 1] == '*')
			type = 1;

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
		//printf("Rule %u: %s\n", i, ruling[i]);
	}

	for (i = 0; i < urls; i++) {
		urling[i] = genurl();
		//printf("URL %u: %s\n", i, urling[i]);
	}
}

static void simple() {

	u32 i, j, sum = 0;
	for (i = 0; i < urls; i++) {
		for (j = 0; j < rules; j++) {
			if (url_simplematch(ruling[j], urling[i])) {
				sum++;
				break;
			}
		}
		if (i % 10000 == 0) {printf("."); fflush(stdout);}
	}
	printf("Total %u matches\n", sum);
}

static urlctx *opti_init() {

	u32 totlen = 1, i;
	for (i = 0; i < rules; i++) {
		totlen += strlen(ruling[i]) + 2;
	}

	char *tmp = calloc(totlen, 1);

	for (i = 0; i < rules; i++) {
		strcat(tmp, ruling[i]);
		strcat(tmp, "\n");
	}

	urlctx *ctx = url_init(tmp);
	free(tmp);

	return ctx;
}

static void opti(const urlctx * const ctx) {

	u32 i, j, sum = 0;
	for (i = 0; i < urls; i++) {
		if (url_match(ctx, urling[i])) {
			sum++;
		}
		if (i % 10000 == 0) {printf("."); fflush(stdout);}
	}
	printf("Total %u matches\n", sum);
}

static void reg_init() {

	u32 totlen = 1, i;
	for (i = 0; i < rules; i++) {
		totlen += strlen(ruling[i]) * 2 + 2;
	}

	char *tmp = calloc(totlen, 1);

	for (i = 0; i < rules; i++) {
		u32 j;

		for (j = 0; ruling[i][j]; j++) {
			if (ruling[i][j] != '*') {
				const char buffy[2] = { ruling[i][j], '\0' };
				strcat(tmp, buffy);
			} else {
				const char buffy[3] = ".*";
				strcat(tmp, buffy);
			}
		}

		if (i != rules - 1) strcat(tmp, "|");
	}

	for (i = 1; i < totlen; i++) {
		if (tmp[i] == '*')
			tmp[i-1] = '.';
	}

	int ret = regcomp(&regex, tmp, REG_EXTENDED | REG_NOSUB);
	if (ret) puts("Failed to compile regex");

	free(tmp);
}

static void reg() {

	u32 i;
	for (i = 0; i < urls; i++) {
		regexec(&regex, urling[i], 0, NULL, 0);

		if (i % 10000 == 0) {printf("."); fflush(stdout);}
	}
}

int main() {

	srand(42);

	printf("Generating %uk urls and %u rules.\n", urls / 1000, rules);
	gen();

	struct timeval start, end;
	u32 ms;

	printf("Starting testing.\n\n");
	gettimeofday(&start, NULL);
	simple();
	gettimeofday(&end, NULL);

	ms = (end.tv_sec - start.tv_sec) * 1000;
	ms += (end.tv_usec - start.tv_usec) / 1000;
	if (!ms) ms = 1;
	printf("Simple backend took %u ms, or %.2f checks per millisecond.\n\n",
		ms, (float) urls / ms);



	gettimeofday(&start, NULL);
	urlctx * const ctx = opti_init();
	gettimeofday(&end, NULL);

	ms = (end.tv_sec - start.tv_sec) * 1000;
	ms += (end.tv_usec - start.tv_usec) / 1000;
	if (!ms) ms = 1;
	printf("Optimized init took %u ms.\n",
		ms);



	gettimeofday(&start, NULL);
	opti(ctx);
	gettimeofday(&end, NULL);
	url_free(ctx);

	ms = end.tv_sec - start.tv_sec;
	ms += (end.tv_usec - start.tv_usec) / 1000;
	if (!ms) ms = 1;
	printf("Optimized backend took %u ms, or %.2f checks per millisecond.\n\n",
		ms, (float) urls / ms);

/*
	glibc regex uses a fuckton of RAM, causing this to die in just some dozen
	iterations. Only enable if you have a sane libc where calling regexec does
	not allocate memory.
*/
#if 0
	gettimeofday(&start, NULL);
	reg_init();
	gettimeofday(&end, NULL);

	ms = (end.tv_sec - start.tv_sec) * 1000;
	ms += (end.tv_usec - start.tv_usec) / 1000;
	if (!ms) ms = 1;
	printf("Regex init took %u ms.\n",
		ms);



	gettimeofday(&start, NULL);
	reg();
	gettimeofday(&end, NULL);

	ms = end.tv_sec - start.tv_sec;
	ms += (end.tv_usec - start.tv_usec) / 1000;
	if (!ms) ms = 1;
	printf("Regex took %u ms, or %.2f checks per millisecond.\n",
		ms, (float) urls / ms);
#endif
	return 0;
}
