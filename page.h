#ifndef PAGE_H
#define PAGE_H

#include "memref.h"

typedef struct page {
	int page_no; // the page number
} page;


page* create_page(memref* ref);

page* create_page_int(int page_no); 
#endif
