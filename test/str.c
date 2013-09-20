#include "test.h"
#include "internal.h"

void test(const char hay[], const char needle[], const u32 pos) {

	const char * const ptr = strrstr(hay, needle);
	const char * const exists = strstr(hay, needle);

	if (!exists) {
		if (ptr)
			fail("False positive\n");
		return;
	}

	const u32 tmp = pos - hay;
	if (tmp != pos)
		fail("%s in %s, wanted %u got %u\n",
			needle, hay, pos, tmp);
}

int main() {

	test("foo", "bar", 0);
	test("foo", "fo", 0);

	test("foo", "foo", 0);
	test("foo", "foofoo", 3);
	test("foo", "barfoo", 3);
	test("foo", "foofoobar", 3);

	return 0;
}
