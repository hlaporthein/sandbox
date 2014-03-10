all:
	gcc -Wl,--stack,42949672 -Wl,--strip-all -o recursive2 main.c
	gcc -Wl,--strip-all -o recursive1 main.c
	xxd recursive1.exe > rec1.txt
	xxd recursive2.exe > rec2.txt
	diff rec1.txt rec2.txt || :

clean:
	rm -f *exe *txt *o
