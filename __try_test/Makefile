CFLAGS = -Wall -Werror -std=gnu99 -masm=intel -fno-asynchronous-unwind-tables

all:
	gcc $(CFLAGS) -c -o __try.o __try.c
	gcc $(CFLAGS) -S -o main.s main.c
	gcc $(CFLAGS) -S -o my_setjmp.s my_setjmp.c
	gcc $(CFLAGS) -c -o my_setjmp.o my_setjmp.c
	ar cru __try.a __try.o my_setjmp.o
	gcc $(CFLAGS) -c -o main.o main.c
	gcc $(CFLAGS) -o try main.o __try.a

clean:
	rm -rf *a *o *exe *s