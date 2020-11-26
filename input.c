#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "input.h"
#include "process.h"


long int contains(process_t** pinfo_list, unsigned long int pid) {
	long int i = 0;
	if (NULL == pinfo_list) {
		fprintf(stderr, "pinfo_list not initialized when running contains().\n");
		exit(1);
	}
	while(pinfo_list[i] != NULL) {
		if (pinfo_list[i]->pid == pid) {
			return i;
		}
		i++;
	}
	return -1;
}

unsigned long int* split(char* buffer) {
	size_t split_idx;
	unsigned long int flag = 0;
	for (size_t i = 0; i < strlen(buffer); ++i) {
		if (buffer[i] != ' ') {
			flag = 1;
			continue;
		}
		if (flag && buffer[i] == ' ') {
			split_idx = i;
			break;
		}
	}
	
	char* v_addr_str = (buffer + split_idx + 1);
	*(v_addr_str + strlen(v_addr_str)) = '\0';
	*(buffer + split_idx) = '\0';
	unsigned long int vaddr = atoi(v_addr_str);
	unsigned long int pid_save = atoi(buffer);
	unsigned long int* ret_arr = malloc(sizeof(unsigned long int) * 2);
	if (NULL == ret_arr) {
		fprintf(stderr, "Error allocating ret_arr in split() in input.c\n");
		exit(1);
	}
	ret_arr[0] = vaddr;
	ret_arr[1] = pid_save;
	return ret_arr;
}


process_t** find_all_processes(char* fpath) {
	const unsigned long int BUFSIZE = 10000;
	// holds all processes
	process_t** process_list = malloc(sizeof(process_t) * BUFSIZE);
	if (NULL == process_list) {
		fprintf(stderr, "Error allocating pinfo_list in findAllProcesses() in input.c\n");
		exit(1);
	}
	char* buffer = malloc(sizeof(char) * BUFSIZE);
	if (NULL == buffer) {
		fprintf(stderr, "Error allocating buffer in findAllProcesses() in input.c\n");
		exit(1);
	}
	FILE* trace_file = fopen(fpath, "r");
	if (NULL == trace_file) {
		fprintf(stderr, "Error opening file \"%s\" in find_all_processes in input.c\n", fpath);
		exit(1);
	}
	unsigned long int process_list_idx = 0;
	unsigned long int curr_row = 0;
	while(NULL != fgets(buffer, BUFSIZE, trace_file)) {
		unsigned long int* ret_arr = split(buffer);
		unsigned long int pid = ret_arr[1];
		long int idx = contains(process_list, pid); // see if process is in list
		if (idx == -1) { // if it isn't, make a new one and add it to the list
			process_t* new_proc = create_process(pid); // TODO: initialize struct members
			new_proc->first_ref = curr_row;
			new_proc->last_ref = curr_row;
			new_proc->blocks[new_proc->num_blocks].start = curr_row;
			new_proc->blocks[new_proc->num_blocks].end = curr_row;
			new_proc->blocks[new_proc->num_blocks].curr_line = 0;
			new_proc->num_blocks++;
			new_proc->fptr = fopen(fpath, "r");
			if (NULL == new_proc->fptr) {
				fprintf(stderr, "Error opening file in findAllProcesses() in input.c\n");
				exit(1);
			}
			process_list[process_list_idx] = new_proc;
			process_list_idx++;
		} else { // process already in table, update row and potentially create new block / update block
			process_t* curr_proc = process_list[idx];
			if (curr_proc->last_ref != curr_row - 1) { // if not a continuation of old block, make new one
				curr_proc->blocks[curr_proc->num_blocks].start = curr_row;
				curr_proc->blocks[curr_proc->num_blocks].end = curr_row;
				curr_proc->blocks[curr_proc->num_blocks].curr_line = 0;
				curr_proc->num_blocks++;
			} else { // still on the same block
				curr_proc->blocks[curr_proc->num_blocks - 1].end = curr_row;
			}
			curr_proc->last_ref = curr_row;
		}
		curr_row++;
		free(ret_arr);
	}
	free(buffer);
	return process_list;
}


unsigned long int read_next(process_t* process) {
	unsigned long int BUFSIZE = 1000;
	char* BUFFER = malloc(sizeof(char) * BUFSIZE);
	char* result;
	while (1) {
		result = fgets(BUFFER, BUFSIZE, process->fptr);
		unsigned long int* res_array = split(result);
		unsigned long int pid = res_array[1];
		if (pid != process->pid) { // done with current block, move to next one
			if (process->curr_block_idx == process->num_blocks - 1) {
				// TODO: free processes' memory
			}
			block_t curr_block = process->blocks[process->curr_block_idx + 1]; // FIXME what if we're at the last block?
			int next_idx = curr_block.start;
			process->curr_block_idx++;
			fseek(process->fptr, next_idx, SEEK_SET); // move to next block in file
			continue;
		}
		unsigned long int vpn = res_array[0];
		return vpn;
	}
}
