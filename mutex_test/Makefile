all:
	gcc -c main.c -o main.o -std=c99
	gcc main.o -o mutex -lpthread

	g++ -c main2.cc -o main2.o
	g++ main2.o -o mutex2 -lpthread

clean:
	rm -rf *.exe *.o