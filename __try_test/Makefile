CFLAGS = -Wall -Werror -std=gnu99 -masm=intel

all:
	gcc $(CFLAGS) -c -o __try.o __try.c
	gcc $(CFLAGS) -S -o main.s main.c
	gcc $(CFLAGS) -S -o setjmp.s setjmp.c
	gcc $(CFLAGS) -c -o setjmp.o setjmp.c
	ar cru __try.a __try.o setjmp.o
	gcc $(CFLAGS) -c -o main.o main.c
	gcc $(CFLAGS) -o try main.o __try.a

clean:
	rm -rf *a *o *exe