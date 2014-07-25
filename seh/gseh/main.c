#include <stdio.h>

#include "gseh.h"

void oops(int b) {
	if (b >= 0) {
		int a = 15;
		int c = a / b;
		printf("a/b=%d\n", c);
		return;
	}
	int *p = NULL;
	*p = 55;
}

int main() {
	__gtry {
		oops(1);
	} __gexcept {
		printf("Exception catched.\n");
	} __gend_except;

	__gtry {
		DEBUG("Start try");
		oops(0);
		DEBUG("End try");
	} __gexcept {
		printf("Exception catched.\n");
	} __gend_except;

	__gtry {
		oops(-1);
	} __gexcept {
		printf("Exception catched.\n");
		printf("Exception catched: flags=0x%08X\n", (unsigned int) GetExceptionInformation()->ExceptionRecord->ExceptionFlags);
		printf("Exception catched: ebp=0x%08X\n", (unsigned int) GetExceptionInformation()->ContextRecord->Ebp);
		printf("Exception catched: esp=0x%08X\n", (unsigned int) GetExceptionInformation()->ContextRecord->Esp);
	} __gend_except;

	printf("Finished.\n");

	return 0;
}
