all:
	gcc -D_DEBUG -fno-asynchronous-unwind-tables -S -Wall -Werror -o drvseh.s drvseh.c
	gcc -D_DEBUG -c -Wall -Werror -o drvseh.o drvseh.c
	gcc -o drvseh_func.o -c drvseh_func2.S
	ar cru drvseh.a drvseh.o drvseh_func.o
	gcc -E -Wall -Werror -o main.i main.c
	gcc -S -Wall -Werror -o main.s main.c
	gcc -D_DEBUG -Wall -Werror -o main main.c drvseh.a


clean:
	rm -rf *.exe *.i *.s