#include <stdlib.h>
#include <stdio.h>
#include "page.h"

page* create_page(memref* ref) {
	page* new_page = malloc(sizeof(page));
	if (NULL == new_page) {
		fprintf(stderr, "Error allocating new_page in create_page() in page.c\n");
		exit(1);
	}
	new_page->page_no = ref->page_no + ref->pid;
	return new_page;
}


page* create_page_int(int page_no) {
	page* new_page = malloc(sizeof(page));
	if (NULL == new_page) {
		fprintf(stderr, "Error allocating new_page in create_page_int() in page.c\n");
		exit(1);
	}
	new_page->page_no = page_no;
	return new_page;

}
