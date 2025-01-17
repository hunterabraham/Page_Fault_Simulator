////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        process_queue.c
// Semester:         CS 537 Fall 2020
//
// Authors:          Hunter Abraham
// Emails:           hjabraham@wisc.edu
// CS Logins:        habraham
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
// 									NONE
////////////////////////////////////////////////////////////////////////////////



#include <stdlib.h>
#include <stdio.h>
#include "process_queue.h"
#include "process.h"
/**
 * Creates a ready and blocked queue struct to store the ready processes and 
 * blocked processes
 *
 * @param size - the size of each queue
 * @return     - the created ready_blocked_queues struct
 */
ready_blocked_queues_t* create_ready_blocked_queues(int size,  process_t** ready_queue) {
	// create and initialize ready queue
	process_queue_t* ready = malloc(sizeof(process_queue_t));
	if (NULL == ready) {
		fprintf(stderr, "Error allocating ready queue in create_ready_blocked_queues() in process_queue.c\n");
		exit(1);
	}
	// ready queue should be all processes found in find_all_processes
	ready->process_array = ready_queue;
	process_t* temp_proc;
	int i = 0;
	// count number of processes
	while((temp_proc = ready_queue[i]) != NULL) {
		i++;
	}
	ready->curr_size = i;
	ready->max_size = size;
	ready->begin = 0;
	ready->end = i; // FIXME: might be wrong
	// allocated blocked queue and initialize & allocate members
	process_queue_t* blocked = malloc(sizeof(process_queue_t));
	if (NULL == blocked) {
		fprintf(stderr, "Error allocating blocked queue in create_ready_blocked_queues() in process_queue.c\n");
		exit(1);
	}
	blocked->process_array = calloc(size, sizeof(process_t));
	if (NULL == ready->process_array) {
		fprintf(stderr, "Error allocating process_array in blocked queue in create_ready_blocked_queues() in process_queue.c\n");
		exit(1);
	}
	
	blocked->curr_size = 0;
	blocked->max_size = size;
	blocked->begin = 0;
	blocked->end = 0;

	
	ready_blocked_queues_t* rb_q = malloc(sizeof(ready_blocked_queues_t));
	if (NULL == rb_q) {
		fprintf(stderr, "Error allocating rb_q in create_ready_blocked_queues() in process_queue.c\n");
		exit(1);
	}
	
	rb_q->ready_queue = ready;
	rb_q->blocked_queue = blocked;
	rb_q->num_procs = i;
	return rb_q;
}



/**
 * When the current process gets blocked, this method finds the next
 * available process and returns it
 *
 * @param queue - the ready and blocked queues to update from
 * @return      - the next process to run
 */
process_t* get_next_process(ready_blocked_queues_t* queue) {
	process_t* proc = queue->ready_queue->process_array[queue->ready_queue->begin];
	return proc;
}


/**
 * Takes a process in the blocked queue and adds it to the end of the ready queue 
 * 
 * @param idx    - the index of the process in blocked
 * @param queues - the queues that are being manipulated
 */
void add_to_ready(ready_blocked_queues_t* queues) {
	// move first element in blocked to last element in ready
	queues->ready_queue->process_array[queues->ready_queue->end] = queues->blocked_queue->process_array[queues->blocked_queue->begin];
	// move begin and end pointers for blocked and ready, respectively, forward
	queues->blocked_queue->begin = (queues->blocked_queue->begin + 1) % queues->blocked_queue->max_size;
	queues->ready_queue->end = (queues->ready_queue->end + 1) % queues->ready_queue->max_size;
	// update size for blocked and ready
	queues->blocked_queue->curr_size--;
	queues->ready_queue->curr_size++;
}


unsigned long int remove_from_ready(ready_blocked_queues_t* queues) {
	if (queues->ready_queue->curr_size == 0) {
		return 0; 
	}
	// find number of pages that this process holds
	unsigned long int ret = queues->ready_queue->process_array[queues->ready_queue->begin]->num_pages;
	// free the process' memory
	free_ptable(queues->ready_queue->process_array[queues->ready_queue->begin]->page_table);
	free(queues->ready_queue->process_array[queues->ready_queue->begin]->blocks);
	free(queues->ready_queue->process_array[queues->ready_queue->begin]);
	// update ready queueu begin pointer and curr size
	queues->ready_queue->begin = (queues->ready_queue->begin + 1) % queues->ready_queue->max_size;
	queues->ready_queue->curr_size -= 1;
	return ret;
}

/**
 * Moves a ready process to the blocked queue when it is blocked
 * Has to update the begin of ready and end of blocked
 * @param queue   - the ready and blocked queues to adjust
 * @param process - the process to move from ready to blocked
 */
void move_to_blocked(ready_blocked_queues_t* queue) {
	if (queue->ready_queue->curr_size == 0) {
		return;
	}
	// move first element of ready to last element in blocked
	queue->blocked_queue->process_array[queue->blocked_queue->end] = queue->ready_queue->process_array[queue->ready_queue->begin];
	// move ready begin index forward
	queue->ready_queue->begin = (queue->ready_queue->begin + 1) % queue->ready_queue->max_size;
	// update size for ready and blocked & update blocked end pointer
	queue->ready_queue->curr_size--;
	queue->blocked_queue->curr_size++;
	queue->blocked_queue->end = (queue->blocked_queue->end + 1) % queue->blocked_queue->max_size;
	
}


process_t* peek_ready(ready_blocked_queues_t* queues) {
	if (queues->ready_queue->curr_size == 0) {
		return NULL;
	}
	// return first element of ready queues
	return queues->ready_queue->process_array[queues->ready_queue->begin];
}

process_t* search_for_process(ready_blocked_queues_t* queues, unsigned long int pid) {
	// linear search through ready and blocked queues to find the process with the matching pid
	process_queue_t* ready = queues->ready_queue;
	unsigned long int i = ready->begin;
	while(ready->process_array[i] != NULL) { 
		if (ready->process_array[i]->pid == pid) {
			return ready->process_array[i];
		}
		i++;
	}
	process_queue_t* blocked = queues->blocked_queue;
	i = blocked->begin;
	while(blocked->process_array[i] != NULL) {
		if (blocked->process_array[i]->pid == pid) {
			return blocked->process_array[i];
		}
		i++;
	}
	return NULL;
}


void free_processes(ready_blocked_queues_t* queues) {
	// free process queues memory
	free(queues->ready_queue->process_array);
	free(queues->blocked_queue->process_array);
	free(queues->ready_queue);
	free(queues->blocked_queue);
	free(queues);
	
}