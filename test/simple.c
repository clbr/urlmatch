#include "test.h"

void test(const char needle[], const char hay[], const int res) {

	if (url_simplematch(needle, hay) != res)
		fail("%s in %s, expected %u\n", needle, hay, res);
}

int main() {

	test("foo", "bar", 0);
	test("foo", "fo", 0);

	test("foo", "foo", 1);
	test("foo", "foofoo", 0);
	test("foo", "barfoo", 0);
	test("foo", "foofoobar", 0);

	test("*", "", 1);
	test("*", "ff", 1);
	test("*", "gdfgfd", 1);
	test("*", "*g****", 1);
	test("*", "*", 1);

	return 0;
}
