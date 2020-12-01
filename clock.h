#ifndef CLOCK_H
#define CLOCK_H

#include "page_table.h"


typedef struct queue_node_t {
    struct queue_node_t* prev;
    struct queue_node_t* next;
    unsigned long int ready;
    page_t* page;
} queue_node_t;

typedef struct queue_t {
    struct queue_node_t* front;
    struct queue_node_t* back;
    unsigned long int curr_size;
    unsigned long int max_size;
    struct queue_node_t* curr_ptr;
} queue_t;

/**
 * Creates an clock node with the specified page
 * 
 * @param page - the page to be held in the created node
 * @return     - the created node 
 */
queue_node_t* create_node(page_t* page);

/**
 * Creates a clock queue and initializes struct members
 * 
 * @param size - the size of the clock queue
 * @return     - a pointer to the clock queue
 */
queue_t* create_queue(int size);



/**
 * Frees a queue and its memory
 * 
 * @param queue - the queue to be freed 
 */
void free_queue(queue_t* queue);

/**
 * Removes all pages specified by this process from
 * the queue
 * 
 * @param queue - the queue to remove items from
 * @param proc  - the process whose pages will be removed
 */
unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid);


/**
 * Runs the clock replacement algorithm
 * 
 * 
 * @param queue - the clock queue
 * @param page  - the page being added to the clock queue
 */
page_t* replacement_algorithm(queue_t* queue, page_t* page);



/**
 * Updates a memory reference and sets its ready bit to 0
 * 
 * @param queue - the queue that the page is held in
 * @param page  - the page in the file that is begin referenced 
 */
void update_mem_reference(queue_t* queue, page_t* page);
































// typedef struct queue_t {
//     page_t** pages;
//     unsigned long int* ready_arr; // FIXME can make unsigned int if memory is tight
//     unsigned long int curr_size;
//     unsigned long int max_size;
//     unsigned long int curr_idx; // marker for current page in clock
// } queue_t;


// /**
//  * Creates a clock array and allocates its memory
//  * 
//  * @param size - the size of the clock array
//  * @return     - the created clock
//  */
// queue_t* create_queue(int size);


// /**
//  * Adds a page to the array. If the page already exists
//  * in the array, set its ready bit to 0. If page does not
//  * exist and the spot at curr_idx is open immediately, the 
//  * queue is not full, so add the page and return. If the 
//  * spot is not open, then iterate until a ready bit is 1. 
//  * Along the way, mark all ready bits to 1
//  * 
//  * @param queue - the clock array that is being added to
//  * @param page        - the page being added
//  */
// page_t* replacement_algorithm(queue_t* queue, page_t* page);


// /**
//  * Frees a clock array and all associated memory
//  * 
//  * @param queue - the clock array to be freed
//  */
// void free_queue(queue_t* queue);


// /**
//  * Removes all pages that belong to the corresponding process
//  * 
//  * @param queue - the queue to remove the pages from
//  * @param pid   - the pid of the target process
//  * @return      - the number of pages removed
//  */
// unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid);


#endif