#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <stdarg.h>

#include "pe_writer.h"

IMAGE_FILE_HEADER g_coff_header;

int pe_writer() {
	int result = 0;
	FILE *fd = NULL;

	fd = fopen(cmd_file, "rb");
	if (!fd) {
		printf("Cannot open %s: (%d) %s\n", cmd_file, errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	char buffer[BUFFER_SIZE];
	size_t size = 0;
	char *line = NULL;

	while ((line = fgets(buffer, BUFFER_SIZE, fd)) != NULL) {
		if (ferror(fd)) {
			printf("Error while reading command: (%d) %s\n", errno, strerror(errno));
			result = 1;
			goto cleanup;
		}

		chomp(line);
		printf("line=%s\n", line);

		TRY(process_cmd(line));
	}

cleanup:
	if (fd) {
		fclose(fd);
	}

	return result;
}

int process_cmd(char *cmd) {
	int result = 0;

	if (STARTS_WITH(cmd, "update_section_characteristics")) {
		char *name = NULL;
		char *valstr = NULL;
		TRY(parse_cmd(cmd, 2, &name, &valstr));

		DEBUG_VAR_STR(name);
		DEBUG_VAR_STR(valstr);

		int val = 0;
		sscanf(valstr, "%x", &val);
		DEBUG_VAR_HEX(val);

		update_section_characteristics(name, val);
	}

cleanup:
	return result;
}

int update_section_characteristics(char *name, int val) {
	int result = 0;
	FILE *fd = NULL;

	fd = fopen(pe_file, "rb+");
	if (!fd) {
		printf("Cannot open %s: (%d) %s\n", pe_file, errno, strerror(errno));
		result = 1;
		goto cleanup;
	}

	int section_table_offset = get_section_table_offset(fd);
	FSEEK(fd, section_table_offset);

	IMAGE_SECTION_HEADER section_header;
	int bfound = 0;
	int index = 0;
	for (int i = 0; i < g_coff_header.NumberOfSections; i++) {
		FREAD(&section_header, sizeof(IMAGE_SECTION_HEADER), 1, fd);

		char section_name[9];
		memset(section_name, 0, 9);
		memcpy(section_name, section_header.Name, 8);
		if (EQUALS(section_name, name)) {
			bfound = 1;
			break;
		}
		index++;
	}

	if (!bfound) {
		printf("Section %s not found\n", name);
		result = 1;
		goto cleanup;
	}

	FSEEK(fd, section_table_offset + (index * sizeof(IMAGE_SECTION_HEADER)));

	section_header.Characteristics = val;
	FWRITE(&section_header, sizeof(IMAGE_SECTION_HEADER), 1, fd);
	DEBUG_VAR_HEX(section_table_offset);

cleanup:
	if (fd) {
		fclose(fd);
	}

	return result;
}

int get_section_table_offset(FILE *fd) {
	int result = 0;

	int coff_header_offset = get_pe_offset(fd) + 4;

	FSEEK(fd, coff_header_offset);
	FREAD(&g_coff_header, sizeof(IMAGE_FILE_HEADER), 1, fd);

	WORD magic = 0;
	FREAD(&magic, sizeof(WORD), 1, fd);

	DEBUG_VAR_WORD(magic);

	if (magic == IMAGE_NT_OPTIONAL_HDR32_MAGIC) {
		result = coff_header_offset + sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER32);
	} else {
		result = coff_header_offset + sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER64);
	}

cleanup:
	return result;
}

int get_pe_offset(FILE *fd) {
	int result = 0;

	FSEEK(fd, 0x3C);
	FREAD(&result, sizeof(DWORD), 1, fd);

cleanup:
	return result;
}

int parse_cmd(char *cmd, int argc, ...) {
	int result = 0;
	char *rest = cmd;
	va_list argp;
	va_start(argp, argc);

	for (int i = 0; i < argc; i++) {
		rest = strstr(rest, " ");
		if (!rest) {
			result = 1;
			goto cleanup;
		}
		rest[0] = 0;
		rest++;

		char **var = va_arg(argp, char**);
		*var = rest;
	}

cleanup:
	va_end(argp);
	return result;
}

void chomp(char* s) {
	int len = strlen(s);
	if (s[len-1] == '\n') {
		s[len-1] = '\0';
	}
}
