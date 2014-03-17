all:
	gcc -c main.cc -o main.o -Wall -std=c++11 -Wno-write-strings
	gcc -o my_string main.o -lstdc++ -Wall -O

clean:
	rm -f *.o *.exe

test: all
	./my_string