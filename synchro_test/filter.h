#ifndef FILTER_H
#define FILTER_H

#include "synchro.h"

typedef struct {
	char filter[BUFFER_SIZE];
	char label[BUFFER_SIZE];
	int is_dir;
	void* regex;
} filter_t;

int set_filter(int length, filter_t* filter_list);
void free_filter();

int is_filtered(int type, const char* name);

#endif //FILTER_H