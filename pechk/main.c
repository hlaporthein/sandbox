#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>

#include <ctype.h>
#include <unistd.h>

#include "pechk.h"

int u_flag = 0;
char *pe_file = NULL;

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

void manage_options(int argc, char *argv[]) {
	int c;

	opterr = 0;

	while ((c = getopt(argc, argv, "vhu")) != -1) {
		switch (c) {
			case 'h':
				help(0);
				break;
 			case 'v':
				version();
				break;
			case 'u':
				u_flag = 1;
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

	if (optind != argc - 1) {
		fprintf(stderr, "Need one arg: <pe_file>.\n");
		help(1);
	}

	pe_file = argv[optind];
}

int main(int argc, char *argv[]) {
	int result = 0;

	manage_options(argc, argv);

	unsigned int current_checksum = 0;
	result = get_current_pe_checksum(pe_file, &current_checksum);
	printf("%s  current checksum=0x%08X\n", pe_file, current_checksum);

	unsigned int expected_checksum = 0;
	result = compute_pe_checksum(pe_file, &expected_checksum);
	printf("%s expected checksum=0x%08X\n", pe_file, expected_checksum);


	if (u_flag) {
		result = update_pe_checksum(pe_file);
		result = get_current_pe_checksum(pe_file, &current_checksum);
		printf("%s      new checksum=0x%08X\n", pe_file, current_checksum);
	}

	return result;
}

