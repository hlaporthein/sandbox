all: gseh.a main.exe my_driver.sys

my_driver.sys: gsehdd.a driver.c cmd.txt
	gcc -c -I/mingw/include/ddk -std=gnu99 -o driver.o driver.c
	gcc -Wl,--base-file,driver.base \
		-Wl,--subsystem,native \
		-Wl,--entry,_DriverEntry@8 \
		-nostartfiles -nostdlib \
		-o junk.tmp \
		driver.o \
		-lntoskrnl -lhal gsehdd.a
	rm junk.tmp
	dlltool --dllname my_driver.sys \
		--base-file driver.base --output-exp driver.exp
	rm driver.base
	gcc -mdll \
		-Wl,--entry,_DriverEntry@8 \
		-Wl,--subsystem,native \
		-Wl,--image-base,0x10000 \
		-Wl,--file-alignment,0x1000 \
		-Wl,--section-alignment,0x1000 \
		-Wl,--stack,0x40000 \
		-Wl,driver.exp \
		-nostdlib -nostartfiles \
		-o my_driver.sys \
		driver.o \
		-lntoskrnl -lhal gsehdd.a

main.exe: gsehd.a main.c gseh.h
	gcc -E -Wall -Werror -o main.i main.c
	gcc -S -Wall -Werror -o main.s main.c
	gcc -D_DEBUG -Wall -Werror -o main main.c gsehd.a

gseh.a: gseh.h gseh.c gseh_func.S
	gcc -fno-asynchronous-unwind-tables -S -Wall -Werror -o gseh.s gseh.c
	gcc -c -Wall -Werror -o gseh.o gseh.c
	gcc -o gseh_func.o -c gseh_func.S
	ar cru gseh.a gseh.o gseh_func.o

gsehd.a: gseh.h gseh.c gseh_func.S
	gcc -fno-asynchronous-unwind-tables -S -Wall -Werror -o gseh.s gseh.c
	gcc -D_DEBUG -c -Wall -Werror -o gseh.o gseh.c
	gcc -o gseh_func.o -c gseh_func.S
	ar cru gsehd.a gseh.o gseh_func.o

gsehdd.a: gseh.h gseh.c gseh_func.S
	gcc -fno-asynchronous-unwind-tables -S -Wall -Werror -o gseh.s gseh.c
	gcc -I/mingw/include/ddk -D_DRIVER -D_DEBUG -c -Wall -Werror -o gseh.o gseh.c
	gcc -o gseh_func.o -c gseh_func.S
	ar cru gsehdd.a gseh.o gseh_func.o

install:
	mkdir -p /usr/local/lib
	cp -f gseh.a /usr/local/lib
	mkdir -p /usr/local/include
	cp -f gseh.h /usr/local/include

clean:
	rm -rf *.exe *.i *.s *.o *.a *.sys *.exp