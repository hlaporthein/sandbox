#include <stdio.h>
#include <errno.h>
#include <string.h>

#include "utf8fs.h"

#define TRY(statement, condition, error_message, ...) \
	statement; \
	if (condition) { \
		printf(error_message, ##__VA_ARGS__); \
		result = -1; \
		goto cleanup; \
	}

#define DO(statement) \
	if (statement) { \
		goto cleanup; \
	}


int write_file(const char *filename, const char *content) {
	int result = 0;
	FILE *fd = NULL;
	fd = TRY(utf8_fopen(filename, "wb"), fd == NULL, "Cannot open file. Error(%d): %s\n", errno, strerror(errno));
	fprintf(fd, "Hello %s\n", content);

cleanup:
	if (fd) {
		fclose(fd);
	}
	return result;

}

int main() {
	int result = 0;
	DO(write_file("αβγ.txt", "Jean-Louis Guénégo"));
	DO(write_file("abc.txt", "Jean-Louis Guénégo"));
cleanup:
	return result;
}