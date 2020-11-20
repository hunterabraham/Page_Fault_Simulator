#include <stdlib.h>
#include <stdio.h>
#include "memref_queue.h"


Queue *create_queue(int size) {
	Queue *q = (Queue*)malloc(sizeof(Queue)); // allocate queue
	if (NULL == q) {
		fprintf(stderr, "Error allocating queue.\n");
		exit(1);
	}
	q->ref_array = (memref**)malloc(size * sizeof(memref)); // allocate queue array
	if (NULL == q->ref_array) {
		fprintf(stderr, "Error allocating queue array\n");
		exit(1);
	}

	// init helper counters
	q->begin = 0;
	q->end = 0;
	q->curr_size =0;
	q->max_size = size;
	q->num_pages = 0;

	// init synchronization variables
	return q;
}

int remove_ref_mem(Queue* q, int k) {
	if(q->curr_size == 0) { // wait for the queue to have elements
		return 0;
	}
	while(q->ref_array[k] != NULL) {
		q->ref_array[k] = q->ref_array[k+1];
		k++;
	}
	q->curr_size--;
	q->end = (q->end - 1) % q->max_size;
	return 1;
}


int add_ref_mem(Queue* q, memref* ref) {

	if (q->max_size == q->curr_size) { 
		fprintf(stderr, "Mem ref queue full. Memory reference skipped.\n");
		return 0;
	}
	// add the element to the queue and update the counter
	q->ref_array[q->end] = ref;
	q->curr_size++;
	q->end = (q->end + 1) % q->max_size;
	return 1;
}
