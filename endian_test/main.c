#include <stdio.h>

typedef union {
	int n;
	struct {
		char a[4];
	} b;
} xxx;

int main() {
	xxx x;

	x.n = 1;

	printf("%d %d %d %d", x.b.a[0], x.b.a[1], x.b.a[2], x.b.a[3]);
	return 0;
}