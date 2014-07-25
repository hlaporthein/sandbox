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
	__gseh_try {
		oops(1);
	} __gseh_except {
		printf("Exception catched.\n");
	} __gseh_end_except;

	__gseh_try {
		DEBUG("Start try");
		oops(0);
		DEBUG("End try");
	} __gseh_except {
		printf("Exception catched.\n");
	} __gseh_end_except;

	__gseh_try {
		oops(-1);
	} __gseh_except {
		printf("Exception catched.\n");
	} __gseh_end_except;

	printf("Finished.\n");

	return 0;
}
