#include <stdio.h>
#include <seh.h>

int print_fs0() {
	int fs0 = -1;
	__asm__ __volatile__(
		"movl %%fs:0, %0"
		: "=r" (fs0)
	);

	printf("fs0=0x%08X\n", fs0);
	return fs0;
}
void oops(int b) {
	int a = 15;
	int c = a / b;
	printf("a/b=%d\n", c);
}

int main() {
	print_fs0();
	__seh_try {

		__seh_try {
			oops(0);
		} __seh_finally {
			printf("Finally.\n");
		} __seh_end_finally

	} __seh_except(1) {
		printf("Exception catched.\n");
	} __seh_end_except

	print_fs0();
	return 0;
}
