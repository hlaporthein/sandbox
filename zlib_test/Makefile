all:
	gcc -c main.c -o main.o
	gcc -o zpipe main.o -lz

clean:
	rm -f *.exe *.o *.z

test:
	./zpipe.exe < hello.txt > hello.z
	./zpipe.exe -d < hello.z > hello2.txt
	diff hello.txt hello2.txt || :
