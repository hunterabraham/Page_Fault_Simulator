////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        page_table.c 
// Semester:         CS 537 Fall 2020
//
// Authors:          Hunter Abraham
// Emails:           hjabraham@wisc.edu
// CS Logins:        habraham
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//  https://github.com/increscent/c_examples/tree/main/tsearch
//  - the TAs example of using tsearch() in C -- it was posted on Piazza by a TA
//  tsearch(3) - Linux Manual Page
////////////////////////////////////////////////////////////////////////////////


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
    if (((page_t *)page1)->vpn < ((page_t *)page2)->vpn) {
        return -1;
    }
    else if (((page_t *)page1)->vpn > ((page_t *)page2)->vpn) {
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
    void *found;
    struct page_t *in_table;
    void** root = &(ptable->root);
    if ((found = tsearch(page, root, compare_pages)) == NULL) {
        // add to node failed
        fprintf(stderr, "Adding to table failed\n");
        exit(EXIT_FAILURE);
    } else {
        // Check if an node with the same key already existed
        in_table = *((struct page_t**)found);
        // if the page is already in the table, free the page
        if (in_table != page) {
            free(page);
        } else {
            // if the page didnt exist, increment size
            ptable->curr_size++;
        }
    }
    return ptable->curr_size;
}

page_t* find_page(void** root, page_t* page) {
    void *found;
    page_t *found_page;
    // search for page. If NULL, return NULL, otherwise return found page
    if ((found = tfind(page, root, compare_pages)) == NULL) {
        found_page = NULL;
    } else {
        found_page = *(struct page_t**)found;
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
void remove_from_ptable(page_table_t* ptable, page_t* page) {

    page_t *page_temp;
    void** root = &(ptable->root);
    // if found page is NULL, there's nothing to delete
    if ((page_temp = find_page(root, page)) == NULL) {
    } else {
        // otherwise, delete the page
        tdelete((void*)page_temp, root, compare_pages);
        free(page_temp);
    }
    ptable->curr_size--;
}


unsigned long int is_in_ptable(page_table_t* table, page_t* page) { 
    page_t* found_page = find_page(&table->root, page);
    // if the page isn't found return 0, otherwise 1
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
    // destroy tree
    tdestroy(table->root, free_node);
    free(table);
}