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
			- *Status:* Done
			- *Functionality:* Finds all processes and blocks which these processes have in the file
		- `unsigned long int read_next(process_t* process)`
			- *Status:* INP
			- *Functionality:* Reads the next memory reference for the process

- **process.h**
	- *Summary:* Stores the info for each process, including page table, etc.
	- *Status:* INP
	- *Files:* process.h, process.c
	- *Methods:*
		- `unsigned long int is_waiting(process_t* process, unsigned long int clock)`
			- *Status:* Done
			- *Functionality:* Determines if a process is waiting or not
		- `process_t* create_process(unsigned long int pid);`
			- *Status:* Done
			- *Functionality:* Creates a process and initializes struct members

- **page_table.h**
	- *Summary:* Stores a page table implemented as a hash map
	- *Status:* INP
	- *Files:* page_table.h, page_table.c
	- *Methods:*
		- `page_table_t* create_page_table(unsigned long int size)`
			- *Status:* Done
			- *Functionality:* Creates a page table and initializes struct members
		- `page_t* get_from_ptable(page_table_t* table, unsigned long int pid, unsigned long int vpn)`
			- *Status:* INP
			- *Functionality:* Gets the page from the page table with the corresponding vpn
		- `void add_to_ptable(page_table_t* ptable, page_t* page)`
			- *Status:* INP
			- *Functionality:* Adds a new page to the page table
		- `page_t* remove_from_ptable(page_table_t* ptable, page_t* page)`
			- *Status:* INP
			- *Functionality:* Removes a page from the page table
		- `unsigned long int hash_ptable(page_table_t* table, page_t* page)`
			- *Status:* INP
			- *Functionality:* Hashes a page to the corresponding index in the page table


- **disk.h**
	- *Summary:* Stores all pages being pushed to disk
	- *Status:* INP
	- *Files:* disk.c, disk.h
	- *Methods:*
		- `disk_t* create_disk(int size)`
			- *Status:* INP
			- *Functionality:* Creates disk and initializes struct members
		- `void add_page_to_disk(disk_t* disk, page_t* page)`
			- *Status:* INP
			- *Functionality:* Adds a page to the disk queue
		- `page_t* remove_page_from_disk(disk_t* disk)`
			- *Status:* INP
			- *Functionality:* Removes a page from the disk queue
		- `unsigned long int is_ready(disk_t* disk, unsigned long int clock)`
			- *Status:* INP
			- *Functionality:* Determines if the disk is ready to start another I/O operation

- **inverted_page_table.h**
	- *Summary:* Stores ppn-><vpn, pid> mapping in a hash map
	- *Status:* INP
	- *Files:* inverted_page_table.h, inverted_table_table.c
	- *Methods:*
		- `inverted_ptable_t* create_inverted_page_table(unsigned long int size)`
			- *Status:* INP
			- *Functionality:* Creates inverted page table and initializes struct members
		- `inverted_page_t* get_from_ptable(inverted_ptable_t* table, unsigned long int pid, unsigned long int vpn)`
			- *Status:* INP
			- *Functionality:* Gets an inverted page from the inverted page table using ppn hashing
		- `void add_to_ptable(inverted_ptable_t* ptable, page_t* page)`
			- *Status:* INP
			- *Functionality:* Adds an inverted page to the page table
		- `page_t* remove_from_ptable(inverted_ptable_t* ptable, page_t* page)`
			- *Status:* INP
			- *Functionality:* Removes an inverted page from the page table
		- `unsigned long int hash_inverted_ptable(inverted_page_table_t* table, inverted_page_t* page)`
			- *Status:* INP
			- *Functionality:* Hashes an inverted page to an index in the inverted page table



- **process_queue.h**
	- *Summary:* Creates a ready and blocked queue for processes
	- *Status:* INP
	- *Files:* process_queue.h, process_queue.c
