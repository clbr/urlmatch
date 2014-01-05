#include "test.h"
#include "internal.h"
#include <unistd.h>

int main() {

	/* Repeatedly save and load the context, then compare it to the first one. */

	urlctx *ctx = url_init(
			"http://*gooogle*\n"
			"ftp://fooo\n"
			"*adwords\n"
			"http*//*.php");

	// Yes yes, insecure mktemp. This is a unit test.
	char name[] = "/tmp/bintestXXXXXX";
	mktemp(name);

	if (url_save_optimized(ctx, name)) fail("save failed\n");

	u32 i;
	urlctx *tmp;
	for (i = 0; i < 20; i++) {
		tmp = url_init_file(name);
		if (!tmp) fail("load failed\n");
		if (url_save_optimized(tmp, name)) fail("save failed\n");
		url_free(tmp);
	}
	tmp = url_init_file(name);

	if (ctxcmp(ctx, tmp)) fail("compare failed\n");

	url_free(ctx);
	url_free(tmp);
	unlink(name);
	return 0;
}
