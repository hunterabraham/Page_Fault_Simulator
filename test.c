#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "cmd_line_processing.h"
#include "statistics.h"
#include "process.h"
#include "page_table.h"
#include "disk.h"
#include "process_queue.h"

void test_moving_queues(ready_blocked_queues_t* queues) {
    process_queue_t* ready = queues->ready_queue;
    process_queue_t* blocked = queues->blocked_queue;
	long unsigned int i = ready->begin;
    fprintf(stderr, "Ready\n");
	while(ready->process_array[i] != NULL) {
		fprintf(stderr, "%ld\n", ready->process_array[i]->pid);
		i++;
	}
	fprintf(stderr, "Total processes: %ld\n", ready->curr_size);
	fprintf(stderr, "Begin:           %ld\n", ready->begin);
	fprintf(stderr, "End:             %ld\n", ready->end);
	fprintf(stderr, "Blocked\n");
    int j = blocked->begin;
    while(blocked->process_array[j] != NULL) {
		fprintf(stderr, "%ld\n", blocked->process_array[j]->pid);
		j++;
	}
	fprintf(stderr, "Total processes: %ld\n", blocked->curr_size);
	fprintf(stderr, "Begin:           %ld\n", blocked->begin);
	fprintf(stderr, "End:             %ld\n", blocked->end);
    fprintf(stderr, "\n**************************MOVING**************************\n\n");

    move_to_blocked(queues);
    
	ready = queues->ready_queue;
    i = ready->begin;
    fprintf(stderr, "Ready\n");
	while(ready->process_array[i] != NULL) {
		fprintf(stderr, "%ld\n", ready->process_array[i]->pid);
		i++;
	}
	fprintf(stderr, "Total processes: %ld\n", ready->curr_size);
	fprintf(stderr, "Begin:           %ld\n", ready->begin);
	fprintf(stderr, "End:             %ld\n", ready->end);
	fprintf(stderr, "Blocked\n");
    j = blocked->begin;
    while(blocked->process_array[j] != NULL) {
		fprintf(stderr, "%ld\n", blocked->process_array[j]->pid);
		j++;
	}
	fprintf(stderr, "Total processes: %ld\n", blocked->curr_size);
	fprintf(stderr, "Begin:           %ld\n", blocked->begin);
	fprintf(stderr, "End:             %ld\n", blocked->end);
	unsigned long int clock = 0;

    fprintf(stderr, "\n**************************UPDATING**************************\n\n");

	update_queues(queues, clock);

    ready = queues->ready_queue;
	i = ready->begin;
    fprintf(stderr, "Ready\n");
	while(ready->process_array[i] != NULL) {
		fprintf(stderr, "%ld: %ld\n", i,  ready->process_array[i]->pid);
        i++;
	}

	fprintf(stderr, "Total processes: %ld\n", ready->curr_size);
	fprintf(stderr, "Begin:           %ld\n", ready->begin);
	fprintf(stderr, "End:             %ld\n", ready->end);
	fprintf(stderr, "Blocked\n");
    j = blocked->begin;
    while(blocked->process_array[j] != NULL) {
		fprintf(stderr, "%ld\n", blocked->process_array[j]->pid);
		j++;
	}
	fprintf(stderr, "Total processes: %ld\n", blocked->curr_size);
	fprintf(stderr, "Begin:           %ld\n", blocked->begin);
	fprintf(stderr, "End:             %ld\n", blocked->end);
}

int main(int argc, char** argv) {
	const unsigned long int BUFSIZE = 4096;
	cmd_args* args = process_args(argc, argv);
	unsigned long int page_table_size = args->real_mem_size / args->page_size;
	char* fpath = args->file_name;
    fprintf(stderr, "%s\n", fpath);
	process_t** list_of_procs = find_all_processes(fpath);
	ready_blocked_queues_t* queues = create_ready_blocked_queues(BUFSIZE, list_of_procs);
	test_moving_queues(queues);

	

	/*
	process_queue_t* ready = queues->ready_queue;
	int i = 0;
	while(ready->process_array[i] != NULL) {
		fprintf(stderr, "%ld\n", ready->process_array[i]->pid);
		i++;
	}
	fprintf(stderr, "Total processes: %d\n", i);
	fprintf(stderr, "Begin:           %ld\n", ready->begin);
	fprintf(stderr, "End:             %ld\n", ready->end);
	*/
	/**
	process_t* proc;
	int i = 0;
	while((proc = list_of_procs[i++]) != NULL) {
		fprintf(stderr, "%ld\n", proc->pid);
		unsigned long int j = 0;
		block_t block_temp;
		while(j < proc->num_blocks) {
			block_temp = proc->blocks[j++];
			fprintf(stderr, "\t%ld->%ld\n", block_temp.start, block_temp.end);
		}
	}
	*/

}
