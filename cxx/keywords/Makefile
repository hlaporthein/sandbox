all:
	gcc -c main.cc -o main.o -Wall -std=c++11 -Wno-write-strings
	gcc -o keywords main.o -lstdc++ -Wall -O

clean:
	rm -f *.o *.exe

test: all
	./keywords