#include "clock.h"

void *clock_s(void *m){
	//int core = cpu.core_kop;
	int i, j, k = 0;

	while(1){
		pthread_mutex_lock(&mutex_tic);
		tick_kop++;
		pthread_mutex_unlock(&mutex_tic);

		for (i = 0; i < pm.cpu_kop; i++){
			struct core *core_s = cpu_s[i].coreak;
			for(j = 0; j < pm.core_kop; j++){
				struct Node_pcb *pcb = core_s[j].ilara;
				while (pcb != NULL){
					if(pcb->data.egoera == 1){
						pcb->data.denbora++;
					}
					pcb = pcb->next;
				}
			}
		}
	}
}