#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/reg.h>
#include <sys/types.h>

int main(int argc, char **argv) {

	pid_t child;

	char *argholder = argv[1];
	int newargc = 0;
	char *execthis = NULL;
	char *iamsolost[10];
	while((iamsolost[newargc] = strtok(argholder, " "))) {
		newargc++;
	}
	execthis = iamsolost[0];
	
	char *const execvpargs[newargc];
	int i = 1;

	for(i = 1; i <= newargc; i++) {
		*execvpargs[(i-1)] = *iamsolost[i];
	}

	child = fork();

	if(child == 0) {
		ptrace(PTRACE_TRACEME);

		execvp(execthis, execvpargs);
	}

	else {
		int status, syscall_id;
		int finished = 0;

		waitpid(child, &status, 0);

		ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);

		while((!(WIFSTOPPED(status) && WSTOPSIG(status) & 0x80)) && finished == 0) {
			ptrace(PTRACE_SYSCALL, child, 0, 0);

			waitpid(child, &status, 0);
			if(WIFEXITED(status)) {
				finished = 1;
			}
		}
		if(finished == 0) {
			syscall_id = ptrace(PTRACE_PEEKUSER, child, sizeof(long)*ORIG_RAX, NULL);
			printf("Look, we called %d\n",syscall_id);
		}
		ptrace(PTRACE_CONT, child, NULL, NULL);
		waitpid(child, NULL, 0);
	}
	return 0;
}