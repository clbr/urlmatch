#include "test.h"

int main() {

	const char pat[] =
		"http://moi\n"
		"https://katti\n"
		"moido\n"
		"http://hoi\n"
		"http://google*";

	urlctx *ctx = url_init(pat);
	url_free(ctx);

	return 0;
}
