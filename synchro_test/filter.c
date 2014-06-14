#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

#include "filter.h"
#include "synchro.h"

extern char* g_error_msg;

filter_t* g_filter_list = NULL;
size_t g_filter_list_length = 0;

int set_filter(int length, filter_t* filter_list) {
	int result = 0;
	g_filter_list_length = length;
	g_filter_list = filter_list;

	for (int i = 0; i < length; i++) {
		regex_t* regex = (regex_t*) malloc(sizeof(regex_t));
		g_filter_list[i].regex = regex;
		result = regcomp(regex, g_filter_list[i].filter, 0);
		DEBUG_VAR_STR(g_filter_list[i].filter);
		if (result) {
			snprintf(g_error_msg, BUFFER_SIZE, "Cannot compile regex: %s", filter_list[i].filter );
			result = -1;
			goto cleanup;
		}
	}

cleanup:
	return result;
}

void free_filter() {
	for (int i = 0; i < g_filter_list_length; i++) {
		regex_t* regex = (regex_t*) g_filter_list[i].regex;
		regfree(regex);
		free(regex);
		g_filter_list[i].regex = NULL;
	}
}

int is_filtered(int is_dir, const char* name) {
	int result = TRUE;

	for (int i = 0; i < g_filter_list_length; i++) {
		if (g_filter_list[i].is_dir != is_dir) {
			continue;
		}
		regex_t* regex = (regex_t*) g_filter_list[i].regex;
		int res = regexec(regex, name, 0, NULL, 0);
		if (res == 0) {
			goto cleanup;
		} else if (res == REG_NOMATCH) {
			continue;
		} else {
			char msgbuf[1024];
			regerror(res, regex, msgbuf, 1024);
			synchro_log("Regex match failed: %s\n", msgbuf);
			goto cleanup;
		}
	}
	result = FALSE;

cleanup:
	return result;
}