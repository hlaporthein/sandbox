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
	printf("Doing : %s\n", #statement); \
	result = statement; \
	if (result) { \
		printf("Error\n"); \
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

int ls(const char *s) {
	int result = 0;

	UTF8_DIR *d = NULL;
	d = utf8_opendir(s);
	if (!d) {
		printf("Cannot open directory for sync %s. Error(%d): %s\n", s, errno, strerror(errno));
		result = -1;
		goto cleanup;
	}

	struct utf8_dirent *dir;
	while ((dir = utf8_readdir(d)) != NULL) {
		char* fname = dir->d_name;
		if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0) {
			continue;
		}
		printf("fname=%s\n", fname);
	}
cleanup:
	if (d) {
		utf8_closedir(d);
	}
	return result;
}

int main() {
	int result = 0;
	DO(write_file("αβγ.txt", "Jean-Louis Guénégo"));
	DO(write_file("abc.txt", "Jean-Louis Guénégo"));
	DO(utf8_mkdir("αβγ"));

	DO(ls("."));
	struct stat buf;
	DO(utf8_stat("αβγ.txt", &buf));
	printf("size of file: %d\n", (int) buf.st_size);
	DO(utf8_rmdir("αβγ"));
	DO(utf8_unlink("αβγ.txt"));
	printf("End with success.");
cleanup:
	return result;
}