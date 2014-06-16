#include "utf8fs.h"

#define BUFFER_SIZE 1 << 16

#include <windows.h>
#include <wchar.h>
#include <io.h>
#include <dirent.h>

int is_ansi(const char*s) {
	//printf("is_ansi(%s)\n", s);
	int result = 1;
	while (*s != 0) {
		//printf("*s=%c\n", *s);
		if (*s < 0) {
			result = 0;
			break;
		}
		s++;
	}
	//printf("result=%d\n", result);
	return result;
}

wchar_t *getWideCharFromUTF8(const char *s) {
	int len = strlen(s) + 1;
	int length = MultiByteToWideChar(CP_UTF8, 0, s, len, 0, 0);
	wchar_t *result = (wchar_t *) malloc(sizeof(wchar_t) * length);
	MultiByteToWideChar(CP_UTF8, 0, s, len, result, length);
	return result;
}

char *getUTF8FromWideChar(const wchar_t *ws) {
	int len = wcslen(ws) + 1;
	int length = WideCharToMultiByte(CP_UTF8, 0, ws, len, 0, 0, NULL, NULL);
	char *result = (char *) malloc(sizeof(char *) * length);
	WideCharToMultiByte(CP_UTF8, 0, ws, len, result, length, NULL, NULL);
	return result;
}

FILE *utf8_fopen(const char *path, const char *mode) {
	if (is_ansi(path)) {
		return fopen(path, mode);
	}

	FILE* fd = 0;
	wchar_t *wpath = NULL;
	wchar_t *wmode = NULL;
	wpath = getWideCharFromUTF8(path);
	wmode = getWideCharFromUTF8(mode);
	fd = _wfopen(wpath, wmode);

	if (wpath) {
		free(wpath);
	}
	if (wmode) {
		free(wmode);
	}
	return fd;
}

int utf8_mkdir(const char *path) {
	if (is_ansi(path)) {
		return mkdir(path);
	}

	int result = 0;
	wchar_t *wpath = NULL;
	wpath = getWideCharFromUTF8(path);
	result = _wmkdir(wpath);
	if (wpath) {
		free(wpath);
	}
	return result;
}

int utf8_rmdir(const char *path) {
	if (is_ansi(path)) {
		return rmdir(path);
	}

	int result = 0;
	wchar_t *wpath = NULL;
	wpath = getWideCharFromUTF8(path);
	result = _wrmdir(wpath);
	if (wpath) {
		free(wpath);
	}
	return result;
}

int utf8_unlink(const char *path) {
	if (is_ansi(path)) {
		return unlink(path);
	}

	int result = 0;
	wchar_t *wpath = NULL;
	wpath = getWideCharFromUTF8(path);
	result = _wunlink(wpath);
	if (wpath) {
		free(wpath);
	}
	return result;
}

#define  ANSI_TYPE 0
#define WCHAR_TYPE 1

UTF8_DIR *utf8_opendir(const char *dirname) {
	UTF8_DIR* result = NULL;
	wchar_t *wdirname = NULL;

	wdirname = getWideCharFromUTF8(dirname);
	_WDIR* dir = _wopendir(wdirname);
	if (dir == NULL) {
		goto cleanup;
	}
	result = (UTF8_DIR*) malloc(sizeof(UTF8_DIR));
	result->dir = dir;
	result->first = NULL;

cleanup:
	if (wdirname) {
		free(wdirname);
	}
	return result;
}

struct utf8_dirent *utf8_readdir(UTF8_DIR *dirp) {
	struct utf8_dirent *result = NULL;
	struct _wdirent* dir_entry = _wreaddir(dirp->dir);
	if (dir_entry == NULL) {
		return NULL;
	}
	result = (struct utf8_dirent *) malloc(sizeof(struct utf8_dirent));
	result->dir_entry = dir_entry;
	result->d_name = getUTF8FromWideChar(dir_entry->d_name);
	result->next = dirp->first;
	dirp->first = result;
	return result;
}

void utf8_free_entry_list(struct utf8_dirent *first) {
	if (first == NULL) {
		return;
	}
	utf8_free_entry_list(first->next);
	free(first->d_name);
	free(first);
}

int utf8_closedir(UTF8_DIR *dirp) {
	if (dirp == NULL) {
		return 0;
	}
	int result = 0;
	result = _wclosedir(dirp->dir);
	// clean the linked list of read entries.
	utf8_free_entry_list(dirp->first);

	free(dirp);
	return result;
}

int utf8_stat(const char *path, struct stat *buf) {
	if (is_ansi(path)) {
		return stat(path, buf);
	}

	int result = 0;
	wchar_t *wpath = NULL;
	wpath = getWideCharFromUTF8(path);
	result = _wstat(wpath, buf);
	if (wpath) {
		free(wpath);
	}
	return result;
}
