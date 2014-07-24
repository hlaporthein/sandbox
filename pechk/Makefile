CFLAGS = -std=gnu99 -Wall -Werror

all:
	gcc $(CFLAGS) -c -o pechk.o pechk.c
	gcc -shared -o pechk.dll pechk.o -Wl,--out-implib=pechk.dll.a
	gcc $(CFLAGS) -c -o test.o test.c
	gcc -o test_pechk.exe test.o pechk.dll

test:
	cp -f pechk.dll pechk2.dll
	./test_pechk

clean:
	rm -f *.exe *.dll *.o *.a

install: all
	mkdir -p /usr/local/bin
	cp -f pechk.dll /usr/local/bin
	mkdir -p /usr/local/lib
	cp -f pechk.dll.a /usr/local/lib
	mkdir -p /usr/local/include
	cp -f pechk.h /usr/local/include
