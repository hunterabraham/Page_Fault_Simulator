#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>
#include "page_table.h"
#include "page_replacement_interface.h"
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
    new_node->ready = 1;
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
    queue_node_t* curr = queue->front;
    while(curr != NULL) {
        if (curr->page->vpn == page->vpn && curr->page->pid == page->pid) {
            curr->ready = 1;
            return;
        }
        curr = curr->next;
    }
}

page_t* replacement_algorithm(queue_t* queue, page_t* page) {
    queue_node_t* new_node = create_node(page);
    // QUEUE IS NOT FULL
    if (queue->curr_size < queue->max_size) {
        if (queue->front == NULL) { // if queue is empty
            queue->front = new_node; // set new node to front and back
            queue->back = new_node;
            queue->curr_size++; // increment size
            queue->curr_ptr = new_node; // put curr_ptr on new_node
            return NULL;
        } else {
            queue->back->next = new_node;
            new_node->prev = queue->back;
            queue->back = new_node;
            queue->curr_size++;
            return NULL;
        }
    }
    // while victim page not found
    unsigned long int flag = 0;
    page_t* ret = NULL;
    while(ret == NULL) {
        if (!queue->curr_ptr->ready) {
            ret = queue->curr_ptr->page; // get page that will be returned
            queue->curr_ptr->page = page; // replace data in this node
            queue->curr_ptr->ready = 1; // set ready bit to 0
        } else {
            queue->curr_ptr->ready = 0;
        }
        if (queue->curr_ptr->next == NULL) {
            queue->curr_ptr = queue->front;
        } else {
            queue->curr_ptr = queue->curr_ptr->next;
        }
        flag = 1;
    }
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
    unsigned long int count = 0; // FIXME can remove
    while(curr != NULL) {
        count++;
        if (curr->page->pid == pid) {
            // unlink node
            if (curr == queue->front) {
                queue->front = curr->next;
                queue->curr_size--;
                if (curr == queue->curr_ptr) { 
                    if (curr->next == NULL) {
                        queue->curr_ptr = queue->front;
                    } else {
                        queue->curr_ptr = curr->next;
                    }
                }
                free(curr);
                return 1;
            }
            // maintain next & prev pointers
            if (curr->next != NULL) {
                curr->next->prev = curr->prev;
            }
            if (curr->prev != NULL) {
                curr->prev->next = curr->next;
            }
            // maintain back pointer
            if (curr == queue->back) {
                queue->back = curr->prev;
                queue->back->next = NULL;
            }
            queue->curr_size--;
            if (curr == queue->curr_ptr) { // FIXME move back
                if (curr->next == NULL) {
                    queue->curr_ptr = queue->front;
                } else {
                    queue->curr_ptr = curr->next;
                }
            }
            free(curr);
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid) {
    unsigned long int flag = 1;
    unsigned long int count = 0;
    while(flag) {
        flag = remove_page(queue, pid);
        count++;
    }
    return count - 1;
}

/**
 * Frees a queue and its memory
 * 
 * @param queue - the queue to be freed 
 */
void free_queue(queue_t* queue) {
    free(queue); // FIXME: front, back pointer?
}



