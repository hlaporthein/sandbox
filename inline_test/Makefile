all:
	gcc -S main1.c -o main1.s
	gcc -S main2.c -o main2.s
	gcc -c main1.s -o main.o
	gcc main.o -o hello
	diff main1.s main2.s ||:

clean:
	rm -rf *.o *.s *.exe
