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
);

void __NOINLINE __REGPARM print(const char *s);

void __NOINLINE __REGPARM display(const char *s) {
	while(*s) {
		__asm__ __volatile__ ("int  0x10" : : "a"(0x0E00 | *s), "b"(7));
		s++;
	}
}

void __NOINLINE __REGPARM run_second_stage() {
	display("This is the second stage.\n\r");
	print("It is working...\n\r");
	display("Coucou JL\n\r");
}