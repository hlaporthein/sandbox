#include <stdio.h>
#include <excpt.h>

EXCEPTION_DISPOSITION coucou(struct _EXCEPTION_RECORD *er, void *v, struct _CONTEXT *context, void *p) {
	printf("Hello we got an exception.\n");
	return ExceptionCollidedUnwind;
}

int main() {
	PEXCEPTION_HANDLER pCoucou = coucou;
	__try1(pCoucou) {

		int a = 0;
		int b = 0;
		int c = a / b;
		printf("a/b=%d\n", c);
	} __except1;

	return 0;
}