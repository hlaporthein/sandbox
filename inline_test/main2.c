#include <stdio.h>

static inline void hello() __attribute__((always_inline));

static inline void hello() {
	puts("Hello World!\n");
}

int main() {
	hello();
	return 0;
}