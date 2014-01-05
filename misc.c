/*
    liburlmatch - a fast URL matcher
    Copyright (C) 2013 Lauri Kasanen

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License as published by
    the Free Software Foundation, version 3 of the License.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "internal.h"

u32 countwilds(const char str[]) {

	u32 sum = 0;

	const char *ptr = str;
	for (; *ptr; ptr++) {
		if (*ptr == '*') sum++;
	}

	return sum;
}

const char *strrstr(const char hay[], const char needle[]) {

	const char *next;
	next = strstr(hay, needle);
	if (!next) return NULL;

	while (1) {
		const char *prev = next;
		next = strstr(next + 1, needle);

		if (!next) return prev;
	}
}

void *xcalloc(size_t nmemb, size_t size) {

	void *tmp = calloc(nmemb, size);
	if (!tmp) die("Out of memory");

	return tmp;
}

void *xmalloc(size_t size) {

	void *tmp = malloc(size);
	if (!tmp) die("Out of memory");

	return tmp;
}

void die(const char s[]) {

	fprintf(stderr, "%s\n", s);
	exit(1);
}

void swrite(const void * const ptr, const size_t size, FILE * const stream) {

	const size_t ret = fwrite(ptr, size, 1, stream);

	if (ret != 1) die("Failed writing");
}

void sread(void * const ptr, const size_t size, FILE * const stream) {

	const size_t ret = fread(ptr, size, 1, stream);

	if (ret != 1) die("Failed reading");
}

void getsuffix(const char str[], char suf[3]) {

	const u32 len = strlen(str);
	if (len == 1) {
		suf[0] = str[0];
		suf[1] = '\0';
		return;
	}

	suf[0] = str[len - 2];
	suf[1] = str[len - 1];
	suf[2] = '\0';
}

void printctx(const struct urlctx * const ctx) {

	u16 p, s, n;
	u16 pmax, smax, nmax;

	pmax = ctx->count;

	printf("URL context has %u prefixes\n", pmax);

	for (p = 0; p < pmax; p++) {
		const struct prefix * const curpref = &ctx->pref[p];

		smax = curpref->count;
		printf("\tPrefix %u '%s' has %u suffixes\n", p, curpref->prefix, smax);

		for (s = 0; s < smax; s++) {
			const struct suffix * const cursuf = &curpref->suf[s];

			nmax = cursuf->count;
			printf("\t\tSuffix %u '%s' has %u needles\n", s, cursuf->suffix,
					nmax);

			for (n = 0; n < cursuf->count; n++) {
				const struct needle * const curneed = &cursuf->need[n];

				printf("\t\t\tNeedle %u: %s\n", n, curneed->needle);
			}
		}
	}
}
