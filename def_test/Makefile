all: dll exe

dll:
	gcc -c mylib.c -o mylib.o
	gcc -shared -o mylib.dll mylib.o mylib.def
	gcc -shared -o truc.dll truc.c

exe:
	gcc -o main.o -c main.c
	gcc -o def.exe main.o mylib.dll
