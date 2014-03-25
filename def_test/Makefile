all: dll exe

dll:
	gcc -c mylib.c -o mylib.o
	gcc -shared -o truc.dll truc.c
	gcc -shared -o mylib.dll mylib.o mylib.def truc.dll -Wl,--strip-all


exe:
	gcc -o main.o -c main.c
	gcc -o def.exe main.o mylib.dll

clean:
	rm *exe *dll *o
