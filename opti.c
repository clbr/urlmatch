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
#include "urlmatch.h"
#include <zlib.h>

static urlctx *initbin(FILE * const f, const u32 inlen) {

	size_t len;
	sread(&len, sizeof(size_t), f);

	u8 * const src = xcalloc(inlen, 1);
	u8 *buf = xcalloc(len, 1);

	sread(src, inlen, f);
	if (uncompress(buf, &len, src, inlen) != Z_OK) return NULL;
	free(src);

	// Cool, unpacked. Read it.
	urlctx * const out = xcalloc(sizeof(urlctx), 1);

	memcpy(&out->count, buf, 2);
	buf += 2;

	out->pref = xcalloc(sizeof(struct prefix), out->count);
	u32 p, s, n;

	for (p = 0; p < out->count; p++) {
		struct prefix * const curpref = &out->pref[p];

		memcpy(&curpref->count, buf, 2);
		buf += 2;
		memcpy(curpref->prefix, buf, 6);
		buf += 6;

		curpref->suf = xcalloc(sizeof(struct suffix), curpref->count);

		for (s = 0; s < curpref->count; s++) {
			struct suffix * const cursuf = &curpref->suf[s];

			memcpy(&cursuf->count, buf, 2);
			buf += 2;
			memcpy(cursuf->suffix, buf, 3);
			buf += 3;

			cursuf->need = xcalloc(sizeof(struct needle), cursuf->count);

			for (n = 0; n < cursuf->count; n++) {
				struct needle * const curneed = &cursuf->need[n];

				memcpy(&curneed->len, buf, 2);
				buf += 2;
				memcpy(&curneed->wilds, buf, 2);
				buf += 2;

				curneed->needle = xcalloc(curneed->len + 1, 1);

				memcpy((char *) curneed->needle, buf, curneed->len + 1);
				buf += curneed->len + 1;
			}
		}
	}

	free(buf);
	return out;
}

urlctx *url_init_file(const char file[]) {

	FILE * const f = fopen(file, "r");
	if (!f) return NULL;

	fseek(f, 0, SEEK_END);
	const long len = ftell(f);
	rewind(f);

	char buf[4] = { 0 };
	fread(buf, 3, 1, f);

	urlctx *out = NULL;

	// Binary format
	if (!strcmp(buf, MAGIC)) {
		out = initbin(f, len - 7);
	} else { // Text format
		rewind(f);

		char *tmp = xcalloc(len, 1);
		if (fread(tmp, len, 1, f) != 1) die("Failed reading");
		out = url_init(tmp);
		free(tmp);
	}

	fclose(f);
	return out;
}

static int cstrcmp(const void * const p1, const void * const p2) {

	const char * const a = * (char * const *) p1;
	const char * const b = * (char * const *) p2;

	int ret = strncmp(a, b, 5);
	if (ret) return ret;

	// Secondary sort by the suffix
	char sufa[3] = { 0 };
	char sufb[3] = { 0 };

	const u32 alen = strlen(a);
	const u32 blen = strlen(b);

	sufa[1] = a[alen - 1];
	sufa[0] = a[alen - 2];
	sufb[1] = b[blen - 1];
	sufb[0] = b[blen - 2];

	return strcmp(sufa, sufb);
}

urlctx *url_init(const char contents[]) {

	u32 lines = 1;
	const char *ptr = contents;
	for (; *ptr; ptr++) {
		if (*ptr == '\n') lines++;
	}

	char **outlines = xcalloc(lines, sizeof(char *));

	// Copy each pattern line to its own space, and optimize on the way
	ptr = contents;
	u32 i = 0;
	while (1) {
		const char * const end = strchrnul(ptr, '\n');
		const u32 len = end - ptr;

		if (len < 2) {
			ptr = end + 1;
			continue;
		}

		char tmp[len + 1];
		tmp[len] = '\0';
		memcpy(tmp, ptr, len);

		outlines[i] = xcalloc(len + 1, 1);

		u32 p, o;
		outlines[i][0] = tmp[0];
		for (p = 1, o = 1; p < len; p++) {
			if (tmp[p - 1] == '*' && tmp[p] == '*') {
				continue;
			}
			outlines[i][o] = tmp[p];

			o++;
		}

		if (!*end) break;
		ptr = end + 1;
		i++;
	}

	lines = i + 1;
	qsort(outlines, lines, sizeof(char *), cstrcmp);

	urlctx * const out = xcalloc(sizeof(urlctx), 1);

	free(outlines);
	return out;
}

int url_save_optimized(const urlctx *ctx, const char file[]) {

	char *buf;
	size_t len;

	FILE *f = open_memstream(&buf, &len);
	if (!f) return 1;

	swrite(&ctx->count, 2, f);

	u32 p, s, n;
	for (p = 0; p < ctx->count; p++) {
		const struct prefix * const curpref = &ctx->pref[p];
		swrite(&curpref->count, 2, f);
		swrite(curpref->prefix, 6, f);

		for (s = 0; s < curpref->count; s++) {
			const struct suffix * const cursuf = &curpref->suf[s];
			swrite(&cursuf->count, 2, f);
			swrite(cursuf->suffix, 3, f);

			for (n = 0; n < cursuf->count; n++) {
				const struct needle * const curneed = &cursuf->need[n];
				swrite(&curneed->len, 2, f);
				swrite(&curneed->wilds, 2, f);
				swrite(curneed->needle, curneed->len + 1, f);
			}
		}
	}

	fclose(f);

	// Cool, a buffer. Let's compress it.
	u64 bound = compressBound(len);
	u8 *dest = xcalloc(bound, 1);
	if (compress2(dest, &bound, (u8 *) buf, len, 9) != Z_OK) return 2;

	free(buf);

	f = fopen(file, "w");
	if (!f) return 1;

	swrite(MAGIC, 3, f);
	swrite(&len, sizeof(size_t), f);
	swrite(dest, bound, f);

	free(dest);
	fclose(f);
	return 0;
}

int url_match(const urlctx *ctx, const char haystack[]) {

}

void url_free(urlctx *ctx) {

	u32 p, s, n;
	for (p = 0; p < ctx->count; p++) {
		struct prefix * const curpref = &ctx->pref[p];

		for (s = 0; s < curpref->count; s++) {
			struct suffix * const cursuf = &curpref->suf[s];

			for (n = 0; n < cursuf->count; n++) {
				free((char *) cursuf->need[n].needle);
			}

			free(cursuf->need);
		}

		free(curpref->suf);
	}

	free(ctx->pref);
	free(ctx);
}
