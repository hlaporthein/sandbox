#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "coff_reader.h"
#include "my_winnt.h"

char s_buffer[BUFFER_SIZE];
COFF_FILE s_coff;
char **section_list = NULL;
extern int dflag;
DWORD s_ImageBase;

void title(const char *title) {
	printf("\n%s-----------------------\n", title);
};

void print_section_list(int max) {
	if (max <= 0) {
		max = s_coff.header.NumberOfSections;
	}
	for (int i = 0; i < max; i++) {
		printf("section[%d]=%s\n", i, section_list[i]);
	}
}

void coff_reader(const char *file) {
	memset(&s_coff, 0, sizeof(COFF_FILE));
	init_map();

	s_coff.fd = fopen(file, "rb");
	if (s_coff.fd == NULL) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	clearerr(s_coff.fd);

	size_t s = FREAD(&s_coff.header, sizeof(IMAGE_FILE_HEADER), 1);

	s_coff.is_32bit = has_flags(IMAGE_FILE_32BIT_MACHINE, s_coff.header.Characteristics);

	coff_print_standard_header();

	int section_header_offset = sizeof(s_coff.header) + s_coff.header.SizeOfOptionalHeader;
	FSEEK(section_header_offset);

	size_t size = sizeof(IMAGE_SECTION_HEADER) * s_coff.header.NumberOfSections;
	s_coff.section_table = (PIMAGE_SECTION_HEADER) malloc(size);
	s = FREAD(s_coff.section_table, sizeof(IMAGE_SECTION_HEADER), s_coff.header.NumberOfSections);

	coff_print_section_table();
	coff_print_symbol_table();

cleanup:
	if (s_coff.fd) {
		fclose(s_coff.fd);
	}

	if (s_coff.section_table) {
		free(s_coff.section_table);
		s_coff.section_table = NULL;
	}
}

void coff_print_standard_header() {
	title("COFF Header");
	// Standard header
	printf("Machine: %s\n", map(SECTION_MACHINE, s_coff.header.Machine));
	printf("Number of sections: %d\n", s_coff.header.NumberOfSections);

	time_t time_as_time_t = s_coff.header.TimeDateStamp;
	struct tm *tm = localtime(&time_as_time_t);
	printf("Creation date: %d/%d/%d - %d:%d:%d\n",
			tm->tm_mday, (1 + tm->tm_mon), (1900 + tm->tm_year), tm->tm_hour, tm->tm_min, tm->tm_sec);
	//printf("Creation date: %s", asctime(tm));

	printf("Symbol table address: 0x%08x\n", s_coff.header.PointerToSymbolTable);
	printf("Number of symbols: %d\n", s_coff.header.NumberOfSymbols);
	printf("Size of optional header: %d\n", s_coff.header.SizeOfOptionalHeader);
	printf("Characteristics:%s\n",
			list_flags(s_buffer, BUFFER_SIZE, SECTION_CHARACTERISTICS, s_coff.header.Characteristics));
}

void coff_print_section_table() {
	title("Section Table");
	section_list = (char**) malloc(s_coff.header.NumberOfSections * sizeof(char*));
	for (int i = 0; i < s_coff.header.NumberOfSections; i++) {
		coff_print_section_header(i);
	}
}

void coff_print_section_header(int index) {
	PIMAGE_SECTION_HEADER p = &(s_coff.section_table[index]);
	section_list[index] = (char*) malloc((IMAGE_SIZEOF_SHORT_NAME + 1) * sizeof(char));
	memset(section_list[index], 0, IMAGE_SIZEOF_SHORT_NAME + 1);
	memcpy(section_list[index], s_coff.section_table[index].Name, IMAGE_SIZEOF_SHORT_NAME);
	printf("**Section name: %s\n", section_list[index]);
	printf("  VirtualSize: %d bytes (0x%x)\n", p->Misc, p->Misc);
	printf("  VirtualAddress: 0x%08x\n", p->VirtualAddress);
	printf("  SizeOfRawData: %d bytes (0x%x)\n", p->SizeOfRawData, p->SizeOfRawData);
	printf("  PointerToRawData: 0x%08x\n", p->PointerToRawData);
	printf("  PointerToRelocations: 0x%08x\n", p->PointerToRelocations);
	printf("  PointerToLinenumbers: 0x%08x\n", p->PointerToLinenumbers);
	printf("  NumberOfRelocations: %d\n", p->NumberOfRelocations);
	printf("  NumberOfLinenumbers: %d\n", p->NumberOfLinenumbers);
	printf("  Characteristics: %s\n",
			list_flags(s_buffer, BUFFER_SIZE, SECTION_SECTION_CHARACTERISTICS, p->Characteristics));

	if (has_flags(IMAGE_SCN_LNK_INFO, p->Characteristics) && has_flags(IMAGE_SCN_LNK_REMOVE, p->Characteristics)) {
		coff_print_directive_section(p);
	}

	print_relocation_table(p);
	printf("\n");
}

