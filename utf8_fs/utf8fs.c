#include "utf8fs.h"

#define BUFFER_SIZE 1 << 16

#include <windows.h>

int is_ansi(const char*s) {
	do {
		if (*s < 0) {
			return 0;
		}
	} while (s++);
	return 1;
}

wchar_t *getWideCharFromUTF8(const char *s) {
	int len = strlen(s) + 1;
	int length = MultiByteToWideChar(CP_UTF8, 0, s, len, 0, 0);
	wchar_t *result = (wchar_t *) malloc(sizeof(wchar_t) * length);
	MultiByteToWideChar(CP_UTF8, 0, s, len, result, length);
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
