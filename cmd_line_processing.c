#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "cmd_line_processing.h"




cmd_args* process_args(int argc, char** argv) {
	// create instance of struct to store flags and filenames	
	cmd_args* argFlags = malloc(sizeof(cmd_args));
	if (NULL == argFlags) {
		fprintf(stderr, "Error allocating argFlags in processArgs() in cmd_line_processing.c\n");
	}
	int option;
	argFlags->page_size = 4096;
	argFlags->real_mem_size = 4194304;
	while((option = getopt(argc, argv, "m:p:")) != -1) {
		switch(option) {
			case 'm': // if f is specified, store filename
				argFlags->real_mem_size = atoi(optarg) * 1048576;
				break;
			case 'p':
				argFlags->page_size = atoi(optarg);
			case '?': 
				break;
			case ':':
				printf("Argument required.\n");
				break;
		}
	}

	for (int i = 1; i < argc; i++) {
		if ((strcmp("-m", argv[i])) && (strcmp("-p", argv[i])) && ((unsigned long int)(atoi(argv[i]))) != argFlags->page_size
				&& (((unsigned long int)atoi(argv[i])) != argFlags->real_mem_size / 1048576) && (strcmp("537pfsim-fifo", argv[i]))) {
			argFlags->file_name = argv[i];
			break;
		}
	}
	return argFlags;
}

