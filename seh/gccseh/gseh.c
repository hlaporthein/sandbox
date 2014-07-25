#include "gseh.h"

int g_eax;
int g_ebp;
int g_esp;

gseh_buf_t *g_buf = NULL;

EXCEPTION_DISPOSITION gseh_handler(EXCEPTION_RECORD *ExceptionRecord,
										void *EstablisherFrame,
										CONTEXT *ContextRecord,
										void *DispatcherContext) {
	DEBUG_START;

	gseh_buf_t *buf = g_buf;

	if (buf->magic != DRVSEH_MAGIC) {
		ERROR("Not reconize magic");
	}

	DEBUG_END;
	gseh_restore_ctx(buf, 1);

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

int gseh_finish(gseh_buf_t *buf) {
	DEBUG_START;
	set_fs0(buf->ex_reg.prev);
	buf->ex_reg.prev = NULL;
	buf->ex_reg.handler = NULL;
	buf->magic = 0xDEADBEEF;
	DEBUG_END;
	return 0;
}
