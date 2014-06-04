CFLAGS = -Wall -Werror -O

ifdef DEBUG
CFLAGS += -DDEBUG_MODE
endif

all:
	gcc -c $(CFLAGS) main.c -o main.o
	gcc -o watch $(CFLAGS) main.o

clean:
	rm -rf *.o *.exe *.dll *.a ./src ./dst

test:
