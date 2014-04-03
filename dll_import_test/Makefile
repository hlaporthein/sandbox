all:
	gcc -c mylib.c -o mylib.o
	ar cru libmylib.a mylib.o
	gcc -shared mylib.c -o mylib.dll -Wl,--out-implib=libmylib.dll.a

	gcc -c main.c -o main.o
	gcc main.o -o main -lmylib -L. -static

clean:
	rm -f *.exe *.o *.dll *.a
