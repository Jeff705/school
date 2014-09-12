#include <stdio.h>


void bullshim_const(void) __attribute__((constructor));
void bullshim_dest(void) __attribute__((destructor));

void bullshim_const(){
	printf("Shim constructor called\n");
}

void bullshim_dest() {
	printf("Shim destructor called\n");
}


