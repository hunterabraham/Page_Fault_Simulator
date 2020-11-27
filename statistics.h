#ifndef STATS_H
#define STATS_H

typedef struct stats_t {
	unsigned long int sum_page_frames;
	double average_memory_utilization;
	unsigned long int sum_runnable_processes;
	double average_runnable_processes;
	unsigned long int total_memory_references;
	unsigned long int total_page_faults;
} stats_t;


stats_t* init_stats();

void print_stats(stats_t* stats, unsigned long int clock);

#endif
