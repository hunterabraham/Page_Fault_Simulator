# this is a comment



all: main_fifo.o input.o cmd_line_processing.o fifo_queue.o
	gcc -o 537pfsim-fifo main_fifo.o input.o cmd_line_processing.o fifo_queue.o
	echo all built successfully!

main_fifo.o: main_fifo.c input.c input.h cmd_line_processing.c cmd_line_processing.h
	gcc -Wall -Wextra -ggdb -c main_fifo.c


input.o: input.c input.h
	gcc -Wall -Wextra -ggdb -c input.c

cmd_line_processing.o: cmd_line_processing.c cmd_line_processing.h
	gcc -Wall -Wextra -ggdb -c cmd_line_processing.c

fifo_queue.o: fifo_queue.c fifo_queue.h input.o
	gcc -Wall -Wextra -ggdb -c fifo_queue.c

clean:
	-rm 537pfsim-fifo
	-rm *.o
