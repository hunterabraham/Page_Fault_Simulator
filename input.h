#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include "memref.h"
#include "process.h"



/**
 * readNext() reads the next line in a trace file into memory and converts
 * it to a memref struct with the pid and page number
 *
 * @param traceFile - the traceFile being read
 * @return          - a reference to a memref struct
 */
memref* readNext(FILE* traceFile, process** proc_list);


/**
 * findAllProcesses does one pass of the trace file and returns an array of processes
 * that are found in the file
 *
 * @param traceFile - a pointer to the traceFile being read
 * @return          - an array of processes
 */
process** findAllProcesses(FILE* traceFile);

#endif
