#include "test.h"

static void test(const char needle[], const char hay[], const int res) {

	const int ret = ratedsearch(needle, hay);
	if (ret != res)
		fail("'%s' in '%s', expected %u got %d\n", needle, hay, res, ret);
}

int main() {

	test("masa", "doigfhdfoighfdoignvoifd hoidfvhiofdhvoifd hoifhgifdo hofdi", 0);
	test("masa", "masa", 1);
	test("  masa  ", "masa", 1);
	test("nightwish  lyric  ", "The best of Nightwish: Sleeping Sun LYRICS", 2);
	test("NIGHtwish  lyric  ", "The best of Nightwish: Sleeping Sun LYRICS", 2);
	test("johnson core coil", "http://www.google.com?search=kalle+masa+kaupassa", 0);
	test("johnson core coil", "http://www.google.com?search=kalle+masa+kaupassa+johnso+coil", 1);
	test("johnson core coil", "http://www.google.com?search=core+coil+johnson", 3);
	test("", "", 0);

	return 0;
}
