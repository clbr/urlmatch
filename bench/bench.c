#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <lrtypes.h>
#include "urlmatch.h"

int main() {

	srand(time(NULL));

	gen();

	struct timeval start, end;

	gettimeofday(&start, NULL);
	simple();
	gettimeofday(&end, NULL);

	gettimeofday(&start, NULL);
	opti();
	gettimeofday(&end, NULL);

	return 0;
}
