CFLAGS=-std=gnu99
SOURCES=$(shell find . -name '*.c')
OBJECTS=$(SOURCES:%.c=%.o)

all: pe_reader.exe

pe_reader.exe: $(OBJECTS)
	gcc -o $@ $(OBJECTS) -Wl,--strip-all

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@

clean:
	rm -f *.exe *.o

install: all
	mkdir -p /usr/local/bin
	cp -f pe_reader.exe /usr/local/bin