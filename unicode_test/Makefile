all:
	gcc -E -o my_liba.i my_lib.c
	gcc -E -D_UNICODE -o my_libw.i my_lib.c
	gcc -c -o my_liba.o my_lib.c
	gcc -c -D_UNICODE -o my_libw.o my_lib.c
	gcc -s -shared -o my_lib.dll my_liba.o my_libw.o

	gcc -D_UNICODE -o mainw.exe main.c my_lib.dll
	gcc -o maina.exe main.c my_lib.dll

clean:
	rm -rf *.exe *.o *.dll *.txt

test: all
	touch abc.txt
	./maina
	./mainw