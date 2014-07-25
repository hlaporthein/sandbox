#include "gseh.h"

int g_eax;
int g_ebp;
int g_esp;

int g_exception_code = 0;
EXCEPTION_POINTERS g_exception_info;
EXCEPTION_RECORD g_ExceptionRecord;
CONTEXT g_ContextRecord;

gseh_buf_t *g_buf = NULL;

int _exception_code() {
	return g_exception_code;
}

EXCEPTION_POINTERS *_exception_info() {
	return &g_exception_info;
}

EXCEPTION_DISPOSITION gseh_handler(EXCEPTION_RECORD *ExceptionRecord,
										void *EstablisherFrame,
										CONTEXT *ContextRecord,
										void *DispatcherContext) {
	DEBUG_START;

	gseh_buf_t *buf = g_buf;

	g_exception_code = ExceptionRecord->ExceptionCode;
	g_exception_info.ExceptionRecord = &g_ExceptionRecord;
	g_exception_info.ContextRecord = &g_ContextRecord;

	memcpy(&g_ExceptionRecord, ExceptionRecord, sizeof(EXCEPTION_RECORD));
	memcpy(&g_ContextRecord, ContextRecord, sizeof(CONTEXT));

	if (buf->magic != GSEH_MAGIC) {
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
