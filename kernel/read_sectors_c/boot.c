#define STRING(x) STRING2(x)
#define STRING2(x) #x

#define SECTORS_PER_TRACK 18
#define HEAD_NBR 2

#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __NORETURN  __attribute__((noreturn))

__asm__(
".code16gcc\n\t"
".org 0x0\n\t"
".global debut\n\t"
"debut:\n\t"
	"cli\n\t"
	"mov iBootDrive,dl\n\t"
	"mov ax,cs\n\t"
	"mov ds,ax\n\t"
	"mov es,ax\n\t"
	"mov ss,ax\n\t"
	"mov bp, 0x7c00\n\t"
	"mov sp, 0x7c00\n\t"
	"sti\n\t"
	"call start\n\t"
	"iBootDrive: .byte  0\n\t"
);

void __NOINLINE __REGPARM load_sectors(int lba, int dest) {
	__asm__ __volatile__ (
		"xor cx, cx\n\t"
		"readsect:\n\t"
		"push ax\n\t"
		"push cx\n\t"
		"push bx\n\t"
		"mov bx, " STRING(SECTORS_PER_TRACK) "\n\t"
		"xor dx, dx\n\t"
		"div bx\n\t"
		"inc dx\n\t"
		"mov cl, dl\n\t"
		"mov bx," STRING(HEAD_NBR) "\n\t"
		"xor dx, dx\n\t"
		"div bx\n\t"
		"mov ch, al\n\t"
		"xchg dl, dh\n\t"
		"mov ax, 0x0201\n\t"
		"mov dl, iBootDrive\n\t"
		"pop bx\n\t"
		"int 0x13\n\t"
		"jc readfail\n\t"
		"pop cx\n\t"
		"pop ax\n\t"
		"jmp readsect_end\n\t"
		"readfail:\n\t"
		"pop cx\n\t"
		"inc cx\n\t"
		"cmp cx, word ptr 4\n\t"
		"je bootFailure\n\t"
		"xor ax, ax\n\t"
		"int 0x13\n\t"
		"pop ax\n\t"
		"jmp readsect\n\t"
		"readsect_end:\n\t"
		: : "a"(lba), "b"(dest));

}

void __NOINLINE __REGPARM reset_hard_drive() {
	__asm__ __volatile__ (
		"xor ax, ax\n\t"
		"int 0x13\n\t"
		"jc bootFailure\n\t");
}

void __NOINLINE __REGPARM print(const char *s) {
	while(*s) {
		__asm__ __volatile__ ("int  0x10" : : "a"(0x0E00 | *s), "b"(7));
		s++;
	}
}

void __NOINLINE __REGPARM wait_key() {
	__asm__ __volatile__ ("int  0x16" : : "a"(0x0000));
}

void __NOINLINE __REGPARM reboot() {
	print("Press any key to reboot...");
	wait_key();
	__asm__ __volatile__ (
		".byte  0xEA\n\t"
		".word  0x0000\n\t"
		".word  0xFFFF\n\t"
	);
}

void __NOINLINE __REGPARM bootFailure() {
	print("Disk Error...\n\r");
	reboot();
}

void __NOINLINE __REGPARM show_cursor(int cursor_shape) {
	__asm__ __volatile__ ("int  0x10" : : "a"(0x0100), "c"(cursor_shape));
}

void __NOINLINE __REGPARM clean_screen() {
	__asm__ __volatile__ ("int  0x10" : : "a"(0x0002));
}


void __NOINLINE __REGPARM run_second_stage();

void __NOINLINE __REGPARM start() {
	//clean_screen();
	//show_cursor(0x0007);
	print("Starting...\n\r");
	print("Hey\n\r");
	reset_hard_drive();
	load_sectors(1, 0x7c00 + 0x200);
	run_second_stage();
	reboot();
}

