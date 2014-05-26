#include <stdio.h>

int main (int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s src dest\n", argv[0]);
		return 1;
	}

	char buf[4096];
	size_t size;

	FILE* source = fopen(argv[1], "rb");
	FILE* dest = fopen(argv[2], "wb");

	while ((size = fread(buf, 1, BUFSIZ, source))) {
		fwrite(buf, 1, size, dest);
	}

	fclose(source);
	fclose(dest);

	return 0;
}