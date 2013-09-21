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

struct urlctx {
	struct prefix *pref;
	u16 count;
};

struct prefix {
	struct suffix *suf;
	u16 count;

	char prefix[6];
};

struct suffix {
	struct needle *need;
	u16 count;

	char suffix[3];
};

struct needle {
	const char *needle;
	u16 len;
	u16 wilds;
};

urlctx *url_init_file(const char file[]);
urlctx *url_init(const char contents[]);

int url_save_optimized(const urlctx *ctx, const char file[]);

int url_match(const urlctx *ctx, const char haystack[]);

void url_free(urlctx *ctx);

