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

#ifndef URLMATCH_H
#define URLMATCH_H

// Let's help the compiler
#if __GNUC__ >= 4

	#define PURE_FUNC __attribute__ ((pure))
	#define NORETURN_FUNC __attribute__ ((noreturn))
	#define CONST_FUNC __attribute__ ((const))
	#define WUR_FUNC __attribute__ ((warn_unused_result))
#else // GNUC

	#define PURE_FUNC
	#define NORETURN_FUNC
	#define CONST_FUNC
	#define WUR_FUNC

#endif // GNUC


int url_simplematch(const char pattern[], const char haystack[]) WUR_FUNC PURE_FUNC;


#undef PURE_FUNC
#undef NORETURN_FUNC
#undef CONST_FUNC
#undef WUR_FUNC

#endif
