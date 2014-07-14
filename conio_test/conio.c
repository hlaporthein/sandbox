#include <conio.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

int test__cgets() {
	printf("Enter something:\n");
	char overflow = 0;
	char buffer[10 + 3];
	char *s = NULL;
	memset(buffer, 0, 10 + 3);
	buffer[0] = 10;
	s = _cgets(buffer);
	if (!s) {
		perror("We have an error.");
	}
	if (s == buffer) {
		printf("s == buffer\n");
	}
	printf("s=0x%08X\n", s);
	printf("buffer=0x%08X\n", buffer);
	printf("&overflow=0x%08X\n", &overflow);
	printf("&overflow-buffer=%d\n", &overflow - buffer);
	printf("overflow=%d\n", overflow);
	printf("buffer[1]=%d\n", buffer[1]);
	printf("buffer=%s\n", buffer + 2);
	printf("s=%s\n", s);

	return 0;
}

int test__cprintf() {
	int      i = -16, h = 0x3A;
	unsigned u = 62511;
	char     c = 'A';
	char     s[] = "Test";

	/* Note that console output does not translate \n as
	* standard output does. Use \r\n instead.
	*/
	_cprintf("%d--%.4x--%u--%c-%s\n%d---%.*s---", i, h, u, c, s, i, 3, s);
	return 0;
}

int test__cputs() {
	_cputs("hello jl...");
	_cputs("hello jl...");
	return 0;
}

int test__cscanf() {
	int result;
	int i[3];

	_cprintf("Enter three integers:\n");
	result = _cscanf("%i %i %i", &i[0], &i[1], &i[2]);
	printf("\nYou entered:\n");
	while (result--) {
		printf("%i ", i[result]);
	}
	printf("\n");
}

int test_getch() {
	printf("Please enter a character:\n");
	char c = (char) getch();
	printf("You have entered: %c\n", c);
	return 0;
}

int test_getche() {
	printf("Please enter a character:\n");
	char c = (char) getche();
	printf("You have entered: %c\n", c);
	return 0;
}

int test_kbhit() {
	printf("About to do a hit key test:\n");
	int k = kbhit();
	printf("k = %d\n", k);
	printf("Wait until key hit:\n");
	while ((k = kbhit()) == 0) {
		// test again
	}
	printf("key hit. code = %d\n", k);
	printf("key hit. char = %c\n", (char) getch());
	return 0;
}

int test_putch() {
	printf("putch(65)=");
	putch(65);
	printf("\n");
	return 0;
}


int test_ungetch() {
	printf("Please enter a character:\n");
	int c = (char) getch();
	printf("char = %c\n", c);
	ungetch(c);
	printf("again char = %c\n", (char) getch());
	return 0;
}

int main() {
	//test__cgets();
	//test__cprintf();
	//test__cputs();
	//test__cscanf();
	//test_getch();
	//test_getche();
	test_kbhit();
	//test_putch();
	//test_ungetch();
	return 0;
}

