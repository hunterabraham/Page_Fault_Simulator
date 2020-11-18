#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "input.h"
#include "process.h"



int contains(process_info** pinfo_list, int pid) {
	int i = 0;
	while(pinfo_list != NULL) {
		if (pinfo_list[i]->pid == pid) {
			return 1;
		}
	}
	return 0;
}

int* split(char* str) {
	size_t splitIdx;
	for (size_t i = 0; i < strlen(buffer); ++i) {
		if (isspace(buffer[i])) {
			splitIdx = i;
			break;
		}
	}
	char* vaddrStr = (buffer + splitIdx + 1);
	*(vaddrStr + strlen(vaddrStr)) = '\0';
	*(buffer + splitIdx) = '\0';
	long int vaddr = atoi(vaddrStr);
	long int pidSave = atoi(buffer);
	int* retArr = malloc(sizeof(long int) * 2);
	retArr[0] = vaddr;
	retArr[1] = pidSave;
	return retArr;
}



//TODO: left off here
process_info** findAllProcesses(FILE* traceFile) {
	const int BUFSIZE = 10000;
	process_info** pinfo_list = malloc(sizeof(process_info) * BUFSIZE);
	char* buffer = malloc(sizeof(char) * BUFSIZE);
	if (NULL == buffer) {
		fprintf(stderr, "Error allocating buffer in findAllProcesses() in main_fifo.c\n");
		exit(1);
	}
	int j = 0;
	while(NULL != fgets(buffer, BUFSIZE, tracefile)) {
		int* retArr = split(buffer);
		int pid = retArr[1];
		if (!contains(pinfo_list, pid)) {
			pinfo_list[j]->pid = pid;
			pinfo_list[j]->
		}

	}

}



memref* readNext(FILE* traceFile) {
	const int BUFSIZE = 10000;
	char* buffer = malloc(sizeof(char) * BUFSIZE);
	if (NULL == buffer) {
		fprintf(stderr, "Error allocating buffer in readNext in input.c\n");
		exit(1);
	}
	fgets(buffer, BUFSIZE, traceFile);
	if (NULL == buffer) {
		return NULL;
	}
	size_t splitIdx;
	for (size_t i = 0; i < strlen(buffer); ++i) {
		if (isspace(buffer[i])) {
			splitIdx = i;
			break;
		}
	}
	char* vaddrStr = (buffer + splitIdx + 1);
	*(vaddrStr + strlen(vaddrStr)) = '\0';
	*(buffer + splitIdx) = '\0';
	long int vaddr = atoi(vaddrStr);
	long int pidSave = atoi(buffer);
//	FILE* outFile = fopen("input_TEST.txt", "a");// FIXME
//	if (NULL == outFile) {
//		fprintf(stderr, "Failed to write to test file in readNext() in input.c\n");
//		exit(1);
//	}
	

	memref* ref = malloc(sizeof(memref));
	if (NULL == ref) {
		fprintf(stderr, "Error allocating memref in readNext() in input.c\n");
		exit(1);
	}
	ref->pid = pidSave;
	ref->page_no = vaddr;
	return ref;



//	fprintf(outFile, "%ld %ld\n", pidSave, vaddr);
//	fclose(outFile);
}




