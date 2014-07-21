all:
	gcc -E -Wall -Werror -I /usr/local/include/seh -o seh.i main.c -lseh
	gcc -Wall -Werror -I /usr/local/include/seh -o seh main.c -lseh

clean:
	rm -rf *.exe *.i