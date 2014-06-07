all:
	gcc -c -o main.o main.c
	gcc -o fat_reader.exe main.o

fat:
	dd if=/dev/sdb of=fat.bin bs=512 count=1000