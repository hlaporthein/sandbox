#include "drvseh.h"

int g_eax;
int g_ebp;
int g_esp;

EXCEPTION_DISPOSITION drvseh_handler(EXCEPTION_RECORD *ExceptionRecord,
										void *EstablisherFrame,
										CONTEXT *ContextRecord,
										void *DispatcherContext) {
	DEBUG_START;

	drv_buf_t *buf = (drv_buf_t *) EstablisherFrame;

	if (buf->magic != DRVSEH_MAGIC) {
		ERROR("Not reconize magic");
	}

	SET_STACK_FRAME(buf->ctx.ebp, buf->ctx.esp);

	DEBUG_END;
	JMP_AND_RET(1, buf->ctx.eip);

	ERROR_UNREACHABLE;
	return ExceptionContinueSearch;
}

int get_fs0() {
	int fs0 = 0xFFFFFFFF;
	__asm__ __volatile__(
		"movl %%fs:0, %0"
		: "=r" (fs0)
	);
	return fs0;
}

void set_fs0(void *val) {
	__asm__ __volatile__(
		"movl %0, %%fs:0"
		: : "r" (val)
	);
}

//int drv_init(drv_buf_t *buf) {
//	DEBUG_START;
//	DEBUG_CPU_STATE("2");
//
//	int eip = 0;
//	GET_EIP(eip);
//	int ebp = 0;
//	int esp = 0;
//	GET_STACK_FRAME_BEFORE_CALL(ebp, esp);
//
//	DEBUG("eip= 0x%08X", eip);
//
//	buf->ex_reg.prev = (EXCEPTION_REGISTRATION *) get_fs0();
//	buf->ex_reg.handler = drvseh_handler;
//	set_fs0(buf);
//
//	buf->magic = DRVSEH_MAGIC;
//
//	// TODO: Memorize context
//	buf->ctx.eip = eip;
//	buf->ctx.esp = esp;
//	buf->ctx.ebp = ebp;
//
//	DEBUG_BUF(buf);
//
//	DEBUG_END;
//	//SET_STACK_FRAME(buf->ctx.ebp, buf->ctx.esp);
//	JMP_AND_RET(0, eip);
//
//	ERROR_UNREACHABLE;
//	return -1;
//}

int drv_end_except(drv_buf_t *buf) {
	return 0;
}

int drv_finish(drv_buf_t *buf) {
	DEBUG_START;
	set_fs0(buf->ex_reg.prev);
	buf->ex_reg.prev = NULL;
	buf->ex_reg.handler = NULL;
	buf->magic = 0xDEADBEEF;
	DEBUG_END;
	return 0;
}
