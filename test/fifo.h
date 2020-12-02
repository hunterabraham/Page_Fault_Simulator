#ifndef FIFO_H
#define FIFO_H
#include <stdlib.h>
#include <stdio.h>
#include "page_table.h"
#include "process.h"


typedef struct queue_node_t {
    struct queue_node_t* prev;
    struct queue_node_t* next;
    page_t* page;
} queue_node_t;

typedef struct queue_t {
    struct queue_node_t* front;
    struct queue_node_t* back;
    unsigned long int curr_size;
    unsigned long int max_size;
} queue_t;

/**
 * Creates an LRU node with the specified page
 * 
 * @param page - the page to be held in the created node
 * @return     - the created node 
 */
queue_node_t* create_node(page_t* page);

/**
 * Creates a lru queue and initializes struct members
 * 
 * @param size - the size of the lru queue
 * @return     - a pointer to the lru queue
 */
queue_t* create_queue(int size);


/**
 * Frees a queue and its memory
 * 
 * @param queue - the queue to be freed 
 */
void free_queue(queue_t* queue);


/**
 * Performs the page replacement algorithm for FIFO. 
 * If queue is not full, add the page to the end of the list
 * If the queue is full, return the first element from the queue
 * while putting the page in that spot, then incrementing pointers
 * 
 * @param queue - the queue to perform on
 * @param page  - the page to add to the queue
 * @return      - the page that is popped from the queue. If no page was popped, NULL
 */
page_t* replacement_algorithm(queue_t* queue, page_t* page);

/**
 * Removes all pages specified by this process from
 * the queue
 * 
 * @param queue - the queue to remove items from
 * @param proc  - the process whose pages will be removed
 */
unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid);


#endif