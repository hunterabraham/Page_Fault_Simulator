#ifndef PTABLE_H
#define PTABLE_H
#include "page.h"


typedef struct ptable {
	page** page_array;
	int curr_size;
	int max_size;
} ptable;

ptable* create_page_table(int size);

void remove_page(ptable* table, int k);

void add_page(ptable* table, memref* ref); 




int search_page(ptable* table, int page_no);




#endif
