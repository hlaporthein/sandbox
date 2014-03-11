#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <png.h>
#include <getopt.h>

#include "qrencode.h"

int main() {
	char *buffer = "Hello";
	bzero( buffer, 6 );

	printf("buffer: %s\n", buffer);
}