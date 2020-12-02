////////////////////////////////////////////////////////////////////////////////
// Main File:        main.c
// This File:        statistics.h
// Semester:         CS 537 Fall 2020
//
// Authors:          Hunter Abraham
// Emails:           hjabraham@wisc.edu
// CS Logins:        habraham
//
/////////////////////////// OTHER SOURCES OF HELP //////////////////////////////
// 									NONE
////////////////////////////////////////////////////////////////////////////////




#ifndef STATS_H
#define STATS_H

/**
 * Holds statistics for the page replacement algorithm
 */
typedef struct stats_t {
	long double sum_page_frames;
	long double average_memory_utilization;
	unsigned long int sum_runnable_processes;
	long double average_runnable_processes;
	unsigned long int total_memory_references;
	unsigned long int total_page_faults;
} stats_t;


/**
 * Initializes statistics and allocates memory
 * 
 * @return - a stats_t struct containing all statistics 
 */
stats_t* init_stats();

/**
 * Prints all statistics and total run time
 * 
 * @param stats - the stats struct that was used throughout the program
 * @param clock - the clock that was used throughout the program
 */
void print_stats(stats_t* stats, unsigned long int clock);

#endif
