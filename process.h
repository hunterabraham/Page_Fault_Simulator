////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        process.h
// Semester:         CS 537 Fall 2020
//
// Authors:          Hunter Abraham
// Emails:           hjabraham@wisc.edu
// CS Logins:        habraham
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
// 									NONE
////////////////////////////////////////////////////////////////////////////////


#ifndef PROCESS_H
#define PROCESS_H
#include <stdio.h>
#include "page_table.h"

/**
 * blocks of memory references in the file
 */
typedef struct block_t {
	unsigned long int start; // the start of the block
	unsigned long int curr_line; // the current line in the block
	unsigned long int end; // the end of the block
} block_t;

/**
 * Process struct to hold pid, time when the process is ready, etc.
 */
typedef struct process_t {
	unsigned long int pid;
	unsigned long int time_when_ready; // compared to clock to determine if blocked
	unsigned long int last_ref;
	page_table_t* page_table;
	block_t* blocks; // sorted, will happen when we do first pass
	unsigned long int num_blocks; // holds the number of blocks
	unsigned long int curr_block_idx; // starts at 0, lets us know which block we're on
	FILE* fptr; // where in the file the current process is
	unsigned long int curr_line_idx;
	unsigned long int num_pages;
} process_t;


/**
 * Creates a process with the pid specified
 * 
 * @param pid - the pid of the process
 * @return    - a pointer to the new process struct
 */
process_t* create_process(unsigned long int pid);


/**
 * Frees a process and its allocated memory
 * 
 * @param process - the process to be freed
 */
void free_process(process_t* process);

#endif
