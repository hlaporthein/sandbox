#include <stdio.h>
#include <unistd.h>

int main(int argc, char*argv[]){
	pid_t current_pid = getpid();
	int pid;
	pid = fork();
	if (pid == 0){       /*child*/
		printf("Child { PID: %d, PPID: %d }\n", getpid(), getppid());
	} else if (pid > 0){  /*parent*/
		printf("Parent{ PID: %d, ChildPID: %d }\n", getpid(), pid);
	}
	sleep(100);
	return 0;
}