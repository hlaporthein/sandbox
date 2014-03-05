#include <stdio.h>

inline void hello() __attribute__((always_inline));

inline void hello() {
	puts("Hello World!\n");
}

int main() {
	hello();
	return 0;
}