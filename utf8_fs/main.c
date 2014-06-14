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

int main() {
	int result = 0;
	FILE *fd = NULL;
	fd = TRY(utf8_fopen("αβγ.txt", "wb"), fd == NULL, "Cannot open file. Error(%d): %s\n", errno, strerror(errno));
	char *name = "Jean-Louis Guénégo";
	fprintf(fd, "Hello %s\n", name);

cleanup:
	if (fd) {
		fclose(fd);
	}
	return result;
}