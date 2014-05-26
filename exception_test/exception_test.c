#include <stdio.h>
#include <windows.h>
//typedef struct _EXCEPTION_RECORD {
//	DWORD ExceptionCode;
//	DWORD ExceptionFlags;
//	struct _EXCEPTION_RECORD *ExceptionRecord;
//	PVOID ExceptionAddress;
//	DWORD NumberParameters;
//	DWORD ExceptionInformation[EXCEPTION_MAXIMUM_PARAMETERS];
//} EXCEPTION_RECORD,*PEXCEPTION_RECORD,*LPEXCEPTION_RECORD;
//typedef struct _EXCEPTION_POINTERS {
//	PEXCEPTION_RECORD ExceptionRecord;
//	PCONTEXT ContextRecord;
//} EXCEPTION_POINTERS,*PEXCEPTION_POINTERS,*LPEXCEPTION_POINTERS;
LPVOID DisplayError(DWORD Err)
{

	HMODULE Hand = LoadLibrary("NTDLL.DLL");
	LPVOID lpMessageBuffer;

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_FROM_HMODULE,
		Hand,
		Err,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR) &lpMessageBuffer,
		0,
		NULL );

	FreeLibrary(Hand);
	return lpMessageBuffer;
}

LONG WINAPI e(LPEXCEPTION_POINTERS ExceptionInfo) {
	printf("Exception Handled...\n");
	char *buf = DisplayError(ExceptionInfo->ExceptionRecord->ExceptionCode);
	printf("ExceptionCode=0x%08x (%s)\n", ExceptionInfo->ExceptionRecord->ExceptionCode, buf);
	printf("ExceptionFlags=%d\n", ExceptionInfo->ExceptionRecord->ExceptionFlags);
	printf("ExceptionAddress=0x%08x\n", ExceptionInfo->ExceptionRecord->ExceptionAddress);
	printf("ExceptionInformation=%s\n", ExceptionInfo->ExceptionRecord->ExceptionInformation);
	printf("NumberParameters=%s\n", ExceptionInfo->ExceptionRecord->NumberParameters);
	return EXCEPTION_EXECUTE_HANDLER;
}

int main() {
	LPTOP_LEVEL_EXCEPTION_FILTER p = SetUnhandledExceptionFilter(e);
	if (p == NULL) {
		printf("Exception Handling Default behavior.\n");
	} else {
		printf("Exception Handling Function = 0x%08x.\n", p);
	}

	int i = 0;
	int j = 0;
	int k = i / j;
	printf("i/j = %d\n", k);
	return 0;
}
