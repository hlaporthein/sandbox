#ifndef PE_READER_H
#define PE_READER_H

#include "my_winnt.h"

#define BUFFER_SIZE 2048

void print_hexa(const char *file);
void print_hexa2(const char *file);

void pe_reader(const char *file);
void pe_print_standard_header();
void pe_print_optional_header_standard();
void pe_print_optional_header_winspec();
void pe_print_optional_header_data_directory();
void pe_print_section_table();
void pe_print_section_header(int index);

char *get_flags(char *buffer, size_t size, int section, int flags);
char* list_flags(char *buffer, size_t size, int section, int flags);
int has_flags(int flag, int flags);

size_t strlcat(char *dst, char *src, size_t size);

void set_record(int section, int code, char *label);
void init_map();
char* map(int section, int code);

#endif // PE_READER_H