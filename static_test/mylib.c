#include <windows.h>
#include <stdio.h>

void hello() {
	HMODULE p = GetModuleHandle("mylib.dll");
	printf("from mylib.dll: p=0x%08x\n", p);
}