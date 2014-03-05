all:
	gcc -c main.c -o main.o -fprofile-arcs -ftest-coverage -std=c99
	gcc main.o -o hello -fprofile-arcs -ftest-coverage

clean:
	rm -rf *.o *.exe *.gcov *.gcda *.gcno