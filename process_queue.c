#include <stdlib.h>
#include <stdio.h>
#include "process_queue.h"

/**
 * Creates a ready and blocked queue struct to store the ready processes and 
 * blocked processes
 *
 * @param size - the size of each queue
 * @return     - the created ready_blocked_queues struct
 */
ready_blocked_queues_t* create_ready_blocked_queues(int size,  process_t** ready_queue) {
	process_queue_t* ready = malloc(sizeof(process_queue_t));
	if (NULL == ready) {
		fprintf(stderr, "Error allocating ready queue in create_ready_blocked_queues() in process_queue.c\n");
		exit(1);
	}
	ready->process_array = ready_queue;
	process_t* temp_proc;
	int i = 0;
	while((temp_proc = ready_queue[i]) != NULL) {
		i++;
	}
	ready->curr_size = i;
	ready->max_size = size;
	ready->begin = 0;
	ready->end = i + 1; // FIXME: might be wrong
	process_queue_t* blocked = malloc(sizeof(process_queue_t));
	if (NULL == blocked) {
		fprintf(stderr, "Error allocating blocked queue in create_ready_blocked_queues() in process_queue.c\n");
		exit(1);
	}
	blocked->process_array = malloc(sizeof(process_t) * size);
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
 * Updates the ready and blocked queues. When the process is unblocked, it
 * is added to the end of the ready queue.
 * 
 * @param queue - the system of ready and blocked queues to grab from
 */
void update_queues(ready_blocked_queues_t* queue) {
	

}


/**
 * Moves a ready process to the blocked queue when it is blocked
 * Has to update the begin of ready and end of blocked
 * @param queue   - the ready and blocked queues to adjust
 * @param process - the process to move from ready to blocked
 */
void move_to_blocked(ready_blocked_queues_t* queue, process_t* proc) {
	queue->blocked_queue->process_array[queue->blocked_queue->end] = queue->ready_queue->process_array[queue->ready_queue->begin];
	queue->ready_queue->begin = (queue->ready_queue->begin + 1) % queue->ready_queue->max_size;
	queue->blocked_queue->end = (queue->blocked_queue->end + 1) % queue->blocked_queue->max_size;
}












