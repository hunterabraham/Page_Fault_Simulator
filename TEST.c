#include <stdlib.h>
#include <stdio.h>
#include "page_table.h"
#include "memref.h"
#include "page.h"
#include "disk.h"
#include "process.h"


int test_disk_advanced() {
	fprintf(stderr, "TESTING test_disk_advanced\n");
	disk* disk_t = create_disk(1000);
	memref ref;
	ref.page_no =1;
	ref.pid = 1;
	process proc;
	proc.time_when_ready = 0;
	int clock= 0;
	ref.proc = &proc;
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);
	add_ref_disk(disk_t, &ref, clock);

	if (disk_t->curr_size != 1) {
		fprintf(stderr, "Disk did not handle duplicates correctly.\n");
		return 1;
	}
	for (int i = 0; i < 619; i++) {
		memref* ref1 = malloc(sizeof(memref));
		ref1->page_no = i * 1000;
		ref1->pid = i * 1000;
		ref1->proc = &proc;
		add_ref_disk(disk_t, ref1, clock);

	}
	


	if (disk_t->curr_size != 620) {
		fprintf(stderr, "Disk does not have correct size. expected 620 but got %d\n", disk_t->curr_size);
	}

	for (int i = 0; i < 10; i++) {
		memref* ref1 = malloc(sizeof(memref));
		ref1->page_no = (i + 1) * 2;
		ref1->pid = (i + 1) * 2;
		ref1->proc = &proc;
		add_ref_disk(disk_t, ref1, clock);
	}
	if (disk_t->curr_size != 630) {
		fprintf(stderr, "Disk does not have correct size. expected 630 but got %d\n", disk_t->curr_size);
	}

	for (int i = 0; i < 200; i++) {
		remove_ref_disk(disk_t);
	}
	if (disk_t->curr_size != 430) {
		fprintf(stderr, "Disk does not have correct size after remove. expected 430 but got %d\n", disk_t->curr_size);

	}
}





int test_page_table_simple() {
	fprintf(stderr, "TESTING page_table_simple\n");
	ptable* table = create_page_table(100);	
	memref ref1;
	ref1.page_no = 1000;
	ref1.pid = 0;
	memref ref2;
	ref2.page_no = 2000;
	ref2.pid = 0;
	add_page(table, &ref1);
	add_page(table, &ref1);
	add_page(table, &ref2);
	remove_page(table, 1);
	if (table->curr_size != 2) {
		fprintf(stderr, "FAIL simple: page table size is not 2. Instead is %d\n", table->curr_size);
		return -1;
	}
	if (table->page_array[0]->page_no != 1000) {
		fprintf(stderr, "FAIL simple: page number of 0th element supposed to be 1000, instead is: %d\n", table->page_array[0]->page_no);
		return -1;
	}
	if (table->page_array[1]->page_no != 2000) {
		fprintf(stderr, "FAIL: page number of 1st element supposed to be 2000, instead is: %d\n", table->page_array[1]->page_no);
		return -1;
	}
	free(table);

	return 1;		
}

int test_page_table_advanced() {
	fprintf(stderr, "TESTING page_table_advanced\n");
	ptable* table = create_page_table(100);	
	memref ref1;
	ref1.page_no = 1000;
	ref1.pid = 0;
	memref ref2;
	ref2.page_no = 2000;
	ref2.pid = 0;
	memref ref3;
	ref3.page_no = 3000;
	ref3.pid = 0;
	memref ref4;
	ref4.page_no = 4000;
	ref4.pid = 0;
	add_page(table, &ref1);
	remove_page(table, 0);
	add_page(table, &ref1);
	add_page(table, &ref1);
	remove_page(table, 1); // [1000, 3000, ...
	add_page(table, &ref3);
	add_page(table, &ref4);
	add_page(table, &ref3);
	add_page(table, &ref2);
	add_page(table, &ref2);// [1, 3, 4, 3, 2, 2,
	remove_page(table, 3);
	add_page(table, &ref1);
	add_page(table, &ref1);
	add_page(table, &ref1);
	add_page(table, &ref3);
	add_page(table, &ref4);
	add_page(table, &ref3);
	add_page(table, &ref2);
	add_page(table, &ref2);
	add_page(table, &ref1);
	add_page(table, &ref2);
	int expected[30] = {1000, 3000, 4000, 2000, 2000, 1000, 1000, 1000, 3000, 4000, 3000, 2000, 2000, 1000, 2000};	
	int expected_search[30] = {0, 1, 2, 3, 3, 0, 0, 0, 1, 2, 1, 3, 3, 0, 3};
	int k = 0;
	while(table->page_array[k] != NULL) {
		if (table->page_array[k]->page_no != expected[k]) {
			fprintf(stderr, "FAIL: at index %d the value %d was expected, instead got %d\n", k, expected[k], table->page_array[k]->page_no);
		}
		int j;
		if ((j = search_page(table, expected[k])) != expected_search[k]) {
			fprintf(stderr, "FAIL: element %d, %d found at index %d instead of %d\n", k, expected[k], j, expected_search[k]);

		}
		k++;
	}
	
	ptable* table2 = create_page_table(10000);	
	for (int i = 0; i < 237; i++) {
		add_page(table2, &ref1);
		add_page(table2, &ref2);
		remove_page(table2, 0);
	}
	int i = 0;
	while(table2->page_array[i] != NULL) {
		fprintf(stderr, "%d, ", table2->page_array[i]->page_no);
		i++;


	}


	if (k != 15) {
		fprintf(stderr, "FAIL: k supposed to be 15 but instead is %d\n", k);
		return -1;

	}
	return 1;		

}


int test_page_table_search() {
	fprintf(stderr, "TESTING page_table_search\n");
	ptable* table = create_page_table(100);	
	memref ref1;
	ref1.page_no = 1000;
	memref ref2;
	ref2.page_no = 2000;
	add_page(table, &ref1);
	add_page(table, &ref1);
	add_page(table, &ref2);
	remove_page(table, 2);
	if (search_page(table, 1000) != 0) {
		fprintf(stderr, "ref1 not at index 0\n");
	
	}
	if (search_page(table, 2000) != -1) {
		fprintf(stderr, "ref2 said to exist\n");
	}
	return 1;		

}

int test_proc_waiting() {
	fprintf(stderr, "TESTING test_proc_waiting\n");
	process proc;

	proc.time_when_ready = 1000;
	
	if (isWaiting(&proc, 1000)) {
		fprintf(stderr, "FAIL test_proc_waiting: proc waiting.\n");
	}
	if (!isWaiting(&proc, 999)) {
		fprintf(stderr, "FAIL test_proc_waiting: proc not waiting.\n");
	}



}



int main() {
	test_page_table_simple();
	test_page_table_advanced();
	test_page_table_search();
	test_proc_waiting();
	test_disk_advanced();
}
