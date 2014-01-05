#include "test.h"
#include "internal.h"

int main() {

	const char pat[] = "moi";

	urlctx *ctx = url_init(pat);
	printctx(ctx);
	url_free(ctx);

	return 0;
}
