#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "cmd_line_processing.h"
#include "statistics.h"
#include "process.h"
#include "page_table.h"
#include "disk.h"
#include "process_queue.h"
#include "clock.h"

int main(int argc, char** argv) {
	const unsigned long int BUFSIZE = 4096;
	// get command line arguments
	cmd_args* args = process_args(argc, argv);
	unsigned long int num_page_tables = args->real_mem_size / args->page_size;
	char* fpath = args->file_name;
	// do first pass over trace file to build process list
	unsigned long int* num_mem_refs = malloc(sizeof(int));
	if (num_mem_refs == NULL) {
		fprintf(stderr, "Error allocating num_mem_refs in main_fifo.c\n");
	}
	process_t** list_of_procs = find_all_processes(fpath, num_mem_refs);
	// build ready and blocked queues for the processes
	ready_blocked_queues_t* queues = create_ready_blocked_queues(BUFSIZE, list_of_procs);
	// initialize disk & statistics
	disk_t* disk = create_disk(BUFSIZE); // FIXME
	stats_t* stats = init_stats();
	unsigned long int clock = 0;
	unsigned long int num_finished_procs = 0;
	long int num_pages = 0;
	unsigned long int num_procs_total = queues->ready_queue->curr_size;
	queue_t* queue = create_queue(num_page_tables);
	// MAIN LOOP
	while(num_finished_procs < num_procs_total) {
		process_t* curr_proc = peek_ready(queues);
		if (curr_proc != NULL) {
			page_t* new_page = read_next(curr_proc); 
			if (new_page == NULL) {
				unsigned long int ret = remove_all_pages(queue, curr_proc->pid);
				free(new_page); 
				num_finished_procs++;
				move_to_finished(queues);
				num_pages -= curr_proc->num_pages;
				continue;
			}
			if (is_in_ptable(curr_proc->page_table, new_page)) { // page is in table, free memory and increment memory references
				stats->total_memory_references += 1;
				update_mem_reference(queue, new_page);
				free(new_page);
			} else { // page is not in table
				fseek(curr_proc->fptr, -1 * ((int)new_page->num_bytes), SEEK_CUR);
				stats->total_page_faults++;
				add_page_to_disk(disk, new_page, clock);
				move_to_blocked(queues);
			}
		}
		if (is_ready(disk, clock)) {
			page_t* page_to_add = remove_page_from_disk(disk, clock);
			if (page_to_add != NULL) {
				page_t* page_to_remove = replacement_algorithm(queue, page_to_add);
				if (page_to_remove != NULL) {
					process_t* process_of_page = search_for_process(queues, page_to_remove->pid);
					if (process_of_page == NULL) { // page has already been removed
					} else {
						remove_from_ptable(process_of_page->page_table, page_to_remove);
						process_of_page->num_pages -= 1;
						num_pages--;
					}
				}
				process_t* process_of_add_page = search_for_process(queues, page_to_add->pid); 
				process_of_add_page->num_pages = add_to_ptable(process_of_add_page->page_table, page_to_add);
				add_to_ready(queues);
				num_pages++;
			}
		}
		stats->sum_page_frames += ((long double)queue->curr_size) / ((long double)num_page_tables);
		stats->sum_runnable_processes += queues->ready_queue->curr_size;
		if (queues->ready_queue->curr_size == 0) {
			stats->sum_page_frames += (((long double)(disk->next_time_for_access - clock)) * ((long double)queue->curr_size)) / ((long double) num_page_tables);
			clock = disk->next_time_for_access;
		} else {
			clock++;
		}
		
		

	}
	stats->average_memory_utilization = ((long double)(((long double)stats->sum_page_frames) / ((long double)clock)));
	stats->average_runnable_processes = ((long double)(((long double)stats->sum_runnable_processes) / ((long double)clock)));
	print_stats(stats, clock);
	free_processes(queues);
	free_disk(disk);
	free_queue(queue);
	free(stats);
	free(args);
	free(num_mem_refs);
}
