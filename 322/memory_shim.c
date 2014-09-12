#define _GNU_SOURCE

#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

static int initialize_memory_shim() __attribute__((constructor));
static int dump_leak_list() __attribute__((destructor));

void *(*original_malloc)(size_t);
void (*original_free)(void*);

typedef struct mallocRecord {
	struct mallocRecord *next;
	void *address;
	int mem_size;
}mallocRecord;

mallocRecord *list_head;
int total_allocated;
int total_leaks = 0;

static int initialize_memory_shim() {
	if (original_malloc == NULL) {
		original_malloc = dlsym(RTLD_NEXT, "malloc");
	}
	if (original_free == NULL) {
		original_free = dlsym(RTLD_NEXT, "free");
	}
	mallocRecord *dummynode = original_malloc(sizeof(mallocRecord));
	list_head = (struct mallocRecord*)dummynode;
	list_head->next = NULL;
	list_head->mem_size = -1;

	return 0;
}

static int dump_leak_list() {	
	struct mallocRecord *current = list_head->next;
	while(current) {
		printf("LEAK:\t\t%d\n", current->mem_size);
		total_leaks++;
		current = current->next;
	}
	printf("TOTAL LEAKS:\t%d\n", total_leaks);
	printf("TOTAL BYTES:\t%d\n", total_allocated);
	return 0;
}

void *malloc(size_t size) {
	struct mallocRecord *current;
	void *newPointer = original_malloc(size);
	struct mallocRecord *item = original_malloc(sizeof(mallocRecord));
	item->address = newPointer;
	item->next = NULL;
	item->mem_size = size;
	
	if(list_head->next == NULL) {
		list_head->next = item;
	}
	
	else {
		current = list_head->next;
		while(current->next != NULL) {
			current = current->next;
		}
		current->next = item;
	}
	total_allocated += size;
	return(newPointer);
}

void free(void *pointer) {
	struct mallocRecord *current = list_head->next;
	struct mallocRecord *prev = list_head;
	while(current) {
		if(current->address == pointer) {
			prev->next = current->next;
			original_free(pointer);
			total_allocated = total_allocated - current->mem_size;
			return; //connect the previous and next entries
		}
		prev = current;
		current = current->next;

	}
	original_free(pointer);
}



