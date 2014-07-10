CFLAGS = -Wall -Werror -I/mingw/include/ddk -std=gnu99 -Wno-multichar

all:
	#FIRST DRIVER
	gcc -c $(CFLAGS) -o driver.o driver.c
	gcc -Wl,--base-file,driver.base \
		-Wl,--subsystem,native \
		-Wl,--entry,_DriverEntry@8 \
		-nostartfiles -nostdlib \
		-o junk.tmp \
		driver.o \
		-lntoskrnl -lhal
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
		-lntoskrnl -lhal
	#TESTER
	gcc -o tester tester.c

clean:
	rm -f *sys *o *base *tmp *pe *exp *def *dll.a *exe *i *s
