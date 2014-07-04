#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "pe_reader.h"
#include "my_winnt.h"

char s_buffer[BUFFER_SIZE];
PE_FILE s_pe;
char **section_list = NULL;
extern int dflag;
DWORD s_ImageBase;

void title(const char *title) {
	printf("\n%s-----------------------\n", title);
};

void print_section_list(int max) {
	if (max <= 0) {
		max = s_pe.header.FileHeader.NumberOfSections;
	}
	for (int i = 0; i < max; i++) {
		printf("section[%d]=%s\n", i, section_list[i]);
	}
}

void pe_reader(const char *file) {
	memset(&s_pe, 0, sizeof(PE_FILE));
	init_map();

	s_pe.fd = fopen(file, "rb");
	if (s_pe.fd == NULL) {
		fprintf(stderr, "ERROR: %s\n", strerror(errno));
		goto cleanup;
	}

	clearerr(s_pe.fd);

	size_t s = FREAD(&s_pe.dos_header, sizeof(IMAGE_DOS_HEADER), 1);

	if (s_pe.dos_header.e_magic != IMAGE_DOS_SIGNATURE) {
		fprintf(stderr, "ERROR: This is not a DOS file.\n");
		goto cleanup;
	}

	printf("PE starts at: 0x%08x\n", s_pe.dos_header.e_lfanew);
	FSEEK(s_pe.dos_header.e_lfanew);

	s = FREAD(&s_pe.header, sizeof(IMAGE_NT_HEADERS), 1);

	if (s_pe.header.Signature != IMAGE_NT_SIGNATURE) {
		fprintf(stderr, "ERROR: This is not a PE file: 0x%08x\n", s_pe.header.Signature);
		goto cleanup;
	}


	s_pe.is_32bit = has_flags(IMAGE_FILE_32BIT_MACHINE, s_pe.header.FileHeader.Characteristics);
	s_pe.dd = s_pe.header.OptionalHeader.oh64.DataDirectory;
	if (s_pe.is_32bit) {
		s_pe.dd = s_pe.header.OptionalHeader.oh32.DataDirectory;
	}

	pe_print_standard_header();
	pe_print_optional_header_standard();
	pe_print_optional_header_winspec();
	pe_print_optional_header_data_directory();

	int section_header_offset = s_pe.dos_header.e_lfanew + sizeof(s_pe.header.Signature) +
			sizeof(s_pe.header.FileHeader) + s_pe.header.FileHeader.SizeOfOptionalHeader;
	FSEEK(section_header_offset);

	size_t size = sizeof(IMAGE_SECTION_HEADER) * s_pe.header.FileHeader.NumberOfSections;
	s_pe.section_table = (PIMAGE_SECTION_HEADER) malloc(size);
	s = FREAD(s_pe.section_table, sizeof(IMAGE_SECTION_HEADER), s_pe.header.FileHeader.NumberOfSections);

	pe_print_section_table();

	pe_print_section_import();
	if (errno) {
		goto cleanup;
	}

	pe_print_section_edata();
	if (errno) {
		goto cleanup;
	}

//	pe_print_section_reloc();
//	if (errno) {
//		goto cleanup;
//	}

	pe_print_section_iat();
	if (errno) {
		goto cleanup;
	}

	if (0) {

		pe_print_section_exception();
		if (errno) {
			goto cleanup;
		}
	}
cleanup:
	if (s_pe.fd) {
		fclose(s_pe.fd);
	}

	if (s_pe.section_table) {
		free(s_pe.section_table);
		s_pe.section_table = NULL;
	}

	if (s_pe.export_address_table) {
		free(s_pe.export_address_table);
		s_pe.export_address_table = NULL;
	}

	if (s_pe.export_name_pointer_table) {
		free(s_pe.export_name_pointer_table);
		s_pe.export_name_pointer_table = NULL;
	}

	if (s_pe.export_ordinal_table) {
		free(s_pe.export_ordinal_table);
		s_pe.export_ordinal_table = NULL;
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
	printf("Creation date: %04d/%02d/%02d - %02d:%02d:%02d\n",
			(1900 + tm->tm_year), (1 + tm->tm_mon), tm->tm_mday, tm->tm_hour, tm->tm_min, tm->tm_sec);
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
		s_ImageBase = p->ImageBase;
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
		printf("CheckSum: 0x%08X\n", p->CheckSum);
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
	}
}

