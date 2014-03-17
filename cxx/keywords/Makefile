all:
	gcc -c main.cc -o main.o -Wall -std=c++11 -Wno-write-strings -Wno-unused-variable -Wno-unused-but-set-variable
	gcc -o keywords main.o -lstdc++ -Wall -O

clean:
	rm -f *.o *.exe

test: all
	./keywords