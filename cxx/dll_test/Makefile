all: 
	g++ -c -o my_lib.o my_lib.cc
	g++ -shared -o my_lib.dll my_lib.o -Wl,--out-implib=libmy_lib.dll.a
	g++ -c -o main.o main.cc
	g++ -o main.exe main.o  my_lib.dll

clean:
	rm -f *.o *.exe *.dll *.a
