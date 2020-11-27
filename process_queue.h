#ifndef PROCESS_Q_H
#define PROCESS_Q_H

#include "process.h"


/**
 * Holds processes in the order in which they should be run based on their last memory reference.
 * When a process is popped from ready, its memory references are executed until it's blocked. 
 * Then, it's added to the blocked queue until it's ready, at which point it's added to the end of 
 * the ready queue.
 */
typedef struct process_queue_t {
	process_t** process_array;
	unsigned long int curr_size;
	unsigned long int max_size;
	unsigned long int begin;
	unsigned long int end;
} process_queue_t;

/**
 * Holds a ready queue and a blocked queue for processes
 */
typedef struct ready_blocked_queues_t {
	process_queue_t* ready_queue;
	process_queue_t* blocked_queue;
	process_queue_t* finished_queue;
	unsigned long int num_procs;
} ready_blocked_queues_t;

/**
 * Creates a ready and blocked queue struct to store the ready processes and 
 * blocked processes
 *
 * @param size - the size of each queue
 * @return     - the created ready_blocked_queues struct
 */
ready_blocked_queues_t* create_ready_blocked_queues(int size, process_t** ready_queue);

/**
 * Creates a process queue
 * 
 * @param size - the size of the process queue
 * @return     - the process queue
 */
process_queue_t* create_process_queue(int size);

/**
 * When the current process gets blocked, this method finds the next
 * available process and returns it
 *
 * @param queue - the ready and blocked queues to update from
 * @return      - the next process to run
 */
process_t* get_next_process(ready_blocked_queues_t* queue);



/**
 * Updates the ready and blocked queues. When the process is unblocked, it
 * is added to the end of the ready queue.
 * 
 * @param queue - the system of ready and blocked queues to grab from
 */
void update_queues(ready_blocked_queues_t* queue, unsigned long int clock);


/**
 * Moves a ready process to the blocked queue when it is blocked
 * Also updates its time until ready
 * @param queue   - the ready and blocked queues to adjust
 * @param process - the process to move from ready to blocked
 */
void move_to_blocked(ready_blocked_queues_t* queue);


/**
 * Peeks at the next ready process
 * 
 * @param queues - the ready and blocked queues system
 * @return       - the next ready process
 */
process_t* peek_ready(ready_blocked_queues_t* queues);


/**
 * Happens when process is done executing. Frees all the process' memory and removes it 
 * from the queue
 * 
 * @param queues - the queues being removed from
 * @return       - the number of pages that this process held in memory
 */
unsigned long int remove_from_ready(ready_blocked_queues_t* queues);


/**
 * Linear search method to find a process by pid
 * 
 * @param queues - the queues being searched through
 * @param pid    - the pid of the target process
 * @return       - the process that is found
 */
process_t* search_for_process(ready_blocked_queues_t* queues, unsigned long int pid);

/**
 * Moves a finished process that's done executing from ready to finished
 * 
 * @param queues - the queues being manipulated
 */
void move_to_finished(ready_blocked_queues_t* queues);


/**
 * Takes a process in the blocked queue and adds it to the end of the ready queue 
 * 
 * @param idx    - the index of the process in blocked
 * @param queues - the queues that are being manipulated
 */
void add_to_ready(ready_blocked_queues_t* queues);

#endif