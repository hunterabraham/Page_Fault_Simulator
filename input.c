#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "input.h"
#include "process.h"
#include "memref_queue.h"


int contains(process** pinfo_list, int pid) {
	int i = 0;
	if (NULL == pinfo_list) {
		fprintf(stderr, "pinfo_list not initialized when running contains().\n");
		exit(1);
	}
	while(pinfo_list[i] != NULL) {
		if (pinfo_list[i]->pid == pid) {
			return i;
		}
		i++;
	}
	return -1;
}

int* split(char* buffer) {
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
	if (NULL == retArr) {
		fprintf(stderr, "Error allocating retArr in split() in input.c\n");
		exit(1);
	}
	retArr[0] = vaddr;
	retArr[1] = pidSave;
	return retArr;
}


/**
 	int pid;
	int num_pages;
	int time_when_ready;
	int first_ref;
	int last_ref;
*/
//TODO: left off here
process** findAllProcesses(FILE* traceFile) {
	const int BUFSIZE = 10000;
	process** pinfo_list = malloc(sizeof(process) * BUFSIZE);
	if (NULL == pinfo_list) {
		fprintf(stderr, "Error allocating pinfo_list in findAllProcesses() in input.c\n");
		exit(1);
	}
	char* buffer = malloc(sizeof(char) * BUFSIZE);
	if (NULL == buffer) {
		fprintf(stderr, "Error allocating buffer in findAllProcesses() in input.c\n");
		exit(1);
	}
	int j = 0;
	int row = 0;
	while(NULL != fgets(buffer, BUFSIZE, traceFile)) {
		int* retArr = split(buffer);
		int pid = retArr[1];
		int idx = contains(pinfo_list, pid); // see if process is in list
		if (idx == -1) { // if it isn't, make a new one and add it to the list
			process* new_proc = malloc(sizeof(process));
			if (NULL == new_proc) {
				fprintf(stderr, "Error allocating new_proc at index %d in findAllProcesses() in input.c\n", j);
				exit(1);
			}
			new_proc->pid = pid;
			new_proc->time_when_ready = 0;
			new_proc->first_ref = row;
			new_proc->last_ref = row;
			pinfo_list[j] = new_proc;
			j++;
		} else {
			pinfo_list[idx]->last_ref = row;
		}
		row++;
		free(retArr);
	}
	free(buffer);
	return pinfo_list;
}



memref* readNext(FILE* traceFile, process** proc_list) {
	const int BUFSIZE = 10000;
	char* buffer = malloc(sizeof(char) * BUFSIZE);
	if (NULL == buffer) {
		fprintf(stderr, "Error allocating buffer in readNext in input.c\n");
		exit(1);
	}
	char* ret = fgets(buffer, BUFSIZE, traceFile);
	if (!ret) {
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
	process* proc;
	int j = 0;
	while((proc = proc_list[j]) != NULL) {
		if (proc->pid == pidSave) {
			ref->proc = proc;
			break;
		}
		j++;
	}

	return ref;



//	fprintf(outFile, "%ld %ld\n", pidSave, vaddr);
//	fclose(outFile);
}




