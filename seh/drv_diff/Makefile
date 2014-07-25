WINDDK = $(shell echo $${WINDDK:-C:\\WinDDK\\7600.16385.1})

CL = $(WINDDK)\\bin\\x86\\x86\\cl.exe
LINK = $(WINDDK)\\bin\\x86\\x86\\link.exe

all:
	#DRIVER1
	$(CL)                               \
	//c                                 \
	//Fod1.obj                          \
	//Gz                                \
	//I$(WINDDK)\\inc\\ddk              \
	//I$(WINDDK)\\inc\\crt              \
	//I$(WINDDK)\\inc\\api              \
	//D_X86_=1                          \
	//DSTD_CALL                         \
	//hotpatch                          \
	.\\d1.c
	#//GS //EHs-c-

	$(LINK)                                                    \
	//out:.\\d1.sys                                        \
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
	.\\d1.obj

	#DRIVER2
	$(CL)                               \
	//c                                 \
	//Fod2.obj                      \
	//EHs-c-                            \
	//Gz                                \
	//GS                                \
	//I$(WINDDK)\\inc\\ddk              \
	//I$(WINDDK)\\inc\\crt              \
	//I$(WINDDK)\\inc\\api              \
	//D_X86_=1                          \
	//DSTD_CALL                         \
	//hotpatch                          \
	.\\d2.c

	$(LINK)                                                    \
	//out:.\\d2.sys                                        \
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
	.\\d2.obj

	-objdump -d -M intel d1.sys > d1.s
	-objdump -d -M intel d2.sys > d2.s

clean:
	rm -rf *.obj *.sys *.s