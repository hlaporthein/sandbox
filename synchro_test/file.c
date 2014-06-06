#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include <dirent.h>

#include "synchro.h"

const char* g_tmp_dir = NULL;

char g_filename[PATH_SIZE];
FILE* g_fd = NULL;

int file_reset(int remove) {
	int result = 0;

	file_close();

	snprintf(g_filename, PATH_SIZE, "%s/synchro_simple.txt", g_tmp_dir);
	if (remove) {
		unlink(g_filename);
	}
	g_file_full = FALSE;
	g_total_op = 0;
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
	g_total_op++;
	if (g_total_op >= g_max_op && g_max_op > 0) {
		g_file_full = TRUE;
	}
	g_total_step++;
cleanup:
	return result;
}

int run_file() {
	int result = 0;
	file_close();

	g_fd = fopen(g_filename, "rb");
	if (!g_fd) {
		ERROR_LOG("Cannot open file %s. Error(%d): %s\n", g_filename, errno, strerror(errno));
		result = errno;
		goto cleanup;
	}

	char line[LINE_SIZE];
	char line2[LINE_SIZE];
	while (fgets(line, LINE_SIZE, g_fd)) {
		if (g_abort) {
			break;
		}
		chomp(line);
		if (strcmp(line, "D") == 0) {
			fgets(line, LINE_SIZE, g_fd);
			chomp(line);
			int res = TRY(mkdir(line), res != 0, "mkdir error: (%d) %s\n", errno, strerror(errno));
		} else if (strcmp(line, "F") == 0) {
			fgets(line, LINE_SIZE, g_fd);
			chomp(line);
			fgets(line2, LINE_SIZE, g_fd);
			chomp(line2);
			DO(cp(line, line2, BUFFER_SIZE));
		}
		g_current_step++;
		inform_progress();
	}

cleanup:
	if (g_fd) {
		fclose(g_fd);
		g_fd = NULL;
	}

	if (g_abort) {
		result = 1;
	}

	if (g_file_full) {
		result = 2;
	}
	return result;
}

void chomp(char* s) {
	int len = strlen(s);
	if (s[len-1] == '\n') {
		s[len-1] = '\0';
	}
}