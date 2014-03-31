#ifndef PE_READER_H
#define PE_READER_H

#include "my_winnt.h"

#define BUFFER_SIZE 2048

#define FSEEK(offset) \
	if (fseek(s_coff.fd, offset, SEEK_SET) != 0) { \
		fprintf(stderr, "ERROR FSEEK: %s\n", strerror(errno)); \
		exit(1); \
	} \

#define FREAD(buffer, size_obj, nbr) \
	fread(buffer, size_obj, nbr, s_coff.fd); \
	if (ferror(s_coff.fd)) { \
		fprintf(stderr, "ERROR FREAD: %s\n", strerror(errno)); \
		exit(1); \
	} \

#define FTELL() \
	ftell(s_coff.fd); \
	if (ferror(s_coff.fd)) { \
		fprintf(stderr, "ERROR FTELL: %s\n", strerror(errno)); \
		exit(1); \
	} \

void print_hexa(const char *file);
void print_hexa2(const char *file);

void coff_reader(const char *file);
void coff_print_standard_header();
void coff_print_section_table();
void coff_print_section_header(int index);
void coff_print_directive_section(PIMAGE_SECTION_HEADER p);
void coff_print_symbol_table();
void print_section_list(int max);
void read_offset(char *buf, size_t size, int offset);
int has_bom(const char *buffer);
void print_relocation_table(PIMAGE_SECTION_HEADER p);

char *get_flags(char *buffer, size_t size, int section, int flags);
char* list_flags(char *buffer, size_t size, int section, int flags);
int has_flags(int flag, int flags);

#ifdef __MINGW32__
size_t strlcat(char *dst, char *src, size_t size);
#endif

void set_record(int section, int code, char *label);
void init_map();
char* map(int section, int code);

#endif // PE_READER_H