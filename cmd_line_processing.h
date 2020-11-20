#ifndef CMD_H
#define CMD_H


// struct to hold parsed command line arguments
typedef struct cmd_args {
	long int page_size;
	long int real_mem_size;
	char* file_name;
} cmd_args;

/**
 * Method to parse command line arguments and return them as cmd_args
 *
 * @param argc - the argument count
 * @param argv - the argument vector containing all the arguments
 *
 * @return     - a cmd_args struct containing processed arguments
 */
cmd_args* processArgs(int argc, char** argv);





#endif
