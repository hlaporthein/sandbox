#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

int is_dir(const char* file) {
	struct stat statbuf;
	if(stat(file, &statbuf) == -1) {
		return 0;
	}
	return S_ISDIR(statbuf.st_mode);
}

void copy_file(const char* srcpath, const char* destpath, int buffer_size) {
	char buf[buffer_size];

	FILE* source = fopen(srcpath, "rb");
	FILE* dest = fopen(destpath, "wb");

	int counter = 0;

//	struct stat statbuf;
//	if(stat(srcpath, &statbuf) == -1) {
//		return;
//	}
//	int total_size = statbuf.st_size;

	size_t size = 0;
	while ((size = fread(buf, 1, buffer_size, source))) {
		fwrite(buf, 1, size, dest);
		counter += size;
		//printf("counter: %d/%d\n", counter, total_size);
	}

	fclose(source);
	fclose(dest);
}

void copy_dir(const char* src, const char* dest) {
	if (!is_dir(dest)) {
		mkdir(dest);
	}
	DIR *d;
	d = opendir(src);
	if (!d) {
		printf("Source does not exist: %s\n", src);
		return;
	}

	struct dirent *dir;
	while ((dir = readdir(d)) != NULL) {
		char* fname = dir->d_name;
		if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0) {
			continue;
		}

		char src_filepath[256];
		sprintf(src_filepath, "%s/%s", src, fname);
		char dest_filepath[256];
		sprintf(dest_filepath, "%s/%s", dest, fname);

		if (is_dir(src_filepath)) {
			copy_dir(src_filepath, dest_filepath);
		} else {
			printf("copying: %s => %s\n", src_filepath, dest_filepath);
			copy_file(src_filepath, dest_filepath, 4096);
		}
	}

	closedir(d);
}