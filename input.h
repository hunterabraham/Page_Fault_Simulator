#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>

typedef struct memref {
	long int pid;
	long int page_no;
} memref;



/**
 * readNext() reads the next line in a trace file into memory and converts
 * it to a memref struct with the pid and page number
 *
 * @param traceFile - the traceFile being read
 * @return          - a reference to a memref struct
 */
memref* readNext(FILE* traceFile);


#endif
