#include <stdio.h>

int get_fs0() {
	int fs0 = -1;
	__asm__ __volatile__(
		"movl %%fs:0, %0"
		: "=r" (fs0)
	);
	return fs0;
}

int main() {
	int fs0 = get_fs0();

	printf("fs0:		0x%08X\n", fs0);

	int level = 1;
	while (fs0 != 0xFFFFFFFF) {
		printf("\nlevel %d\n", level);
		printf("  Prev:		0x%08X\n", ((int *)fs0)[0]);
		printf("  Handler:	0x%08X\n", ((int *)fs0)[1]);

		fs0 = ((int *)fs0)[0];
		level++;
	}

	return 0;
}