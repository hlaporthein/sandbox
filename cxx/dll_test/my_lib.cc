#include <stdio.h>
#include "my_lib.h"

using namespace julia_thomias;

int julia_thomias::hello_coucou(int x, int y, const char *name) {
	printf("x=%d\n", x);
	printf("y=%d\n", y);
	printf("hello %s!\n", name);
	return 0;
}

int julia_thomias::hello_coucou(const char *name) {
	printf("hello %s!\n", name);
	return 0;
}

void h(int y) {
	printf("y=%d\n", y);
}

