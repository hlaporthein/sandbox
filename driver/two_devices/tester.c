#include <stdio.h>
#include "public.h"

int main() {
	int result = 0;
	HANDLE firstFile = INVALID_HANDLE_VALUE;
	HANDLE secondFile = INVALID_HANDLE_VALUE;

	printf("Opening: " FIRST_FILE_PATH "\n");
	firstFile = CreateFile(FIRST_FILE_PATH, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(firstFile == INVALID_HANDLE_VALUE) {
		printf("Invalid handle\n");
		result = 1;
		goto cleanup;
	}

	printf("Opening: " SECOND_FILE_PATH "\n");
	secondFile = CreateFile(SECOND_FILE_PATH, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	if(secondFile == INVALID_HANDLE_VALUE) {
		printf("Invalid handle\n");
		result = 1;
		goto cleanup;
	}

	printf("Press enter to continue...");

	char str[16];
	gets(str);

cleanup:
	if (firstFile != INVALID_HANDLE_VALUE) {
		CloseHandle(firstFile);
	}
	if (secondFile != INVALID_HANDLE_VALUE) {
		CloseHandle(secondFile);
	}
	return result;
}