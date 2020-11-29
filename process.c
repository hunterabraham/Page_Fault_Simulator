#include <stdio.h>
#include <stdlib.h>
#include "process.h"

process_t* create_process(unsigned long int pid, unsigned long int page_table_size) {
	unsigned long int BUFSIZE = 4096;
	process_t* new_proc = (process_t*)malloc(sizeof(process_t));
	if (NULL == new_proc) {
		fprintf(stderr, "Error allocating new_proc in create_process() in process.c");
		exit(1);
	}
	new_proc->pid = pid;
	new_proc->time_when_ready = 0;
	new_proc->first_ref = 0;
	new_proc->last_ref = 0;
	new_proc->page_table = create_page_table(page_table_size);
	new_proc->blocks = (block_t*)malloc(sizeof(block_t) * BUFSIZE);
	if (NULL == new_proc->blocks) {
		fprintf(stderr, "Error allocating blocks in create_processes() in process.c\n");
		exit(1);
	}
	new_proc->num_blocks = 0;
	new_proc->curr_block_idx = 0;
	new_proc->curr_line_idx = 0;
	return new_proc;
}

unsigned long int is_waiting(process_t* process, unsigned long int clock) {
	if (process->time_when_ready <= clock) {
		return 0;
	}
	return 1;
}

void free_process(process_t* process) {
	free_ptable(process->page_table);
	free(process->blocks);
	free(process);
}