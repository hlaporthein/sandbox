#include <windows.h>
#include <stdio.h>

char buffer[8] = "coucou";
char buffer2[8] = "hello";

void hello2() {
	HMODULE p = GetModuleHandle("mylib2.dll");
	printf("from mylib2.dll: p=0x%08x\n", p);
	printf("from mylib2.dll: buffer=0x%08x\n", buffer);
	printf("from mylib2.dll: buffer2=0x%08x\n", buffer2);
	sprintf(buffer, "0123456789");
	printf("from mylib2.dll: buffer=%s\n", buffer);
	printf("from mylib2.dll: buffer2=%s\n", buffer2);
}