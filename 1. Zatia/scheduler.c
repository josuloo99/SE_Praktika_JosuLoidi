#include "scheduler.h"

void *scheduler_t(void *m){
	// int indize = 0;
	// struct parametroak pm = *(struct parametroak*) m;
	// int proz_kop = pm.proz_kop;
	while(1){
		sem_wait(&sch);
		// struct pcb proz = pqueue.ilara[indize % proz_kop];
		// printf("Queue, indize: %d, num: %d\n", indize, proz.pid);
		// indize++;
		printf("Scheduler!\n");
	}
}