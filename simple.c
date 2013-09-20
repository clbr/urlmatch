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

int simplematch(const char find[], const char hay[]) {

	const u32 wilds = countwilds(find);

	// Easiest path: no wildcards
	if (!wilds) {
		return strcmp(find, hay) == 0;
	}
}

u32 countwilds(const char str[]) {

	u32 sum = 0;

	const char *ptr = str;
	for (; *ptr; ptr++) {
		if (*ptr == '*') sum++;
	}

	return sum;
}
