#ifndef DELAY
#error "You must define the DELAY value passing -DDELAY=VALUE to the compiler, VALUE must be a whole number"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/wait.h>

void sleep_sort(int size, char **value) {
	int i = 0;
	int pid;
recall:
	if(i < size) {
		pid = fork();
		if(pid == 0) {
			i++;
			goto recall;
		} else {
			int n = atoi(value[i]);
			usleep(n * DELAY);
			printf("%d ", n);
			fflush(stdout);
			waitpid(pid, NULL, 0);
		}
	}
	if(i == 0) puts("");
}

int main(int argc, char** argv) {
	sleep_sort(argc-1, argv+1);
	return 0;
}
