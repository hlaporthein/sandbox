all:
	gcc -c main.c -o main.o
	#gcc -o zpipe main.o /usr/lib/libz.a -llog
	gcc -o zpipe main.o -lz -llog

clean:
	rm -f *.exe *.o *.z hello2.txt log.txt

test:
	./zpipe.exe < hello.txt > hello.z
	./zpipe.exe -d < hello.z > hello2.txt
	diff hello.txt hello2.txt || :
