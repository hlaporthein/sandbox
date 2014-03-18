CFLAGS=-std=c11

all:
	gcc $(CFLAGS) -c main.c -o main.o
	gcc $(CFLAGS) -c hexa.c -o hexa.o
	gcc -o pe_reader main.o hexa.o

clean:
	rm -f *.exe *.o