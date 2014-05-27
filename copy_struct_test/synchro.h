#ifndef SYNCHRO_H
#define SYNCHRO_H

int is_dir(const char* file);
int exists(const char* file);
void cp(const char* srcpath, const char* destpath, int buffer_size);
void copy_dir(const char* src, const char* dest);
void sync_dir(const char* src, const char* dest);
int is_more_recent(const char* src, const char* dst);


typedef void (*print_t)(const char* buf);
void set_print(print_t print);

void synchro_log(const char* format, ...);

#ifdef DEBUG_MODE
#define DEBUG(format, ...) synchro_log(format, ##__VA_ARGS__)
#else
#define DEBUG(format, ...)
#endif

#endif