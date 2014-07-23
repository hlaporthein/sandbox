WINDDK = C:\\WinDDK\\7600.16385.1

CL = $(WINDDK)\\bin\\x86\\x86\\cl.exe
LINK = $(WINDDK)\\bin\\x86\\x86\\link.exe

all:
	$(CL)                               \
	//c                                 \
	//Fodriver.obj                      \
	//EHs-c-                            \
	//Gz                                \
	//I$(WINDDK)\\inc\\ddk              \
	//I$(WINDDK)\\inc\\crt              \
	//I$(WINDDK)\\inc\\api              \
	//D_X86_=1                          \
	//DSTD_CALL                         \
	//hotpatch                          \
	.\\driver.c

	$(LINK)                                       \
	//out:.\\driver.sys                           \
	//NODEFAULTLIB                                \
	//driver                                      \
	//base:0x10000                                \
	//STACK:0x40000,0x1000                        \
	//subsystem:native,6.01                       \
	//entry:DriverEntry@8                         \
	//INCREMENTAL:NO                              \
	$(WINDDK)\\lib\\win7\\i386\\ntoskrnl.lib      \
	//safeseh                                     \
	//release                                     \
	//WX                                          \
	//functionpadmin:5                            \
	.\\driver.obj

clean:
	rm -rf *.obj *.sys winddk/obj* winddk/build*