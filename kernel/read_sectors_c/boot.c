__asm__(
".code16gcc\n\t"
".section .boot,\"x\" \n\t"
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

#define __NOINLINE  __attribute__((noinline))
#define __REGPARM   __attribute__ ((regparm(3)))
#define __NORETURN  __attribute__((noreturn))
#define __MBR_SECTION __attribute__ ((section (".boot")))


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

void __MBR_SECTION __NOINLINE __REGPARM reset_hard_drive() {
	__asm__ __volatile__ (
		"mov  dl, iBootDrive\n\t"
		"int  0x13" : : "a"(0x0000));
}

#define SECTORS_PER_TRACK 18
#define HEAD_NBR 2

int __MBR_SECTION __NOINLINE get_hard_drive() {
	int result;
	__asm__ __volatile__ (
		"xor dx,dx\n\t"
		"mov dl, iBootDrive\n\t" :
			"=d"(result)
	);
	return result;
}

void __MBR_SECTION __NOINLINE __REGPARM load_sectors(int lba) {
	int cylinder_ch = (lba / SECTORS_PER_TRACK) / HEAD_NBR;
	int head_dh = (lba / SECTORS_PER_TRACK) % HEAD_NBR;
	int sector_cl = (lba % SECTORS_PER_TRACK) + 1;
	int drive_dl = get_hard_drive();
	int dx = (head_dh << 8) | drive_dl;
	int cx = (cylinder_ch << 8) | sector_cl;
	__asm__ __volatile__ ("int  0x13" : : "a"(0x0201), "b"(0x7c00 + 0x200*lba),
		"c" (cx),
		"d"(dx) );
}

void __MBR_SECTION start() {
	reset_hard_drive();
	load_sectors(1);
	clean_screen();
	show_cursor(0x0007);
	print("Hello JLG...\n\r");
	print("Hello Suzana GUENEGO...\n\r");
	reboot("Press any key to reboot...");
}

