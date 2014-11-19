

#define _GNU_SOURCE

#include <stdlib.h>
#include <sys/mman.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>


#define PAGESIZE 4096
#define MAX_OBJ_EXP 10
#define MIN_OBJ_EXP 3
#define MAX_OBJ_SIZE 1024
#define MIN_OBJ_SIZE 8
//MAX and MIN's must be powers of 2!

typedef struct obj_t obj_t;
struct obj_t {
	obj_t *next;
	int valid; //0 if deleted, 1 if valid
	void *contents;
};

typedef struct page_t page_t;

struct page_t {
	page_t *next;
	page_t *prev;
	int num_objects;
	int content_size;
	obj_t objspace;
};

static int init_allocator() __attribute__((constructor));
//FILE *zero;
page_t *page_list[MAX_OBJ_EXP + 1]; //so that page_list[MAX_OBJ_EXP] will
				    			//contain objects of MAX_OBJ_SIZE size

/*
 * regular pages are to be used when the size is implied by membership to
 * the global page list of 2^3 - 2^10. if an object is allocated that is
 * larger than 1024, it gets an odd_chunk and is appended to the linked
 * list at global_page_list[0].
 */
page_t *page_create(page_t *prev, size_t content_size);
page_t *big_page_create(page_t *prev, size_t size);
obj_t *obj_create(page_t *pageptr, int objsize);
int pageFull(int num_objects, int objsize);
void obj_invalidate(page_t *pageptr, void *ptr);
void page_unmap(page_t *pageptr);

static int init_allocator()
{
	//zero = fopen("/dev/zero","r");
	return 0;
}

/* malloc:
 * 	check argument size-
 *	if 0, return NULL
 *	
 *	else if <= 1024, put it in the appropriate page
 *		if page exists, check to see if there's space
			put it in the page
		else create page and point to it
	else create an odd chunk and stash the object there
 *
 *	return pointer to the OBJECT ALLOCATED
 *
 */

void *malloc(size_t size) {
	obj_t *new_obj = NULL;
	if(size > 0 && size <= MAX_OBJ_SIZE)
	{
//object belongs on a regular page
//determine which page obj belongs on
		int quantum = MIN_OBJ_SIZE;
		int exponent = MIN_OBJ_EXP;
		while(quantum < size)
		{
			quantum <<= 1; //increase exponent to mod by
			exponent++;
		}
		page_t *obj_page;
//at this point we have a power of 2 for the page this obj belongs on
//as well as the integer size in bytes of the obj size for this page (quantum)
		if(page_list[exponent] == NULL)
		{
			obj_page = page_create(NULL,quantum);
			page_list[exponent] = obj_page;
		}
		else
		{
//page exists, start obj_page at the base page in the page_list
			obj_page = page_list[exponent];
			while(obj_page->next && pageFull(obj_page->num_objects,quantum))
			{
			//skip the full pages
				obj_page = obj_page->next;
			}
			if(pageFull(obj_page->num_objects, quantum))
			{
			//last page is full, make new page and link to this one
				obj_page->next = page_create(obj_page,quantum);
				obj_page = obj_page->next;
			}
		}
	// obj_page is now defined as the earliest page with space for
	// an object of 'size'
		new_obj = obj_create(obj_page, quantum);
	}

//at this point, we mmap a new page for the object and stuff it in the
//linked list at page_list[0]
	else if(size > MAX_OBJ_SIZE)
	{
		page_t *current = page_list[0];
		if(current == NULL) {
			page_list[0] = big_page_create(NULL,size);
			new_obj = &page_list[0]->objspace;
		}
		else
		{
			while(current->next)
			{
				current = current->next;
			}
			current->next = big_page_create(current, size);
			current = current->next;
			new_obj = &current->objspace;
		}
	}
	if(new_obj == NULL) {
		return new_obj;
	}
	else
	{
		return &new_obj->contents;
	}
}


void free(void *pointer) {
	if(pointer != NULL)
	{
		page_t *page_ptr = (void*)((uintptr_t)pointer & ~((uintptr_t)0xFFF));
		obj_invalidate(page_ptr,pointer);

		if(page_ptr->num_objects == 0)
		{
			page_unmap(page_ptr);
		}
	}
}

