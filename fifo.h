#ifndef FIFO_H
#define FIFO_H
#include <stdlib.h>
#include <stdio.h>
#include "page_table.h"
typedef struct fifo_queue_t {
    page_t** pages;
    unsigned long int curr_size;
    unsigned long int begin;
    unsigned long int end;
    unsigned long int max_size;
} fifo_queue_t;

/**
 * Creates a fifo queue and initializes struct members
 * 
 * @param size - the size of the fifo queue
 * @return     - a pointer to the fifo queue
 */
fifo_queue_t* create_fifo_queue(int size);

/**
 * Pops the first element from the fifo queue
 * 
 * @param size - the size of the fifo queue
 * @return     - the index that is popped from the queue
 */
page_t* pop_from_fifo_queue(fifo_queue_t* queue);

/**
 * Pushes an element to the end of the queue
 * 
 * @param queue - the queue being pushed to
 * @param index - the index being pushed
 */
void push_to_fifo_queue(fifo_queue_t* queue, page_t*);


/**
 * Frees a queue and its memory
 * 
 * @param queue - the queue to be freed 
 */
void free_queue(fifo_queue_t* queue);

#endif