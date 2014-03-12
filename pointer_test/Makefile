all:
	gcc -c main.c -o main.o -Wall
	gcc -c main.cc -o main2.o -Wall
	gcc -o swap main.o -Wall
	gcc -o swap2 main2.o -Wall -lstdc++

clean:
	rm -f *.o *.exe