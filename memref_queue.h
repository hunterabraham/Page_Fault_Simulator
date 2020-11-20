#ifndef MEM_REF_QUEUE_H
#define MEM_REF_QUEUE_H
#include "memref.h"

typedef struct Queue {
	memref** ref_array;
	int begin;
	int end;
	int curr_size;
	int max_size;
	int num_pages;
} Queue;

Queue* create_queue(int size);

int remove_ref_mem(Queue* q, int k);

int add_ref_mem(Queue* q, memref* ref);

#endif
