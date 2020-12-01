#include <stdlib.h>
#include <stdio.h>
#include "fifo.h"

/**
 * Creates an LRU node with the specified page
 * 
 * @param page - the page to be held in the created node
 * @return     - the created node 
 */
queue_node_t* create_node(page_t* page) {
    queue_node_t* new_node = malloc(sizeof(queue_node_t));
    if(NULL == new_node) {
        fprintf(stderr, "Error allocating new_node in create_node in fifo.c\n");
        exit(1);
    }
    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->page = page;
    return new_node;
}

/**
 * Creates a lru queue and initializes struct members
 * 
 * @param size - the size of the lru queue
 * @return     - a pointer to the lru queue
 */
queue_t* create_queue(int size) {
    queue_t* new_queue = malloc(sizeof(queue_t));
    if (NULL == new_queue) {
        fprintf(stderr, "Error allocating new_queue in create_queue() in fifo.c\n");
        exit(1);
    }
    new_queue->front = NULL;
    new_queue->back = NULL;
    new_queue->curr_size = 0;
    new_queue->max_size = size;
    return new_queue;
}


/**
 * Frees a queue and its memory
 * 
 * @param queue - the queue to be freed 
 */
void free_queue(queue_t* queue) {
    free(queue);
}


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
page_t* replacement_algorithm(queue_t* queue, page_t* page) {
    if (queue->curr_size < queue->max_size) {
        queue_node_t* new_node = create_node(page);
        new_node->next = queue->front;
        if (queue->curr_size == 0) {
            queue->back = new_node;
            queue->front = new_node;
        } else {
            new_node->next = queue->front;
            queue->front->prev = new_node;
            queue->front = new_node;
        }
        queue->curr_size++;
        return NULL;
    }
    page_t* ret = queue->back->page;
    queue->back = queue->back->prev; // remove node from end of list
    queue->back->next = NULL;
    queue_node_t* new_node = create_node(page);
    new_node->next = queue->front;
    queue->front->prev = new_node;
    queue->front = new_node;
    return ret;
}


/**
 * Removes the first page found that matches the pid passed
 * 
 * @param pid   - the pid being searched for
 * @param queue - the queue being searched through
 * @return      - 1 if a page was found, 0 otherwise
 */
unsigned long int remove_page(queue_t* queue, unsigned long int pid) {
    queue_node_t* curr = queue->front;
    while(curr != NULL) {
        if (curr->page->pid == pid) {
            // unlink node
            if (curr == queue->front) {
                queue->front = curr->next;
                queue->curr_size--;
                return 1;
            }
            curr->prev->next = curr->next;
            // maintain prev pointers
            if (curr->next != NULL) {
                curr->next->prev = curr->prev;
            }
            // maintain back pointer
            if (curr == queue->back) {
                queue->back = curr->prev;
                queue->back->next = NULL;
            }
            queue->curr_size--;
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

/**
 * Removes all pages specified by this process from
 * the queue
 * 
 * @param queue - the queue to remove items from
 * @param proc  - the process whose pages will be removed
 */
unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid) {
    unsigned long int flag = 1;
    unsigned long int count = 0;
    while(flag) {
        count++;
        flag = remove_page(queue, pid);
    }
    return count;
}

