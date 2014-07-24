WINDDK = $(shell echo $${WINDDK:-C:\\WinDDK\\7600.16385.1})

CL = $(WINDDK)\\bin\\x86\\x86\\cl.exe
LINK = $(WINDDK)\\bin\\x86\\x86\\link.exe

all:
	#DRIVER_WIN
	$(CL)                               \
	//c                                 \
	//Fodriver_win.obj                  \
	//EHs-c-                            \
	//Gz                                \
	//GS                                \
	//I$(WINDDK)\\inc\\ddk              \
	//I$(WINDDK)\\inc\\crt              \
	//I$(WINDDK)\\inc\\api              \
	//D_X86_=1                          \
	//DSTD_CALL                         \
	//hotpatch                          \
	.\\driver.c

	$(LINK)                                                    \
	//out:.\\driver_win.sys                                    \
	//NODEFAULTLIB                                             \
	//driver                                                   \
	//base:0x10000                                             \
	//STACK:0x40000,0x1000                                     \
	//subsystem:native,6.01                                    \
	//entry:DriverEntry@8                                      \
	//INCREMENTAL:NO                                           \
	$(WINDDK)\\lib\\win7\\i386\\ntoskrnl.lib                   \
	$(WINDDK)\\lib\\win7\\i386\\BufferOverflowK.lib            \
	//safeseh                                                  \
	//release                                                  \
	//WX                                                       \
	//functionpadmin:5                                         \
	//IGNORE:4198,4010,4037,4039,4065,4070,4078,4087,4089,4221 \
	.\\driver_win.obj

	#DRIVER_GCC
	gcc -c -I/mingw/include/ddk -std=gnu99 -o driver_gcc.o driver.c
	gcc -Wl,--base-file,driver_gcc.base \
		-Wl,--subsystem,native \
		-Wl,--entry,_DriverEntry \
		-nostartfiles -nostdlib \
		-o junk.tmp \
		driver_gcc.o \
		-lntoskrnl -lhal
	rm junk.tmp
	dlltool --dllname driver_gcc.sys \
		--base-file driver_gcc.base --output-exp driver_gcc.exp
	rm driver_gcc.base
	gcc -mdll \
		-Wl,--entry,_DriverEntry \
		-Wl,--subsystem,native \
		-Wl,--image-base,0x10000 \
		-Wl,--file-alignment,0x1000 \
		-Wl,--section-alignment,0x1000 \
		-Wl,--stack,0x40000 \
		-Wl,driver_gcc.exp \
		-nostdlib -nostartfiles \
		-o driver_gcc.sys \
		driver_gcc.o \
		-lntoskrnl -lhal

	-objdump -d -M intel driver_win.sys > driver_win.s
	-objdump -d -M intel driver_gcc.sys > driver_gcc.s
	-diff driver_win.s driver_gcc.s > driver.diff

clean:
	rm -rf *.obj *.sys *.s *.o *.exp *.diff