#include <stdio.h>
#include <stdlib.h>

#include "pe_reader.h"

char *basename(const char *);

int main(int argc, char *argv[]) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <file>\n", basename(argv[0]));
		exit(1);
	}

	char *file = argv[1];
	print_hexa(file);
	return 0;
}