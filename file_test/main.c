#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>

void write_text() {
	char *fname = "test.txt";
	FILE *file = fopen(fname, "wb");
	if (!file) {
		printf("Can't open the file %s\n", fname);
		return;
	}

	char *msg = "Hello World\n";
	size_t s = fwrite(msg, sizeof(char), strlen(msg), file);
	if (ferror(file)) {
		printf("File write error: %s\n", strerror(errno));
		goto cleanup;
	}

cleanup:
	fclose(file);
}

void write_bin() {
	char *fname = "test2.txt";
	FILE *file = fopen(fname, "wb");
	if (!file) {
		printf("Can't open the file %s\n", fname);
		return;
	}

	unsigned char msg[6] = {0, 1, 0, 2, 0, 3};
	size_t s = fwrite(msg, sizeof(unsigned char), 6, file);
	if (ferror(file)) {
		printf("File write error: %s\n", strerror(errno));
		goto cleanup;
	}

cleanup:
	fclose(file);
}

void write_text2() {
	char *fname = "test3.txt";
	int fd = open(fname, O_WRONLY | O_CREAT);
	if (fd == -1) {
		printf("Can't open the file %s: %s\n", fname, strerror(errno));
		return;
	}

	char *msg = "Hello World\n";
	ssize_t s = write(fd, msg, strlen(msg));
	if (s == -1) {
		printf("File write error: %s\n", strerror(errno));
		goto cleanup;
	}

cleanup:
	close(fd);
}

void write_bin2() {
	char *fname = "test4.txt";
	int fd = open(fname, O_WRONLY | O_CREAT);
	if (fd == -1) {
		printf("Can't open the file %s: %s\n", fname, strerror(errno));
		return;
	}

	unsigned char msg[6] = {0, 1, 0, 2, 0, 3};
	ssize_t s = write(fd, msg, 6);
	if (s == -1) {
		printf("File write error: %s\n", strerror(errno));
		goto cleanup;
	}

cleanup:
	close(fd);
}

int main() {
	write_text();
	write_bin();
	write_text2();
	write_bin2();
	return 0;
}