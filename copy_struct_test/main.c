#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "synchro.h"

int g_total_step = 0;

void my_print(const char* buf) {
	printf("%s", buf);
}

void my_progress_value(const int value) {
	int percent = (int) (100.0 * value) / g_total_step;
	printf("%02d%%\n", percent);
}

int main (int argc, char **argv) {
	if (argc != 3) {
		printf("Usage: %s src dest\n", argv[0]);
		return 1;
	}

	set_print(my_print);
	set_progress_value(my_progress_value);
	set_progress_min_delay(0);
	set_mode(PREVIEW_MODE);
	printf("total step: %d\n", get_total_step());
	int result = sync_dir(argv[1], argv[2]);
	if (result) {
		goto cleanup;
	}
	g_total_step = get_total_step();
	printf("total step: %d\n", get_total_step());
	set_mode(REAL_MODE);
	result = sync_dir(argv[1], argv[2]);

cleanup:
	return (unsigned char) result;
}

//	int buf_sizes[] = {128, 512, 1024, 2048, 4096, 1<<13, 1<<14, 1<<15, 1<<16, 1<<17, 1<<18, 1<<19, 1<<20};
//	int n = sizeof(buf_sizes) / sizeof(int);
//
//	for (int i = 0; i < n; i++) {
//		clock_t start = clock();
//		cp(argv[1], argv[2], buf_sizes[i]);
//		clock_t end = clock();
//		unlink(argv[2]);
//		printf("duration[%d]: %d\n", buf_sizes[i], (int) (end - start));
//	}
//	printf("CLOCKS_PER_SEC: %ld\n", CLOCKS_PER_SEC);
