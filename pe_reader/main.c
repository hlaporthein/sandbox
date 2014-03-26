#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include <ctype.h>
#include <unistd.h>

#include "pe_reader.h"

#define VERSION "0.0.1"

char *progname = NULL;

void help(int status) {
	fprintf(stderr, "Usage: %s [-xh] <file>\n", basename(progname));
	exit(status);
}

void version() {
	fprintf(stderr, "Version: %s\n", VERSION);
	exit(0);
}

int xflag = 0;
int dflag = 0;
char *file = NULL;

void manage_options(int argc, char *argv[]) {
	int index;
	int c;

	opterr = 0;

	while ((c = getopt(argc, argv, "vxhd")) != -1) {
		switch (c) {
			case 'h':
				help(0);
				break;
			case 'x':
				xflag = 1;
				break;
			case 'v':
				version();
				break;
			case 'd':
				dflag = 1;
				break;
			case '?':
				if (isprint(optopt)) {
					fprintf(stderr, "Unknown option '-%c'.\n", optopt);
				} else {
					fprintf(stderr, "Unknown option character '\\x%x'.\n", optopt);
				}
				help(1);
			default:
				help(1);
		}
	}

	if (optind != argc -1) {
		fprintf(stderr, "Need one filename.\n");
		help(1);
	}

	file = argv[optind];
}

int main(int argc, char *argv[]) {
	progname = argv[0];

	manage_options(argc, argv);

	if (xflag) {
		print_hexa2(file);
	} else {
		pe_reader(file);
	}
	return 0;
}

