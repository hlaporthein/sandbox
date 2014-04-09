#include <signal.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("Usage: %s PID\n", argv[0]);
		return 1;
	}
	pid_t pid = atoi(argv[1]);
	kill(pid, SIGUSR1);
	return 0;
}