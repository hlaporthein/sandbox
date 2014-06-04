#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>

#include "synchro.h"

int g_file_full = FALSE;
const char* g_tmp_dir = NULL;

char g_filename[BUFFER_SIZE];
FILE* g_fd = NULL;

int file_reset(int remove) {
	int result = 0;

	file_close();

	snprintf(g_filename, BUFFER_SIZE, "%s/synchro_simple.txt", g_tmp_dir);
	if (remove) {
		unlink(g_filename);
	}
	g_fd = fopen(g_filename, "ab");
	if (!g_fd) {
		ERROR_LOG("Cannot open file %s. Error(%d): %s\n", g_filename, errno, strerror(errno));
		result = errno;
		goto cleanup;
	}

cleanup:
	return result;
}

void file_close() {
	if (g_fd) {
		fclose(g_fd);
		g_fd = NULL;
	}
}

int file_push_mkdir(const char* dir) {
	return file_append("D\n%s\n", dir);
}

int file_push_cp(const char* src, const char* dst) {
	return file_append("F\n%s\n%s\n", src, dst);
}

int file_append(const char* format, ...) {
	int result = 0;
	if (!g_fd) {
		result = file_reset(FALSE);
		if (result) {
			goto cleanup;
		}
	}
	char sBuffer[BUFFER_SIZE];
	va_list params;
	va_start(params, format);
	vsnprintf(sBuffer, BUFFER_SIZE, format, params);
	va_end(params);

	size_t count = strlen(sBuffer);
	size_t s = fwrite(sBuffer, 1, count, g_fd);
	if (s != count) {
		ERROR_LOG("Cannot append to file %s. Error(%d): %s\n", g_filename, errno, strerror(errno));
		result = 1;
		goto cleanup;
	}
	g_total_step++;
cleanup:
	return result;
}
