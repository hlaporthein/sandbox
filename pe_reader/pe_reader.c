#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "pe_reader.h"
#include "my_winnt.h"

void pe_reader(const char *file) {
	init_map();

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

	if (dos_header.e_magic != 0x5A4D) {
		fprintf(stderr, "ERROR: This is not a DOS file.\n");
		goto cleanup;
	}

	printf("PE starts at: %08x\n", dos_header.e_lfanew);

	if (fseek(fd, dos_header.e_lfanew, SEEK_SET) != 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	IMAGE_NT_HEADERS nt_header;
	s = fread(&nt_header, sizeof(IMAGE_NT_HEADERS), 1, fd);
	if (ferror(fd)) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	if (nt_header.Signature != IMAGE_NT_SIGNATURE) {
		fprintf(stderr, "ERROR: This is not a PE file: %08x\n", nt_header.Signature);
		goto cleanup;
	}

	printf("PE signature: %08x\n", nt_header.Signature);
	printf("Machine: %s\n", map(SECTION_MACHINE, nt_header.FileHeader.Machine));
//	WORD  Machine;
//	WORD  NumberOfSections;
//	DWORD TimeDateStamp;
//	DWORD PointerToSymbolTable;
//	DWORD NumberOfSymbols;
//	WORD  SizeOfOptionalHeader;
//	WORD  Characteristics;

cleanup:
	if (fd) {
		fclose(fd);
	}
}