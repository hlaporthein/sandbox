#ifndef PE_READER_H
#define PE_READER_H

#define BUFFER_SIZE 1024

void print_hexa(const char *file);
void print_hexa2(const char *file);

void pe_reader(const char *file);
char *get_flags(char *buffer, size_t size, int section, int flags);
int has_flags(int flag, int flags);

void set_record(int section, int code, char *label);
void init_map();
char* map(int section, int code);

#endif // PE_READER_H