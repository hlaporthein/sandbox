all:
	gcc -c -o test.o test.c
	gcc -shared -o test.dll test.o -Wl,--out-implib=test.lib
	ar cru libtest.a test.o

	gcc -c -o main.o main.c
	gcc -o test main.o -ltest -L.
	gcc -o test2 main.o -l:test.lib

clean:
	rm -f *.exe *.o *.dll *.a