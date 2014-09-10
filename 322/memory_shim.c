#include <dlfcn.h>
#include <queue.h>

struct memAllocation {
	char *variable;
	int mem_size;
	LIST_ENTRY(memAllocation, ptrs);
};

LIST_HEAD(allocatedPtrs, memAllocation);

struct allocatedPtrs *allocList = NULL;

static int initialize_memory_shim() __attribute__((constructor));

static int initialize_memory_shim() {
	

void *malloc(size_t size) {
	void *tmp = original_malloc(size);
	memAllocation new = original_malloc(sizeof(memAllocation));
	new->variable = tmp;
	new.mem_size = size;

}
