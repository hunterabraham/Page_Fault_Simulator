all: main_fifo.o input.o statistics.o cmd_line_processing.o process.o process_queue.o disk.o page_table.o fifo.o
	gcc -o 537pfsim-fifo main_fifo.o input.o cmd_line_processing.o process.o statistics.o process_queue.o disk.o page_table.o fifo.o
	echo all built successfully!

process_queue.o: process_queue.h process_queue.c process.o
	gcc -Wextra -Wall -ggdb -c process_queue.c

#inverted_page_table.o: inverted_page_table.c inverted_page_table.h
#	gcc -Wextra -Wall -ggdb -c inverted_page_table.c

fifo.o: fifo.c fifo.h
	gcc -Wall -Wextra -ggdb -c fifo.c

input.o: input.c input.h
	gcc -Wall -Wextra -ggdb -c input.c

main_fifo.o: main_fifo.c input.c input.h cmd_line_processing.c cmd_line_processing.h
	gcc -Wall -Wextra -ggdb -c main_fifo.c

statistics.o: statistics.c statistics.h
	gcc -Wall -Wextra -ggdb -c statistics.c

cmd_line_processing.o: cmd_line_processing.c cmd_line_processing.h
	gcc -Wall -Wextra -ggdb -c cmd_line_processing.c

page_table.o: page_table.c page_table.h
	gcc -Wall -Wextra -ggdb -c page_table.c


disk.o: disk.c disk.h
	gcc -Wall -Wextra -ggdb -c disk.c

process.o: process.c process.h
	gcc -Wall -Wextra -ggdb -c process.c

clean:
	-rm 537pfsim-fifo
	-rm *.o

test.o: test.c 
	gcc -Wall -Wextra -ggdb -c test.c

test: test.o input.o statistics.o cmd_line_processing.o process.o process_queue.o page_table.o disk.o fifo.o
	gcc -o test_suite test.o input.o cmd_line_processing.o process.o statistics.o process_queue.o page_table.o disk.o fifo.o
