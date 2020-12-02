////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        main.c 
// Semester:         CS 537 Fall 2020
//
// Authors:          Hunter Abraham
// Emails:           hjabraham@wisc.edu
// CS Logins:        habraham
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//  https://github.com/increscent/c_examples/tree/main/multiple_modules
//  - the TAs example of using interfaces in C
////////////////////////////////////////////////////////////////////////////////

#ifndef PR_INTERFACE_H
#define PR_INTERFACE_H

#include "page_table.h"


// holds a node in the linked list for each replacement algo
typedef struct queue_node_t {
    struct queue_node_t* prev;
    struct queue_node_t* next;
    unsigned long int ready;
    page_t* page;
} queue_node_t;

// queue doubly-linked list with front and back pointers to hold pages in replacement algorithm
typedef struct queue_t {
    struct queue_node_t* front;
    struct queue_node_t* back;
    unsigned long int curr_size;
    unsigned long int max_size;
    struct queue_node_t* curr_ptr;
} queue_t;


/**
 * Creates a queue and initializes struct members
 * 
 * @param size - the size of the clock queue
 * @return     - a pointer to the clock queue
 */
queue_t* create_queue(unsigned long int size);


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
 * Runs the replacement algorithm
 * 
 * 
 * @param queue - the clock queue
 * @param page  - the page being added to the clock queue
 */
page_t* replacement_algorithm(queue_t* queue, page_t* page);



/**
 * Updates a memory reference for LRU and Clock algorithms
 * 
 * @param queue - the queue that the page is held in
 * @param page  - the page in the file that is begin referenced 
 */
void update_mem_reference(queue_t* queue, page_t* page);

#endif