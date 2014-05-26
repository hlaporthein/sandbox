#ifndef SYNCHRO_H
#define SYNCHRO_H

int is_dir(const char* file);
void copy_file(const char* srcpath, const char* destpath, int buffer_size);
void copy_dir(const char* src, const char* dest);
void rmfile(const char* file);

#endif