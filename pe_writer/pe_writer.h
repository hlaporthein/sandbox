#ifndef PE_READER_H
#define PE_READER_H

#include "my_winnt.h"

#define BUFFER_SIZE 2048

#define FSEEK(fd, offset) \
	if (fseek(fd, offset, SEEK_SET) != 0) { \
		fprintf(stderr, "ERROR FSEEK: %s\n", strerror(errno)); \
		exit(1); \
	} \

#define FREAD(buffer, size_obj, nbr, fd) \
	fread(buffer, size_obj, nbr, fd); \
	if (ferror(fd)) { \
		fprintf(stderr, "ERROR FREAD: %s\n", strerror(errno)); \
		exit(1); \
	} \

#define FWRITE(buffer, size_obj, nbr, fd) \
	fwrite(buffer, size_obj, nbr, fd); \
	if (ferror(fd)) { \
		fprintf(stderr, "ERROR FWRITE: %s\n", strerror(errno)); \
		exit(1); \
	} \

#define FTELL() \
	ftell(s_pe.fd); \
	if (ferror(s_pe.fd)) { \
		fprintf(stderr, "ERROR FTELL: %s\n", strerror(errno)); \
		exit(1); \
	} \

#define TRY(x) \
	if (x) { \
		result = 1; \
		goto cleanup; \
	}

#ifdef _DEBUG
#define DEBUG_VAR_STR(var) printf(#var"=%s\n", var)
#define DEBUG_VAR_INT(var) printf(#var"=%d\n", var)
#define DEBUG_VAR_HEX(var) printf(#var"=0x%08X\n", var)
#define DEBUG_VAR_WORD(var) printf(#var"=0x%04X\n", var)
#else
#define DEBUG_VAR_STR(var)
#define DEBUG_VAR_INT(var)
#define DEBUG_VAR_HEX(var)
#define DEBUG_VAR_WORD(var)
#endif // _DEBUG

#define STARTS_WITH(str, prefix) (strstr(str, prefix) == str)
#define EQUALS(s1, s2) (strcmp(s1, s2) == 0)

extern char *pe_file;
extern char *cmd_file;

int pe_writer();
int process_cmd(char *cmd);
int parse_cmd(char *cmd, int argc, ...);
void chomp(char* s);
int update_section_characteristics(char *name, int val, int mode);
int get_section_table_offset(FILE *fd);
int get_pe_offset(FILE *fd);


#define MODE_SET_FLAGS 0
#define MODE_ADD_FLAGS 1
#define MODE_REMOVE_FLAGS 2

#endif // PE_READER_H