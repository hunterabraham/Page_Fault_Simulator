#include <stdio.h>
#include <stdlib.h>
#include "input.h"
#include "cmd_line_processing.h"
#include "fifo_queue.h"
#include "statistics.h"
int main(int argc, char** argv) {
	stats_t* stats = (stats_t*)malloc(sizeof(stats));
	cmd_args* flags = processArgs(argc, argv);
	FILE* fptr = fopen("test/4000.addrtrace", "r");
	if (NULL == fptr) {
		fprintf(stderr, "Error opening file \"%s\"", "test/4000.addrtrace");
		exit(1);
	}
	memref* ref1;
	long int BUFSIZE = flags->real_mem_size / flags->page_size;
	Queue* fifo_queue = CreateRefQueue(BUFSIZE);
	int* clock = malloc(sizeof(int));
	if (NULL == clock) {
		fprintf(stderr, "Error allocating clock in main() in main_fifo.c\n");
		exit(1);
	}
	*clock = 0;

	for (int i = 0; i < 4000; i++) {
		ref1 = readNext(fptr);
		int ret = EnqueueRef(fifo_queue, ref1, stats, clock);
		//fprintf(stderr, "%ld %ld\n", ref1->pid, ref1->page_no);
	}
	fclose(fptr);
	fprintf(stderr, "%d\n", *clock);

}
