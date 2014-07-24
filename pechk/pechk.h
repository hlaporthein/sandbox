#ifndef _PECHK_H
#define _PECHK_H

#include <windows.h>

int get_current_pe_checksum(const char *filename, unsigned int *checksum);
int compute_pe_checksum(const char *filename, int *chksum);
unsigned short int checksum(short int current, unsigned short int *buffer, int dw_nbr);

#endif // _PECHK_H

