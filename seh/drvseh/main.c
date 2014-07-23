#include <stdio.h>

#include "drvseh.h"

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
	__drv_try {
		oops(1);
	} __drv_except {
		printf("Exception catched.\n");
	} __drv_end_except;

	__drv_try {
		oops(0);
	} __drv_except {
		printf("Exception catched.\n");
	} __drv_end_except;

	__drv_try {
		oops(-1);
	} __drv_except {
		printf("Exception catched.\n");
	} __drv_end_except;

	printf("Finished.\n");

	return 0;
}
