#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 
#include <time.h>

#include "processGenerator.h"
#include "globals.h"


void *processGenerator(void *pmt){
	struct parametroak pm = *(struct parametroak*) pmt;
	int proz_kop = pm.proz_kop;
	int proz_t = pm.proz_t;
	int i = 0;
	time_t t;

	struct processQueue pq;
	pq.ilara = malloc(proz_kop*sizeof(struct pcb));
	
	srand((unsigned) time(&t));
	while(1){
		int r = rand() % proz_t;
		if(r != 0){
			sleep(r);
			generateProcess(i, pq);
			printf("Idatzi da, %d zenbakia\n", pq.ilara[i].pid);
			i++;
		}
	}
}

void generateProcess(int i, struct processQueue queue){
	struct pcb prozesua;
	prozesua.pid = i;
	queue.ilara[i] = prozesua;
}