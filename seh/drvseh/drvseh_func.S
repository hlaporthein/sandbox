#define STDCALL_SYM(x, y)  _ ## x ##@## y
#define CDECL_SYM(x) _ ## x

#define DRVSEH_MAGIC $0xDEADBEEF

.global STDCALL_SYM(drv_init,4);

STDCALL_SYM(drv_init,4):
	popl %edx;								// eip
	popl %eax;								// arg#1=&buf

	movl %fs:0, %ecx;						// ecx = fs:0
	movl %ecx, 0(%eax);						// buf->ex_reg.prev = ecx (= fs:0)
	leal CDECL_SYM(drvseh_handler), %ecx;	// ecx = &drvseh_handler
	movl %ecx, 4(%eax);						// buf->ex_reg.handler = ecx (= &drvseh_handler)

	movl %eax, %fs:0;						// fs:0 = &buf (replace previous exception handler by ours)

	movl DRVSEH_MAGIC, 8(%eax);				// buf->magic = 0xDEADBEEF;
	movl %edx, 12(%eax);					// buf->ctx.eip = eip
	movl %ebp, 16(%eax);					// buf->ctx.ebp = ebp
	movl %esp, 20(%eax);					// buf->ctx.esp = esp

	xorl %eax, %eax; 						// return 0
	jmp *%edx; 								// return to the eip
