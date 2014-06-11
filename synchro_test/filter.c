#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "filter.h"
#include "synchro.h"

extern char* g_error_msg;

regex_t* g_file_filter_list = NULL;
regex_t* g_dir_filter_list = NULL;
size_t g_file_filter_list_length = 0;
size_t g_dir_filter_list_length = 0;

static int compile_regex(regex_t** regex, int length, char** filter_list) {
	int result = 0;
	regex_t* r = (regex_t*) malloc(sizeof(regex_t) * length);
	for (int i = 0; i < length; i++) {
		result = regcomp(&r[i], filter_list[i], 0);
		DEBUG_VAR_STR(filter_list[i]);
		if (result) {
			snprintf(g_error_msg, BUFFER_SIZE, "Cannot compile regex: %s", filter_list[i]);
			result = -1;
			goto cleanup;
		}
	}

cleanup:
	*regex = r;
	return result;
}

int set_filter(int type, int length, char** filter_list) {
	int result = 0;
	if (type == FILE_TYPE) {
		DEBUG_LOG("File filters\n");
		result = TRY(compile_regex(&g_file_filter_list, length, filter_list), result < 0, "Could not compile regex for file: %s\n", g_error_msg);
		g_file_filter_list_length = length;
	} else if (type == DIR_TYPE) {
		DEBUG_LOG("Dir filters\n");
		result = TRY(compile_regex(&g_dir_filter_list, length, filter_list), result < 0, "Could not compile regex for dir: %s\n", g_error_msg);
		g_dir_filter_list_length = length;
	}

cleanup:
	return result;
}

int is_filtered(int type, const char* name) {
	int result = TRUE;
	int length = 0;
	regex_t* filter_list = NULL;
	if (type == FILE_TYPE) {
		length = g_file_filter_list_length;
		filter_list = g_file_filter_list;
	} else if (type == DIR_TYPE) {
		length = g_dir_filter_list_length;
		filter_list = g_dir_filter_list;
	}

	for (int i = 0; i < length; i++) {
		int res = regexec(&filter_list[i], name, 0, NULL, 0);
		if (res == 0) {
			goto cleanup;
		} else if (res == REG_NOMATCH) {
			continue;
		} else {
			char msgbuf[1024];
			regerror(res, &filter_list[i], msgbuf, 1024);
			synchro_log("Regex match failed: %s\n", msgbuf);
			goto cleanup;
		}
	}
	result = FALSE;

cleanup:
	return result;
}