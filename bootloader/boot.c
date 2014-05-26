__asm__(
".code16gcc\n\t"
".org 0x0\n\t"
".global debut\n\t"
"debut:\n\t"
	"cli\n\t"
	"mov ax,cs\n\t"
	"mov ds,ax\n\t"
	"mov es,ax\n\t"
	"mov ss,ax\n\t"
	"mov bp, 0x7c00\n\t"
	"mov sp, 0x7c00\n\t"
	"sti\n\t"

	// clear screen
	"mov al, 0x02\n\t"
	"mov ah, 0x00\n\t"
	"int 0x10\n\t"

	// show cursor
	"mov cx, 0x0007\n\t"
	"mov ah, 0x01\n\t"
	"int 0x10\n\t"

	// print msg
	"lea si, msg\n\t"
"putloop:\n\t"
	"mov al,[si]\n\t"
	"add si,1\n\t"
	"cmp al,0\n\t"
	"je end_printing\n\t"
	"mov ah, 0x0e\n\t"
	"mov bx, 15\n\t"
	"int 0x10\n\t"
	"jmp putloop\n\t"
"end_printing:\n\t"

"finish:\n\t"
	"hlt\n\t"
	"jmp finish\n\t"
"msg: .asciz \"Hello SUPER OS! \"\n\t"
);

//__asm__(
//".fill (510 - (. - debut)), 1, 0\n\t"
//"BootMagic:  .int 0xAA55\n\t"
//);

