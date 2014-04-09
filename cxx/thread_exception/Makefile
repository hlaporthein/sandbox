all:
	gcc -c main.cc -o main.o -Wall -std=c++11
	g++ -o exception main.o -Wall -O -lpthreadGCE2

clean:
	rm -f *.o *.exe

test: all
	./exception