#include <stdio.h>

void jlg_log(const char *message) {
	FILE *stream = fopen("log.txt", "a");
	fprintf(stream, message);
	fprintf(stream, "\n");
	fclose(stream);
}