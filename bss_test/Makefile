all:
	gcc -Wl,--strip-all -Wl,-Map,test.map -Wl,-verbose -o main.exe main.c
	#gcc -c main.c -o main.o
	#ld --strip-all main.o linker.spec -o main.exe

clean:
	rm -f *exe *o *map
