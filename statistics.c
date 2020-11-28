#include <stdlib.h>
#include <stdio.h>
#include "statistics.h"




stats_t* init_stats() {
	stats_t* stats = malloc(sizeof(stats_t));
	if (NULL == stats) {
		fprintf(stderr, "Error allocating stats in initStats() in statistics.c\n");
		exit(1);
	}
	stats->sum_page_frames = 0.0;
	stats->average_memory_utilization = 0.0;
	stats->sum_runnable_processes = 0;
	stats->average_runnable_processes = 0.0;
	stats->total_memory_references = 0;
	stats->total_page_faults = 0;
	return stats;
}

void print_stats(stats_t* stats, unsigned long int clock) {
	printf("Average Memory Utilization (AMU): %f\n", stats->average_memory_utilization);
	printf("Average Runnable Processes (ARP): %f\n", stats->average_runnable_processes);
	printf("Total Memory References (TMR):    %ld\n", stats->total_memory_references);
	printf("Total Page Faults (TPF):          %ld\n", stats->total_page_faults);
	printf("Total Running Time:               %ld\n", clock);
}


