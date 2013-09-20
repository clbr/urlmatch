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

#include "urlmatch.h"
#include "internal.h"

int url_simplematch(const char find[], const char hay[]) {

	const u32 wilds = countwilds(find);

	// Easiest path: no wildcards
	if (!wilds) {
		return strcmp(find, hay) == 0;
	}

	const u32 len = strlen(find);
	u32 i, h = 0;

	for (i = 0; i < len; i++) {
		if (find[i] != '*') {
			if (find[i] != hay[h])
				return 0;
			h++;
		} else {
			if (i == len - 1)
				return 1;

			// Wildcard, not last
			const char * const ender = strchrnul(&find[i + 1], '*');
			const u32 dist = ender - &find[i + 1];

			char piece[dist + 1];
			memcpy(piece, &find[i + 1], dist);
			piece[dist] = '\0';

			const char * const lastmatch = strrstr(&hay[h], piece);
			if (!lastmatch)
				return 0;

			const u32 move = lastmatch - &hay[h];
			h += move;
		}
	}

	return 1;
}
