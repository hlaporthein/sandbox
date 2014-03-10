all:
	gcc -c -o main.o main.c
	gcc -shared main.o -o log.dll -Wl,--out-implib=liblog.dll.a
	ar cru liblog.a main.o

clean:
	rm -f *.o *.a *.dll

install:
	mkdir -p /usr/local/lib
	install -m 644 liblog.dll.a /usr/local/lib
	install -m 644 liblog.a /usr/local/lib
	install -m 755 log.dll /usr/local/bin
	install -m 644 jlg_log.h /usr/local/include
