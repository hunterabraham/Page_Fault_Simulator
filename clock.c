////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        clock.c 
// Semester:         CS 537 Fall 2020
//
// Authors:          Hunter Abraham
// Emails:           hjabraham@wisc.edu
// CS Logins:        habraham
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
//                                  NONE
////////////////////////////////////////////////////////////////////////////////


#include <stdlib.h>
#include <stdio.h>
#include "page_table.h"
#include "page_replacement_interface.h"
#include "process.h"

queue_node_t* create_node(page_t* page) {
    queue_node_t* new_node = malloc(sizeof(queue_node_t)); // allocate node
    if (NULL == new_node) {
        fprintf(stderr, "Error allocating new_node in create_node in lru.c\n");
        exit(1);
    }
    new_node->prev = NULL; // initialize prev and next to NULL
    new_node->next = NULL;
    new_node->page = page; // set the data in the node
    new_node->ready = 1; // set ready bit to 1
    return new_node;
}


queue_t* create_queue(unsigned long int size) {
    // allocate queue & initialize members
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
    while(curr != NULL) { // do linear search for page that was referenced
        if (curr->page->vpn == page->vpn && curr->page->pid == page->pid) {
            curr->ready = 1; // reset its ready bit
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
            // Add node to end of list
            new_node->prev = queue->back;
            queue->back->next = new_node;
            queue->back = new_node;
            queue->curr_size++;
            return NULL;
        } 
    }
    page_t* ret = NULL;
    // search for ready page
    while(ret == NULL) {
        // if page found, get data to return & replace data in that node 
        if (!queue->curr_ptr->ready) {
            ret = queue->curr_ptr->page; 
            queue->curr_ptr->page = page; 
            queue->curr_ptr->ready = 1; 
        } else {
            // if page not found, set ready bit to 0
            queue->curr_ptr->ready = 0;
        }
        // advance curr_ptr
        if (queue->curr_ptr->next == NULL) {
            queue->curr_ptr = queue->front;
        } else {
            queue->curr_ptr = queue->curr_ptr->next;
        }
    }
    return ret;
}


/**
 * Unlinks a node from the queue with a matching pid.
 * 
 * @param queue - the queue to unlink from
 * @param page  - the page to unlink
 */
unsigned long int remove_page(queue_t* queue, unsigned long int pid) {
    queue_node_t* curr = queue->front;
    // search for node with corresponding pid
    while(curr != NULL) {
        if (curr->page->pid == pid) {
            // if front, make new front & deal with pointer if necessary
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
            // else
            // maintain next & prev pointers
            if (curr->prev != NULL) {
                curr->prev->next = curr->next;
            }
            if (curr->next != NULL) {
                curr->next->prev = curr->prev;
            }
            
            // maintain back pointer
            if (curr == queue->back) {
                queue->back = curr->prev;
                queue->back->next = NULL;
            }
            queue->curr_size--;
            // update curr pointer if necessary
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
        curr = curr->next;
    }
    // node not found
    return 0;
}

unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid) {
    unsigned long int flag = 1;
    unsigned long int count = 0;
    // until a node isn't found, remove page
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
    free(queue); 
}



