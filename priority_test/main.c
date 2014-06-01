#include <stdio.h>
#include <errno.h>
#include <windows.h>
#include <winbase.h>
#include <tchar.h>

void my_sleep(int seconds) {
	Sleep(seconds * 1000);
}

void set_background_priority(int status) {
	DWORD dwError, dwPriClass;

	dwPriClass = GetPriorityClass(GetCurrentProcess());
	_tprintf(TEXT("Current priority class is 0x%x\n"), dwPriClass);

	if (!SetPriorityClass(GetCurrentProcess(), status)) {
		dwError = GetLastError();
		_tprintf(TEXT("Failed to enter background mode (%d)\n"), dwError);
	} else {
		_tprintf(TEXT("succeed to set priority.\n"));
	}
	// Display priority class
	dwPriClass = GetPriorityClass(GetCurrentProcess());
	_tprintf(TEXT("Current priority class is 0x%x\n"), dwPriClass);
}

int main (int argc, char **argv) {
	my_sleep(10);
	set_background_priority(NORMAL_PRIORITY_CLASS);
	set_background_priority(BELOW_NORMAL_PRIORITY_CLASS);
	my_sleep(500);

	return 0;
}