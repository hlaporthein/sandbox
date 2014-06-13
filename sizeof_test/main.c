#include <stdio.h>
#include <windows.h>

#define PRINT_SIZE(type) printf("sizeof(" #type ")=%d bit\n", sizeof(type) * 8)

int main() {
	PRINT_SIZE(int);
	PRINT_SIZE(DWORD);
	PRINT_SIZE(LARGE_INTEGER);
	PRINT_SIZE(long long);
	PRINT_SIZE(LONGLONG);
	PRINT_SIZE(wchar_t);

	return 0;
}