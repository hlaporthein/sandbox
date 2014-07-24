#ifndef _PECHK_H
#define _PECHK_H

int get_current_pe_checksum(const char *filename, unsigned int *checksum);
int compute_pe_checksum(const char *filename, unsigned int *chksum);
int update_pe_checksum(const char *filename);

unsigned short int checksum(short int current, unsigned short int *buffer, int dw_nbr);

#endif // _PECHK_H

