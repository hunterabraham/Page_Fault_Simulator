#ifndef PROCESS_H
#define PROCESS_H

typedef struct process_info {
	int pid;
	int num_pages;
	int time_when_ready;
	int first_ref;
	int last_ref;
}


#endif
