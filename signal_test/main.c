#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

typedef void (*signal_func)(int);

void myfunction(int signal) {
	printf("Coucou\n");
}

int main() {
	//signal_func previous = signal(SIGABRT, myfunction);

	abort();

	return 0;
}