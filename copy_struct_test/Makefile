CFLAGS = -Wall -Werror -O

all:
	gcc -c $(CFLAGS) -std=gnu99 synchro.c -o synchro.o
	gcc -c $(CFLAGS) -std=gnu99 main.c -o main.o

	gcc -shared -o synchro.dll $(CFLAGS) synchro.o
	ar cru libsynchro.a synchro.o
	gcc -o synchro $(CFLAGS) main.o libsynchro.a

install:
	mkdir -p /usr/local/include
	install -c synchro.h /usr/local/include/synchro.h
	mkdir -p /usr/local/lib
	install -c libsynchro.a /usr/local/lib/libsynchro.a

clean:
	rm -f *.o *.exe *.dll *.a

test:
	./synchro ./src ./dest