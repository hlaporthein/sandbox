#include <stdio.h>
#include "__try.h"

void print_env(my_jmp_buf env) {
	printf("------------------------\n");
	printf("&env=%08X\n", (unsigned int) env);
	printf("value=%d\n", env[0]);
	printf("eax=%08X\n", env[1]);
	printf("ebx=%08X\n", env[2]);
	printf("ecx=%08X\n", env[3]);
	printf("edx=%08X\n", env[4]);
	printf("esi=%08X\n", env[5]);
	printf("edi=%08X\n", env[6]);
	printf("ebp=%08X\n", env[7]);
	printf("[ebp]=%08X\n", env[8]);
	printf("esp=%08X\n", env[9]);
	printf("eflags=%08X\n", env[10]);
	printf("eip=%08X\n", env[11]);
	printf("eip2=%08X\n", env[12]);
}

int main1() {
	my_jmp_buf env;
	memset(&env, 0, sizeof(env));
	int a = my_setjmp(env);
	print_env(env);
	if (a != 123) {
		printf("Exception catched!\n");
		goto cleanup;
	}
	my_longjmp(env, 125);
	printf("longjmp done\n");
	print_env(env);
cleanup:
	printf("cleanup\n");
	return 0;
}

int main(int argc, char** argv) {
	__try {
		int x = argc - 1;
		printf("x=%d\n", x);
		int y = x / x;
		printf("y=%d\n", y);
	} __except (2) {
		printf("Exception catched\n");
	}
	printf("finished\n");
	return 0;
}