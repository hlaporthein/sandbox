#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

void print_hexa(const char *file) {
	FILE *fd = fopen(file, "rb");
	if (fd == NULL) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	unsigned char buffer[17];
	memset(buffer, 0, 17);
	size_t s = 0;
	int address = 0;

	clearerr(fd);
	while ((s = fread(buffer, sizeof(unsigned char), 16, fd)) > 0) {
		char buffer_hexa[64] = "";
		for (int i = 0; i < s; i++) {
			char space[2] = "";
			if (i % 4 == 0) {
				snprintf(space, 2, " ");
			}
			char str[32];
			int x = (int) buffer[i];
			snprintf(str, 32, " %s%02x", space, x);
			strcat(buffer_hexa, str);
		}
		printf("%08x:%s\n", address, buffer_hexa);
		address += 16;
	}
	if (ferror(fd)) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}
	if (feof(fd)) {
		printf("File end.\n");
	}

cleanup:
	if (fd) {
		fclose(fd);
	}
}

void print_hexa2(const char *file) {
	int fh = open(file, O_RDONLY | O_BINARY);
	if (fh == -1) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	unsigned char buffer[17];
	memset(buffer, 0, 17);
	size_t s = 0;
	int address = 0;

	while ((s = read(fh, buffer, 16)) > 0) {
		char buffer_hexa[64] = "";
		for (int i = 0; i < s; i++) {
			char space[2] = "";
			if (i % 4 == 0) {
				snprintf(space, 2, " ");
			}
			char str[32];
			int x = (int) buffer[i];
			snprintf(str, 32, " %s%02x", space, x);
			strcat(buffer_hexa, str);
		}
		printf("%08x:%s\n", address, buffer_hexa);
		address += 16;
	}

cleanup:
	if (fh != -1) {
		close(fh);
	}
}