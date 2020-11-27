#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "cmd_line_processing.h"
#include "statistics.h"
#include "process.h"
#include "page_table.h"
#include "disk.h"
#include "process_queue.h"


int main(int argc, char** argv) {
	const unsigned long int BUFSIZE = 4096;
	cmd_args* args = process_args(argc, argv);
	unsigned long int page_table_size = args->real_mem_size / args->page_size;
	char* fpath = args->file_name;
	process_t** list_of_procs = find_all_processes(fpath, page_table_size);
	ready_blocked_queues_t* queues = create_ready_blocked_queues(BUFSIZE, list_of_procs);
	unsigned long int clock = 0;
	unsigned long int num_finished_procs = 0;
	while(num_finished_procs != queues->num_procs) {
		process_t* curr_proc = peek_ready(queues);
		page_t* new_page = read_next(curr_proc); // TODO: have to check if NULL
		if (is_in_ptable(curr_proc->page_table, new_page)) {
			continue;
		} else { // page is not in table
			add_to_disk
		}
	}
	

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
