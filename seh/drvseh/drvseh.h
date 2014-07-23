#ifndef _DRVSEH_H_
#define _DRVSEH_H_

#include <excpt.h>
#include <stdio.h>

extern int g_eax;
extern int g_ebp;
extern int g_esp;

#define DRVSEH_MAGIC 0xDEADBEEF

#ifdef _DEBUG

#define DEBUG(message, ...) \
	printf("DEBUG [" __FILE__ ":%d (%s)]: " message "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__); \
	fflush(stdout)

#define DEBUG_CPU_STATE(msg)        \
	DEBUG("CPU STATE " msg ":");    \
	__asm__ __volatile__ (          \
		"movl %%eax, %0"            \
		: "=c" (g_eax)              \
	);                              \
	DEBUG("eax=0x%08X", g_eax);     \
	__asm__ __volatile__ (          \
		"movl %%ebp, %0"            \
		: "=c" (g_ebp)              \
	);                              \
	DEBUG("ebp=0x%08X", g_ebp);     \
	__asm__ __volatile__ (          \
		"movl %%esp, %0"            \
		: "=c" (g_esp)              \
	);                              \
	DEBUG("esp=0x%08X", g_esp);     \
	DEBUG("stack_width=%d(%d)", g_ebp - g_esp, (g_ebp - g_esp) / 4);

#define DEBUG_BUF(buf) \
	DEBUG("BUF:\n\tprv=0x%08X\n\thdl=0x%08X\n\tmgc=0x%08X\n\teip=0x%08X\n\tesp=0x%08X\n\tebp=0x%08X", \
			(unsigned int) buf->ex_reg.prev, (unsigned int) buf->ex_reg.handler, buf->magic, buf->ctx.eip, buf->ctx.esp, buf->ctx.ebp); \

#define ERROR(message, ...) \
	printf("ERROR [" __FILE__ ":%d (%s)]: " message "\n", __LINE__, __FUNCTION__, ##__VA_ARGS__); \
	fflush(stdout)

#define DEBUG_START DEBUG("START")
#define DEBUG_END DEBUG("END")

#define ERROR_UNREACHABLE ERROR("Unreachable")

#else

#define DEBUG(message, ...)
#define DEBUG_CPU_STATE(msg)
#define DEBUG_BUF(buf)
#define ERROR(message, ...)
#define DEBUG_START
#define DEBUG_END
#define ERROR_UNREACHABLE

#endif // _DEBUG


#define GET_EIP(eip)            \
	__asm__ __volatile__ (      \
		"movl 4(%%ebp), %0"     \
		: "=r" (eip)            \
	)                           \

#define JMP_AND_RET(ret, addr)    \
	DEBUG("About to jump to 0x%08X with eax=%d", addr, ret); \
	__asm__ __volatile__ (        \
		"jmp *%%ecx"              \
		: : "a" (ret), "c" (addr) \
	)                             \


#define SET_REGISTER(name, val) \
	__asm__ __volatile__ (      \
		"movl %0, %%" name      \
		: : "c" (val)           \
	)                           \

#define GET_STACK_FRAME_BEFORE_CALL(ebp, esp)         \
	__asm__ __volatile__ (                            \
		"movl (%%ebp), %0"                            \
		: "=r" (ebp)                                  \
	);                                                \
	__asm__ __volatile__ (                            \
		"movl %%ebp, %0"                              \
		: "=r" (esp)                                  \
	);                                                \
	esp += 8;                                         \

#define SET_STACK_FRAME(ebp, esp)                     \
	__asm__ __volatile__ (                            \
		"movl %0, %%ebp;"                             \
		"movl %1, %%esp;"                             \
		: : "a" (ebp), "c" (esp)                      \
	);                                                \

#define LEAVE()                                       \
	__asm__ __volatile__ (                            \
		"leave"                                       \
	);                                                \

typedef struct {
	unsigned int eip;
	unsigned int ebp;
	unsigned int esp;
} drv_context_t;

typedef struct {
	EXCEPTION_REGISTRATION ex_reg;
	int magic;
	drv_context_t ctx;
} drv_buf_t;

int __stdcall drv_init(drv_buf_t *buf);
void __stdcall drv_restore_ctx(drv_buf_t *buf, int retval);
int drv_end_except(drv_buf_t *buf);
int drv_finish(drv_buf_t *buf);

#define __drv_try                          \
	{                                      \
		__label__ _label_try_start;        \
		__label__ _label_except_start;     \
		__label__ _label_except_end;       \
		if (0) {                           \
			goto _label_try_start;         \
			goto _label_except_start;      \
			goto _label_except_end;        \
		}                                  \
		drv_buf_t _drvbuf;                 \
		DEBUG_CPU_STATE("1");              \
		int a = 0;                         \
		switch (a = drv_init(&_drvbuf)) {  \
			case 0:                        \
				DEBUG("case 0");           \
				DEBUG_CPU_STATE("case 0"); \
				goto _label_try_start;     \
			case 1:                        \
				DEBUG("case 1");           \
				DEBUG_CPU_STATE("case 1"); \
				goto _label_except_start;  \
		}                                  \
		DEBUG("a=0x%08X (%d)", a, a);      \
_label_try_start:                          \


#define __drv_except                     \
		goto _label_except_end;          \
_label_except_start:                     \
		;                                \



#define __drv_end_except                \
		drv_end_except(&_drvbuf);       \
_label_except_end:                      \
		drv_finish(&_drvbuf);           \
	}                                   \

#endif // _DRVSEH_H_