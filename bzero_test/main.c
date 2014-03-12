#include <stdio.h>
#include <stdlib.h>

int main() {
	char buffer[1024];
	snprintf(buffer, 1024, "Coucou asdfasdf");
	printf("buffer: %s\n", buffer);
	bzero(buffer, 1024);
	//memset(buffer, 0, 6);
	printf("buffer: %s\n", buffer);
	return 0;
}