#include <windows.h>

#include "__try.h"

static LPTOP_LEVEL_EXCEPTION_FILTER g_old_exception_handler = NULL;

my_jmp_buf g_env;

static LONG WINAPI exception_handler(LPEXCEPTION_POINTERS ExceptionInfo) {
	my_longjmp(g_env, -1);
	// Normally we don't go here, so we can return whatever
	return EXCEPTION_CONTINUE_EXECUTION;
}

void set_handler() {
	g_old_exception_handler = SetUnhandledExceptionFilter(exception_handler);
}

void restore_handler() {
	SetUnhandledExceptionFilter(g_old_exception_handler);
}
