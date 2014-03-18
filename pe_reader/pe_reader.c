#include <stdio.h>
#include <errno.h>
#include <string.h>

#define BUFFER_SIZE 1<<16

typedef unsigned short WORD;
typedef long LONG;

typedef struct _IMAGE_DOS_HEADER {      // DOS .EXE header
	WORD e_magic;                     // Magic number
	WORD e_cblp;                      // Bytes on last page of file
	WORD e_cp;                        // Pages in file
	WORD e_crlc;                      // Relocations
	WORD e_cparhdr;                   // Size of header in paragraphs
	WORD e_minalloc;                  // Minimum extra paragraphs needed
	WORD e_maxalloc;                  // Maximum extra paragraphs needed
	WORD e_ss;                        // Initial (relative) SS value
	WORD e_sp;                        // Initial SP value
	WORD e_csum;                      // Checksum
	WORD e_ip;                        // Initial IP value
	WORD e_cs;                        // Initial (relative) CS value
	WORD e_lfarlc;                    // File address of relocation table
	WORD e_ovno;                      // Overlay number
	WORD e_res[4];                    // Reserved words
	WORD e_oemid;                     // OEM identifier (for e_oeminfo)
	WORD e_oeminfo;                   // OEM information; e_oemid specific
	WORD e_res2[10];                  // Reserved words
	LONG   e_lfanew;                    // File address of new exe header
} IMAGE_DOS_HEADER, *PIMAGE_DOS_HEADER;


void pe_reader(const char *file) {
	FILE *fd = fopen(file, "rb");
	if (fd == NULL) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	IMAGE_DOS_HEADER dos_header;
	clearerr(fd);

	size_t s = fread(&dos_header, sizeof(IMAGE_DOS_HEADER), 1, fd);
	if (ferror(fd)) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	printf("PE starts at: %08x\n", dos_header.e_lfanew);

cleanup:
	if (fd) {
		fclose(fd);
	}
}