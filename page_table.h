#ifndef PTABLE_H
#define PTABLE_H
#include <search.h>

// page to be held in page table
typedef struct page_t {
	unsigned long int pid;
	unsigned long int vpn;
	long int page_table_idx;
	unsigned long int num_bytes;
} page_t;

// page table to hold all pages for a process
typedef struct page_table_t { 
	page_t* root;
	unsigned long int curr_size;
	unsigned long int max_size;
} page_table_t;


/**
 * Allocate memory for all structs, initialize curr_size to 0 and max_size to size param
 * 
 * @param size - the size of the page table
 * @return     - a pointer to the new page table
 */ 
page_table_t* create_page_table(unsigned long int size);

/**
 * Searches ptable for page with <pid, vpn>
 * 
 * @param table - the page table being searched
 * @param pid   - the pid of the page being searched for
 * @param vpn   - the vpn of the page being searched for
 * @return      - the page if found, otherwise NULL
 */
page_t* get_from_ptable(page_table_t* table, unsigned long int pid, unsigned long int vpn);

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
