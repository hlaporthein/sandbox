#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

void hello(const char* format, int n, ...) {
	va_list argp;
	va_start(argp, n);

	for (int i = 0; i < n; i++) {
		int val = va_arg(argp, int);
		printf(format, val);
	}
	va_end(argp);
}

int main() {
	for (int i = 0; i < 10; i++) {
		printf("Hello World %d!!!\n", i);
	}
	system("sh echo coucou");
	hello("Hello %d\n" , 4, 1, 2, 4, 5);
}