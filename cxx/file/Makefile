all: file_reader.exe

file_reader.exe: main.o file_reader.o
	g++ -o file_reader main.o file_reader.o -Wall -O

main.o: main.cc
	gcc -c main.cc -o main.o -Wall -std=c++11 -Wno-write-strings -Wno-unused-variable -Wno-unused-but-set-variable

file_reader.o: file_reader.cc
	gcc -c file_reader.cc -o file_reader.o -Wall -std=c++11 -Wno-write-strings -Wno-unused-variable -Wno-unused-but-set-variable

clean:
	rm -f *.o *.exe

test: all
	./keywords