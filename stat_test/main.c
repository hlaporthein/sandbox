#include <unistd.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define PRINT_STAT(name) \
	if (print_stat(name) != 0) { \
		goto cleanup; \
	}

void my_strmode(char* buf, size_t len, mode_t mode) {
	snprintf(buf, len, "%06o", mode);
}

void print_statbuf(struct stat statbuf) {
	char buf[1024];
	my_strmode(buf, 1024, statbuf.st_mode);
	printf("---------------\n");
	printf("mode=%s\n", buf);
	printf("size=%d\n", statbuf.st_size);
	int blocks = statbuf.st_size / 512;
	if (statbuf.st_size % 512) {
		blocks++;
	}
	printf("blocks=%d\n", blocks);
	printf("size on disk=%d\n", (blocks*512));
	printf("sizeof(mode)=%d\n", sizeof(statbuf.st_mode));
	printf("isDir? %s\n", (S_ISDIR(statbuf.st_mode))? "yes" : "no");
	printf("isFile? %s\n", (S_ISREG(statbuf.st_mode))? "yes" : "no");
	//printf("isSimlnk? %s\n", (S_ISLNK(statbuf.st_mode))? "yes" : "no");
	printf("isReadable? %s\n", (statbuf.st_mode & S_IRUSR)? "yes" : "no");
	printf("isWritable? %s\n", (statbuf.st_mode & S_IWUSR)? "yes" : "no");
	printf("\n");
}

int print_stat(const char* name) {
	struct stat statbuf;
	int result = stat(name, &statbuf);
	if (result == -1) {
		fprintf(stderr, "Error stating \"%s\"(%d): %s\n", name, errno, strerror(errno));
		goto cleanup;
	}

	printf("\nStating: %s\n", name);
	print_statbuf(statbuf);
cleanup:
	return result;
}

int main(int argc, char* argv) {
	int result = 0;
	char* file = "main.c";
	char* dir = ".";
	char* simlnk = "lnk";

	printf("PATH_MAX=%d\n", PATH_MAX);

	PRINT_STAT(file);
	PRINT_STAT(dir);
	PRINT_STAT(simlnk);

cleanup:
	return result;
}