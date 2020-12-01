#ifndef LRU_H
#define LRU_H

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
 * Pops the first element from the lru queue
 * 
 * @param size - the size of the lru queue
 * @return     - the index that is popped from the queue
 */
page_t* pop_from_queue(queue_t* queue);

/**
 * Pushes an element to the end of the queue
 * 
 * @param queue - the queue being pushed to
 * @param index - the index being pushed
 */
void push_to_queue(queue_t* queue, page_t*);


/**
 * Frees a queue and its memory
 * 
 * @param queue - the queue to be freed 
 */
void free_queue(queue_t* queue);

/**
 * Moves the specified page to the back of the queue
 * 
 * @param queue - the queue to manipulate
 * @param page  - the page to move to the back
 */
void update_mem_reference(queue_t* queue, page_t* page);

/**
 * Removes all pages specified by this process from
 * the queue
 * 
 * @param queue - the queue to remove items from
 * @param proc  - the process whose pages will be removed
 */
unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid);


/**
 * Runs the LRU replacement algorithm
 * 
 * 
 * @param queue - the LRU queue
 * @param page  - the page being added to the LRU queue
 */
page_t* replacement_algorithm(queue_t* queue, page_t* page);

#endif