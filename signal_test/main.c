#ifdef __MINGW32__

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

typedef void (*signal_func)(int);

void myfunction(int signal) {
	printf("Coucou: %d\n", signal);
}

int main() {
	signal_func previous = signal(SIGINT, myfunction);
	signal_func previous2 = signal(SIGKILL, myfunction);

	//Sleep(10000);
	while(1) {
	}

	return 0;
}

#else

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

typedef void (*signal_func)(int);

void myfunction(int signal, siginfo_t *siginfo, void *context) {
	printf("Coucou: %d\n", signal);
}

int main() {
	struct sigaction action;
	memset(&action, 0, sizeof(action));
	action.sa_sigaction = myfunction;
	action.sa_flags = SA_NOCLDSTOP | SA_SIGINFO;

	if (sigaction(SIGUSR1, &action, NULL) == -1) {
		perror("sigaction");
	}

	printf("SIGUSR1=%d\n", SIGUSR1);

	sleep(200);

	return 0;
}

#endif