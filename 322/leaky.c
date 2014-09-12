#include <stdlib.h>
#include <stdio.h>

//contains 1 leak
int main(int argc, char **argv)
{
	void *p1, *p2, *p3, *p4;
	printf("calling malloc\n");
	p1 = malloc(1345);
	printf("calling malloc\n");
	p2 = malloc(2);
	free(p1);
	printf("calling malloc\n");
	p1 = malloc(134);
	free(p2);
	printf("calling malloc\n");
	p3 = malloc(1);
	printf("calling malloc\n");
	p4 = malloc(981);
	free(p4);
	free(p3);
return 0;
}
