CFLAGS = -Wall -Werror -O

all:
	gcc -c $(CFLAGS) -std=gnu99 utf8fs.c -o utf8fs.o
	gcc -c $(CFLAGS) -std=gnu99 main.c -o main.o

	gcc -shared -o utf8fs.dll $(CFLAGS) utf8fs.o -Wl,--out-implib=utf8fs.dll.a
	ar cru utf8fs.a utf8fs.o
	gcc -o utf8fs $(CFLAGS) main.o utf8fs.a

install:
	mkdir -p /usr/local/include
	install -c utf8fs.h /usr/local/include/utf8fs.h
	mkdir -p /usr/local/lib
	install -c utf8fs.a /usr/local/lib/utf8fs.a

clean:
	rm -rf *.o *.exe *.dll *.a *.txt

test:
