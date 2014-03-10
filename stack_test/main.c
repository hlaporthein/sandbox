#include <stdio.h>
#include <string.h>
#include <errno.h>

#ifndef __MINGW32__
#include <sys/resource.h>
#endif

void manage_stacksize(int stack_size) {
#ifndef __MINGW32__
	const rlim_t kStackSize = (rlim_t) stack_size;
	struct rlimit rl;
	int result;

	result = getrlimit(RLIMIT_STACK, &rl);
	if (result == 0) {
		printf("rlim_cur=%d\n", rl.rlim_cur);
		if (rl.rlim_cur < kStackSize) {
			rl.rlim_cur = kStackSize;
			result = setrlimit(RLIMIT_STACK, &rl);
			if (result != 0) {
				fprintf(stderr, "setrlimit returned result = %d, errno=%d: %s\n", result, errno, strerror(errno));
			}
		}
	}
#endif
}


void recursion(int size) {
	int c[32];
	memset(&c, 0, 32);
	if (size % 1000 == 0) {
		printf("recursion - size = %5d\n", size);
	}
	if (size > 0) {
		recursion(size - 1);
	} else {
		printf("c=%d\n", c[0]);
	}
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Usage: %s <recursion_nbr>\n");
		return 1;
	}
	manage_stacksize(16 * 1024 * 1024);
	int size = atoi(argv[1]);
	recursion(size);
	return 0;
}