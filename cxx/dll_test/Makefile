all: 
	g++ -c -o my_lib.o my_lib.cc
	g++ -shared -o my_lib.dll my_lib.o
	g++ -c -o main.o main.cc
	g++ -o main.exe main.o  my_lib.dll

clean:
	rm -f *.o *.exe *.dll
