#include "setjmp.h"

int setjmp(jmp_buf env) {
	__asm__ __volatile__ (
		"add ebp, 80\n\t"
		"mov esp, ebp\n\t"
		"push 123\n\t"
		"pushf\n\t"
		"pusha\n\t"
		"call my_next\n\t"
		"my_next:\n\t"
		"sub esp, 36\n\t"
		"mov ebp, esp\n\t"
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