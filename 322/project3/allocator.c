/* 
 * TODO: do we need structs, or just to track stuff mathematically
 * 	within functions? - probably just track via math to save
 * 	space and complexity
`
things that need to be tracked:

global --
	list of pages (array of pointers, indices of which
	correspond to the power of 2 for the pages they're
	pointing to). this would be 10 words long (10 pointers)
	and if 8 is the minimum obj size, would still have a couple
	pointers to play with (maybe 0 could be the linked-list to
	oddly sized allocations).

local (to each page) --

	a pointer to the next allocated page IN THE LIST TO WHICH 
	IT BELONGS, followed by the first object allocated

		this solution has the advantage of simple calculation
		for where an object will fit - if there's an object-
		sized hole between 'this' and 'next', or 'next' is 
		NULL and there's still room in the page, allocate!

		if all objects in the page are freed AND next page
		is NULL, we can give back the page to the OS

		need functionality to remove objects (map the 'next'
		pointer of the previous object to the 'next' ptr of
		the object being deleted).

*/

#define _GNU_SOURCE

#include <stdlib.h>
#include <sys/mman.h>

#define PAGESIZE 4096
#define MAX_OBJSIZE 1024
#define MIN_OBJSIZE 8

static int init_allocator() __attribute__((constructor));

typedef struct mapped_object {
	mapped_object *next;
	void *data;
}mapped_obj;
/*
 * regular pages are to be used when the size is implied by membership to
 * the global page list of 2^3 - 2^10. if an object is allocated that is
 * larger than 1024, it gets an odd_chunk and is appended to the linked
 * list at global_page_list[0].
 */


typedef struct regular_page regular_page {
	regular_page *next_page;
	regular_page *prev_page;
	void *first_object;
};

 struct odd_chunk odd_chunk {
	odd_chunk *next;
	odd_chunk *prev;
	void *obj_space;
};

/* Constructor functions:
 * 	Initialize object list
 * 	open /dev/zero and assign to a global ptr
 *	create an address mask and assign to global
 * 	That's it... I think.
 *
 */


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

/* free:
 * 	if argument == null - return
 *	else get page address by masking
 *	
 *	use iterator function to determine where in the page the
 *	object is, do a linked-list deletion (forwarding prev obj
 * 	to the next obj).
 *	
 *	if the object is the only object on the page, link the previous
 *	page to the next page and munmap the page
 */

/*
 * calloc:
 *	if > 1024, just malloc a big page -- it'll be initialized to
	zero anyway
 *
 *	else call malloc with the appropriate size, then memset it
 *	to zero
 */

/* realloc:
 * 	we'll figure this out after we get the rest of the functions
 *	finished up.
 *
 */

/*
 * HELPER FUNCTIONS:
 *
 * int obj_remove(void* start_addr,void* remove_me)
 * 	links prev obj to next obj on page. if this is the only obj on
 * 	the page (remove_me == start_addr; remove_me->next == NULL)
 *	return 1, else return 0.
 *
 * void page_remove(void *page_to_remove, void* <list_where_page_resides>)
 * 	same type of thing as obj_remove... but for a page
 *
 * void *get_free_space(void *page,int obj_size)
 * 	starts at a page ptr, then starts iterating over objects, checking
 *	to see whether a gap of 'obj_size + (sizeof(void *))' exists 
 *	between objects on the page. returns pointer to the first gap 
 *	it finds, or the next free space in the page, OR will return 
 *	NULL if the page is empty
 *
 */