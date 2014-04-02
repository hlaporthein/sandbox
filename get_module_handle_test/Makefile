all:
	gcc -shared mylib.c mylib.def -o mylib.dll
	gcc -shared mylib2.c mylib2.def -o mylib2.dll
	gcc -Wl,--strip-all main.c mylib.dll mylib2.dll -o get_module_handle

clean:
	rm -f *.exe *.o *.dll
