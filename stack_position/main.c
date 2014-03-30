#include <stdio.h>
#include <windows.h>

int main() {
	int i = 3;
	HMODULE h = GetModuleHandle(NULL);
	printf("address of image base = 0x%08x\n", h);
	printf("address of i = 0x%08x\n", &i);

	return 0;
}

