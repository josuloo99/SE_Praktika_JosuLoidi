#include "scheduler.h"

void *scheduler_t(void *m){
	
	initialize();
	while(1){
		sem_wait(&sch);
		addToCores();
	}
}

void addToCores(){
		struct Node_pcb *current_node = linkedQueue;

	    //Prozesuak coreetako harietan sartu eta originaletik kendu
	    int i,j;
	   	while ( current_node != NULL) {

		    for (i = 0; i < pm.cpu_kop; i++){
		    	struct core *cpu_core = cpu_s[i].coreak;

		    	for (j = 0; j < pm.core_kop; j++){
		    		
        			struct Node_pcb *new = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
        			new->data = current_node->data;
        			new->next = NULL;

        			pthread_mutex_lock(&cpu_core->mutex_ilara);

		        	if (cpu_core[j].ilara == NULL) { 
				       cpu_core[j].ilara = new; 
				    } else{
				    	struct Node_pcb *last = cpu_core[j].ilara;
					    while (last->next != NULL) 
					        last = last->next; 
					    last->next = new; 
					}
					pthread_mutex_unlock(&cpu_core->mutex_ilara);

					if(current_node->next == NULL){
						linkedQueue = NULL;
						return;
					} else{
		    			current_node = current_node->next;
		    		}
		    	}
		    }

		}
		//Suposatuz, beti kabituko direla prozesu guztiak coreetako harietan
		linkedQueue = NULL;
}

static int *th_queue_kop;

void initialize(){
	int cpuK = pm.cpu_kop;
	int core = pm.core_kop;
	//int h = pm.h_kop;

	int core_hari_kop = cpuK * core;

	th_queue_kop = malloc(core * sizeof(int));
	pthread_t *core_hariak;

	core_hariak = malloc(core_hari_kop * sizeof(pthread_t));

	int i = 0;
	int j = 0;
	int id = 0;
	int err;

	struct core_thread_parameters *ctP;
	ctP = malloc(core_hari_kop*sizeof(struct core_thread_parameters));
	
	for (i = 0; i < cpuK; i++){
		for (j = 0; j < core; j++){
			ctP[id].id = id;
			ctP[id].cpu_id = i;
			ctP[id].core_p = &cpu_s[i].coreak[j];
			err = pthread_create(&core_hariak[id], NULL, core_haria, (void *) &ctP[id++]);
			if(err > 0){
            	fprintf(stderr, "Errore bat gertatu da core hariak sortzean.\n");
            	exit(1);
	    	}
		}
	}
}

void *core_haria(void *param){
	// Core honen parametroak
	struct core_thread_parameters *ctP_h = (struct core_thread_parameters *)param;
	// Core honen ID
	int zenb = ctP_h->id;
	// Core honen core objektua
	struct core *core_p = ctP_h->core_p;

	while(1){
		struct h *h = core_p->hariak;

		if(h->prozesua == NULL){
			if(core_p->ilara != NULL){
				struct Node_pcb *ilara_pr = core_p->ilara; // Hartu ilarako lehenengo prozesua
				if(ilara_pr->data.egoera == 0){ // Prozesua wait egoeran baldin badago
					ilara_pr->data.egoera = 1; // Prozesua execute egoeran jarri			

					h->prozesua = ilara_pr; // Hariari prozesua duen nodoa esleitu
					// 3.Praktika, 1. zatia
					/*
					h->PTBR = ilara_pr->data.pMemoria->pgb; (Orri taularen helbide fisikoa adierazten du)
					- Prozesuak agindua exekutatu
					- Lehen agindua hartu .text-etik (helbide birtuala -> orri zenbakia + desplazamendua)
					- Helbide fisikoa lortzeko:
						- Marko (frame) zenbakia behar da -> orri taula atzitu behar du
						- MMUk PTBR eta orri zenbakia erabiliz frame zenbakia lortzen du orri taulatik
						- Helbide fisikoa daukagu. Orain memoriatik hartu behar da tokatzen dena.
					- Frame zenbakia eta desplazamenduarekin memoriatik hartu
					- Exekutatu
					*/
					//printf("Harian %d prozesua sartu da\n", h->prozesua->data.pid);
				}
			}
		} else{
			if(h->prozesua->data.denbora > h->prozesua->data.quantum){ // Quantuma pasa baldin bada
				int ziklo = ++h->prozesua->data.kont; // Ziklo bat gehiago kontatu
				//printf("Harian %d prozesuko quamtuma amaitu da. Ziklo %d/%d-tik \n", h->prozesua->data.pid, ziklo, h->prozesua->data.kop);
				if(ziklo == h->prozesua->data.kop){ // Ziklo kopurua pasa badu
					h->prozesua->data.egoera = 2; // Amaierako egoera
					//printf("Harian %d prozesua amaitu da, %d ziklo\n", h->prozesua->data.pid, ziklo);
					pthread_mutex_lock(&core_p->mutex_ilara);
					core_p->ilara = h->prozesua->next; // Ilaran hurrengora pasa, aurrekoa atzean utzita
					pthread_mutex_unlock(&core_p->mutex_ilara);
				}
				else{
					h->prozesua->data.egoera = 0; // Wait egoeran jarri
					pthread_mutex_lock(&core_p->mutex_ilara);

					// Coreko ilararen amaieran sartu amaitutako prozesua
					struct Node_pcb *last = core_p->ilara;
				    while (last->next != NULL) {
				        last = last->next;
				    }
				    last->next = h->prozesua;
				    core_p->ilara = h->prozesua->next;
				    h->prozesua->next = NULL; //Azken prozesua izango denez next = NULL
					pthread_mutex_unlock(&core_p->mutex_ilara);

				}
				h->prozesua->data.denbora = 0;
				h->prozesua = NULL;
			}
		}
	}
}