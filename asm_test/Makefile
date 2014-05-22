all:
	gcc -S -masm=intel -fno-asynchronous-unwind-tables -o main.s main.c
	gcc -c -masm=intel -o main.o main.s
	gcc -Wl,--strip-all -o main.exe main.o

clean:
	rm -f *exe *o *map *.s
