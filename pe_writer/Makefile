CFLAGS=-std=gnu99
SOURCES=$(shell find . -name '*.c')
OBJECTS=$(SOURCES:%.c=%.o)

all: pe_writer.exe

pe_writer.exe: $(OBJECTS)
	gcc -o $@ $(OBJECTS) -Wl,--strip-all -lcfgmap -lpechk

%.o: %.c
	gcc $(CFLAGS) -c $< -o $@
	gcc -E $(CFLAGS) $< -o $@.i

clean:
	rm -f *.exe *.o *.i

install: all
	mkdir -p /usr/local/bin
	cp -f pe_writer.exe /usr/local/bin