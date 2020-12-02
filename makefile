all: 537pfsim-fifo 537pfsim-lru 537pfsim-clock

537pfsim-fifo: main.o fifo.o process_queue.o process.o input.o statistics.o cmd_line_processing.o page_table.o disk.o
	gcc -ggdb -o 537pfsim-fifo main.o fifo.o process_queue.o process.o input.o statistics.o cmd_line_processing.o page_table.o disk.o

537pfsim-lru: main.o lru.o process_queue.o process.o input.o statistics.o cmd_line_processing.o page_table.o disk.o
	gcc -ggdb -o 537pfsim-lru main.o lru.o process_queue.o process.o input.o statistics.o cmd_line_processing.o page_table.o disk.o

537pfsim-clock: main.o clock.o process_queue.o process.o input.o statistics.o cmd_line_processing.o page_table.o disk.o
	gcc -ggdb -o 537pfsim-clock main.o clock.o process_queue.o process.o input.o statistics.o cmd_line_processing.o page_table.o disk.o

clock.o: clock.c page_replacement_interface.h
	gcc -c -Wall -Wextra -ggdb clock.c

fifo.o: fifo.c page_replacement_interface.h
	gcc -c -Wall -Wextra -ggdb fifo.c

lru.o: lru.c page_replacement_interface.h
	gcc -c -Wall -Wextra -ggdb lru.c

main.o: main.c page_replacement_interface.h
	gcc -c -Wall -Wextra -ggdb main.c

process_queue.o: process_queue.h process_queue.c process.o
	gcc -Wextra -Wall -ggdb -c process_queue.c

input.o: input.c input.h
	gcc -Wall -Wextra -ggdb -c input.c

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
	-rm 537pfsim-lru
	-rm 537pfsim-clock
	-rm 537pfsim-fifo
	-rm *.o

