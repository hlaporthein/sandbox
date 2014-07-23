all: drvseh.a main.exe my_driver.sys

my_driver.sys: drvseh.a driver.c
	gcc -c -I/mingw/include/ddk -std=gnu99 -o driver.o driver.c -D__USE_DIRECT__
	gcc -Wl,--base-file,driver.base \
		-Wl,--subsystem,native \
		-Wl,--entry,_DriverEntry@8 \
		-nostartfiles -nostdlib \
		-o junk.tmp \
		driver.o \
		-lntoskrnl -lhal drvseh.a
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
		-lntoskrnl -lhal drvseh.a

main.exe: drvseh.a main.c drvseh.h
	gcc -E -Wall -Werror -o main.i main.c
	gcc -S -Wall -Werror -o main.s main.c
	gcc -Wall -Werror -o main main.c drvseh.a

drvseh.a: drvseh.h drvseh.c drvseh_func.S
	gcc -fno-asynchronous-unwind-tables -S -Wall -Werror -o drvseh.s drvseh.c
	gcc -c -Wall -Werror -o drvseh.o drvseh.c
	gcc -o drvseh_func.o -c drvseh_func.S
	ar cru drvseh.a drvseh.o drvseh_func.o

install:
	mkdir -p /usr/local/lib
	cp -f drvseh.a /usr/local/lib
	mkdir -p /usr/local/include
	cp -f drvseh.h /usr/local/include

clean:
	rm -rf *.exe *.i *.s *.o *.a