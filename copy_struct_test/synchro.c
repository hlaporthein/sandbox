#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>

#include "synchro.h"

int exists(const char* file) {
	struct stat statbuf;
	if (stat(file, &statbuf) == -1) {
		printf("%s does not exist\n", file);
	} else {
		printf("%s exists\n", file);
	}
	return stat(file, &statbuf) != -1;
}

int is_dir(const char* file) {
	struct stat statbuf;
	if(stat(file, &statbuf) == -1) {
		return 0;
	}
	return S_ISDIR(statbuf.st_mode);
}

void cp(const char* srcpath, const char* destpath, int buffer_size) {
	char buf[buffer_size];

	FILE* source = fopen(srcpath, "rb");
	FILE* dest = fopen(destpath, "wb");

	int counter = 0;

	size_t size = 0;
	while ((size = fread(buf, 1, buffer_size, source))) {
		fwrite(buf, 1, size, dest);
		counter += size;
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
			cp(src_filepath, dest_filepath, 1<<16);
		}
	}

	closedir(d);
}

void sync_dir(const char* src, const char* dst) {
	if (!is_dir(dst)) {
		copy_dir(src, dst);
		return;
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
		sprintf(dest_filepath, "%s/%s", dst, fname);

		if (is_dir(src_filepath)) {
			sync_dir(src_filepath, dest_filepath);
		} else {
			if (!exists(dest_filepath) || is_more_recent(src_filepath, dest_filepath)) {
				printf("Copying: %s => %s\n", src_filepath, dest_filepath);
				cp(src_filepath, dest_filepath, 1<<16);
			}
		}
	}

	closedir(d);
}

int is_more_recent(const char* src, const char* dst) {
	struct stat src_statbuf;
	stat(src, &src_statbuf);
//	if(stat(src, &src_statbuf) == -1) {
//		return 0;
//	}
	struct stat dst_statbuf;
	stat(dst, &dst_statbuf);
//	if(stat(dst, &dst_statbuf) == -1) {
//		return 0;
//	}

	char *c = "no";
	if (src_statbuf.st_mtime > dst_statbuf.st_mtime) {
		c = "yes";
	}

	printf("is src more recent (%s)? %s\n", src, c);
	return src_statbuf.st_mtime > dst_statbuf.st_mtime;
}