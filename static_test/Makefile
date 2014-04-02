all:
	gcc -c mylib.c -o mylib.o
	ar cru libmylib.a mylib.o
	gcc -shared mylib.c -o mylib.dll -Wl,--out-implib=libmylib.dll.a

	gcc -Wl,--strip-all main.c -lmylib -o main -L.
	gcc -static -Wl,--strip-all main.c -lmylib -o main2 -L.

clean:
	rm -f *.exe *.o *.dll
