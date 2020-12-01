#ifndef PTABLE_INV_H
#define PTABLE_INV_H
#include "page.h"

// page to be held in page table
typedef struct inverted_page_t {
	unsigned long int ppn;
} page_t;

// page table to hold all pages for a process
typedef struct inverted_ptable_t { 
	inverted_page_t** page_array;
	unsigned long int curr_size;
	unsigned long int max_size;
} ptable_t;


/**
 * Allocate memory for all structs, initialize curr_size to 0 and max_size to size param
 * 
 * @param size - the size of the page table
 * @return     - a pointer to the new page table
 */ 
inverted_ptable_t* create_inverted_page_table(unsigned long int size);

/**
 * Searches ptable for page with <pid, vpn>
 * 
 * @param table - the page table being searched
 * @param pid   - the pid of the page being searched for
 * @param vpn   - the vpn of the page being searched for
 * @return      - the page if found, otherwise NULL
 */
inverted_page_t* get_from_ptable(inverted_ptable_t* table, unsigned long int pid, unsigned long int vpn);

/**
 * Adds a page to the ptable using hash function
 * 
 * @param ptable - the table being added to
 * @param page   - the page being added
 */ 
void add_to_ptable(inverted_ptable_t* ptable, page_t* page);


/**
 * Removes a page from the ptable and returns it
 * 
 * @param page   - the page being removed
 * @param ptable - the page table being remove from 
 * @return       - the page if found, otherwise NULL
 */
page_t* remove_from_ptable(inverted_ptable_t* ptable, page_t* page);

/**
 * Hash function to index pages into the page table hash map
 *  
 * @param page - the page that is being hashed
 * @return     - the index that results from the hash function
 */
unsigned long int hash_inverted_ptable(inverted_page_table_t* table, page_t* page);


#endif
