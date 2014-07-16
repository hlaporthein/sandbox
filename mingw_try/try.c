#include <stdio.h>
#include <excpt.h>

EXCEPTION_DISPOSITION coucou(struct _EXCEPTION_RECORD *er, void *v, struct _CONTEXT *context, void *p) {
	printf("Hello we got an exception. Exception code=0x%08X\n", er->ExceptionCode);
	int ebx;
	__asm__ __volatile__ ("movl %0, %%ebx; movl $1, 8(%%ebx)" : "=r" (ebx) : "r" (context->Ebp));
	
	printf("ebx=0x%08X\n", ebx);
	return ExceptionContinueExecution;
}

void oops(int b) {
	int a = 5;
	int c = a / b;
	printf("a/b=%d\n", c);
}

int main() {
	PEXCEPTION_HANDLER pCoucou = coucou;
	__try1(pCoucou) {
		oops(0);
		
	} __except1;
	oops(2);

	return 0;
}