#include <stdio.h>
#include <windows.h>

print_context(CONTEXT *context) {
	printf("ContextFlags=%d\n", context->ContextFlags);
	printf("Dr0      =%d (0x%08X)\n", context->Dr0, context->Dr0);
	printf("Dr1      =%d (0x%08X)\n", context->Dr1, context->Dr1);
	printf("Dr2      =%d (0x%08X)\n", context->Dr2, context->Dr2);
	printf("Dr3      =%d (0x%08X)\n", context->Dr3, context->Dr3);
	printf("Dr6      =%d (0x%08X)\n", context->Dr6, context->Dr6);
	printf("Dr7      =%d (0x%08X)\n", context->Dr7, context->Dr7);
	printf("SegGs    =%d (0x%08X)\n", context->SegGs, context->SegGs);
	printf("SegFs    =%d (0x%08X)\n", context->SegFs, context->SegFs);
	printf("SegEs    =%d (0x%08X)\n", context->SegEs, context->SegEs);
	printf("SegDs    =%d (0x%08X)\n", context->SegDs, context->SegDs);
	printf("Edi      =%d (0x%08X)\n", context->Edi, context->Edi);
	printf("Esi      =%d (0x%08X)\n", context->Esi, context->Esi);
	printf("Ebx      =%d (0x%08X)\n", context->Ebx, context->Ebx);
	printf("Edx      =%d (0x%08X)\n", context->Edx, context->Edx);
	printf("Ecx      =%d (0x%08X)\n", context->Ecx, context->Ecx);
	printf("Eax      =%d (0x%08X)\n", context->Eax, context->Eax);
	printf("Ebp      =%d (0x%08X)\n", context->Ebp, context->Ebp);
	printf("Eip      =%d (0x%08X)\n", context->Eip, context->Eip);
	printf("SegCs    =%d (0x%08X)\n", context->SegCs, context->SegCs);
	printf("EFlags   =%d (0x%08X)\n", context->EFlags, context->EFlags);
	printf("Esp      =%d (0x%08X)\n", context->Esp, context->Esp);
	printf("SegSs    =%d (0x%08X)\n", context->SegSs, context->SegSs);
	//printf("FloatSave=%d\n", context->FloatSave);
	
}

int main() {
	HANDLE t =  GetCurrentThread();
	CONTEXT context;
	memset(&context, 0, sizeof(CONTEXT));
	context.ContextFlags = CONTEXT_FULL | CONTEXT_EXTENDED_REGISTERS | CONTEXT_DEBUG_REGISTERS | CONTEXT_FLOATING_POINT;
	BOOL b = GetThreadContext(t, &context);
	printf("b=%d\n", b);
	print_context(&context);
	b = GetThreadContext(t, &context);
	printf("b=%d\n", b);
	print_context(&context);
	BOOL c = SetThreadContext(t, &context);
	printf("c=%d\n", c);
	return 0;
}
