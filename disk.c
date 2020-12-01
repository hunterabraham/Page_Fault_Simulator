#include <stdio.h>
#include <stdlib.h>
#include "disk.h"
#include "statistics.h"
#include "process_queue.h"



/**
 * Creates a disk of size size and initializes all members
 * 
 * @param size - the current size of the disk
 * @return     - a pointer to the new disk
 */
disk_t* create_disk(int size) {
	disk_t* new_disk = malloc(sizeof(disk_t));
	if (NULL == new_disk) {
		fprintf(stderr, "Error allocating new_disk in create_disk() in disk.c\n");
		exit(1);
	}
	new_disk->page_array = malloc(sizeof(page_t) * size);
	if (NULL == new_disk->page_array) {
		fprintf(stderr, "Error allocating new_disk->page_array in create_disk in disk.c\n");
		exit(1);
	}
	new_disk->num_faults = 0;
	new_disk->next_time_for_access = 0;
	new_disk->curr_size = 0;
	new_disk->begin = 0;
	new_disk->end = 0;
	new_disk->max_size = size;
	return new_disk;
}

/**
 * Adds a page to the disk 
 * 
 * @param disk - the disk being added to
 * @param page - the page being added to the disk
 */
void add_page_to_disk(disk_t* disk, page_t* page, unsigned long int clock) {
	if (disk->curr_size == 0) { // if there is no page in disk, update I/O time
		disk->next_time_for_access = clock + 2000000;
	}
	disk->page_array[disk->end] = page;
	disk->end = (disk->end + 1) % disk->max_size;
	disk->curr_size++;
}

/**
 * removes the page at the beginning of the queue from the disk
 * 
 * @param disk - the disk being removed from
 * @return     - the page that is removed (the first one in the queue)
 */
page_t* remove_page_from_disk(disk_t* disk, unsigned long int clock) {
	if (disk->curr_size != 0) { // if there is a page available, update I/O times
		disk->next_time_for_access = clock + 2000000;
	}
	page_t* page = disk->page_array[disk->begin];
	disk->begin = (disk->begin + 1) % disk->max_size;
	disk->curr_size--;
	
	return page;
}

/**
 * Checks to see if the disk has completed its I/O and is ready
 * 
 * @param disk  - the disk to check if its ready
 * @param clock - the clock to compare to
 * @return      - 1 if ready, 0 if not ready
 */
unsigned long int is_ready(disk_t* disk, unsigned long int clock) {
	if (disk->next_time_for_access <= clock) {
		return 1;
	}
	return 0;
}


void free_disk(disk_t* disk) {
	unsigned int i = disk->begin;
	/* FIXME should be able to free
	while(disk->page_array[i] != NULL) {
		free(disk->page_array[i]);
		i = (i + 1) % disk->max_size;
	}
	*/
	free(disk->page_array);
	free(disk);
}
