# 537pfsim

## Page fault simulator using various page replacement algorithms

## Main Algorithm

1. Get command line arguments (page size, real memory size, and trace file name)
2. Do first pass of the trace file, creating the process list with maps to its blocks in the trace file
3. Build ready and blocked queues for the processes. Put all processes in the ready queue
4. Initialize disk, inverted page table (??), and statistics
5. Start main loop
6. If no current process exists, pop a process from ready queue
7. Get next memory reference for that process
8. If page is in table
	- continue
9. If page is not in table
	- Add page to disk queue
	- Move process to blocked queue
	- Set current process pointer to NULL (Cause memory reference error?)
10. Check if disk is ready
	- if it is, pop page from disk queue
		- if page table is full, use algorithm to determine which page to remove
		- Remove page
		- Add new page to page table
11. Increment clock
12. Update ready and blocked queues

## Modules

- **cmd_line_processing.h:**
	- *Summary:* Parses command line arguments to construct page table
	- *Status:* Completed
	- *Files:* cmd_line_processing.h, cmd_line_processing.c
	- *Methods:*
		- `cmd_args process_args(int argc, char** argv)`
			- *Status:* Done
			- *Functionality:* Processes command line arguments to get file name, mem size, and page size
	

- **input.h**
	- *Summary:* Parses input file to construct process queue and get next memory reference
	- *Status:* INP
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
		- `unsigned long int is_waiting(process_t* process, unsigned long int clock)`
			- *Status:* DONE, TESTED
			- *Functionality:* Determines if a process is waiting or not
		- `process_t* create_process(unsigned long int pid);`
			- *Status:* DONE, TESTED
			- *Functionality:* Creates a process and initializes struct members

- **page_table.h**
	- *Summary:* Stores a page table implemented as a binary tree
	- *Status:* INP
	- *Files:* page_table.h, page_table.c
	- *Methods:*
		- `page_table_t* create_page_table(unsigned long int size)`
			- *Status:* DONE, TESTED
			- *Functionality:* Creates a page table and initializes struct members
		- `page_t* get_from_ptable(page_table_t* table, unsigned long int pid, unsigned long int vpn)`
			- *Status:* DONE, TESTED
			- *Functionality:* Gets the page from the page table with the corresponding vpn
		- `void add_to_ptable(page_table_t* ptable, page_t* page)`
			- *Status:* DONE, TESTED
			- *Functionality:* Adds a new page to the page table
		- `page_t* remove_from_ptable(page_table_t* ptable, page_t* page)`
			- *Status:* DONE, TESTED
			- *Functionality:* Removes a page from the page table

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
		- `process_t* get_next_process(ready_blocked_queues_t* queue)`
			- *Status:* DONE, TESTED
			- *Functionality:* Gets the next ready process
		- `void update_queues(ready_blocked_queues_t* queue, unsigned long int clock)`
			- *Status:* DONE, TESTED
			- *Functionality:* Moves the first element in blocked to ready if it is ready
		- `void move_to_blocked(ready_blocked_queues_t* queue)`
			- *Status:* DONE, TESTED
			- *Functionality:* Moves a process to the blocked queue
		- `void add_to_ready(ready_blocked_queues_t* queues)`
			- *Status:* DONE, TESTED
			- *Functionality:* Moves a process to the ready queue
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
			- *Status:* DONE, TESTED
			- *Functionality:* Updates the memory reference for the page if necessary


 