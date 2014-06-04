#ifndef SYNCHRO_H
#define SYNCHRO_H

#define PATH_SIZE 1<<16
#define BUFFER_SIZE 1<<16

int is_dir(const char* file);
int exists(const char* file);
void cp(const char* srcpath, const char* destpath, int buffer_size);
int copy_dir(const char* src, const char* dest);
int sync_dir(const char* src, const char* dst, int level);
int is_more_recent(const char* src, const char* dst);
void inform_progress();


typedef void (*print_t)(const char* buf);
typedef void (*progress_value_t)(const int v);
void set_print(print_t print);
void set_progress_value(progress_value_t progress_value);
void set_abort();
void reset_abort();
int is_aborted();
void set_mode(int mode);
int get_total_step();
void set_progress_min_delay(int min_delay);

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

#define REAL_MODE_EXEC(statement) \
	if (g_mode == REAL_MODE) { \
		statement; \
		g_current_step++; \
		inform_progress(); \
	} else { \
		g_total_estimated_step++; \
	}

#endif