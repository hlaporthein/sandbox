CFLAGS = -Wall -Werror -O -I/usr/local/include/tre

ifdef DEBUG
CFLAGS += -DDEBUG_MODE
endif

all:
	gcc -c $(CFLAGS) -std=gnu99 synchro.c -o synchro.o
	gcc -c $(CFLAGS) -std=gnu99 main.c -o main.o
	gcc -c $(CFLAGS) -std=gnu99 file.c -o file.o
	gcc -c $(CFLAGS) -std=gnu99 filter.c -o filter.o

	gcc -shared -o synchro.dll $(CFLAGS) synchro.o file.o filter.o ../utf8_fs/utf8fs.a -Wl,--out-implib=libsynchro.dll.a -ltre
	ar cru libsynchro.a synchro.o file.o filter.o
	gcc -o synchro $(CFLAGS) main.o libsynchro.a ../utf8_fs/utf8fs.a -ltre

install:
	mkdir -p /usr/local/include
	install -c synchro.h /usr/local/include/synchro.h
	mkdir -p /usr/local/lib
	install -c libsynchro.a /usr/local/lib/libsynchro.a

clean:
	rm -rf *.o *.exe *.dll *.a ./src ./dst *.txt *.i

test:
	mkdir -p ./src/test ./src/kiki/toto
	cp -f *.a ./src
	cp -f *.dll ./src/test
	cp -f *.a ./src/kiki
	cp -f *.c ./src/kiki/toto
	./synchro ./src ./dst