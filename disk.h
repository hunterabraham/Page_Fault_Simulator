#include "memref.h"

// struct to hold data for the disk
typedef struct disk {
	int next_time_for_access;
	int num_faults;
	memref** ref_queue;
	int curr_size;
	int begin;
	int end;
	int max_size;
} disk;
/**
 * Adds a reference to the end of the disk queue O(1)
 *
 * @param disk_t - the disk to add the reference to
 * @param ref    - the reference to add to the disk
 * @return       - 1 upon success, 0 upon failure (i.e., the disk queue is full)
 **/
int add_ref_disk(disk* disk_t, memref* ref, int clock);

/**
 * Removes the first reference from teh end of the disk queue O(1)
 *
 * @param disk_t - the disk to remove from the disk
 * @return       - 1 upon success, 0 upon failure (i.e., the disk is empty)
 **/
memref* remove_ref_disk(disk* disk_t);


/**
 * Allocates a disk struct and initializes its members
 *
 * @param size - the size of the disk queue
 * @return     - the newly created disk
 **/
disk* create_disk(int size);

