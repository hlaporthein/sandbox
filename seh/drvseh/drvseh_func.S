#define STDCALL_SYM(x, y)  _ ## x ##@## y
#define CDECL_SYM(x) _ ## x

#define DRVSEH_MAGIC $0xDEADBEEF

// When using the macro, be carefull to not insert space before args.
// STDCALL_SYM(drv_init, 4); will make a syntax error.
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


.global STDCALL_SYM(drv_restore_ctx,8);
STDCALL_SYM(drv_restore_ctx,8):
	addl $4, %esp;			// Return address... we don't need it.
	popl %ebx;				// Context buffer
	popl %eax;				// Return value
	movl 12(%ebx), %edx;	// eip
	movl 16(%ebx), %ebp;	// ebp
	movl 20(%ebx), %esp;	// esp
	jmp *%edx;

