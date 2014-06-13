all:
	gcc -c -o main.o main.c
	gcc -o sizeof main.o -lkernel32

clean:
	rm -rf *.exe *.o

test: all
	./sizeof