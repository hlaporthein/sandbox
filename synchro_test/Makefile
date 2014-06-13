CFLAGS = -Wall -Werror -O -I/usr/local/include/tre -std=gnu99
LDFLAGS = -Wall -Werror

ifdef DEBUG
CFLAGS += -DDEBUG_MODE
endif

all: compile_ansi compile_unicode
	gcc -o synchro_a $(LDFLAGS) main_a.o libsynchro_a.a -ltre
	gcc -o synchro_w $(LDFLAGS) -D_MY_UNICODE main_w.o libsynchro_w.a -ltre

compile_ansi:
	gcc -c $(CFLAGS) synchro.c -o a_synchro.o
	gcc -c $(CFLAGS) file.c -o a_file.o
	gcc -c $(CFLAGS) filter.c -o a_filter.o
	gcc -E $(CFLAGS) main.c -o main_a.i
	gcc -c $(CFLAGS) main.c -o main_a.o
	gcc -shared -o synchro.dll $(CFLAGS) a_*.o -Wl,--out-implib=libsynchro.dll.a -ltre
	ar cru libsynchro_a.a a_*.o

compile_unicode:
	gcc -c $(CFLAGS) -D_MY_UNICODE synchro.c -o w_synchro.o
	gcc -c $(CFLAGS) -D_MY_UNICODE file.c -o w_file.o
	gcc -c $(CFLAGS) -D_MY_UNICODE filter.c -o w_filter.o
	gcc -c $(CFLAGS) -D_MY_UNICODE main.c -o main_w.o
	gcc -E $(CFLAGS) -D_MY_UNICODE main.c -o main_w.i
	gcc -shared -o synchro_w.dll $(CFLAGS) w_*.o -Wl,--out-implib=libsynchro_w.dll.a -ltre
	ar cru libsynchro_w.a w_*.o

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
	./synchro_a ./src ./dst
	./synchro_w ./src ./dst_w