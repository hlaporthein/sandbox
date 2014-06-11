#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 1024

void print_chararray(int length, char** array) {
	for (int i = 0; i < length; i++) {
		printf("array[%d]=%s\n", i, array[i]);
	}
}

//void array_copy(char** dst, char src, int length) {
//	void** src2 = (void**) src;
//	for (int i = 0; i < length; i++) {
//		dst[i] = (char*) src2[i];
//	}
//}

int main () {
	int size = 2;
	char array[2][BUFFER_SIZE];// = { "coucou", "hello" };
	strncpy(array[0], "coucou", BUFFER_SIZE);
	strncpy(array[1], "hello", BUFFER_SIZE);

	char* array2[2];

	for (int i = 0; i < 2; i++) {
		array2[i] = array[i];
	}

	//array_copy(array2, array, 2);

	print_chararray(size, array);

	return 0;
}