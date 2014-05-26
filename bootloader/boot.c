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
	"call start\n\t"
);

#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __NORETURN  __attribute__((noreturn))

/* BIOS interrupts must be done with inline assembly */
void __NOINLINE __REGPARM print(const char *s) {
	while(*s){
		__asm__ __volatile__ ("int  0x10" : : "a"(0x0E00 | *s), "b"(7));
		s++;
	}
}

void __NOINLINE __REGPARM show_cursor(int cursor_shape) {
	__asm__ __volatile__ ("int  0x10" : : "a"(0x0100), "c"(cursor_shape));
}

void __NOINLINE __REGPARM clean_screen() {
	__asm__ __volatile__ ("int  0x10" : : "a"(0x0002));
}

void __NOINLINE __REGPARM wait_key() {
	__asm__ __volatile__ ("int  0x16" : : "a"(0x0000));
}

void __NOINLINE __REGPARM reboot(const char *s) {
	print(s);
	wait_key();
	__asm__ __volatile__ (
		".byte  0xEA\n\t"
		".word  0x0000\n\t"
		".word  0xFFFF\n\t"
	);
}

void start() {
	clean_screen();
	show_cursor(0x0607);
	print("Hello JLG...\n\r");
	print("Hello Suzana GUENEGO...\n\r");
	reboot("Press any key to reboot...");
}

