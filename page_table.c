

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include "page_table.h"

/**
 * Allocate memory for all structs, initialize curr_size to 0 and max_size to size param
 * 
 * @param size - the size of the page table
 * @return     - a pointer to the new page table
 */ 
page_table_t* create_page_table() {
    page_table_t* page_table = malloc(sizeof(page_table_t)); 
    if (NULL == page_table) {
        fprintf(stderr, "Error allocating page_table in create_page_table() in page_table.c\n");
        exit(1);
    }
    page_table->curr_size = 0;
    //page_table->max_size = size;

    page_table->root = NULL;
    return page_table;
}

int compare_pages(const void* page1, const void* page2) {
    page_t* page1_ptype = (page_t *)page1;
    page_t* page2_ptype = (page_t *)page2;
    if (page1_ptype->vpn < page2_ptype->vpn) {
        return -1;
    }
    else if (page1_ptype->vpn > page2_ptype->vpn) {
        return 1;
    }
    else {
        return 0;
    }
    
}

/**
 * Adds a page to the ptable using hash function
 * 
 * @param ptable - the table being added to
 * @param page   - the page being added
 */ 
unsigned long int add_to_ptable(page_table_t* ptable, page_t* page) {
    void *result;
    struct page_t *existing;
    void** root = &(ptable->root);
    if ((result = tsearch(page, root, compare_pages)) == NULL) {
        // Failed to add the node
        // exit_with_message("Insufficient memory");
        fprintf(stderr, "Adding to table failed\n");
        exit(EXIT_FAILURE);
    } else {
        // Check if an node with the same key already existed
        existing = *((struct page_t**)result);

        if (existing != page) {
            //printf("Node with key already exists. ");
            //printf("key: %ld, pid: %ld\n", page->vpn, page->pid);
            free(page);
        } else {
            ptable->curr_size++;

            //printf("Added node. %ld\n", page->vpn);
        }
    }

    return ptable->curr_size;
}

page_t* find_page(void** root, page_t* page) {
    void *result;
    page_t *found_page;
    


    if ((result = tfind(page, root, compare_pages)) == NULL) {
        // No node found
        //printf("No node found. key: %ld\n", page->vpn);
        found_page = NULL;
    } else {
        // Node found
        found_page = *(struct page_t**)result;
        //printf("Found node. key: %ld\n", page->vpn);
    }

    return found_page;
}



/**
 * Removes a page from the ptable and returns it
 * 
 * @param page   - the page being removed
 * @param ptable - the page table being remove from 
 * @return       - the page if found, otherwise NULL
 */
void remove_from_ptable(page_table_t* ptable, page_t* page) {//unsigned long int vpn) {//
    page_t *page_temp;
    void** root = &(ptable->root);
    if ((page_temp = find_page(root, page)) == NULL) {
    } else {
        tdelete((void*)page_temp, root, compare_pages); //FIXME
        //printf("Deleted node. key: %d, value: %d\n", node->key, node->value);
        // It's important to free the only after deleting it
        free(page_temp);
    }
    ptable->curr_size--;
}


unsigned long int is_in_ptable(page_table_t* table, page_t* page) { // FIXME
    page_t* found_page = find_page(&table->root, page);
    if (found_page == NULL) {
        return 0;
    }
    return 1;
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
    free(table);
}