void *calloc(size_t num, size_t size)
{
	void *dataptr;
	size_t totalSize = num * size;
	dataptr = malloc(totalSize);

	if(totalSize < MAX_OBJ_SIZE)
	{
		memset(dataptr, 0, totalSize);
	}

	return dataptr;
}

/* realloc:
 * 	we'll figure this out after we get the rest of the functions
 *	finished up.
 *
 */

/* void *realloc(void *ptr, size_t size)
{
	void *newptr = ptr;

	page_t *page_ptr = (void*)((uintptr_t)ptr & ~((uintptr_t)0xFFF));
	int currentsize = page_ptr->content_size;
	if(size > currentsize)
	{
		newptr = malloc(size);
		memcpy(newptr, ptr, currentsize); //doesn't matter, memory is allocated
		free(ptr);
	}

	return newptr;
}
*/
/* 
 * page_create
 * 
 * this function assumes you're creating a page at the end of a list. it will
 * return a page with NULL as <next>, prev pointed to arg0, and pointer
 * pointing to the beginning of the page.
 */

page_t *page_create(page_t *prev, size_t content_size) {
	page_t *pageptr = mmap(NULL,PAGESIZE, PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,-1,0);
	
	if(pageptr == (page_t *)-1)
	{
		perror("mmap");
	}

	pageptr->prev = prev;
	pageptr->content_size = content_size;
	pageptr->num_objects = 0;

	return pageptr;
}

page_t *big_page_create(page_t *prev, size_t size) {
	page_t *pageptr = mmap(NULL,(size + 32), PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS, -1,0);

	if(pageptr == (page_t *)-1)
	{
		perror("mmap");
	}

	pageptr->prev = prev;
	pageptr->num_objects = 0;
	pageptr->content_size = size;
	return pageptr;
}

/*
 * obj_create
 * 
 * assumption: this will NEVER be called on a full page, so it will always find
 * an appropriate spot within the legal page boundaries
 *
 */

obj_t *obj_create(page_t *pageptr, int objsize) {
	obj_t *new_obj;
	obj_t *current = &pageptr->objspace;
	obj_t *prev = NULL;
	while(current->valid)
	{
		prev = current;
		if(current->next) 
		{
			current = current->next;	
		}
		else current = (obj_t *)((char *)current + sizeof(obj_t) + objsize);
		
	}
	if(!current->valid)
	{
		new_obj = current;
	}
	else new_obj = (current + (objsize + sizeof(obj_t *) + sizeof(int)));
	if(prev != NULL)
	{
		prev->next = new_obj;
	}
	current->valid = 1;
	pageptr->num_objects++;
	
	return new_obj;
}

int pageFull(int num_objects, int objsize) {
	int projectedPageNeed = ((num_objects + 1) * objsize + 32);
	if(projectedPageNeed > PAGESIZE)
	{
		return 1;
	}
	else return 0; 
}

//sets object within page to invalid when the proper object is found

void obj_invalidate(page_t *pageptr, void *ptr)
{
	obj_t *current = (obj_t *)&pageptr->objspace;
	while(&current->contents != ptr)
	{
		current = current->next;
	}
	current->valid = 0;
	pageptr->num_objects--;
}

void page_unmap(page_t *pageptr)
{
	size_t bytesToUnmap = pageptr->content_size;

	if(pageptr->prev == NULL)
	{
	//page is head of a list
		
			int quantum = MIN_OBJ_SIZE;
			int exponent = MIN_OBJ_EXP;
			while(quantum < pageptr->content_size)
			{
				quantum <<= 1;
				exponent++;
			}
			if(quantum > MAX_OBJ_SIZE)
			{
				exponent = 0;
			}
			page_list[exponent] = pageptr->next;
			if(page_list[exponent] != NULL)
			{
				page_list[exponent]->prev = NULL;	
			}
			
	}
	else if(pageptr->next)
	{
	//page is not the last in a list
		pageptr->next->prev = pageptr->prev;
		pageptr->prev->next = pageptr->next;
	}
	else
	{
	//page is last in a list
		pageptr->prev->next = pageptr->next;
	}
	
	munmap(pageptr,bytesToUnmap);
}
