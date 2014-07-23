all:
	gcc -fno-asynchronous-unwind-tables -S -Wall -Werror -o drvseh.s drvseh.c
	gcc -c -Wall -Werror -o drvseh.o drvseh.c
	gcc -o drvseh_func.o -c drvseh_func.S
	ar cru drvseh.a drvseh.o drvseh_func.o
	gcc -E -Wall -Werror -o main.i main.c
	gcc -S -Wall -Werror -o main.s main.c
	gcc -Wall -Werror -o main main.c drvseh.a

install:
	mkdir -p /usr/local/lib
	cp -f drvseh.a /usr/local/lib
	mkdir -p /usr/local/include
	cp -f drvseh.h /usr/local/include

clean:
	rm -rf *.exe *.i *.s