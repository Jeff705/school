#include <dlfcn.h>
#include <queue.h>

struct memAllocation {
	char *variable;
	int mem_size;
};



void *malloc(size_t size) {
	void *tmp = original_malloc(size);
	memAllocation new = original_malloc(sizeof(memAllocation));
	new->variable = tmp;
	new.mem_size = size;

}