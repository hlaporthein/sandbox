CFLAGS = -Wall -Werror -O

ifdef DEBUG
CFLAGS += -DDEBUG_MODE
endif

all:
	gcc -c $(CFLAGS) -std=gnu99 synchro.c -o synchro.o
	gcc -c $(CFLAGS) -std=gnu99 main.c -o main.o
	gcc -c $(CFLAGS) -std=gnu99 file.c -o file.o

	gcc -shared -o synchro.dll $(CFLAGS) synchro.o file.o -Wl,--out-implib=libsynchro.dll.a
	ar cru libsynchro.a synchro.o file.o
	gcc -o synchro $(CFLAGS) main.o libsynchro.a

install:
	mkdir -p /usr/local/include
	install -c synchro.h /usr/local/include/synchro.h
	mkdir -p /usr/local/lib
	install -c libsynchro.a /usr/local/lib/libsynchro.a

clean:
	rm -rf *.o *.exe *.dll *.a ./src ./dst *.txt

test:
	mkdir -p ./src/test ./src/kiki/toto
	cp -f *.a ./src
	cp -f *.dll ./src/test
	cp -f *.a ./src/kiki
	cp -f *.c ./src/kiki/toto
	./synchro ./src ./dst