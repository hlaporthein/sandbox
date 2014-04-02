#include <windows.h>
#include <stdio.h>

int main() {
	HMODULE p = GetModuleHandle(NULL);
	printf("p=0x%08x\n", p);
	hello();
	return 0;
}
