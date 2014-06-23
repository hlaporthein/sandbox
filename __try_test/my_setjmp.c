#include <stdio.h>
#include "setjmp.h"

int my_setjmp(jmp_buf env) {
	__asm__ __volatile__ (
		"pushf\n\t"
		"pop ecx\n\t"

		"mov DWORD PTR [eax], 123\n\t"
		"mov [eax+4], eax\n\t"
		"mov [eax+8], ebx\n\t"
		"mov [eax+12], ecx\n\t"
		"mov [eax+16], edx\n\t"
		"mov [eax+20], esi\n\t"
		"mov [eax+24], edi\n\t"

		"mov [eax+28], ebp\n\t" // ebp1
		"mov ebx, [ebp]\n\t" // [ebp1]
		"mov [eax+32], ebx\n\t" // [ebp1]
		"mov ebx, [eax+8]\n\t" // restore ebx

		"mov [eax+36], esp\n\t"
		"mov [eax+40], ecx\n\t" // push eflags
		"call my_next\n\t"
		"my_next:\n\t"
		"pop edx\n\t"
		"mov [eax+44], edx\n\t" // push eip2

		"mov edx, [esp+4]\n\t"
		"mov DWORD PTR [eax+48], edx\n\t" // push eip1
		:: "a" (env)
	);
	return env[0];
}

void my_longjmp(jmp_buf env, int value) {
	__asm__ __volatile__ (
		"mov DWORD PTR [eax], %[val]\n\t" // set value
		// "mov eax, [eax+4]\n\t" // no need
		"mov ebx, [eax+8]\n\t"
		"mov ecx, [eax+12]\n\t"
		"mov edx, [eax+16]\n\t"
		"mov esi, [eax+20]\n\t"
		"mov edi, [eax+24]\n\t"
		"mov ebp, [eax+28]\n\t"

		"mov esp, [eax+36]\n\t"
		"mov ecx, [eax+40]\n\t" // pop eflags
		"push ecx\n\t"
		"popf\n\t"
		"mov ecx, [eax+12]\n\t"

		"add esp, 8\n\t" // remove ebp eip3

		"mov edx, [eax+48]\n\t" // read eip1
		"push edx\n\t"// push eip return setjmp (eip1)

		"push ebp\n\t" // push setjmp ebp1
		"mov ebx, [eax+32]\n\t" // restore ebp value
		"mov [ebp], ebx\n\t"
		"mov ebx, [eax+8]\n\t" // restore ebx

		"mov edx, [eax+44]\n\t" // read eip2
		"push edx\n\t" // will pop eip2 after jmp
		"push edx\n\t" // for the ret
		"mov edx, [eax+16]\n\t"
		"ret\n\t"
		:: "a" (env), [val] "r" (value)
	);
}