void print_relocation_table(PIMAGE_SECTION_HEADER p) {
	long previous_offset = FTELL();
	FSEEK(p->PointerToRelocations);
	int size = p->NumberOfRelocations * sizeof(IMAGE_RELOCATION);
	PIMAGE_RELOCATION reloc_entries = (PIMAGE_RELOCATION) malloc(size);
	FREAD(reloc_entries, sizeof(IMAGE_RELOCATION), p->NumberOfRelocations);

	printf("  Relocations:\n");
	for (int i = 0; i < p->NumberOfRelocations; i++) {
		printf("    VirtualAddress: 0x%08x\n", reloc_entries[i].VirtualAddress);
		printf("    SymbolTableIndex: %d\n", reloc_entries[i].SymbolTableIndex);
		switch(s_coff.header.Machine) {
			case IMAGE_FILE_MACHINE_I386:
				printf("    Type: 0x%04x (%s)\n", reloc_entries[i].Type, map(SECTION_COFF_RELOC_I386, reloc_entries[i].Type));
				break;
			case IMAGE_FILE_MACHINE_AMD64:
				printf("    Type: 0x%04x (%s)\n", reloc_entries[i].Type, map(SECTION_COFF_RELOC_AMD64, reloc_entries[i].Type));
				break;
		}
	}

	FSEEK(previous_offset);
}

void coff_print_directive_section(PIMAGE_SECTION_HEADER p) {
	long previous_offset = FTELL();
	FSEEK(p->PointerToRawData);

	int size = p->SizeOfRawData * sizeof(char);
	char *directive_buffer = (char *) malloc(size + 1);
	FREAD(directive_buffer, size, 1);
	directive_buffer[size] = 0;

	int is_utf8 = has_bom(directive_buffer);
	printf("  Has BOM: %s\n", (is_utf8) ? "yes" : "no");
	if (is_utf8) {
		directive_buffer += 2;
	}

	printf("  Content: %s\n", directive_buffer);

	FSEEK(previous_offset);
}

void coff_print_symbol_table() {
	title("Symbols Table");
	long previous_offset = FTELL();
	FSEEK(s_coff.header.PointerToSymbolTable);

	int size = s_coff.header.NumberOfSymbols * sizeof(IMAGE_SYMBOL);
	PIMAGE_SYMBOL symbol_entries = (PIMAGE_SYMBOL) malloc(size);
	FREAD(symbol_entries, sizeof(IMAGE_SYMBOL), s_coff.header.NumberOfSymbols);

	int string_table_offset = s_coff.header.PointerToSymbolTable + size;

	for (int i = 0; i < s_coff.header.NumberOfSymbols; i++) {
		char buf[1024];
		memset(buf, 0, 1024);
		if (symbol_entries[i].N.Name.Short == 0) {
			read_offset(buf, 1024, string_table_offset + symbol_entries[i].N.Name.Long);
			printf("%d/Name: %s (long)\n", i, buf);
		} else {
			char buf[9];
			strncpy(buf, symbol_entries[i].N.ShortName, 8);
			buf[8] = 0;
			printf("%d/Name: %s (short)\n", i, buf);
		}

		printf("  Value: 0x%08x\n", symbol_entries[i].Value);

		if (symbol_entries[i].SectionNumber <= 0) {
			printf("  SectionNumber: %d (%s)\n",
				symbol_entries[i].SectionNumber, map(SECTION_SECTION_NUMBER_VALUE, symbol_entries[i].SectionNumber));
		} else {
			printf("  SectionNumber: %d (%s)\n",
				symbol_entries[i].SectionNumber, s_coff.section_table[symbol_entries[i].SectionNumber - 1].Name);
		}

		printf("  Type: %s\n", map(SECTION_SYMBOL_TYPE, symbol_entries[i].Type));
		printf("  StorageClass: %s\n", map(SECTION_STORAGE_CLASS, symbol_entries[i].StorageClass));
		printf("  NumberOfAuxSymbols: %d\n", symbol_entries[i].NumberOfAuxSymbols);
		printf("\n");
	}

	FSEEK(previous_offset);
}

void read_offset(char *buf, size_t size, int offset) {
	long previous_offset = FTELL();
	FSEEK(offset);

	size_t s = FREAD(buf, size, 1);
	FSEEK(previous_offset);

cleanup:
	;
}

int has_bom(const char *buffer) {
	WORD bom = (WORD) *buffer;

	switch (bom) {
		case 0xEF:
		case 0xBB:
		case 0xBF:
			return 1;
	}
	return 0;
}