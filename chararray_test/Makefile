all:
	gcc -c main.c -o main.o -Wall -std=gnu99
	gcc -o chararray main.o -Wall -O

clean:
	rm -f *.o *.exe

test: all
	./chararray