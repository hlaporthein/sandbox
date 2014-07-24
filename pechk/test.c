#include "pechk.h"
#include <stdio.h>

int main() {
	int result = 0;
	unsigned int current_checksum = 0;
	result = get_current_pe_checksum("pechk2.dll", &current_checksum);

	printf("Current checksum=0x%08X\n", current_checksum);

	int expected_checksum = 0;
	result = compute_pe_checksum("pechk2.dll", &expected_checksum);

	printf("Expected checksum=0x%08X\n", expected_checksum);

	return result;
}
