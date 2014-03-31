MASM32_DIR = "c:/masm32"
DRIVE_C = "c:"

all:
	gcc -c lib.cc -o lib.o
	gcc -shared lib.o -o lib.dll -lstdc++ -Wl,--strip-all

	$(MASM32_DIR)/bin/ml //c //coff //Folibasm.o //Cp lib.asm
	ld -shared libasm.o $(DRIVE_C)/Windows/System32/msvcrt.dll -o libasm.dll

	gcc -c main.c -o main.o
	gcc main.o libasm.dll

	$(MASM32_DIR)/bin/ml //c //coff //Foextern.o //Cp extern.asm
	ld extern.o libasm.dll $(DRIVE_C)/Windows/System32/kernel32.dll -o extern.exe

clean:
	rm -f *.exe *.o *.dll
