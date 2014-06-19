#include "setjmp.h"

int setjmp(jmp_buf env) {
	__asm__ __volatile__ (
		"mov ebx, ebp\n\t"

		"mov eax, %[my_env]\n\t"
		"add eax, 44\n\t"
		"mov esp, eax\n\t"
		"mov ebp, eax\n\t"

		"push 123\n\t"
		"pushf\n\t"
		"pusha\n\t"
		"call my_next\n\t"
		"my_next:\n\t"

		"mov esp, ebx\n\t"
		"mov ebp, esp\n\t"
		:: [my_env] "r" (env)
	);
	return env[10];
}

void longjmp(jmp_buf env, int value) {
//	__asm__ __volatile__ (
//		"pop eax\n\t"
//		"pushf\n\t"
//		"pusha\n\t"
//		"call my_next\n\t"
//		"my_next:\n\t"
//	);
}