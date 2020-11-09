#include "processGenerator.h"
struct Node_pcb* linkedQueue;

void *processGenerator(void *pmt){
	struct parametroak pm = *(struct parametroak*) pmt;
	int proz_kop = pm.proz_kop;
	int proz_t = pm.proz_t;
	int i = 0;
	time_t t;

	/*pqueue.ilara = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
	pqueue.actual = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
	pqueue.actual = pqueue.ilara;*/

	//linkedQueue = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
	//linkedQueue->next = NULL;
	struct Node_pcb* aux;// = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
	linkedQueue = NULL;
	aux = linkedQueue;
	
	srand((unsigned) time(&t));
	while(1){
		int r = rand() % proz_t;
		//printf("Rand zenbakia: %d Proz_timer: %d\n", r, proz_t);
		if(r != 0){
			sleep(r);

			struct pcb prozesua;
			prozesua.pid = i;
			// gehitu hemen prozesuari egoera, quantum...

			aux = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
			aux->data = prozesua;
			struct Node_pcb* aux2;
			aux2 = NULL;
			aux->next = aux2;
			
			//generateProcess(i);
			printf("Idatzi da, %d zenbakia\n", aux->data.pid);
			aux = aux2;
			i++;
		}
	}
}

/*void generateProcess(int i){
	struct pcb prozesua;
	prozesua.pid = i;

	struct Node_pcb *node = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
	node->data = prozesua;

	pqueue.actual->next = node;
	pqueue.actual = node;
}*/