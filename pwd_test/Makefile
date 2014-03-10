all:
	gcc -c main.c -o main.o
	gcc -c pwd.c -o pwd.o
	gcc -c grp.c -o grp.o
	gcc -o test_pwd main.o pwd.o grp.o

clean:
	rm -f *.o *.exe