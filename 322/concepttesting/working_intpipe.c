#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h>  /* for printf */

int main(int argc, char **argv) 
{ 
	int fd[2];
	int stuff[10] = {34, 345, 12, 34089, 234, 2095, 2090, 92485, 23, 0}; 
	int *temp = malloc(sizeof(int));

	pipe(fd);
	write(fd[1], stuff, sizeof(stuff));
	close(fd[1]); /* gives Forrest a reason to stop running */

	while(read(fd[0], temp, sizeof(int))) { /*we're reading ints, after all!*/
		printf("read %d \n", *temp);
	}
	free(temp);

	exit(0); 
}
