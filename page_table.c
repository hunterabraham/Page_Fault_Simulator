
#include <stdlib.h>
#include <stdio.h>
#include "page_table.h"


/**
 * Allocate memory for all structs, initialize curr_size to 0 and max_size to size param
 * 
 * @param size - the size of the page table
 * @return     - a pointer to the new page table
 */ 
page_table_t* create_page_table(unsigned long int size) {
    page_table_t* page_table = malloc(sizeof(page_table_t)); // FIXME need to free !!
    if (NULL == page_table) {
        fprintf(stderr, "Error allocating page_table in create_page_table() in page_table.c\n");
        exit(1);
    }
    page_table->page_array = malloc(sizeof(page_t) * size);
    if (NULL == page_table->page_array) {
        fprintf(stderr, "Error allocating page_array in create_page_table() in page_table.c\n");
        exit(1);
    }
    page_table->curr_size = 0;
    page_table->max_size = size;
    page_table->free_list = malloc(sizeof(unsigned long int) * size);
    if (NULL == page_table->free_list) {
        fprintf(stderr, "Error allocating page_table->free_list in create_page_table() in page_table.c\n");
        exit(1);
    }
    for (unsigned long int i = 0; i < size; i++) {
        page_table->free_list[i] = (long unsigned int)1;
    }
    return page_table;
}


/**
 * Adds a page to the ptable using hash function
 * 
 * @param ptable - the table being added to
 * @param page   - the page being added
 */ 
unsigned long int add_to_ptable(page_table_t* ptable, page_t* page) {
    unsigned long int index = hash_ptable(ptable, page);
    while(ptable->free_list[index] == 0) {
        index++;
    }
    ptable->free_list[index] = 0;
    ptable->page_array[index] = page;
    page->page_table_idx = index;
    return index;
}


/**
 * Removes a page from the ptable and returns it
 * 
 * @param page   - the page being removed
 * @param ptable - the page table being remove from 
 * @return       - the page if found, otherwise NULL
 */
page_t* remove_from_ptable(page_table_t* ptable, page_t* page) {
    ptable->free_list[page->page_table_idx] = (unsigned long int)1;
    return page;
}

/**
 * Hash function to index pages into the page table hash map
 *  
 * @param page - the page that is being hashed
 * @return     - the index that results from the hash function
 */
unsigned long int hash_ptable(page_table_t* table, page_t* page) {
    unsigned long int index = (unsigned long int)(page->vpn % table->max_size);
    return index;
}

unsigned long int is_in_ptable(page_table_t* table, page_t* page) {
    if (page->page_table_idx == -1) {
        return 0;
    }
    unsigned long int index = page->page_table_idx;
    page_t* found_page = table->page_array[index];
    if (page->vpn == found_page->vpn && !table->free_list[index]) {
        return 1;
    }
    return 0;
}
