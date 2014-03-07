all: dll
	gcc -Wl,--strip-all -o strip_main.exe main.c bye.dll
	gcc -o main.exe main.c bye.dll
	pe_map.exe strip_main.exe | grep -i local > strip_str.txt
	pe_map.exe       main.exe | grep -i local >       str.txt
	diff strip_str.txt str.txt || :
	ls -la *exe
	strings strip_main.exe > strip_str.txt
	strings       main.exe >       str.txt
	diff strip_str.txt str.txt || :

dll:
	gcc -shared -Wl,--strip-all -o bye.dll bye.c

clean:
	rm -f *.exe *.txt
