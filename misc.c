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
