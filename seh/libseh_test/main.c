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
	__libseh_try {
		int y[1000];
		y[0] = 0;
		y[0]++;
		int x = 0;

		int fs0 = print_fs0();
		printf("&x=0x%08X\n", (unsigned int) &x);
		//printf("&y=0x%08X\n", (unsigned int) &y);
		printf("width=%d\n", ((int)&x) - fs0);
		printf("width=%d\n", -2);
		oops(x);
	} __libseh_except(1) {
		printf("Exception catched.\n");
	} __libseh_end_except;
	print_fs0();
	return 0;
}
