all:
	gcc -c main.c -o main.o -Wall -std=gnu99
	gcc -o cp_yannis main.o -Wall -O

clean:
	rm -f *.o *.exe

test: all
	./cp_yannis