////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        lru.c
// Semester:         CS 537 Fall 2020
//
// Authors:          Hunter Abraham
// Emails:           hjabraham@wisc.edu
// CS Logins:        habraham
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
// fseek(3) - Linux Manual
////////////////////////////////////////////////////////////////////////////////

#include <stdlib.h>
#include <stdio.h>
#include <search.h>
#include <string.h>
#include "page_replacement_interface.h"
#include "process.h"



queue_node_t* create_node(page_t* page) {
    // create node and initialize struct members, all NULL except for page
    queue_node_t* new_node = malloc(sizeof(queue_node_t));
    if (NULL == new_node) {
        fprintf(stderr, "Error allocating new_node in create_node in lru.c\n");
        exit(1);
    }
    new_node->prev = NULL;
    new_node->next = NULL;
    new_node->page = page;
    return new_node;
}


queue_t* create_queue(unsigned long int size) {
    // allocate queue & initialize struct members
    queue_t* new_queue = malloc(sizeof(queue_t));
    if (NULL == new_queue) {
        fprintf(stderr, "Error allocating new_queue in create_queue() in lru.c\n");
        exit(1);
    }
    new_queue->front = NULL;
    new_queue->back = NULL;
    new_queue->curr_size = 0;
    new_queue->max_size = size;
    return new_queue;
}


page_t* replacement_algorithm(queue_t* queue, page_t* page) {
    // if queue is not full
    if (queue->curr_size < queue->max_size) {
        // add page to front of queue
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
    // if queue is full
    page_t* ret = queue->back->page;
    // remove node from end of list
    queue->back = queue->back->prev; 
    free(queue->back->next);
    queue->back->next = NULL;
    // add node to front of list
    queue_node_t* new_node = create_node(page);
    new_node->next = queue->front;
    queue->front->prev = new_node;
    queue->front = new_node;
    return ret;
}

void update_mem_reference(queue_t* queue, page_t* page) {
    if (queue->curr_size == 0) {
        return;
    }
    unsigned long int i = 0;
    queue_node_t* curr = queue->front;
    // search for page in queue
    while(curr != NULL) {
        if (curr->page->vpn == page->vpn &&  curr->page->pid == page->pid) {
            // if found & not front
            if (curr != queue->front) {
                // remove node from queue
                curr->prev->next = curr->next;
                if (curr->next != NULL) {
                    curr->next->prev = curr->prev;
                }
                // maintain back pointer
                if (curr == queue->back) {
                    queue->back = curr->prev;
                    queue->back->next = NULL;
                }
                // put node in front of list
                curr->next = queue->front;
                curr->prev = NULL;
                // reset prev pointer of old front to new front
                curr->next->prev = curr;
                // change to new front
                queue->front = curr;
            }
            return;
        }
        i++;
        curr = curr->next;
    }
    fprintf(stderr, "Not found %ld\n", page->pid);
}

/**
 * Unlinks a node from the queue.
 * 
 * @param queue - the queue to unlink from
 * @param page  - the page to unlink
 */
unsigned long int remove_page(queue_t* queue, unsigned long int pid) {
    queue_node_t* curr = queue->front;
    // search for page with matching pid
    while(curr != NULL) {
        if (curr->page->pid == pid) {
            // remove node from queue
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
    return 0;
}

unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid) {
    unsigned long int flag = 1;
    unsigned long int count = 0;
    // while there is a node with the specified pid
    while(flag) {
        // remove it
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
    free(queue); 
}



