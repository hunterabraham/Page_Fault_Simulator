////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        page_table.h
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


#ifndef PTABLE_H
#define PTABLE_H

// page to be held in page table
typedef struct page_t {
	unsigned long int pid;
	unsigned long int vpn;
	unsigned long int num_bytes;
} page_t;

// page table to hold all pages for a process
typedef struct page_table_t { 
	void* root;
	unsigned long int curr_size;
} page_table_t;


/**
 * Allocate memory for all structs, initialize curr_size to 0 and max_size to size param
 * 
 * @param size - the size of the page table
 * @return     - a pointer to the new page table
 */
page_table_t* create_page_table();

/**
 * Compares two pages when they are being added to the binary tree
 * 
 * @param page1 - the first page being compared
 * @param page2 - the second page being compared
 * @return      - 1 if page1 vpn > page 2 vpn, -1 if <, 0 if ==
 */
int compare_pages(const void* page1, const void* page2);


/**
 * Adds a page to the ptable using hash function
 * 
 * @param ptable - the table being added to
 * @param page   - the page being added
 */ 
unsigned long int add_to_ptable(page_table_t* ptable, page_t* page);


/**
 * Removes a page from the ptable and returns it
 * 
 * @param page   - the page being removed
 * @param ptable - the page table being remove from 
 * @return       - the page if found, otherwise NULL
 */
void remove_from_ptable(page_table_t* ptable, page_t* page);


/**
 * Finds a page in the page table
 * 
 * @param root - the root of the page table
 * @param page - the page to search for
 * @return     - the reference of the page in the tree
 */
page_t* find_page(void** root, page_t* page);


/**
 * Determines if a page is in the current page table for this process
 * 
 * @param page  - the page that is being searched for
 * @param table - the table that is being searched through
 * @return      - 1 if found, 0 otherwise
 */
unsigned long int is_in_ptable(page_table_t* table, page_t* page);

/**
 * Frees all the memory allocated for this page table
 * 
 * @param table - the table to be freed 
 */
void free_ptable(page_table_t* table);

#endif
