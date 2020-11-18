# 537pfsim

## Page fault simulator using various page replacement algorithms

## Modules

- *cmd_line_processing.h:* 
	- *Summary:* Parses command line arguments to construct page table
	- *Status:* Completed
	- *Files:* cmd\_line\_processing.h, cmd\_line\_processing.c

- *input*
	- *Summary:* Parses input file to construct process queue and get next memory reference
	- *Status:* INP
	- *Files:* input.c, input.h

- *memref*
	- *Summary:* Stores a memory reference
	- *Status:* Completed
	- *Files:* memref.h

## Features

- User can specify which target they would like to execute

- **-f: <fname>** Allows user to specify the name of the makefile

## Checklist


- [x] A target line always begins in the first column (i.e., starts on the first character of a line).
- [x] The target (which is is usually the name of a file being created by one of the commands in the build specification) is followed by a ":" character and then a list of dependence names, each separated by white space (any number of spaces or tabs).
- [x] A dependence is either the name of another target or the name of a file. It is an error if there is no file or target that matches the dependence.
- [x] If there is a cycle in the chain in any of the dependences, then that is an error.
- [x] A command line always starts with a tab character (not spaces).
- [x] The list of commands for a target ends either when a new target starts or at the end of the files.
- [x] Blank lines are ignored and can appear in the middle of a list of commands. Blank means an empty line or one with only white space.
- [x] A line that begins with a "#" as the first character on the line is a comment line. The rest of the line is ignored. Comment lines are ignored and can appear in the middle of a list of commands. Note that if a comment line is too long, it is reported as an error.
- [x] Lines that contain a null (zero) byte within the line are invalid and reported as an error.
- [x] Non-blank lines must begin with a target name or tab; otherwise they are invalid and reported as an error.
- [x] Any command that exits with an error (a non-zero completion code) terminates the make process.
- [x] When you find an error while parsing the makefile, you will print an error message to stderr. This error message will look like:
- [x] 10: <error message>: "blah blah blah" where "<error message>" is an informative error message and "10" is the line number in the makefile of the bad line and "blah blah blah" is the contents of the bad line. After printing the error message, your program will exit.
- [x] When you find an error while executing lines in your makefile, such as file not found, you will print an error message to stderr.
- [x] On any error, after printing the error message, your program should exit.
- [x] Set your maximum line length to 4K (4096), including terminating null byte.


