#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include "process.h"
#include "page_table.h"


/**
 * read_next() reads the next line in a trace file into memory and converts
 * it to a memref struct with the pid and page number
 *
 * @param trace_file - the traceFile being read
 * @param process    - the process whose next trace is being read
 * @return           - a reference to a memref struct
 */

page_t* read_next(process_t* process);


/**
 * findAllProcesses does one pass of the trace file and returns an array of processes
 * that are found in the file
 *
 * @param traceFile - a pointer to the traceFile being read
 * @param fpath     - the file path trace_file
 * @return          - an array of processes
 */
process_t** find_all_processes(char* fpath, unsigned long int* num_mem_refs);

#endif
