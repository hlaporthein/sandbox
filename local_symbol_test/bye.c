#include <stdio.h>

__declspec(dllexport) puts(const char*);
char * coucou = "coucou Yannis";

void bye(char *name) {
	printf("Byexxx %s.\n", name);
	puts("toto\n");
}