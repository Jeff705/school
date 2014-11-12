#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#define PAGE_SIZE 4096

int main() {


	
	void *ptr = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,-1,0);

	if(ptr == (int*)-1) {
		perror("mmap");
	}

	*(int*)ptr = 42;


	void *wackyptr = mmap(NULL,25000,PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	void *otherptr = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,-1,0);


	printf("Mapped addresses: %p\n%p\n%p\n",ptr, otherptr,wackyptr);

	return 0;

}

