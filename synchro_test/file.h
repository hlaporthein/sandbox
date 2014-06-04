#ifndef FILE_H
#define FILE_H

extern int g_file_full;
extern const char* g_tmp_dir;

int file_reset();
void file_close();
int file_push_mkdir(const char* dir);
int file_push_cp(const char* src, const char* dst);
int file_append(const char* format, ...);

#endif