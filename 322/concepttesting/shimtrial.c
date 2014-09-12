#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv, char **envp) {

	printf("I'm in main!\n");
	
	pid_t chpid = fork();

	if(chpid == 0)
	{
		char *newargv[3];
		
		newargv[0] = "cat";
		newargv[1] = argv[1];
		newargv[2] = NULL;

		char * const pre[] = {"LD_PRELOAD=./testshim.so"};
		printf("about to execute 'cat' with environment variable %s\n\n",*pre);
		execve("/bin/cat", newargv, pre);
	}

	else
	{
		printf("This executes before the wait\n");
		waitpid(chpid,NULL,0);
		printf("Child is done, getting out of here\n");
	}
	return 0;

}
