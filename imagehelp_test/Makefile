IMAGEHLP_DLL = "C:\WINDOWS\system32\imagehlp.dll"
MSVCRT_DLL = "C:\WINDOWS\system32\msvcrt.dll"

all:
	gcc -o main.o -c main.c
	gcc -o jlg_help.exe main.o -limagehlp

clean:
	rm -f *exe *o
