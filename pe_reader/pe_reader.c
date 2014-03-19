#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "pe_reader.h"
#include "my_winnt.h"

void pe_reader(const char *file) {
	char buffer[BUFFER_SIZE];

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

	printf("PE starts at: 0x%08x\n", dos_header.e_lfanew);

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
		fprintf(stderr, "ERROR: This is not a PE file: 0x%08x\n", nt_header.Signature);
		goto cleanup;
	}

	printf("PE signature: 0x%08x\n", nt_header.Signature);


	printf("\nCOFF Header--------------------------\n");
	// Standard header
	printf("Machine: %s\n", map(SECTION_MACHINE, nt_header.FileHeader.Machine));
	printf("Number of sections: %d\n", nt_header.FileHeader.NumberOfSections);

	time_t time_as_time_t = nt_header.FileHeader.TimeDateStamp;
	struct tm *tm = localtime(&time_as_time_t);
	printf("Creation date: %d/%d/%d - %d:%d:%d\n",
			tm->tm_mday, (1 + tm->tm_mon), (1900 + tm->tm_year), tm->tm_hour, tm->tm_min, tm->tm_sec);
	//printf("Creation date: %s", asctime(tm));

	printf("Symbol table address: 0x%08x\n", nt_header.FileHeader.PointerToSymbolTable);
	printf("Number of symbols: %d\n", nt_header.FileHeader.NumberOfSymbols);
	printf("Size of optional header: %d\n", nt_header.FileHeader.SizeOfOptionalHeader);
	printf("Characteristics:%s\n",
			get_flags(buffer, BUFFER_SIZE, SECTION_CHARACTERISTICS, nt_header.FileHeader.Characteristics));

	// Optional header
	int is_32bit = has_flags(IMAGE_FILE_32BIT_MACHINE, nt_header.FileHeader.Characteristics);
	int magic = nt_header.OptionalHeader.oh64.standard.Magic;
	int maj_link_ver = nt_header.OptionalHeader.oh32.standard.MajorLinkerVersion;
	int min_link_ver = nt_header.OptionalHeader.oh32.standard.MinorLinkerVersion;
	int size_code = nt_header.OptionalHeader.oh32.standard.SizeOfCode;
	int size_init_data = nt_header.OptionalHeader.oh32.standard.SizeOfInitializedData;
	int size_uninit_data = nt_header.OptionalHeader.oh32.standard.SizeOfUninitializedData;
	int addr_entry_pt = nt_header.OptionalHeader.oh32.standard.AddressOfEntryPoint;
	int base_code = nt_header.OptionalHeader.oh32.standard.BaseOfCode;
	if (is_32bit) {
		magic = nt_header.OptionalHeader.oh32.standard.Magic;
		maj_link_ver = nt_header.OptionalHeader.oh32.standard.MajorLinkerVersion;
		min_link_ver = nt_header.OptionalHeader.oh32.standard.MinorLinkerVersion;
		size_code = nt_header.OptionalHeader.oh32.standard.SizeOfCode;
		size_init_data = nt_header.OptionalHeader.oh32.standard.SizeOfInitializedData;
		size_uninit_data = nt_header.OptionalHeader.oh32.standard.SizeOfUninitializedData;
		addr_entry_pt = nt_header.OptionalHeader.oh32.standard.AddressOfEntryPoint;
		base_code = nt_header.OptionalHeader.oh32.standard.BaseOfCode;
	}

	//32bit
	printf("\nOptional Header----------------------\n");
	printf("Magic: %s\n", map(SECTION_MAGIC, magic));
	printf("MajorLinkerVersion: %d\n", maj_link_ver);
	printf("MinorLinkerVersion: %d\n", min_link_ver);
	printf("SizeOfCode: %d bytes\n", size_code);
	printf("SizeOfInitializedData: %d bytes\n", size_init_data);
	printf("SizeOfUninitializedData: %d bytes\n", size_uninit_data);
	printf("AddressOfEntryPoint: 0x%08x\n", addr_entry_pt);
	printf("BaseOfCode: 0x%08x\n", base_code);
	if (is_32bit) {
		printf("BaseOfData: 0x%08x\n", nt_header.OptionalHeader.oh32.winspec.BaseOfData);
	}

	//64bit Spec

	//Both

cleanup:
	if (fd) {
		fclose(fd);
	}
}