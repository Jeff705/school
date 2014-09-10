/* The simplest example of pipe I could think of */ 

#include <unistd.h>
#include <stdlib.h> 
#include <stdio.h>  /* for printf */ 
#include <string.h> /* for strlen */ 

int main(int argc, char **argv) 
{ 
	int fd[2];
	int stuff[2] = {34, 345}; 
	int temp = 0;
	pipe(fd); 
	write(fd[1], stuff, sizeof(stuff));
	close(fd[1]);
	while(read(fd[0], &temp, 4)) {
		printf("read %d \n", temp);
	}

	exit(0); 
}
