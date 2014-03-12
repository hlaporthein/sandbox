all:
	gcc -E main.cc -o main.ii -Wall
	gcc -c main.cc -o main.o -Wall
	gcc -c hello.cc -o hello.o -Wall
	gcc -o hello main.o hello.o -lstdc++ -Wall

clean:
	rm -f *.o *.exe *.ii