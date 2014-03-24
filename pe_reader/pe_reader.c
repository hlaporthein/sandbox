#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "pe_reader.h"
#include "my_winnt.h"

char s_buffer[BUFFER_SIZE];
PE_FILE s_pe;


void title(const char *title) {
	printf("\n%s-----------------------\n", title);
};

void pe_reader(const char *file) {
	memset(&s_pe, 0, sizeof(PE_FILE));
	init_map();

	s_pe.fd = fopen(file, "rb");
	if (s_pe.fd == NULL) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	clearerr(s_pe.fd);

	size_t s = fread(&s_pe.dos_header, sizeof(IMAGE_DOS_HEADER), 1, s_pe.fd);
	if (ferror(s_pe.fd)) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	if (s_pe.dos_header.e_magic != IMAGE_DOS_SIGNATURE) {
		fprintf(stderr, "ERROR: This is not a DOS file.\n");
		goto cleanup;
	}

	printf("PE starts at: 0x%08x\n", s_pe.dos_header.e_lfanew);

	if (fseek(s_pe.fd, s_pe.dos_header.e_lfanew, SEEK_SET) != 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	s = fread(&s_pe.header, sizeof(IMAGE_NT_HEADERS), 1, s_pe.fd);
	if (ferror(s_pe.fd)) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	if (s_pe.header.Signature != IMAGE_NT_SIGNATURE) {
		fprintf(stderr, "ERROR: This is not a PE file: 0x%08x\n", s_pe.header.Signature);
		goto cleanup;
	}


	s_pe.is_32bit = has_flags(IMAGE_FILE_32BIT_MACHINE, s_pe.header.FileHeader.Characteristics);

	pe_print_standard_header();
	pe_print_optional_header_standard();
	pe_print_optional_header_winspec();
	pe_print_optional_header_data_directory();

	int section_header_offset = s_pe.dos_header.e_lfanew + sizeof(s_pe.header.Signature) +
			sizeof(s_pe.header.FileHeader) + s_pe.header.FileHeader.SizeOfOptionalHeader;
	printf("section_header_offset=%08x\n", section_header_offset);
	if (fseek(s_pe.fd, section_header_offset, SEEK_SET) != 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	size_t size = sizeof(IMAGE_SECTION_HEADER) * s_pe.header.FileHeader.NumberOfSections;
	s_pe.section_table = (PIMAGE_SECTION_HEADER) malloc(size);
	s = fread(s_pe.section_table, sizeof(IMAGE_SECTION_HEADER), s_pe.header.FileHeader.NumberOfSections, s_pe.fd);
	if (ferror(s_pe.fd)) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	pe_print_section_table();

	pe_print_section_idata();
	if (errno) {
		goto cleanup;
	}

cleanup:
	if (s_pe.fd) {
		fclose(s_pe.fd);
	}

	if (s_pe.section_table) {
		free(s_pe.section_table);
		s_pe.section_table = NULL;
	}
}

void pe_print_standard_header() {
	printf("PE signature: 0x%08x\n", s_pe.header.Signature);
	title("COFF Header");
	// Standard header
	printf("Machine: %s\n", map(SECTION_MACHINE, s_pe.header.FileHeader.Machine));
	printf("Number of sections: %d\n", s_pe.header.FileHeader.NumberOfSections);

	time_t time_as_time_t = s_pe.header.FileHeader.TimeDateStamp;
	struct tm *tm = localtime(&time_as_time_t);
	printf("Creation date: %d/%d/%d - %d:%d:%d\n",
			tm->tm_mday, (1 + tm->tm_mon), (1900 + tm->tm_year), tm->tm_hour, tm->tm_min, tm->tm_sec);
	//printf("Creation date: %s", asctime(tm));

	printf("Symbol table address: 0x%08x\n", s_pe.header.FileHeader.PointerToSymbolTable);
	printf("Number of symbols: %d\n", s_pe.header.FileHeader.NumberOfSymbols);
	printf("Size of optional header: %d\n", s_pe.header.FileHeader.SizeOfOptionalHeader);
	printf("Characteristics:%s\n",
			list_flags(s_buffer, BUFFER_SIZE, SECTION_CHARACTERISTICS, s_pe.header.FileHeader.Characteristics));
}

void pe_print_optional_header_standard() {
	POPTIONAL_HEADER_STANDARD p = &s_pe.header.OptionalHeader.oh32.standard;
	// Optional header (32 snd 64 bits)

	title("Optional Header");
	title("Standard");
	printf("Magic: %s\n", map(SECTION_MAGIC, p->Magic));
	printf("MajorLinkerVersion: %d\n", p->MajorLinkerVersion);
	printf("MinorLinkerVersion: %d\n", p->MinorLinkerVersion);
	printf("SizeOfCode: %d bytes\n", p->SizeOfCode);
	printf("SizeOfInitializedData: %d bytes\n", p->SizeOfInitializedData);
	printf("SizeOfUninitializedData: %d bytes\n", p->SizeOfUninitializedData);
	printf("AddressOfEntryPoint: 0x%08x\n", p->AddressOfEntryPoint);
	printf("BaseOfCode: 0x%08x\n", p->BaseOfCode);
	if (s_pe.is_32bit) {
		printf("BaseOfData: 0x%08x\n", s_pe.header.OptionalHeader.oh32.winspec.BaseOfData);
	}
}

void pe_print_optional_header_winspec() {
	title("Windows Spec");
	if (s_pe.is_32bit) {
		POPTIONAL_HEADER_WINSPEC32 p = &s_pe.header.OptionalHeader.oh32.winspec;
		printf("ImageBase: 0x%08x\n", p->ImageBase);
		printf("Section alignment in RAM: %d bytes (0x%x)\n", p->SectionAlignment, p->SectionAlignment);
		printf("Section alignment in File: %d bytes (0x%x)\n", p->FileAlignment, p->FileAlignment);
		printf("MajorOperatingSystemVersion: %d\n", p->MajorOperatingSystemVersion);
		printf("MinorOperatingSystemVersion: %d\n", p->MinorOperatingSystemVersion);
		printf("MajorImageVersion: %d\n", p->MajorImageVersion);
		printf("MinorImageVersion: %d\n", p->MinorImageVersion);
		printf("MajorSubsystemVersion: %d\n", p->MajorSubsystemVersion);
		printf("MinorSubsystemVersion: %d\n", p->MinorSubsystemVersion);
		printf("Win32VersionValue: %d\n", p->Win32VersionValue);
		printf("SizeOfImage: %d bytes\n", p->SizeOfImage);
		printf("SizeOfHeaders: %d bytes\n", p->SizeOfHeaders);
		printf("CheckSum: %d\n", p->CheckSum);
		printf("Subsystem: %s\n", map(SECTION_SUBSYSTEM, p->Subsystem));
		printf("DllCharacteristics: %s\n",
				list_flags(s_buffer, BUFFER_SIZE, SECTION_DLL_CHARACTERISICS, p->DllCharacteristics));
		printf("SizeOfStackReserve: %d bytes\n", p->SizeOfStackReserve);
		printf("SizeOfStackCommit: %d bytes\n", p->SizeOfStackCommit);
		printf("SizeOfHeapReserve: %d bytes\n", p->SizeOfHeapReserve);
		printf("SizeOfHeapCommit: %d bytes\n", p->SizeOfHeapCommit);
		printf("LoaderFlags: %d\n", p->LoaderFlags);
		printf("NumberOfRvaAndSizes: %d\n", p->NumberOfRvaAndSizes);
	} else {
		POPTIONAL_HEADER_WINSPEC64 p = &s_pe.header.OptionalHeader.oh64.winspec;
		printf("ImageBase: 0x%08x\n", p->ImageBase);
		printf("SectionAlignment: %d bytes\n", p->SectionAlignment);
		printf("FileAlignment: %d bytes\n", p->FileAlignment);
		printf("MajorOperatingSystemVersion: %d\n", p->MajorOperatingSystemVersion);
		printf("MinorOperatingSystemVersion: %d\n", p->MinorOperatingSystemVersion);
		printf("MajorImageVersion: %d\n", p->MajorImageVersion);
		printf("MinorImageVersion: %d\n", p->MinorImageVersion);
		printf("MajorSubsystemVersion: %d\n", p->MajorSubsystemVersion);
		printf("MinorSubsystemVersion: %s\n", p->MinorSubsystemVersion);
		printf("Win32VersionValue: %d\n", p->Win32VersionValue);
		printf("SizeOfImage: %d bytes\n", p->SizeOfImage);
		printf("SizeOfHeaders: %d bytes\n", p->SizeOfHeaders);
		printf("CheckSum: %d\n", p->CheckSum);
		printf("Subsystem: %s\n", map(SECTION_SUBSYSTEM, p->Subsystem));
		printf("DllCharacteristics: %s\n",
				list_flags(s_buffer, BUFFER_SIZE, SECTION_DLL_CHARACTERISICS, p->DllCharacteristics));
		printf("SizeOfStackReserve: %d bytes\n", p->SizeOfStackReserve);
		printf("SizeOfStackCommit: %d bytes\n", p->SizeOfStackCommit);
		printf("SizeOfHeapReserve: %d bytes\n", p->SizeOfHeapReserve);
		printf("SizeOfHeapCommit: %d bytes\n", p->SizeOfHeapCommit);
		printf("LoaderFlags: %d\n", p->LoaderFlags);
		printf("NumberOfRvaAndSizes: %d\n", p->NumberOfRvaAndSizes);
	}
}

void pe_print_optional_header_data_directory() {
	title("Data Directories");
	PIMAGE_DATA_DIRECTORY dd = s_pe.header.OptionalHeader.oh32.DataDirectory;
	for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++) {
		printf("Name: %s (RVA: 0x%08x, Size: %d bytes)\n", map(SECTION_DATA_DIRECTORY, i), dd[i].VirtualAddress, dd[i].Size);
	}
}

void pe_print_section_table() {
	title("Section Table");
	for (int i = 0; i < s_pe.header.FileHeader.NumberOfSections; i++) {
		pe_print_section_header(i);
	}
}

void pe_print_section_header(int index) {
	PIMAGE_SECTION_HEADER p = &(s_pe.section_table[index]);
	char buf[IMAGE_SIZEOF_SHORT_NAME + 1];
	memset(buf, 0, IMAGE_SIZEOF_SHORT_NAME + 1);
	memcpy(buf, s_pe.section_table[index].Name, IMAGE_SIZEOF_SHORT_NAME);
	printf("**Name: %s\n", buf);
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
	printf("\n");

	if (strcmp(buf, ".idata") == 0) {
		printf(".idata_offset=%x\n", p->PointerToRawData);
		s_pe.idata_offset = p->PointerToRawData;
	}
}

void pe_print_section_idata() {
	title("Import table: .idata");
	if (fseek(s_pe.fd, s_pe.idata_offset, SEEK_SET) != 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}
	IMAGE_IMPORT_DESCRIPTOR import_descriptors_table;
	while (1) {
		size_t s = fread(&import_descriptors_table, sizeof(IMAGE_IMPORT_DESCRIPTOR), 1, s_pe.fd);
		if (ferror(s_pe.fd)) {
			fprintf(stderr, "ERROR: %s\n", strerror(errno));
			goto cleanup;
		}
		if (import_descriptors_table.Name == 0) {
			break;
		}

		char dll_name[BUFFER_SIZE];
		read_rva(dll_name, import_descriptors_table.Name);

//		printf("**OriginalFirstThunk: 0x%08x\n", import_descriptors_table.DUMMYUNIONNAME);
//		printf("  TimeDateStamp: %d\n", import_descriptors_table.TimeDateStamp);
//		printf("  ForwarderChain: 0x%08x\n", import_descriptors_table.ForwarderChain);
//		printf("  Name: 0x%08x (%s)\n", import_descriptors_table.Name, dll_name);
//		printf("  FirstThunk: 0x%08x\n", import_descriptors_table.FirstThunk);
//		printf("\n");
		pe_print_section_idata_lookup(dll_name, (rva_t) import_descriptors_table.DUMMYUNIONNAME.OriginalFirstThunk);
	}

//	size_t size = sizeof(IMAGE_IMPORT_DESCRIPTOR);
//	s_pe.import_descriptors = (PIMAGE_IMPORT_DESCRIPTOR) malloc(size);

cleanup:
	;
}

void pe_print_section_idata_lookup(const char *dll_name, rva_t rva) {
	long previous_offset = ftell(s_pe.fd);
	if (ferror(s_pe.fd)) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	long offset = rva2offset(rva);
	if (fseek(s_pe.fd, offset, SEEK_SET) != 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	size_t size = sizeof(ULONGLONG);
	if (s_pe.is_32bit) {
		DWORD IMAGE_ORDINAL_FLAG = IMAGE_ORDINAL_FLAG32;
		DWORD entry;
		while(1) {
			size_t s = fread(&entry, sizeof(DWORD), 1, s_pe.fd);
			if (ferror(s_pe.fd)) {
				fprintf(stderr, "ERROR: %s\n", strerror(errno));
				goto cleanup;
			}
			if (entry == 0) {
				break;
			} else if (entry & IMAGE_ORDINAL_FLAG) {
				printf("Look per ordinal: %d\n", entry & ((1 << 16) - 1));
			} else {
				char buf[BUFFER_SIZE];
				read_rva(buf, entry + 2);
				printf("%s: %s\n", dll_name, buf);
			}
		}
	}

	if (fseek(s_pe.fd, previous_offset, SEEK_SET) != 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

cleanup:
	;
}

void read_rva(char *buffer, rva_t address) {
	long previous_offset = ftell(s_pe.fd);
	if (ferror(s_pe.fd)) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	long offset = rva2offset(address);
	if (fseek(s_pe.fd, offset, SEEK_SET) != 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	size_t s = fread(buffer, BUFFER_SIZE, 1, s_pe.fd);
	if (ferror(s_pe.fd)) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	if (fseek(s_pe.fd, previous_offset, SEEK_SET) != 0) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

cleanup:
	;
}

long rva2offset(rva_t address) {
	long section_offset = 0;
	int index = 0;
	for (int i = 0; i < s_pe.header.FileHeader.NumberOfSections; i++) {
		if (address > s_pe.section_table[i].VirtualAddress) {
			section_offset = address - s_pe.section_table[i].VirtualAddress;
			index = i;
		} else {
			break;
		}
	}
	char buf[IMAGE_SIZEOF_SHORT_NAME + 1];
	memset(buf, 0, IMAGE_SIZEOF_SHORT_NAME + 1);
	memcpy(buf, s_pe.section_table[index].Name, IMAGE_SIZEOF_SHORT_NAME);
	long result = section_offset + s_pe.section_table[index].PointerToRawData;
	return result;
}