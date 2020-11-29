#include "fifo.h"

/**
 * Creates a fifo queue and initializes struct members
 * 
 * @param size - the size of the fifo queue
 * @return     - a pointer to the fifo queue
 */
fifo_queue_t* create_fifo_queue(int size) {
    fifo_queue_t* fifo_queue = malloc(sizeof(fifo_queue_t));
    if (NULL == fifo_queue) {
        fprintf(stderr, "Error allocating fifo_queue in create_fifo_queue() in fifo.c\n");
        exit(1);
    }
    fifo_queue->pages = malloc(sizeof(page_t) * size);
    if (NULL == fifo_queue->pages) {
        fprintf(stderr, "Error allocating fifo_queue->pages in create_fifo_queue() in fifo.c\n");
        exit(1);
    }
    fifo_queue->curr_size = 0;
    fifo_queue->begin = 0;
    fifo_queue->end = 0;
    fifo_queue->max_size = size;
    return fifo_queue;
}

/**
 * Pops the first element from the fifo queue
 * 
 * @param size - the size of the fifo queue
 * @return     - the index that is popped from the queue
 */
page_t* pop_from_fifo_queue(fifo_queue_t* queue) {
    page_t* ret = queue->pages[queue->begin];
    queue->begin = (queue->begin + 1) % queue->max_size;
    queue->curr_size--;
    return ret;
}

/**
 * Pushes an element to the end of the queue
 * 
 * @param queue - the queue being pushed to
 * @param index - the index being pushed
 */
void push_to_fifo_queue(fifo_queue_t* queue, page_t* page) {
    queue->pages[queue->end] = page;
    queue->end = (queue->end + 1) % queue->max_size;
    queue->curr_size++;
}



void free_queue(fifo_queue_t* queue)  {
    free(queue->pages);
    free(queue);
}
