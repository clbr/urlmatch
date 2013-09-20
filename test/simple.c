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

	return 0;
}
