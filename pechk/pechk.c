#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <windows.h>

#include "pechk.h"

// Macro to find the offset of a field of a structure
#ifndef offsetof
#define offsetof(st, m) ((size_t) ( (char *)&((st *)(0))->m - (char *)0 ))
#endif

#define BUFFER_SIZE 0x10000

#define TRY(x) \
	if (x) { \
		result = 1; \
		goto cleanup; \
	}

#define FREAD(buffer, size_obj, nbr, fd) \
	fread(buffer, size_obj, nbr, fd); \
	if (ferror(fd)) { \
		fprintf(stderr, "ERROR FREAD: %s\n", strerror(errno)); \
		result = 1; \
		goto cleanup; \
	} \


#define FWRITE(buffer, size_obj, nbr, fd) \
	fwrite(buffer, size_obj, nbr, fd); \
	if (ferror(fd)) { \
		fprintf(stderr, "ERROR FWRITE: %s\n", strerror(errno)); \
		result = 1; \
		goto cleanup; \
	} \

#define FSEEK(fd, offset) \
	if (fseek(fd, offset, SEEK_SET) != 0) { \
		fprintf(stderr, "ERROR FSEEK: %s\n", strerror(errno)); \
		result = 1; \
		goto cleanup; \
	} \

// NT Signature + IMAGE_FILE_HEADER + Most of IMAGE_OPTIONAL_HEADER
// This is relative to the PE Header Offset
#define CHECKSUM_OFFSET sizeof(DWORD)+sizeof(IMAGE_FILE_HEADER)+offsetof(IMAGE_OPTIONAL_HEADER, CheckSum)


int get_current_pe_checksum(const char *filename, unsigned int *checksum) {
	int result = 0;

	IMAGE_DOS_HEADER dosh;
	FILE *fd = NULL;
	fd = fopen(filename, "rb");
	if (!fd) {
		printf("Cannot open %s: (%d) %s\n", filename, errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	FREAD(&dosh, sizeof(IMAGE_DOS_HEADER), 1, fd);
	FSEEK(fd, dosh.e_lfanew + CHECKSUM_OFFSET);
	FREAD(checksum, sizeof(int), 1, fd);

cleanup:
	if (fd) {
		fclose(fd);
	}
	return result;
}

int update_pe_checksum(const char *filename) {
	int result = 0;

	unsigned int expected_checksum = 0;
	TRY(compute_pe_checksum(filename, &expected_checksum));

	FILE *fd = NULL;
	fd = fopen(filename, "rb+");
	if (!fd) {
		printf("Cannot open %s: (%d) %s\n", filename, errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	IMAGE_DOS_HEADER dosh;
	FREAD(&dosh, sizeof(IMAGE_DOS_HEADER), 1, fd);
	FSEEK(fd, dosh.e_lfanew + CHECKSUM_OFFSET);
	FWRITE(&expected_checksum, sizeof(unsigned int), 1, fd);

cleanup:
	if (fd) {
		fclose(fd);
	}
	return result;
}

int compute_pe_checksum(const char *filename, unsigned int *chksum) {
	int result = 0;

	unsigned int current_checksum = 0;
	TRY(get_current_pe_checksum(filename, &current_checksum));

	FILE *fd = NULL;
	fd = fopen(filename, "rb");
	if (!fd) {
		printf("Cannot open %s: (%d) %s\n", filename, errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	unsigned char buffer[BUFFER_SIZE];
	unsigned int chk = 0;
	size_t size = 0;
	int file_size = 0;
	while ((size = fread(buffer, 1, BUFFER_SIZE, fd)) > 0) {
		file_size += size;
		chk = checksum((short int) chk, (unsigned short int *) buffer, size / 2);
	}

	if (size & 1) {
		chk += buffer[size - 1];
		chk = (chk >> 16) + (chk & 0x0000FFFF);
	}

	// Adjust the checksum
	unsigned int yy = ((chk - 1 < current_checksum) ? (chk - 1) : chk) - current_checksum;
	yy = (yy & 0xFFFF) + (yy >> 16);
	yy = (yy & 0xFFFF) + (yy >> 16);
	*chksum = yy + file_size;

cleanup:
	if (fd) {
		fclose(fd);
	}
	return result;
}

unsigned short int checksum(short int oldChk, unsigned short int *ptr, int len) {
	unsigned int c = oldChk, l, j;
	while (len) {
		l = min(len, 0x4000);
		len -= l;
		for (j = 0; j < l; j++) {
			c += *ptr;
			ptr++;
		}
		c = (c&0xffff) + (c>>16);
	}
	c = (c&0xffff) + (c>>16);
	return (unsigned short int) c;
}