all:
	gcc -c main.c -o main.o
	windres -i menu.rc -o menu.o
	gcc -o resource main.o menu.o #-Wl,--subsystem,windows

clean:
	rm -f *.exe *.res *.o