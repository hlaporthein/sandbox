#include <stdio.h>
#include "__try.h"


int main() {
	my_jmp_buf env;
	memset(&env, 0, sizeof(env));
	int a = my_setjmp(env);
	printf("a=%d\n", a);
	for (int i = 0; i < 11; i++) {
		printf("env[%d]=%d\n", i, env[i]);
	}
	my_longjmp(env, 125);
	printf("a=%d\n", a);
	for (int i = 0; i < 11; i++) {
		printf("env[%d]=%d\n", i, env[i]);
	}
//	__try {
//		int i = 10;
//		int* p = (int*) i;
//		printf("%x\n", *p);
//	} __except
	return 0;
}