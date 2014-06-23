#ifndef __TRY_H
#define __TRY_H

#include <windows.h>

#include "my_setjmp.h"

void set_handler();
void restore_handler();

extern my_jmp_buf g_env;

#define __try \
	int __run_exception_catched = 0; \
	if (my_setjmp(g_env) != 123) { \
		__run_exception_catched = 1; \
		goto __try_exception_catched; \
	} \
	set_handler();

#define __except(x) \
__try_exception_catched: \
	printf("x=%d\n", x); \
	restore_handler(); \
	if (__run_exception_catched)

#endif // __TRY_H