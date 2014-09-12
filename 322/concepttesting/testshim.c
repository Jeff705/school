#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>

void testshim_const(void) __attribute__((constructor));
void testshim_dest(void) __attribute__((destructor));


void *(*original_malloc)(size_t);
void (*original_free)(void*);

void testshim_const(){
	printf("Shim constructor called\n");
	if (original_malloc == NULL) {
		original_malloc = dlsym(RTLD_NEXT, "malloc");
	}
	if (original_free == NULL) {
		original_free = dlsym(RTLD_NEXT, "free");
	}
}

void testshim_dest() {
	printf("Shim destructor called\n");
}


void *malloc(size_t size) {
	printf("malloc() called with a size of %d\n\n", (int)size);
	return((void *)original_malloc(size));
}

void free(void *pointer) {
	printf("free() called\n\n");
	original_free(pointer);
}

