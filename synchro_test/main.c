#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "synchro.h"

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
	set_temp_dir(".");
	set_print(my_print);
	set_progress_value(my_progress_value);
	set_progress_min_delay(0);

	printf("total step: %d\n", g_total_step);
	int result = sync_dir_build_cmd(argv[1], argv[2], 0);
	if (result) {
		goto cleanup;
	}

	printf("total step: %d\n", g_total_step);
	result = run_file(argv[1], argv[2], 0);

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
