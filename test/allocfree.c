#include "test.h"

int main() {

	urlctx *ctx = url_init("http://moi\nhttps://katti\nmoido\nhttp://google*");
	url_free(ctx);

	return 0;
}
