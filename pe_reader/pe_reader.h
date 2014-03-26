#ifndef PE_READER_H
#define PE_READER_H

#include "my_winnt.h"

#define BUFFER_SIZE 2048

#define FSEEK(offset) \
	if (fseek(s_pe.fd, offset, SEEK_SET) != 0) { \
		fprintf(stderr, "ERROR FSEEK: %s\n", strerror(errno)); \
		exit(1); \
	} \

#define FREAD(buffer, size_obj, nbr) \
	fread(buffer, size_obj, nbr, s_pe.fd); \
	if (ferror(s_pe.fd)) { \
		fprintf(stderr, "ERROR FREAD: %s\n", strerror(errno)); \
		exit(1); \
	} \

#define FTELL() \
	ftell(s_pe.fd); \
	if (ferror(s_pe.fd)) { \
		fprintf(stderr, "ERROR FTELL: %s\n", strerror(errno)); \
		exit(1); \
	} \

void print_hexa(const char *file);
void print_hexa2(const char *file);

void pe_reader(const char *file);
void pe_print_standard_header();
void pe_print_optional_header_standard();
void pe_print_optional_header_winspec();
void pe_print_optional_header_data_directory();
void pe_print_section_table();
void pe_print_section_header(int index);
void read_rva(char *buffer, rva_t address);
long rva2offset(rva_t address);
rva_t offset2rva(long offset);
void pe_print_section_import();
void pe_print_section_import_lookup(const char *dll_name, rva_t rva);
void pe_print_section_edata();
void pe_print_section_reloc();
char *get_section(rva_t rva);
void print_section_list(int max);
void pe_print_section_iat();
void pe_print_section_exception();

int is_in_export_section(rva_t rva);
int is_in_code_section(rva_t rva);

char *get_flags(char *buffer, size_t size, int section, int flags);
char* list_flags(char *buffer, size_t size, int section, int flags);
int has_flags(int flag, int flags);

size_t strlcat(char *dst, char *src, size_t size);

void set_record(int section, int code, char *label);
void init_map();
char* map(int section, int code);

#endif // PE_READER_H