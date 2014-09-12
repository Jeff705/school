#include <stdlib.h>
#include <stdio.h>


int main(int argc, char **argv)
{

	void ** pointer = malloc(sizeof(void *)*1000);
	void ** originalpointer = pointer;
	int i;

	// malloc 1000 int'sizeof
	for(i = 1; i <= 1000; i++)
	{
	*pointer = (void *) malloc(sizeof(int));
	// Did not use *pointer++ because I don't know the operator precedence in this case.
	pointer++;
	}

	//	Free every odd pointer for a total of 500 pointers;
	pointer = originalpointer;
	for(i = 1; i<=500; i++)
	{
		// You want to free the pointer pointed to by pointer.
		free(*pointer);
		// Skip the next pointer
		pointer++;
		pointer++;
	}


/* At this point You should have 500 leaks, 4 bytes each for a total of 2000 bytes */

/* RE - malloc every pointer to a char. This should give you a total of 2000 leaks + 1000 leaks */

	pointer = originalpointer;
	for(i = 1; i <=1000; i++)
	{
		*pointer = (void *) malloc(sizeof(char));
		pointer++;
	}

	/* Final number of leaks = 1500, bytes = 3000 */
	//free(pointer); // This will cause your world to segfault because the pointer is now at the end of the malloc'd block
	free(originalpointer);
	return 0;
}
