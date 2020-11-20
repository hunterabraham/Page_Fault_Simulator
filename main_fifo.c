#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "cmd_line_processing.h"
#include "statistics.h"
#include "process.h"
#include "memref.h"
#include "memref_queue.h"
#include "page_table.h"
#include "disk.h"


int Enqueue(memref* ref, Queue* q, stats_t* stats, int clock, ptable* table, disk* disk_t) {
	// if page is in table and process is not waiting, do nothing
	if (NULL != ref) {
		add_ref_mem(q, ref);
	}
	int k = q->begin;
	while(q->ref_array[k] != NULL) {
		if (search_page(table, ref->page_no + ref->pid) != -1 && !isWaiting(ref->proc, clock)) { // good to go!
			stats->total_memory_references++;
			remove_ref_mem(q, k);
			return 1;
		} else if (search_page(table, ref->page_no + ref->pid) == -1) { // PAGE FAULT!
			fprintf(stderr, "Fault.");
			add_ref_disk(disk_t, ref, clock);
			stats->total_page_faults++;
		} else if (isWaiting(ref->proc, clock)) { // process is waiting
			k++;
			continue;
		}
		k++;
	}
	return 0;
}


int main(int argc, char** argv) {
	stats_t* stats = initStats(); 
	cmd_args* flags = processArgs(argc, argv);
	FILE* fptr = fopen(flags->file_name, "r");

	if (NULL == fptr) {
		fprintf(stderr, "Error opening file \"%s\"\n", flags->file_name);
		exit(1);
	}
	long int BUFSIZE = flags->real_mem_size / flags->page_size;
	process** process_list = findAllProcesses(fptr); // build proc list
	fclose(fptr);
	// FIXMEfptr = fopen(flags->file_name, "r");
	fptr = fopen("test/12.addrtrace", "r");
	if (NULL == fptr) {
		fprintf(stderr, "Error opening file \"%s\"\n the second time in main() in main_fifo.c\n", flags->file_name);
		exit(1);
	}
	Queue* queue = create_queue(10000);
	memref* ref_t;
	ptable* p_table = create_page_table(BUFSIZE);
	int clock = 0;
	int num_added = 0;
	disk* disk_t = create_disk(1000000);
	ref_t = readNext(fptr, process_list);
	Enqueue(ref_t, queue, stats, clock, p_table, disk_t);
	while(queue->curr_size != 0) {
		ref_t = readNext(fptr, process_list);
		if (clock % 2 == 0) {
		//	if (disk_t->curr_size != 0) {
				memref* ref_temp = remove_ref_disk(disk_t);
				if (p_table->curr_size == p_table->max_size) {
					remove_page(p_table, 0);
				}
				num_added++;
				fprintf(stderr, "NUM ADDED: %d DISK SIZE: %d, PAGE FAULTS: %d\n", num_added, disk_t->curr_size, stats->total_page_faults);
				add_page(p_table, ref_temp);
		//	}
		}
		Enqueue(ref_t, queue, stats, clock, p_table, disk_t);
		clock += 1;
	}
	printStats(stats, clock);
	process* p_temp;
	int k = 0;
	while((p_temp = process_list[k]) != NULL) {
		free(p_temp);
		k++;
	}


	fprintf(stderr, "Number of pages added to table: %d\n", num_added);
	// cleaning up resources
	free(process_list);
	fclose(fptr);
	free(stats);
	free(flags);
	return 0;
}
