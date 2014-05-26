CFLAGS = -Wall -Werror -O

all:
	gcc -c $(CFLAGS) -std=gnu99 synchro.c -o synchro.o
	gcc -c $(CFLAGS) -std=gnu99 main.c -o main.o

	gcc -shared -o synchro.dll $(CFLAGS) synchro.o
	gcc -o synchro $(CFLAGS) main.o synchro.dll


clean:
	rm -f *.o *.exe *.dll

test:
	./synchro ./src ./dest