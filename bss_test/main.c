#include <stdio.h>

const char name[50000] = "coucou";

char firstname[32] __attribute__ ((section (".jlg"))) = "Jean-Louis";

int main() {
	puts("Hello World!");
	puts(firstname);
	//snprintf(name, 1024, "coucou.");
	puts(name);
	return 0;
}