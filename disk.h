#include "statistics.h"

/**
 * Queue of completion times. Peek at first element. If <= clock, remove from queue of completion
 * times. Send page to page table (global and process). If disk isn't empty, next completion time
 * = clock + 2000. If it is full, add_to_disk will handle completion time.
 */
typedef struct completion_times {
	unsigned long int** times;
	unsigned long int begin;
	unsigned long int end;
	unsigned long int curr_size;
	unsigned long int max_size;
} completion_times;


/**
 * A struct to hold the disk QUEUE
 */
typedef struct disk_t {
	unsigned long int next_time_for_access;
	unsigned long int num_faults;
	page_t** page_array;
	unsigned long int curr_size;
	unsigned long int begin;
	unsigned long int end;
	unsigned long int max_size; // ?? use realloc if max_size <= curr_size
	unsigned long int** completion_times;
} disk_t;


/**
 * Creates a disk of size size and initializes all members
 * 
 * @param size - the current size of the disk
 * @return     - a pointer to the new disk
 */
disk_t* create_disk(int size);

/**
 * Adds a page to the disk 
 * 
 * @param disk - the disk being added to
 * @param page - the page being added to the disk
 */
void add_page_to_disk(disk_t* disk, page_t* page);

/**
 * removes the page at the beginning of the queue from the disk
 * 
 * @param disk - the disk being removed from
 * @return     - the page that is removed (the first one in the queue)
 */
page_t* remove_page_from_disk(disk_t* disk);

/**
 * Checks to see if the disk has completed its I/O and is ready
 * 
 * @param disk  - the disk to check if its ready
 * @param clock - the clock to compare to
 * @return      - 1 if ready, 0 if not ready
 */
unsigned long int is_ready(disk_t* disk, unsigned long int clock);