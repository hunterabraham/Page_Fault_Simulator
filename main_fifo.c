#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "cmd_line_processing.h"
#include "statistics.h"
#include "process.h"
#include "page_table.h"
#include "disk.h"
#include "process_queue.h"
#include "fifo.h"

int main(int argc, char** argv) {
	const unsigned long int BUFSIZE = 4096;
	// get command line arguments
	cmd_args* args = process_args(argc, argv);
	unsigned long int num_page_tables = args->real_mem_size / args->page_size;
	char* fpath = args->file_name;
	free(args);
	//fpath = "test/4000.addrtrace"; // FIXME
	// do first pass over trace file to build process list
	process_t** list_of_procs = find_all_processes(fpath, num_page_tables);
	// build ready and blocked queues for the processes
	ready_blocked_queues_t* queues = create_ready_blocked_queues(BUFSIZE, list_of_procs);
	// initialize disk & statistics
	disk_t* disk = create_disk(BUFSIZE);
	stats_t* stats = init_stats();
	unsigned long int clock = 0;
	unsigned long int num_finished_procs = 0;
	unsigned long int num_pages = 0;
	//page_table_t* global_page_table = create_page_table(num_page_tables); // <process_t*, page_t*>
	fifo_queue_t* fifo_queue = create_fifo_queue(1000000);
	// MAIN LOOP
	while(num_finished_procs != queues->num_procs) { //|| disk->curr_size != 0) { // FIXME: most likely wrong
		process_t* curr_proc = peek_ready(queues);
		if (curr_proc != NULL) {
			page_t* new_page = read_next(curr_proc); // TODO: have to check if NULL
			if (new_page == NULL) {
				free(new_page); // FIXME: needed?
				num_finished_procs++;
				unsigned long int num_pages_proc = move_to_finished(queues);
				num_pages -= num_pages_proc;
				continue;
			}
			if (is_in_ptable(curr_proc->page_table, new_page)) {
				stats->total_memory_references += 1;
			} else { // page is not in table
				// have to move process' pointer back one
				fseek(curr_proc->fptr, -1 * ((int)new_page->num_bytes), SEEK_CUR);
				stats->total_page_faults += 1;
				add_page_to_disk(disk, new_page, clock);
				move_to_blocked(queues);
			}
		}
		if (is_ready(disk, clock)) {
			page_t* page_to_add = remove_page_from_disk(disk, clock);
			if (page_to_add != NULL) {
				if (num_pages >= num_page_tables) { // if page table is full, evict page
					page_t* page_to_remove = pop_from_fifo_queue(fifo_queue);
					process_t* process_of_page = search_for_process(queues, page_to_remove->pid); // BOTTLENECK
					remove_from_ptable(process_of_page->page_table, page_to_remove);
					num_pages--;
				}
				process_t* process_of_add_page = search_for_process(queues, page_to_add->pid); // BOTTLENECK
				add_to_ptable(process_of_add_page->page_table, page_to_add);
				add_to_ready(queues);
				push_to_fifo_queue(fifo_queue, page_to_add);
				process_of_add_page->num_pages++;
				num_pages++;
			}
		}
		stats->sum_page_frames += ((double)num_pages) / ((double)num_page_tables);
		stats->sum_runnable_processes += queues->ready_queue->curr_size;
		clock++;
	}

	stats->average_memory_utilization = ((double)(((double)stats->sum_page_frames) / ((double)clock)));
	stats->average_runnable_processes = ((double)(((double)stats->sum_runnable_processes) / ((double)clock)));
	print_stats(stats, clock);
	free_processes(queues);
	free_disk(disk);
	free_queue(fifo_queue);
	free(stats);
}
