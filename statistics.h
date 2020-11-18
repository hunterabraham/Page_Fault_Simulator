#ifndef STATS_H
#define STATS_H

typedef struct stats_t {
	int sum_page_frames;
	double average_memory_utilization;
	int sum_runnable_processes;
	double average_runnable_processes;
	int total_memory_references;
	int total_page_faults;
} stats_t;


#endif
