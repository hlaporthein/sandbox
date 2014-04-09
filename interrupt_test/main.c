#include <stdio.h>
#include <windows.h>

LONG WINAPI e(LPEXCEPTION_POINTERS ExceptionInfo) {
	printf("Exception Handled...\n");
	char buf[8192];
	memset(buf, 0, 8192);

	void * pArgs[ExceptionInfo->ExceptionRecord->NumberParameters];
	for (int i = 0; i < ExceptionInfo->ExceptionRecord->NumberParameters; i++) {
		printf("arg[%d] = %d\n", i, ExceptionInfo->ExceptionRecord->ExceptionInformation[i+1]);
		pArgs[i] = (void *) ExceptionInfo->ExceptionRecord->ExceptionInformation[i+1];
	}
	HMODULE Hand = LoadLibrary("NTDLL.DLL");
	int res = FormatMessage(
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_FROM_HMODULE |
		FORMAT_MESSAGE_ARGUMENT_ARRAY |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		Hand,
		ExceptionInfo->ExceptionRecord->ExceptionCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		buf,
		8192,
		(va_list *) pArgs);
	printf("res=%d\n", res);
	FreeLibrary(Hand);

	printf("ExceptionCode=0x%08x (%s)\n", ExceptionInfo->ExceptionRecord->ExceptionCode, buf);
	printf("ExceptionFlags=%d\n", ExceptionInfo->ExceptionRecord->ExceptionFlags);
	printf("ExceptionAddress=0x%08x\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);
	printf("NumberParameters=%d\n", ExceptionInfo->ExceptionRecord->NumberParameters);
	printf("ExceptionInformation=%s\n", ExceptionInfo->ExceptionRecord->ExceptionInformation);


	return EXCEPTION_EXECUTE_HANDLER;
}



int main() {
	LPTOP_LEVEL_EXCEPTION_FILTER p = SetUnhandledExceptionFilter(e);
//	asm volatile (
//		"movl %cr0, %eax\n\t"
//		"or %eax, 1\n\t"
//		"movl %eax, %cr0\n\t"
//	);
//
//	asm volatile (
//		"cli\n\t"
//	);

	for (int i = 10; i < 256; i++) {
		int *p = (int *) i;
		printf("address pointed by p = 0x%08x\n", *p);
	}
}