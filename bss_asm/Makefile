MASM32_DIR = "c:/masm32/bin"

all:
	$(MASM32_DIR)/ml //c //coff //Fomain.o //Cp main.asm
	$(MASM32_DIR)/Link //SUBSYSTEM:WINDOWS //out:main.exe main.o

clean:
	rm -f *.exe *.o