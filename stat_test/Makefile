all:
	gcc -c -o main.o main.c
	gcc -o stat main.o

clean:
	rm -f *exe *o

test: all
	./stat
