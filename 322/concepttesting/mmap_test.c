#include <sys/mman.h>
#include <stdio.h>
#include <stdlib.h>
#define PAGE_SIZE 4096

typedef struct my_page {
	struct my_page *next;
	struct my_page *prev;
	int num_objects;
	void *objspace;
}my_page;


int main() {


	
	my_page *page = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS,-1,0);

	if(page == (my_page*) -1) {
		perror("mmap");
	}


	printf("Address list:\n");
	printf("Where page lives:\t%p\n",&page);
	printf("Page base:\t%p\n",page);
	printf("&Page->next:\t%p\n",&(page->next));
	printf("Page->next:\t%p\n",(page->next));
	printf("Page->prev:\t%p\n",&(page->prev));
	printf("Page->num_objs:\t%p\n",&(page->num_objects));
	printf("Page->objspace:\t%p\n",&(page->objspace));
	
	my_page *page2 = mmap(NULL, PAGE_SIZE, PROT_READ|PROT_WRITE,
			MAP_PRIVATE|MAP_ANONYMOUS, -1,0);

	page->next = page2;
	page2->prev = page;
	page2->num_objects = 4;

	printf("Address list:\n");
	printf("Where page2 lives:\t%p\n",&page2);
	printf("Page2 base:\t%p\n",page2);
	printf("Page2->next:\t%p\n",&(page2->next));
	printf("Page2->prev:\t%p\n",&(page2->prev));
	printf("Page2->num_objs:\t%p\n",&(page2->num_objects));
	printf("Page2->objspace:\t%p\n",&(page2->objspace));
	
	printf("\nPage->next is now:\t%p\n",(page->next));

	printf("\nPage2->prev:\t%p\n",page2->prev);
	printf("Page2->num_objs:\t%d\n",page2->num_objects);
	return 0;

}

