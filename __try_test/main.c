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
	printf("esp=%08X\n", env[8]);
	printf("eflags=%08X\n", env[9]);
	printf("eip=%08X\n", env[10]);
	printf("eip2=%08X\n", env[11]);
}

int main() {
	my_jmp_buf env;
	memset(&env, 0, sizeof(env));
	int a = my_setjmp(env);
	a++;
	print_env(env);
//	if (a != 123) {
//		printf("Exception catched!");
//		goto cleanup;
//	}
	my_longjmp(env, 125);
	printf("longjmp done\n");
	print_env(env);
//cleanup:
	return 0;
}