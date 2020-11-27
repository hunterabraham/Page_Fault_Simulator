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


    fprintf(stderr, "\n**************************REMOVING**************************\n\n");

    remove_from_ready(queues);
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
    fprintf(stderr, "\n**************************SEARCHING**************************\n\n");
    move_to_blocked(queues);
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

    process_t* ret1 = search_for_process(queues, 3);
    if (ret1 == NULL) {
        fprintf(stderr, "Error searching for 3 in process table\n");
    }
    process_t* ret2 = search_for_process(queues, 28);
    if (ret2 != NULL) {
        fprintf(stderr, "Error searching for 28 in process table\n");
    }
    process_t* ret3 = search_for_process(queues, 23);
    if (ret3 == NULL) {
        fprintf(stderr, "Error searching for 23 in process table\n");
    }
    process_t* ret4 = search_for_process(queues, 45);
    if (ret4 == NULL) {
        fprintf(stderr, "Error searching for 45 in process table\n");
    }

    process_t* ret5 = peek_ready(queues);
    fprintf(stderr, "Peeked: %ld\n", ret5->pid);
}

void test_peek_ready(ready_blocked_queues_t* queues) {
    process_t* next_proc = peek_ready(queues);
    fprintf(stderr, "PID: %ld\n", next_proc->pid);
}

void test_get_next(ready_blocked_queues_t* queues) {
    process_t* next_proc = peek_ready(queues);
    unsigned long int count = 0;
    page_t* page_temp;
    
    unsigned long int block_num = -1;
    fprintf(stderr, "Process %ld.\n", next_proc->pid);
    while((page_temp = read_next(next_proc)) != NULL) {
        if (next_proc->curr_block_idx != block_num) {
            block_num = next_proc->curr_block_idx;
            fprintf(stderr, "%ld\n", block_num);
        }
        //fprintf(stderr, "%ld\n", page_temp->pid);

        count += 1;
    }
    move_to_blocked(queues);
    next_proc = peek_ready(queues);
    fprintf(stderr, "Process %ld.\n", next_proc->pid);
    block_num = -1;
    while((page_temp = read_next(next_proc)) != NULL) {
        if (next_proc->curr_block_idx != block_num) {
            block_num = next_proc->curr_block_idx;
            fprintf(stderr, "%ld\n", block_num);
        }
        //fprintf(stderr, "%ld\n", page_temp->pid);

        count += 1;
    }
    move_to_blocked(queues);
    next_proc = peek_ready(queues);
    fprintf(stderr, "Process %ld.\n", next_proc->pid);
    block_num = -1;
    while((page_temp = read_next(next_proc)) != NULL) {
        if (next_proc->curr_block_idx != block_num) {
            block_num = next_proc->curr_block_idx;
            fprintf(stderr, "%ld\n", block_num);
        }
        //fprintf(stderr, "%ld\n", page_temp->pid);
        count += 1;
    }
    move_to_blocked(queues);
    next_proc = peek_ready(queues);
    fprintf(stderr, "Process %ld.\n", next_proc->pid);
    block_num = -1;
    while((page_temp = read_next(next_proc)) != NULL) {
        if (next_proc->curr_block_idx != block_num) {
            block_num = next_proc->curr_block_idx;
            fprintf(stderr, "%ld\n", block_num);
        }
        //fprintf(stderr, "%ld\n", page_temp->pid);

        count += 1;
    }
    fprintf(stderr, "COUNT: %ld\n", count);

    
}

void test_ptable() {
    page_table_t* page_table = create_page_table(4096);
    page_t* new_page = malloc(sizeof(page_t));
    new_page->vpn = 1000;
    new_page->pid = 1000;
    fprintf(stderr, "%ld\n", page_table->free_list[1000]);

    add_to_ptable(page_table, new_page);
    fprintf(stderr, "%ld\n", page_table->page_array[new_page->page_table_idx]->vpn);
    fprintf(stderr, "%ld\n", page_table->free_list[new_page->page_table_idx]);
    page_t* new_page_2 = malloc(sizeof(page_t));
    new_page_2->vpn = 5000;
    new_page_2->pid = 1000;
    fprintf(stderr, "%ld\n", page_table->free_list[904]);

    add_to_ptable(page_table, new_page_2);

    fprintf(stderr, "%ld\n", page_table->page_array[new_page_2->page_table_idx]->vpn);
    fprintf(stderr, "%ld\n", page_table->free_list[new_page_2->page_table_idx]);
    remove_from_ptable(page_table, new_page_2);
    fprintf(stderr, "%ld\n", page_table->free_list[new_page_2->page_table_idx]);
    unsigned long int is_in = is_in_ptable(page_table, new_page);
    unsigned long int is_in_2 = is_in_ptable(page_table, new_page_2);
    fprintf(stderr, "Should be (1, 0): (%ld, %ld)\n", is_in, is_in_2);
}

void test_disk() {
    disk_t* disk = create_disk(100);
    page_t* page1 = malloc(sizeof(page_t));
    page1->vpn = 1000;
    page1->pid = 1000;
    unsigned long int clock = 100;
    add_page_to_disk(disk, page1, clock);
    fprintf(stderr, "Should be 2100: %ld\n", disk->next_time_for_access);
    page_t* ret_page = remove_page_from_disk(disk, clock);
    fprintf(stderr, "Should be 1000: %ld\n", ret_page->pid);
    for (int i = 0; i < 23; i++) {
        add_page_to_disk(disk, page1, clock);
        clock += 20;
    }
    fprintf(stderr, "Should be 2100: %ld\n", disk->next_time_for_access);

    unsigned long int clock_2 = 1800;
    fprintf(stderr, "Should be 0: %ld\n", is_ready(disk, clock_2));
    unsigned long int clock_3 = 2100;
    fprintf(stderr, "Should be 1: %ld\n", is_ready(disk, clock_3));
}


void test_ptable_2() {
    page_table_t* page_table = create_page_table(4096);
    page_t* new_page = malloc(sizeof(page_t));
    new_page->vpn = 1000;
    new_page->pid = 1000;
    add_to_ptable(page_table, new_page);
    for (int i = 0; i < 290; i++) {
        new_page->vpn = new_page->vpn + 1;
        add_to_ptable(page_table, new_page);
    }
    page_t* page2 = malloc(sizeof(page_t));
    page2->vpn = 4000;
    page2->pid = 1001;
    add_to_ptable(page_table, new_page);
    unsigned long int ret = is_in_ptable(page_table, new_page);
    fprintf(stderr, "%ld\n", ret);
    page_t* page3 = malloc(sizeof(page_t));
    page3->vpn = 1039;
    page3->pid = 1000;
    unsigned long int ret2 = is_in_ptable(page_table, new_page);
    fprintf(stderr, "%ld\n", ret2);
}

int main(int argc, char** argv) {
	const unsigned long int BUFSIZE = 4096;
	cmd_args* args = process_args(argc, argv);
	unsigned long int page_table_size = args->real_mem_size / args->page_size;
	char* fpath = args->file_name;
	process_t** list_of_procs = find_all_processes(fpath, BUFSIZE);
	ready_blocked_queues_t* queues = create_ready_blocked_queues(BUFSIZE, list_of_procs);
	//test_moving_queues(queues); // RUN WITH test/4000.addrtrace
    //test_peek_ready(queues);
    //test_get_next(queues);
    //test_ptable();
    //test_disk();
    //test_find_all_processes(queues);
    test_ptable_2();
}
