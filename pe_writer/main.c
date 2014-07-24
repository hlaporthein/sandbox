#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include <ctype.h>
#include <unistd.h>
#include <pechk.h>

#include "pe_writer.h"

#define VERSION "0.0.1"

char *progname = NULL;

void help(int status) {
	fprintf(stderr, "Usage: %s [-hv] <pe_file> <cmd_file>\n", basename(progname));
	exit(status);
}

void version() {
	fprintf(stderr, "Version: %s\n", VERSION);
	exit(0);
}

char *pe_file = NULL;
char *cmd_file = NULL;

void manage_options(int argc, char *argv[]) {
	int index;
	int c;

	opterr = 0;

	while ((c = getopt(argc, argv, "vh")) != -1) {
		switch (c) {
			case 'h':
				help(0);
				break;
			case 'v':
				version();
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

	if (optind != argc - 2) {
		fprintf(stderr, "Need two args: <pe_file> <cmd_file>  .\n");
		help(1);
	}

	pe_file = argv[optind];
	cmd_file = argv[optind + 1];
}

int main(int argc, char *argv[]) {
	int result = 0;

	progname = argv[0];

	manage_options(argc, argv);

	printf("pe_file: %s\n", pe_file);
	printf("cmd_file: %s\n", cmd_file);

	TRY(pe_writer());
	TRY(update_pe_checksum(pe_file));

cleanup:
	return result;
}

