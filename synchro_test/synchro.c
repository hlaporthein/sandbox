#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdarg.h>
#include <windows.h>
#include <errno.h>
#include <string.h>
#include <time.h>

#include "synchro.h"

char* mode_map[] = {
	"preview", "real"
};

print_t g_print = NULL;
progress_value_t g_progress_value = NULL;
int g_abort = FALSE;
int g_total_step = 0;
int g_current_step = 0;
clock_t g_last_call = 0;
clock_t g_min_delay = 300;
int g_log_level = 0;
int g_max_op = 0;
int g_file_full = FALSE;
int g_total_op = 0;

void set_print(print_t print) {
	g_print = print;
}

void set_progress_value(progress_value_t progress_value) {
	g_progress_value = progress_value;
}

void set_progress_min_delay(int min_delay) {
	g_min_delay = (clock_t) min_delay;
}

void set_abort() {
	g_abort = 1;
}

void reset_abort() {
	g_abort = 0;
}

int is_aborted() {
	return g_abort;
}

void synchro_log(const char* format, ...) {
	char buf[BUFFER_SIZE];
	va_list params;
	va_start(params, format);
	vsnprintf(buf, BUFFER_SIZE, format, params);
	va_end(params);
	g_print(buf);
}

int exists(const char* file) {
	struct stat statbuf;
	if (stat(file, &statbuf) == -1) {
		DEBUG_LOG("%s does not exist\n", file);
	} else {
		DEBUG_LOG("%s exists\n", file);
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

void set_temp_dir(const char* tmp_dir) {
	g_tmp_dir = tmp_dir;
}

void set_max_op(int max_op) {
	g_max_op = max_op;
}

int cp(const char* srcpath, const char* destpath, int buffer_size) {
	int result = 0;
	INFO_LOG("Copying: %s => %s\n", srcpath, destpath);
	char buf[buffer_size];

	FILE* source = fopen(srcpath, "rb");
	FILE* dest = fopen(destpath, "wb");

	size_t size = 0;
	while ((size = fread(buf, 1, buffer_size, source))) {
		if (g_abort) {
			break;
		}

		size_t s = fwrite(buf, 1, size, dest);
		if (s != size) {
			ERROR_LOG("Cannot write data. Error(%d): %s\n", errno, strerror(errno));
			result = 1;
			goto cleanup;
		}
		g_current_step += size;
		inform_progress();
	}

cleanup:
	fclose(source);
	fclose(dest);

	if (g_abort) {
		unlink(destpath);
	}

	return result;
}

int sync_dir_build_cmd(const char* src, const char* dst, int level) {
	DEBUG_LOG("Starting sync_dir_build_cmd: %s => %s\n", src, dst);
	if (level == 0) {
		g_total_step = 0;
		file_reset(TRUE);
	}
	int result = 0;

	DIR *d = NULL;
	d = opendir(src);
	if (!d) {
		ERROR_LOG("Cannot open directory for sync %s. Error(%d): %s\n", src, errno, strerror(errno));
		result = -1;
		goto cleanup;
	}

	if (!is_dir(dst)) {
		file_push_mkdir(dst);
	}

	struct dirent *dir;
	while ((dir = readdir(d)) != NULL) {
		if (g_abort || g_file_full) {
			break;
		}
		char* fname = dir->d_name;
		if (strcmp(fname, ".") == 0 || strcmp(fname, "..") == 0) {
			continue;
		}

		char src_filepath[PATH_SIZE];
		snprintf(src_filepath, PATH_SIZE, "%s/%s", src, fname);
		char dest_filepath[PATH_SIZE];
		snprintf(dest_filepath, PATH_SIZE, "%s/%s", dst, fname);

		if (is_dir(src_filepath)) {
			sync_dir_build_cmd(src_filepath, dest_filepath, level + 1);
		} else {
			int do_copy = FALSE;
			struct stat src_statbuf;
			result = TRY(stat(src_filepath, &src_statbuf), result == -1, "stat error(%d): %s\n", errno, strerror(errno));

			if (!exists(dest_filepath)) {
				do_copy = TRUE;
			} else {
				struct stat dst_statbuf;
				result = TRY(stat(dest_filepath, &dst_statbuf), result == -1, "stat error(%d): %s\n", errno, strerror(errno));
				if (src_statbuf.st_mtime > dst_statbuf.st_mtime) {
					do_copy = TRUE;
				}
			}

			if (do_copy) {
				file_push_cp(src_filepath, dest_filepath);
				g_total_step += src_statbuf.st_size;
			}
		}
	}

cleanup:
	if (d) {
		closedir(d);
	}

	if (result == 0 && g_abort) {
		result = 1;
	}

	if (level == 0) {
		file_close();
	}
	return result;
}

void inform_progress() {
	if (clock() - g_last_call < g_min_delay) {
		return;
	}
	g_last_call = clock();
	g_progress_value(g_current_step);
}