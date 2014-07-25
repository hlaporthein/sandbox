#ifndef _GSEH_H_
#define _GSEH_H_

#ifdef _DRIVER

#include <ntddk.h>
#define printf DbgPrint
#define fflush(x)

#else

#include <stdio.h>
#include <string.h>

#endif

#include <excpt.h>

extern int g_eax;
extern int g_ebp;
extern int g_esp;

#define GSEH_MAGIC 0xDEADBEEF



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

typedef struct {
	unsigned int eip;
	unsigned int ebp;
	unsigned int esp;
	unsigned int esi;
	unsigned int edi;
	unsigned int ebx;
} gseh_context_t;

typedef struct {
	EXCEPTION_REGISTRATION ex_reg;
	int magic;
	gseh_context_t ctx;
} gseh_buf_t;

extern gseh_buf_t *g_buf;

int __stdcall gseh_init(gseh_buf_t *buf);
void __stdcall gseh_restore_ctx(gseh_buf_t *buf, int retval);
int gseh_finish(gseh_buf_t *buf);
int _exception_code();
EXCEPTION_POINTERS *_exception_info();

#define GetExceptionCode _exception_code
#define GetExceptionInformation _exception_info

#define __gtry                                 \
	{                                          \
		__label__ _label_try_start;            \
		__label__ _label_except_start;         \
		__label__ _label_except_end;           \
		if (0) {                               \
			goto _label_try_start;             \
			goto _label_except_start;          \
			goto _label_except_end;            \
		}                                      \
		gseh_buf_t _gsehbuf;                   \
		g_buf = &_gsehbuf;                     \
		DEBUG_CPU_STATE("1");                  \
		int a = 0;                             \
		switch (a = gseh_init(&_gsehbuf)) {    \
			case 0:                            \
				DEBUG("case 0");               \
				DEBUG_CPU_STATE("case 0");     \
				goto _label_try_start;         \
			case 1:                            \
				DEBUG("case 1");               \
				DEBUG_CPU_STATE("case 1");     \
				goto _label_except_start;      \
		}                                      \
		DEBUG("a=0x%08X (%d)", a, a);          \
_label_try_start:                              \


#define __gexcept                       \
		goto _label_except_end;         \
_label_except_start:                    \
		;                               \


#define __gend_except                   \
_label_except_end:                      \
		gseh_finish(&_gsehbuf);         \
	}                                   \

#endif // _GSEH_H_