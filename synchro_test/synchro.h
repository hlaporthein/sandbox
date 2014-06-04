#ifndef SYNCHRO_H
#define SYNCHRO_H

#include "file.h"

#define PATH_SIZE 1<<16
#define BUFFER_SIZE 1<<16
#define LINE_SIZE 1<<16

#define TRUE 1
#define FALSE 0

extern int g_total_step;

int is_dir(const char* file);
int exists(const char* file);
void cp(const char* srcpath, const char* destpath, int buffer_size);
int sync_dir_build_cmd(const char* src, const char* dst, int level);
int is_more_recent(const char* src, const char* dst);
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

void synchro_log(const char* format, ...);

#ifdef DEBUG_MODE
#define DEBUG_LOG(format, ...) synchro_log(format, ##__VA_ARGS__)
#else
#define DEBUG_LOG(format, ...)
#endif

#define ERROR_LOG(format, ...) synchro_log(format, ##__VA_ARGS__)
#define INFO_LOG(format, ...) synchro_log(format, ##__VA_ARGS__)

#define PREVIEW_MODE 0
#define REAL_MODE 1

#endif