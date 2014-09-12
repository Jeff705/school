#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char *argv[]) {

	char * const envp[] = {"LD_PRELOAD=./memory_shim.so"};

	pid_t childp = fork();

	if(childp == 0) {
		/* I'm the child process */
		int newargsize = argc - 2;
		char **newargv = NULL;
		if(newargsize > 0) {
			newargv = malloc(sizeof(char*)*newargsize);
			int i;

			for(i = 0; i < newargsize; i++) {
				newargv[i] = argv[i + 1];
			}
		}
		execve(argv[1], newargv, envp);
	}

	else {
		/*TWIDDLE YOUR THUMBS, PARENT PROCESS!*/
		waitpid(childp, NULL, 0);
			
	}
	return 0;

}