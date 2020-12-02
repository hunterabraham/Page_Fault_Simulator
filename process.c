////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        process.c
// Semester:         CS 537 Fall 2020
//
// Authors:          Hunter Abraham
// Emails:           hjabraham@wisc.edu
// CS Logins:        habraham
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
// 									NONE
////////////////////////////////////////////////////////////////////////////////



#include <stdio.h>
#include <stdlib.h>
#include "process.h"

process_t* create_process(unsigned long int pid) {
	// creates a process & initializes its page table
	unsigned long int BUFSIZE = 4096;
	process_t* new_proc = (process_t*)malloc(sizeof(process_t));
	if (NULL == new_proc) {
		fprintf(stderr, "Error allocating new_proc in create_process() in process.c");
		exit(1);
	}
	new_proc->pid = pid;
	new_proc->time_when_ready = 0;
	//new_proc->first_ref = 0;
	new_proc->last_ref = 0;
	new_proc->page_table = create_page_table();
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


void free_process(process_t* process) {
	// frees a process & its memory
	free_ptable(process->page_table);
	free(process->blocks);
	free(process);
}