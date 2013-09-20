#include "test.h"
#include "internal.h"

void test(const char str[], const u32 result) {

	u32 foo = countwilds(str);

	if (foo != result)
		fail("Got %u expected %u for %s\n", foo, result, str);
}

int main() {

	test("", 0);
	test("ddd", 0);
	test("dgfdsfsdgd", 0);

	test("*", 1);
	test("**", 2);
	test("***", 3);
	test("****", 4);

	test("*foo*bar", 2);
	test("*foobar*", 2);

	return 0;
}
