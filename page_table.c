#include <stdlib.h>
#include <stdio.h>


#include "page_table.h"
#include "page.h"
ptable* create_page_table(int size) {
	ptable* table = malloc(sizeof(ptable));
	if (NULL == table) {
		fprintf(stderr, "Error allocating page table in create_page_table() in page_table.c\n");
		exit(1);
	}

	table->page_array = malloc(sizeof(page) * size);
	if (table->page_array == NULL) {
		fprintf(stderr, "Error allocating page_array in create_page_table() in page_table.c\n");
		exit(1);
	}
	table->curr_size = 0;
	table->max_size = size;
	
	return table;
}


void remove_page(ptable* table, int k) {
	table->page_array[k] = NULL;
	while((table->page_array[k + 1]) != NULL) {
		table->page_array[k] = table->page_array[k+1];		
		k++;
	}

	table->page_array[k] = NULL;
	table->curr_size--;
//	int j = 0;
//	while(table->page_array[j] != NULL) { //FIXME
///		fprintf(stderr, "%d, ", table->page_array[j]->page_no);
//		j++;
//	}
//	fprintf(stderr, "\n");
	


}

void add_page(ptable* table, memref* ref) {
	if (search_page(table, ref->page_no) != -1) {
		return;
	}
	page* new_page = create_page(ref);
	table->page_array[table->curr_size] = new_page;
	table->curr_size++;
}



int search_page(ptable* table, int page_no) {
	page* p_temp;
	int k = 0;
	while((p_temp = table->page_array[k]) != NULL) {
		fprintf(stderr, "%d\n", p_temp->page_no); // FIXME
		if (p_temp->page_no == page_no) {
			return k;
		}
		k++;
	}
//	int j = 0;
//	while(table->page_array[j] != NULL) { //FIXME
//		fprintf(stderr, "%d, ", table->page_array[j]->page_no);
//		j++;
//	}
//	fprintf(stderr, "\n");
	return -1;
}