void pe_print_optional_header_data_directory() {
	title("Data Directories");
	for (int i = 0; i < IMAGE_NUMBEROF_DIRECTORY_ENTRIES; i++) {
		printf("Name: %s (RVA: 0x%08x, Size: %d bytes)\n", map(SECTION_DATA_DIRECTORY, i), s_pe.dd[i].VirtualAddress, s_pe.dd[i].Size);
	}

	s_pe.idata_rva = s_pe.dd[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress;
	s_pe.edata_rva = s_pe.dd[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	s_pe.reloc_rva = s_pe.dd[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress;
	s_pe.iat_rva = s_pe.dd[IMAGE_DIRECTORY_ENTRY_IAT].VirtualAddress;
	s_pe.except_rva = s_pe.dd[IMAGE_DIRECTORY_ENTRY_EXCEPTION].VirtualAddress;
	s_pe.delay_rva = s_pe.dd[IMAGE_DIRECTORY_ENTRY_DELAY_IMPORT].VirtualAddress;
}

void pe_print_section_table() {
	title("Section Table");
	section_list = (char**) malloc(s_pe.header.FileHeader.NumberOfSections * sizeof(char*));
	for (int i = 0; i < s_pe.header.FileHeader.NumberOfSections; i++) {
		pe_print_section_header(i);
	}
}

void pe_print_section_header(int index) {
	PIMAGE_SECTION_HEADER p = &(s_pe.section_table[index]);
	section_list[index] = (char*) malloc((IMAGE_SIZEOF_SHORT_NAME + 1) * sizeof(char));
	memset(section_list[index], 0, IMAGE_SIZEOF_SHORT_NAME + 1);
	memcpy(section_list[index], s_pe.section_table[index].Name, IMAGE_SIZEOF_SHORT_NAME);
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
	printf("\n");

	if (strcmp(section_list[index], ".idata") == 0) {
		s_pe.idata_rva = p->VirtualAddress;
	} else if (strcmp(section_list[index], ".edata") == 0) {
		s_pe.edata_rva = p->VirtualAddress;
	} else if (strcmp(section_list[index], ".reloc") == 0) {
		s_pe.reloc_rva = p->VirtualAddress;
		s_pe.reloc_size = p->Misc.VirtualSize;
	}
}

void pe_print_section_import() {
	title("Import table");
	rva_t rva = s_pe.idata_rva;
	if (rva == 0) {
		printf("No import table.\n");
		return;
	}
	long offset = rva2offset(rva);
	FSEEK(offset);

	IMAGE_IMPORT_DESCRIPTOR import_descriptors_table;
	int max = -1;
	int n = 0;
	while (max < 0 || n < max) {
		n++;
		size_t s = FREAD(&import_descriptors_table, sizeof(IMAGE_IMPORT_DESCRIPTOR), 1);
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
		pe_print_section_import_lookup(dll_name, (rva_t) import_descriptors_table.DUMMYUNIONNAME.OriginalFirstThunk);
	}

cleanup:
	;
}

void pe_print_section_import_lookup(const char *dll_name, rva_t rva) {
	long previous_offset = FTELL();

	long offset = rva2offset(rva);
	FSEEK(offset);

	if (s_pe.is_32bit) {
		DWORD entry;
		while(1) {
			size_t s = FREAD(&entry, sizeof(DWORD), 1);

			if (entry == 0) {
				break;
			} else if (entry & IMAGE_ORDINAL_FLAG32) {
				printf("(Ordinal) %d\n", entry & ((1 << 16) - 1));
			} else {
				PIMAGE_IMPORT_BY_NAME p = (PIMAGE_IMPORT_BY_NAME) malloc(1024);
				long p_offset = FTELL();
				FSEEK(rva2offset(entry));
				s = FREAD(p, 1024, 1);
				FSEEK(p_offset);
				printf("(Name) %s: %s %d(0x%04x)\n", dll_name, p->Name, p->Hint, p->Hint);
			}
		}
	} else {
		ULONGLONG entry;
		int max = -1;
		int n = 0;
		while(max < 0 || n < max) {
			n++;
			size_t s = FREAD(&entry, sizeof(ULONGLONG), 1);
			if (entry == 0) {
				break;
			} else if (entry & IMAGE_ORDINAL_FLAG64) {
				printf("(Ordinal) %d\n", entry & ((1 << 16) - 1));
			} else {
				PIMAGE_IMPORT_BY_NAME p = (PIMAGE_IMPORT_BY_NAME) malloc(1024);
				long p_offset = FTELL();
				FSEEK(rva2offset(entry));
				s = FREAD(p, 1024, 1);
				FSEEK(p_offset);
				printf("(Name) %s: %s %d(0x%04x)\n", dll_name, p->Name, p->Hint, p->Hint);
			}
		}
	}

	FSEEK(previous_offset);

cleanup:
	;
}

void pe_print_section_edata() {
	title("Export directory table");

	rva_t rva = s_pe.edata_rva;
	if (rva == 0) {
		printf("No export table.\n");
		return;
	}

	long offset = rva2offset(rva);
	FSEEK(offset);

	PIMAGE_EXPORT_DIRECTORY edtp = &s_pe.export_directory_table;
	size_t s = FREAD(edtp, sizeof(IMAGE_EXPORT_DIRECTORY), 1);
	printf("Export flag: %d\n", edtp->Characteristics);
	printf("TimeDateStamp: %d\n", edtp->TimeDateStamp);
	printf("MajorVersion: %d\n", edtp->MajorVersion);
	printf("MinorVersion: %d\n", edtp->MinorVersion);
	char buf[BUFFER_SIZE];
	read_rva(buf, edtp->Name);
	printf("Name: 0x%08x (%s)\n", edtp->Name, buf);
	printf("Base: %d\n", edtp->Base);
	printf("NumberOfFunctions: %d\n", edtp->NumberOfFunctions);
	printf("NumberOfNames: %d\n", edtp->NumberOfNames);
	printf("AddressOfFunctions: 0x%08x\n", edtp->AddressOfFunctions);
	printf("AddressOfNames: 0x%08x\n", edtp->AddressOfNames);
	printf("AddressOfNameOrdinals: 0x%08x\n", edtp->AddressOfNameOrdinals);


	offset = rva2offset(edtp->AddressOfFunctions);
	FSEEK(offset);
	s_pe.export_address_table = (PIMAGE_THUNK_DATA32) malloc(sizeof(IMAGE_THUNK_DATA32) * edtp->NumberOfFunctions);
	s = FREAD(s_pe.export_address_table, sizeof(IMAGE_THUNK_DATA32), edtp->NumberOfFunctions);

	offset = rva2offset(edtp->AddressOfNames);
	FSEEK(offset);
	s_pe.export_name_pointer_table = (DWORD*) malloc(sizeof(DWORD) * edtp->NumberOfNames);
	s = FREAD(s_pe.export_name_pointer_table, sizeof(DWORD), edtp->NumberOfNames);

	FSEEK(rva2offset(edtp->AddressOfNameOrdinals));
	s_pe.export_ordinal_table = (WORD*) malloc(sizeof(WORD) * edtp->NumberOfNames);
	s = FREAD(s_pe.export_ordinal_table, sizeof(WORD), edtp->NumberOfNames);

	for (int i = 0; i < edtp->NumberOfFunctions; i++) {
		if (i < edtp->NumberOfNames) {
			char buf[BUFFER_SIZE];
			read_rva(buf, s_pe.export_name_pointer_table[i]);
			printf("ExportSymbol: %s (0x%08x), ", buf, s_pe.export_name_pointer_table[i]);
		} else {
			printf("ExportSymbol: [Not defined], ");
		}

		if (is_in_export_section(s_pe.export_address_table[i].u1.AddressOfData)) {
			read_rva(buf, s_pe.export_address_table[i].u1.ForwarderString);
			printf("Forwarder RVA: 0x%08x (%s), ", s_pe.export_address_table[i].u1.ForwarderString, buf);
		} else if (is_in_code_section(s_pe.export_address_table[i].u1.AddressOfData)) {
			printf("Function RVA: 0x%08x, ", s_pe.export_address_table[i].u1.Function);
		} else {
			printf("Variable RVA: 0x%08x, ", s_pe.export_address_table[i].u1.AddressOfData);
		}
		printf("Ordinal: %d\n", s_pe.export_ordinal_table[i] + edtp->Base);
	}

cleanup:
	;
}

int is_in_export_section(rva_t rva) {
	DWORD address = s_pe.dd[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress;
	DWORD size = s_pe.dd[IMAGE_DIRECTORY_ENTRY_EXPORT].Size;
	return rva >= address && rva <= address + size;
}

int is_in_code_section(rva_t rva) {
	DWORD BaseOfCode = s_pe.header.OptionalHeader.oh32.standard.BaseOfCode;
	DWORD SizeOfCode = s_pe.header.OptionalHeader.oh32.standard.SizeOfCode;
	return rva >= BaseOfCode && rva <= BaseOfCode + SizeOfCode;
}

void pe_print_section_reloc() {
	title("Relocation table");
	rva_t rva = s_pe.reloc_rva;
	if (rva == 0) {
		printf("No relocation table.\n");
		return;
	}
	long offset = rva2offset(rva);
	FSEEK(offset);
	size_t reloc_size = s_pe.reloc_size;

	char *reloc_buffer = (char*) malloc(reloc_size);
	size_t s = FREAD(reloc_buffer, 1, reloc_size);
	char *cursor = reloc_buffer;
	while (cursor < reloc_buffer + s) {
		PIMAGE_BASE_RELOCATION headerp = (PIMAGE_BASE_RELOCATION) cursor;
		cursor += sizeof(IMAGE_BASE_RELOCATION);
		printf("Page RVA: 0x%08x (%s)\n", headerp->VirtualAddress, get_section(headerp->VirtualAddress));
		printf("Block size: %d bytes (0x%08x)\n", headerp->SizeOfBlock, headerp->SizeOfBlock);

		int field_nbr = (headerp->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / sizeof(WORD);
		WORD *fields = (WORD*) cursor;
		cursor += headerp->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION);

		printf("Number of fields: %d\n", field_nbr);
		for (int i = 0; i < field_nbr; i++) {
			int type = (fields[i] & 0xf000) >> 12;
			int offset = fields[i] & 0x0fff;
			printf("Type: %s, Offset: 0x%04x\n", map(SECTION_RELOC, type), offset);
		}

//		printf("cursor=%x\n", cursor);
//		printf("reloc_buffer=%x\n", reloc_buffer);
//		printf("s=%x\n", s);
//		printf("reloc_buffer + s=%x\n", reloc_buffer + s);
		printf("\n");
	}

cleanup:
	;
}

void pe_print_section_iat() {
	title("Import Address Table (IAT)");
	rva_t rva = s_pe.iat_rva;
	if (rva == 0) {
		printf("No IAT.\n");
		return;
	}
	long offset = rva2offset(rva);
	FSEEK(offset);
	size_t iat_size = s_pe.dd[IMAGE_DIRECTORY_ENTRY_IAT].Size;

	if (s_pe.is_32bit) {
		int iat_nbr = iat_size / sizeof(IMAGE_THUNK_DATA32);

		PIMAGE_THUNK_DATA32 iatp = (PIMAGE_THUNK_DATA32) malloc(iat_size);
		size_t s = FREAD(iatp, sizeof(IMAGE_THUNK_DATA32), iat_nbr);
		for (int i = 0; i < iat_nbr; i++) {
			rva_t key = rva + i * sizeof(IMAGE_THUNK_DATA32);
			rva_t value = (rva_t) iatp[i].u1.Function;
			if (value == 0) {
				printf("RVA 0x%08x: 0x%08x\n", key, value);
			} else {
				PIMAGE_IMPORT_BY_NAME p = (PIMAGE_IMPORT_BY_NAME) malloc(1024);
				long p_offset = FTELL();
				FSEEK(rva2offset(value));
				s = FREAD(p, 1024, 1);
				FSEEK(p_offset);
				printf("RVA 0x%08x: 0x%08x (Name: %s, Ordinal: %d)\n", key, value, p->Name, p->Hint);
			}
		}
	} else {
		int iat_nbr = iat_size / sizeof(IMAGE_THUNK_DATA64);

		PIMAGE_THUNK_DATA64 iatp = (PIMAGE_THUNK_DATA64) malloc(iat_size);
		size_t s = FREAD(iatp, sizeof(IMAGE_THUNK_DATA64), iat_nbr);
		//iat_nbr = (iat_nbr > 10) ? 10 : iat_nbr;
		for (int i = 0; i < iat_nbr; i++) {
			rva_t key = rva + i * sizeof(IMAGE_THUNK_DATA64);
			ULONGLONG value = (ULONGLONG) iatp[i].u1.Function;
			if (value > 0x00000000ffffffff) {
				printf("IAT not parsable.\n");
				break;
			}
			if (value == 0) {
				printf("RVA 0x%08x: 0x%016llx\n", key, value);
			} else {
				if (value & IMAGE_ORDINAL_FLAG64) {
					printf("(Ordinal) %d\n", value & ((1 << 16) - 1));
				} else {
					PIMAGE_IMPORT_BY_NAME p = (PIMAGE_IMPORT_BY_NAME) malloc(1024);
					DWORD val = (DWORD) (value & 0x00000000ffffffff);
					long p_offset = FTELL();
					FSEEK(rva2offset(val));
					s = FREAD(p, 1024, 1);
					FSEEK(p_offset);
					printf("RVA 0x%08x: 0x%08x (Name: %s, Ordinal: %d)\n", key, val, p->Name, p->Hint);
				}
			}
		}
	}

cleanup:
	;
}

void pe_print_section_exception() {
	title("Exception Table");
	rva_t rva = s_pe.except_rva;
	if (rva == 0) {
		printf("No exception table.\n");
		return;
	}
	long offset = rva2offset(rva);
	FSEEK(offset);
	size_t except_size = s_pe.dd[IMAGE_DIRECTORY_ENTRY_EXCEPTION].Size;

	switch (s_pe.header.FileHeader.Machine) {
		case IMAGE_FILE_MACHINE_MIPS16:
		case IMAGE_FILE_MACHINE_MIPSFPU:
		case IMAGE_FILE_MACHINE_MIPSFPU16:
			break;

		case IMAGE_FILE_MACHINE_ARM:
		case IMAGE_FILE_MACHINE_POWERPC:
		case IMAGE_FILE_MACHINE_POWERPCFP:
		case IMAGE_FILE_MACHINE_SH3:
		case IMAGE_FILE_MACHINE_SH3DSP:
		case IMAGE_FILE_MACHINE_SH4:
			break;

		case IMAGE_FILE_MACHINE_AMD64:
		case IMAGE_FILE_MACHINE_IA64:
			{
				int exception_nbr = except_size / sizeof(IMAGE_PDATA_ENTRY);
				PIMAGE_PDATA_ENTRY p = (PIMAGE_PDATA_ENTRY) malloc(sizeof(IMAGE_PDATA_ENTRY) * exception_nbr);
				FREAD(p, sizeof(IMAGE_PDATA_ENTRY), exception_nbr);

				for (int i = 0; i < exception_nbr; i++) {
					printf("StartingAddress: 0x%08x (0x%08x)\n", p[i].StartingAddress, rva2offset(p[i].StartingAddress));
					printf("EndingAddress: 0x%08x (0x%08x)\n", p[i].EndingAddress, rva2offset(p[i].EndingAddress));
					printf("UnwindInfo: 0x%08x (0x%08x)\n", p[i].UnwindInfo, rva2offset(p[i].UnwindInfo));
					printf("\n");
				}
			}
			break;

		default:
			printf("Cannot read exception table (.pdata): Unknown architechture.\n");
	}

cleanup:
	;
}

char *get_section(rva_t rva) {
	long section_offset = 0;
	int index = 0;
	for (int i = 0; i < s_pe.header.FileHeader.NumberOfSections; i++) {
		if (rva >= s_pe.section_table[i].VirtualAddress) {
			if (rva <= s_pe.section_table[i].VirtualAddress + s_pe.section_table[i].SizeOfRawData) {
				return section_list[i];
			}
		}
	}
	return "Unknown";
}

void read_rva(char *buffer, rva_t address) {
	long previous_offset = FTELL();

	long offset = rva2offset(address);
	FSEEK(offset);

	size_t s = FREAD(buffer, BUFFER_SIZE, 1);
	FSEEK(previous_offset);

cleanup:
	;
}

long rva2offset(rva_t address) {
	long section_offset = 0;
	int index = -1;
	for (int i = 0; i < s_pe.header.FileHeader.NumberOfSections; i++) {
		if (address >= s_pe.section_table[i].VirtualAddress) {
			section_offset = address - s_pe.section_table[i].VirtualAddress;
			index = i;
		} else {
			break;
		}
	}
	if (index == -1) {
		return address;
	}
	long result = section_offset + s_pe.section_table[index].PointerToRawData;
	return result;
}

rva_t offset2rva(long offset) {
	rva_t rva = 0;
	int index = -1;
	for (int i = 0; i < s_pe.header.FileHeader.NumberOfSections; i++) {
		if (offset >= s_pe.section_table[i].PointerToRawData) {
			rva = offset - s_pe.section_table[i].PointerToRawData + s_pe.section_table[i].VirtualAddress;
			index = i;
		} else {
			break;
		}
	}
	if (index == -1) {
		return offset;
	}
	return rva;
}