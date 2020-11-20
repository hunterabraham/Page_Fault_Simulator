

all: main_fifo.o input.o cmd_line_processing.o statistics.o memref_queue.o page_table.o disk.o process.o page.o
	gcc -o 537pfsim-fifo main_fifo.o input.o cmd_line_processing.o memref_queue.o page_table.o disk.o process.o page.o statistics.o
	echo all built successfully!

main_fifo.o: main_fifo.c input.c input.h cmd_line_processing.c cmd_line_processing.h
	gcc -Wall -Wextra -ggdb -c main_fifo.c

statistics.o: statistics.c statistics.h
	gcc -Wall -Wextra -ggdb -c statistics.c

input.o: input.c input.h
	gcc -Wall -Wextra -ggdb -c input.c

cmd_line_processing.o: cmd_line_processing.c cmd_line_processing.h
	gcc -Wall -Wextra -ggdb -c cmd_line_processing.c

memref_queue.o: memref_queue.c memref_queue.h
	gcc -Wall -Wextra -ggdb -c memref_queue.c

page_table.o: page_table.c page_table.h page.h
	gcc -Wall -Wextra -ggdb -c page_table.c

disk.o: disk.c disk.h
	gcc -Wall -Wextra -ggdb -c disk.c

process.o: process.c process.h
	gcc -Wall -Wextra -ggdb -c process.c

page.o: page.c page.h
	gcc -Wall -Wextra -ggdb -c page.c

clean:
	-rm 537pfsim-fifo
	-rm *.o

test: all TEST.o
	gcc -o test_suite TEST.o page_table.o page.o process.o disk.o

TEST.o: TEST.c
	gcc -Wall -Wextra -ggdb -c TEST.c
