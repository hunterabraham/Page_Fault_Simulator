#include "process.h"


int isWaiting(process* proc, int clock) {
	if (proc->time_when_ready <= clock) {
		return 0;
	}
	return 1;
}
