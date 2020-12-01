#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>
#include "page_table.h"
#include "clock.h"
#include "process.h"

queue_node_t* create_node(page_t* page) {
    queue_node_t* new_node = malloc(sizeof(queue_node_t));
    if (NULL == new_node) {
        fprintf(stderr, "Error allocating new_node in create_node in lru.c\n");
        exit(1);
    }
    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->page = page;
    new_node->ready = 0;
    return new_node;
}


queue_t* create_queue(int size) {
    queue_t* new_queue = malloc(sizeof(queue_t));
    if (NULL == new_queue) {
        fprintf(stderr, "Error allocating new_queue in create_queue() in lru.c\n");
        exit(1);
    }
    new_queue->front = NULL;
    new_queue->back = NULL;
    new_queue->curr_ptr = NULL;
    new_queue->curr_size = 0;
    new_queue->max_size = size;
    return new_queue;
}

void update_mem_reference(queue_t* queue, page_t* page) {
    if (queue->curr_size == 0) {
        return;
    }
    queue_node_t* curr = queue->front;
    while(curr != NULL) {
        if (curr->page->vpn == page->vpn && curr->page->pid == page->pid) {
            curr->ready = 0;
            return;
        }
        curr = curr->next;
    }
    //fprintf(stderr, "Not found %ld\n", page->pid);
}


page_t* replacement_algorithm(queue_t* queue, page_t* page) {
    queue_node_t* new_node = create_node(page);
    // QUEUE IS NOT FULL
    if (queue->curr_size == 0) { // curr size is 0, initialize list
        queue->front = new_node;
        queue->back = new_node;
        queue->curr_size++;
        queue->curr_ptr = new_node;
        return NULL;
    }
    if (queue->curr_size < queue->max_size) { // queue isn't full, add to end
        queue->curr_ptr->next = new_node;
        new_node->prev = queue->curr_ptr;
        queue->curr_size++;
        queue->curr_ptr = queue->curr_ptr->next;
        queue->back = new_node;
        return NULL;
    }
    // QUEUE IS FULL
    unsigned long int flag = 0;
    while(!queue->curr_ptr->ready) {  // search for a node that is ready to be replaced
        queue->curr_ptr->ready = 1; // along the way, set ready bits to 1
        if (queue->curr_ptr->next == NULL) { // if at the end of the list, set next to front (circular)
            queue->curr_ptr = queue->front;
        } else {
            queue->curr_ptr = queue->curr_ptr->next; // otherwise advance one
        }
    }
    // When node is found
    page_t* ret;
    ret = queue->curr_ptr->page; // get page that will be returned
    queue->curr_ptr->page = page; // replace data in this node
    queue->curr_ptr->ready = 0; // set ready bit to 0
    return ret;
}


/**
 * Unlinks a node from the queue.
 * 
 * @param queue - the queue to unlink from
 * @param page  - the page to unlink
 */
unsigned long int remove_page(queue_t* queue, unsigned long int pid) {
    queue_node_t* curr = queue->front;
    unsigned long int count = 0;
    while(curr != NULL) {
        count++;
        if (curr->page->pid == pid) {
            // unlink node
            if (curr == queue->front) {
                queue->front = curr->next;
                queue->curr_size--;
                free(curr);
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
            free(curr);
            return 1;
        }
        curr = curr->next;
    }
    fprintf(stderr, "%ld, %ld\n", count, queue->curr_size);
    return 0;
}

unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid) {
    unsigned long int flag = 1;
    unsigned long int count = 0;
    while(flag) {
        flag = remove_page(queue, pid);
        count++;
    }
    return count;
}

/**
 * Frees a queue and its memory
 * 
 * @param queue - the queue to be freed 
 */
void free_queue(queue_t* queue) {
    free(queue); // FIXME: front, back pointer?
}



































// #include <stdlib.h>
// #include <stdio.h>

// #include "clock.h"

// queue_t* create_queue(int size) {
//     queue_t* queue = malloc(sizeof(queue_t));
//     if (NULL == queue) {
//         fprintf(stderr, "Error allocating queue in create_queue() in clock.c\n");
//         exit(1);
//     }
//     queue->pages = malloc(sizeof(page_t) * size);
//     if (NULL == queue->pages) {
//         fprintf(stderr, "Error allocating queue->pages in create_queue() in clock.c\n");
//         exit(1);
//     }
//     queue->ready_arr = calloc(size, sizeof(int));
//     if (NULL == queue->ready_arr) {
//         fprintf(stderr, "Error allocating queue->ready_arr in create_queue() in clock.c\n");
//         exit(1);
//     }
//     queue->curr_size = 0;
//     queue->max_size = size;
//     queue->curr_idx = 0;
//     return queue;
// }

// void free_queue(queue_t* queue) {
//     free(queue->ready_arr);
//     free(queue->pages);
//     free(queue);
// }

// page_t* replacement_algorithm(queue_t* queue, page_t* page) {
//     //fprintf(stderr, "%ld, %ld\n", queue->curr_size, queue->max_size);
//     while(queue->pages[queue->curr_idx] != NULL && !queue->ready_arr[queue->curr_idx]) {
//         queue->ready_arr[queue->curr_idx] = 1;
//         queue->curr_idx = (queue->curr_idx + 1) % queue->max_size;
//     }
//     queue->curr_idx = (queue->curr_idx + 1) % queue->max_size;
//     page_t* ret = queue->pages[queue->curr_idx];
//     if (ret == NULL) {
//         queue->curr_size++;
//     }

//     queue->pages[queue->curr_idx] = page;
//     return ret;
// }



// unsigned long int remove_page(queue_t* queue, unsigned long int pid) {
//     unsigned long int idx = queue->curr_idx;
//     while(queue->pages[idx] != NULL && !queue->ready_arr[idx]) {
//         if (queue->pages[idx]->pid == pid) {
//             while(queue->pages[idx] != NULL) {
//                 queue->pages[idx] = queue->pages[(idx + 1) % queue->max_size];
//                 idx = (idx + 1) % queue->max_size;
//                 fprintf(stderr, "%ld\n", idx);
//             }
//             return 1;
//         }
//         idx = (idx + 1) % queue->max_size;
//     }
//     return 0;

// }

// unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid) {
//     fprintf(stderr, "here %ld\n", pid);
//     unsigned long int count = 0;
//     unsigned long int flag = 1;
//     while(flag) {
//         flag = remove_page(queue, pid);
//         count++;
//     }
//     return count;
// }














