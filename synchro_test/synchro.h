#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "file.h"
#include "filter.h"

#define PATH_SIZE 1<<15
#define BUFFER_SIZE 1<<16
#define LINE_SIZE 1<<16
#define CP_BUFFER_SIZE 1 << 16

#define TRUE 1
#define FALSE 0

#define DIR_TYPE 0
#define FILE_TYPE 1

#define MKDIR_STEP 100

#define DO(statement) \
	if ((result = statement)) { \
		goto cleanup; \
	}

#define TRY(statement, condition, error_message, ...) \
	statement; \
	if (condition) { \
		synchro_log(error_message, ##__VA_ARGS__); \
		result = -1; \
		goto cleanup; \
	}

extern int g_abort;
extern int g_total_step;
extern int g_current_step;
extern int g_max_op;
extern int g_total_op;
extern int g_file_full;

int is_dir(const char* file);
int exists(const char* file);
int cp(const char* srcpath, const char* destpath, int buffer_size);
int sync_dir_build_cmd(const char* src, const char* dst, int level);
void inform_progress();

typedef void (*print_t)(const char* buf);
typedef void (*progress_value_t)(const int v);
void set_print(print_t print);
void set_progress_value(progress_value_t progress_value);
void set_abort();
void reset_abort();
int is_aborted();
void set_progress_min_delay(int min_delay);
void set_temp_dir(const char* tmp_dir);
void set_max_op(int max_op);

void synchro_log(const char* format, ...);

#ifdef DEBUG_MODE
#define DEBUG_LOG(format, ...) synchro_log(format, ##__VA_ARGS__)
#else
#define DEBUG_LOG(format, ...)
#endif

#define DEBUG_VAR_STR(var) DEBUG_LOG(#var"=%s\n", var)
#define DEBUG_VAR_INT(var) DEBUG_LOG(#var"=%d\n", var)

#define ERROR_LOG(format, ...) synchro_log("file: %s, line: %d ", __FILE__, __LINE__); \
	synchro_log(format, ##__VA_ARGS__)
#define INFO_LOG(format, ...) synchro_log(format, ##__VA_ARGS__)


#endif