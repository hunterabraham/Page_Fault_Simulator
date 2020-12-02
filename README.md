# 537pfsim

## Page fault simulator using various page replacement algorithms

## 537pfsim-fifo
Runs the page fault simulator with a FIFO page replacement algorithm

## 537pfsim-lru
Runs the page fault simulator with an LRU page replacement algorithm

## 537pfsim-clock
Runs the page fault simulator with a Clock page replacement algorithm

## Arguments
* **-p <page_size>** - the size of a page in physical memory
* **-m <mem_size>** - the size of physical memory
* **<file_name>** - the file path to the trace file   


## Main Algorithm

1. Get command line arguments (page size, real memory size, and trace file name)
2. Do first pass of the trace file, creating the process list with maps to its blocks in the trace file
3. Build ready and blocked queues for the processes. Put all processes in the ready queue
4. Initialize disk, data structure, and statistics
5. Start main loop
6. If no current process exists, pop a process from ready queue
7. Get next memory reference for that process
	- If process is done, free memory
8. If page is in table
	- continue
9. If page is not in table
	- Add page to disk queue
	- Move process to blocked queue
10. Check if disk is ready
	- if it is, pop page from disk queue
		- if page table is full, use algorithm to determine which page to remove
		- Remove page
		- Add new page to page table
11. Increment clock
	- if no processes are ready, "fast forward" to when disk is ready
12. Update ready and blocked queues

## Modules

- **cmd_line_processing.h:**
	- *Summary:* Parses command line arguments to construct page table
	- *Status:* DONE
	- *Files:* cmd_line_processing.h, cmd_line_processing.c
	- *Methods:*
		- `cmd_args process_args(int argc, char** argv)`
			- *Status:* DONE, TESTED
			- *Functionality:* Processes command line arguments to get file name, mem size, and page size
	

- **input.h**
	- *Summary:* Parses input file to construct process queue and get next memory reference
	- *Status:* DONE
	- *Files:* input.c, input.h
	- *Methods:*
		- `process_t** find_all_processes(char* fpath)`
			- *Status:* DONE, TESTED
			- *Functionality:* Finds all processes and blocks which these processes have in the file
		- `unsigned long int read_next(process_t* process)`
			- *Status:* DONE, TESTED
			- *Functionality:* Reads the next memory reference for the process

- **process.h**
	- *Summary:* Stores the info for each process, including page table, etc.
	- *Status:* DONE, TESTED
	- *Files:* process.h, process.c
	- *Methods:*
		- `process_t* create_process(unsigned long int pid);`
			- *Status:* DONE, TESTED
			- *Functionality:* Creates a process and initializes struct members
		- `void free_process(process_t* process)`
			- *Status:* DONE, TESTED
			- *Functionality:* Frees a process and its memory

- **page_table.h**
	- *Summary:* Stores a page table implemented as a binary tree
	- *Status:* INP
	- *Files:* page_table.h, page_table.c
	- *Methods:*
		- `page_table_t* create_page_table(unsigned long int size)`
			- *Status:* DONE, TESTED
			- *Functionality:* Creates a page table and initializes struct members
		- `int compare_pages(const void* page1, const void* page2)`
			- *Status:* DONE, TESTED
			- *Functionality:* Compares pages in the table to determine their position in the tree
		- `void add_to_ptable(page_table_t* ptable, page_t* page)`
			- *Status:* DONE, TESTED
			- *Functionality:* Adds a new page to the page table
		- `page_t* remove_from_ptable(page_table_t* ptable, page_t* page)`
			- *Status:* DONE, TESTED
			- *Functionality:* Removes a page from the page table
		- `page_t* find_page(void** root, page_t* page)`
			- *Status:* DONE, TESTED
			- *Functionality:* Finds a page in the tree
		- `unsigned long int is_in_ptable(page_table_t* table, page_t* page)`
			- *Status:* DONE, TESTED
			- *Functionality:* Determines if a page is in the page table or not
		- `void free_ptable(page_table_t* table)`
			- *Status:* DONE, TESTED
			- *Functionality:* Frees the page table's allocated memory


- **disk.h**
	- *Summary:* Stores all pages being pushed to disk
	- *Status:* INP
	- *Files:* disk.c, disk.h
	- *Methods:*
		- `disk_t* create_disk(int size)`
			- *Status:* DONE, TESTED
			- *Functionality:* Creates disk and initializes struct members
		- `void add_page_to_disk(disk_t* disk, page_t* page)`
			- *Status:* DONE, TESTED
			- *Functionality:* Adds a page to the disk queue
		- `page_t* remove_page_from_disk(disk_t* disk)`
			- *Status:* DONE, TESTED
			- *Functionality:* Removes a page from the disk queue
		- `unsigned long int is_ready(disk_t* disk, unsigned long int clock)`
			- *Status:* DONE, TESTED
			- *Functionality:* Determines if the disk is ready to start another I/O operation


- **process_queue.h**
	- *Summary:* Creates a ready and blocked queue for processes
	- *Status:* INP
	- *Files:* process_queue.h, process_queue.c
	- *Methods:*
		- `ready_blocked_queues_t* create_ready_blocked_queues(int size, process_t** ready_queue)`
			- *Status:* DONE, TESTED
			- *Functionality:* Creates the ready and blocked queues system and initializes struct members
		- `process_queue_t* create_process_queue(int size)`
			- *Status:* DONE, TESTED
			- *Functionality:* Creates an individual queue (ready or blocked) for the ready-blocked system
		- `void move_to_blocked(ready_blocked_queues_t* queue)`
			- *Status:* DONE, TESTED
			- *Functionality:* Moves a process to the blocked queue
		- `void add_to_ready(ready_blocked_queues_t* queues)`
			- *Status:* DONE, TESTED
			- *Functionality:* Moves a process to the ready queue
		- `process_t* peek_ready(ready_blocked_queues_t* queues)`
			- *Status:* DONE, TESTED
			- *Functionality:* Peeks at the first process in ready queue
		- `process_t* search_for_process(ready_blocked_queues_t* queues, unsigned long int pid)`
			- *Status:* DONE, TESTED
			- *Functionality:* Searches for a process with the given PID
		- `void free_processes(ready_blocked_queues_t* queues)`
			- *Status:* DONE, TESTED
			- *Functionality:* Frees the processes and all of its members, including blocks and page tables

- **page_replacement_interface.h**
	- *Summary:* Interface for page replacement algorithms
	- *Status:* DONE, TESTED
	- *Files:* page_replacement_interface.h, fifo.c, lru.c, clock.c
	- *Methods:*
		- `queue_t* create_queue(int size)`
			- *Status:* DONE, TESTED
			- *Functionality:* Creates the data structure for the algorithm and allocates structures & memory
		- `void free_queue(queue_t* queue)`
			- *Status:* DONE, TESTED
			- *Functionality:* Frees the data structure & its memory
		- `unsigned long int remove_all_pages(queue_t* queue, unsigned long int pid)`
			- *Status:* DONE, TESTED
			- *Functionality:* Frees the memory of the process with the specified pid
		- `page_t* replacement_algorithm(queue_t* queue, page_t* page)`
			- *Status:* DONE, TESTED
			- *Functionality:* Runs the page replacement algo of the executable (E.g. FIFO, LRU, Clock)
		- `void update_mem_reference(queue_t* queue, page_t* page)`
			- *Status:* INP - clock not working correctly
			- *Functionality:* Updates the memory reference for the page if necessary
 