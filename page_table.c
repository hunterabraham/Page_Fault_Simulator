
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
    page_table_t* page_table = malloc(sizeof(page_table_t)); 
    if (NULL == page_table) {
        fprintf(stderr, "Error allocating page_table in create_page_table() in page_table.c\n");
        exit(1);
    }
    page_table->curr_size = 0;
    page_table->max_size = size;

    page_table->root = NULL;
    return page_table;
}

int compare_pages(const void* page1, const void* page2) {
    //fprintf(stderr, "%ld\n", ((const page_t*)page1)->vpn);
    //fprintf(stderr, "%ld\n\n\n", ((const page_t*)page2)->vpn);
    if (((const page_t*)page1)->vpn < ((const page_t*)page2)->vpn) {
        return -1;
    }
    else if (((const page_t*)page1)->vpn > ((const page_t*)page2)->vpn) {
        return 1;
    }
    return 0;
}

/**
 * Adds a page to the ptable using hash function
 * 
 * @param ptable - the table being added to
 * @param page   - the page being added
 */ 
unsigned long int add_to_ptable(page_table_t* ptable, page_t* page) {
    tsearch((void *)page, (void **)&ptable->root, compare_pages);
    ptable->curr_size++;
    return 0;
}




/**
 * Removes a page from the ptable and returns it
 * 
 * @param page   - the page being removed
 * @param ptable - the page table being remove from 
 * @return       - the page if found, otherwise NULL
 */
void remove_from_ptable(page_table_t* ptable, page_t* page) {
    //tdelete((void*)page, (void**)&ptable->root, compare_pages);
    tdelete(page, (void**)&ptable->root, compare_pages);
    ptable->curr_size--;
    free(page);

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
    //fprintf(stderr, "%ld\n", page->vpn);
    void* found_page;
    found_page = tfind((void*)page, (void**)&table->root, compare_pages);
    page_t* found_page_page = (page_t*) found_page;
    if (found_page_page != NULL) {
        //fprintf(stderr, "%ld\n", (found_page_page)->vpn);
        return 1;
    }
    return 0;
}

/**
 * Frees a node in the BST  
 * 
 * @param ptr - the pointer to the node to be freed
 */
void free_node(void *ptr) {
    page_t* page = ptr;
    free(page);
}

void free_ptable(page_table_t* table) {
    tdestroy(table->root, free_node);
}