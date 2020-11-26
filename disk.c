#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "statistics.h"

int isIn(disk* disk_t, memref* ref) { // can modify into R-B Tree later for O(logN)
	for (int i = disk_t->begin; i < disk_t->begin +  disk_t->curr_size; ++i) {
		if (i >= disk_t->max_size) {
			break;
		}
		if (disk_t->ref_queue[i]->page_no == ref->page_no && disk_t->ref_queue[i]->pid == ref->pid) {
			return 1;
		}
	}
	return 0;
}

int add_ref_disk(disk* disk_t, memref* ref, int clock, stats_t* stats) {
	if (disk_t->max_size == disk_t->curr_size) { 
		fprintf(stderr, "Disk queue full all %d/%d spots occupied. Memory reference skipped.\n", disk_t->curr_size, disk_t->max_size);
		return 0;
	}
	if (isIn(disk_t, ref)) { // if request already queued, ignore
		return 1;
	}
	// add the element to the queue and update the counter
	stats->total_page_faults++;
	disk_t->ref_queue[disk_t->end] = ref;
	disk_t->curr_size++;
	disk_t->end = (disk_t->end + 1) % disk_t->max_size;
	ref->proc->time_when_ready = clock + 2000;
	
	return 1;
}


memref* remove_ref_disk(disk* disk_t) {
	if(disk_t->curr_size == 0) { // wait for the queue to have elements
		return NULL;
	}
	memref* ref = disk_t->ref_queue[disk_t->begin];
	disk_t->curr_size--;
	disk_t->begin = (disk_t->begin + 1) % disk_t->max_size;
	return ref;
}


disk* create_disk(int size) {
	// allocate struct
	disk* new_disk = malloc(sizeof(disk));
	if (NULL == new_disk) {
		fprintf(stderr, "Error allocating new_disk in create_disk in disk.c\n");
		exit(1);
	}
	// initialize struct
	new_disk->ref_queue = malloc(sizeof(memref) * size);
	new_disk->begin = 0;
	new_disk->end = 0;
	new_disk->curr_size = 0;
	new_disk->max_size = size;
	new_disk->num_faults = 0;
	new_disk->next_time_for_access = 0;
	return new_disk;
}



