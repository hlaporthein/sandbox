#include <stdio.h>
#include <excpt.h>


int get_fs0() {
	int fs0;
	__asm__ __volatile__ (
		"movl %%fs:0, %0"
		: "=r" (fs0)
	);
	return fs0;
}
void set_fs0(int fs0) {
	__asm__ __volatile__ (
		"movl %0, %%fs:0"
		: : "r" (fs0)
	);
}


#define __try2(pHandler) \
	{ \
		int buf[2]; \
		buf[0] = get_fs0(); \
		buf[1] = (int) pHandler; \
		set_fs0((int) buf);

#define __except2 \
		set_fs0(buf[0]); \
	}


void print_fs0() {
	int fs0 = -1;
	__asm__ __volatile__(
		"movl %%fs:0, %0"
		: "=r" (fs0)
	);

	printf("fs0=0x%08X\n", fs0);
}

void print_EstablisherFrame(int *v) {
	printf("EstablisherFrame=0x%08X\n", (unsigned int) v);
	int i = 0;
	for (i = 0; i < 10; i++) {
		printf("EstablisherFrame[%d]=0x%08X\n", i, v[i]);
	}
}

EXCEPTION_DISPOSITION coucou(struct _EXCEPTION_RECORD *er, void *v, struct _CONTEXT *context, void *p) {
	print_fs0();
	print_EstablisherFrame((int *) v);
	printf("Hello we got an exception. Exception code=0x%08X\n", er->ExceptionCode);
	printf("ExceptionFlags=0x%08X\n", er->ExceptionFlags);
	int ebx = -1;
	printf("context->Ebp=0x%08X\n", context->Ebp);
	__asm__ __volatile__ ("movl %1, %%ebx; movl $10, 8(%%ebx); movl 8(%%ebx), %0" : "=r" (ebx) : "r" (context->Ebp));

	printf("ebx=0x%08X\n", ebx);
	print_EstablisherFrame((int *) v);
	return ExceptionContinueExecution;
}

void oops(int b) {
	int a = 15;
	int c = a / b;
	printf("a/b=%d\n", c);
}

int main() {
	print_fs0();
	__try2(&coucou) {
		print_fs0();
		int x = 0;
		int y = 0xdeadbeef;
		printf("&x =0x%08X\n", (unsigned int) &x);
		printf("buf=0x%08X\n", (unsigned int) buf);
		oops(x);
	} __except2

	//print_fs0();
	oops(2);
	print_fs0();
	return 0;
}