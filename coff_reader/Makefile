CFLAGS=-std=gnu99
SOURCES=$(shell find . -name '*.c')
OBJECTS=$(SOURCES:%.c=%.o)

all: coff_reader.exe

coff_reader.exe: $(OBJECTS)
	gcc -o $@ $(OBJECTS) -Wl,--strip-all

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.exe *.o

install: all
	mkdir -p /usr/local/bin
	cp -f coff_reader.exe /usr/local/bin