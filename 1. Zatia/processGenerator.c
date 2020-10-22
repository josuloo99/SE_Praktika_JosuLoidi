#include "processGenerator.h"

struct processQueue pqueue;

void *processGenerator(void *pmt){
	struct parametroak pm = *(struct parametroak*) pmt;
	int proz_kop = pm.proz_kop;
	int proz_t = pm.proz_t;
	int i = 0;
	time_t t;

	pqueue.ilara = malloc(proz_kop*sizeof(struct pcb));
	
	srand((unsigned) time(&t));
	while(1){
		int r = rand() % proz_t;
		if(r != 0){
			sleep(r);
			generateProcess(i, pqueue);
			printf("Idatzi da, %d zenbakia\n", pqueue.ilara[i].pid);
			i++;
		}
	}
}

void generateProcess(int i, struct processQueue queue){
	struct pcb prozesua;
	prozesua.pid = i;
	queue.ilara[i] = prozesua;
}