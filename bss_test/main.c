#include <stdio.h>

char name[5000];

int main() {
	puts("Hello World!");
	snprintf(name, 1024, "coucou.");
	puts(name);
	return 0;
}