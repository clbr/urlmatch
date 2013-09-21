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

static urlctx *initbin(FILE * const f) {

}

urlctx *url_init_file(const char file[]) {

	FILE * const f = fopen(file, "r");
	if (!f) return NULL;

	char buf[4] = { 0 };
	fread(buf, 3, 1, f);

	urlctx *out = NULL;

	// Binary format
	if (!strcmp(buf, MAGIC)) {
		out = initbin(f);
	} else { // Text format
		rewind(f);
		fseek(f, 0, SEEK_END);
		const long len = ftell(f);
		rewind(f);

		char *tmp = xcalloc(len, 1);
		if (fread(tmp, len, 1, f) != 1) die("Failed reading");
		out = url_init(tmp);
		free(tmp);
	}

	fclose(f);
	return out;
}

urlctx *url_init(const char contents[]) {

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
