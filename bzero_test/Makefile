all:
	gcc -S -o main1.s main.c -fverbose-asm
	gcc -S -O -o main2.s main.c -fverbose-asm
	gcc -c -o main.o main.c -O -fopt-info-all -std=c90
	gcc main.o -o bzero
clean:
	rm -f *.o *.exe *.s