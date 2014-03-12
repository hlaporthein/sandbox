all:
	gcc -c main.c -o main.o -Wall -std=c99
	gcc -o array main.o  -Wall

clean:
	rm -f *.o *.exe

test: all
	./array