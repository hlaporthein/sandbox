#ifndef SYNCHRO_H
#define SYNCHRO_H

int is_dir(const char* file);
int exists(const char* file);
void cp(const char* srcpath, const char* destpath, int buffer_size);
void copy_dir(const char* src, const char* dest);
void sync_dir(const char* src, const char* dest);
int is_more_recent(const char* src, const char* dst);

#endif