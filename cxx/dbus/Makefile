all:
	gcc -c main.cc -o main.o -Wall -std=c++11 -I/usr/local/include/dbus-1.0
	g++ -o dbus main.o -Wall -O

clean:
	rm -f *.o *.exe

test: all
	./dbus