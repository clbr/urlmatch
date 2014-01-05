#include "test.h"
#include "internal.h"

static void test(const char hay[], const char needle[], const u32 pos) {

	const char * const ptr = strrstr(hay, needle);
	const char * const exists = strstr(hay, needle);

	if (!exists) {
		if (ptr)
			fail("False positive\n");
		return;
	}

	const u32 tmp = ptr - hay;
	if (tmp != pos)
		fail("%s in %s, wanted %u got %u\n",
			needle, hay, pos, tmp);
}

static void suf(const char one[], const char two[], const u32 nomatch) {
	const u32 res = suffixcmp(one, two);

	if (res != nomatch)
		fail("suffixcmp %s %s got %u expected %u\n",
			one, two, res, nomatch);
}

int main() {

	test("foo", "bar", 0);
	test("foo", "fo", 0);

	test("foo", "foo", 0);
	test("foo", "foofoo", 3);
	test("foo", "barfoo", 3);
	test("foo", "foofoobar", 3);

	suf("aa", "ab", 1);
	suf("bb", "ab", 1);
	suf("a", "ab", 1);
	suf("aa", "b", 1);

	suf("aa", "aa", 0);
	suf("bb", "bb", 0);
	suf("aa", "a", 0);
	suf("a", "aa", 0);

	return 0;
}
