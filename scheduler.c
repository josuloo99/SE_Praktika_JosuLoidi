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
		    			//printf("CPU: %d, CORE: %d, lastID: %d\n", i, j, last->data.pid);
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
		//free(linkedQueue);
		//Suposatuz, beti kabituko direla prozesu guztiak coreetako harietan
		linkedQueue = NULL;
}

static int *th_queue_kop;

void initialize(){
	int cpuK = pm.cpu_kop;
	int core = pm.core_kop;
	int h = pm.h_kop;

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

	int a = 0;
	while(1){
		for(a = 0; a < pm.h_kop; a++){
			struct h *h = &core_p->hariak[a];
			//printf("Sartu naiz, haria %d\n", a);

			//while(core_p->ilara == NULL); // Itxaron corearen ilaran prozesuren bat egon arte
			struct Node_pcb *ilara_pr = core_p->ilara; // Hartu ilarako lehenengo prozesua

			if(h->prozesua == NULL){
				if(core_p->ilara != NULL){
					struct Node_pcb *ilara_pr = core_p->ilara; // Hartu ilarako lehenengo prozesua
					if(ilara_pr->data.egoera == 0){ // Prozesua wait egoeran baldin badago
						ilara_pr->data.egoera == 1; // Prozesua execute egoeran jarri
						printf("%d prozesuaren egoera %d\n", ilara_pr->data.pid, ilara_pr->data.egoera); // egoera 0!!


						//ilara_pr = ilara_pr->next; // Ilarako

						h->prozesua = &(ilara_pr->data); // Hariari prozesua esleitu
						printf("%d haria %d prozesuarekin.\n", a, h->prozesua->pid);
					}
				}
			} else{
				//printf("HEMEN denbora: %d, quantum: %d\n", h->prozesua->denbora, h->prozesua->quantum);
				if(h->prozesua->denbora > h->prozesua->quantum){ // Quantuma pasa baldin bada
					int ziklo = h->prozesua->kont++; // Ziklo bat gehiago kontatu
					printf("%d haria %d prozesuarekin ziklo %d \n", a, h->prozesua->pid, ziklo);
					if(ziklo == h->prozesua->kop){ // Ziklo kopurua pasa badu
						h->prozesua->egoera = 2; // Amaierako egoera
						printf("%d haria %d prozesuarekin zikloak amaituta \n", a, h->prozesua->pid);
						pthread_mutex_lock(&core_p->mutex_ilara);
						core_p->ilara = ilara_pr->next; // Ilaran hurrengora pasa, aurrekoa atzean utzita
						pthread_mutex_unlock(&core_p->mutex_ilara);
					}
					else{
						h->prozesua->egoera = 0; // Wait egoeran jarri
						pthread_mutex_lock(&core_p->mutex_ilara);
						// Coreko ilararen amaieran sartu amaitutako prozesua
						struct Node_pcb *last = ilara_pr;
					    while (last->next != NULL) {
					        last = last->next;
					    }
					    last->next = ilara_pr;
					    core_p->ilara = ilara_pr->next;
					    ilara_pr->next = NULL; //Azken prozesua izango denez next = NULL
					    //printf("Next eginda\n");
						pthread_mutex_unlock(&core_p->mutex_ilara);

					}
					h->prozesua->denbora = 0;
					h->prozesua = NULL;
				}
			}
		}
	}




	/*// Harware harien thread-ak sortu
	pthread_t *hardware_hariak;
	hardware_hariak = malloc(pm.h_kop * sizeof(pthread_t));

	struct hari_thread_parameters *htP;
	htP = malloc(pm.h_kop*sizeof(struct hari_thread_parameters));

	int i = 0;
	int err;
	for(i = 0; i < pm.h_kop; i++){
		htP[i].id = i;
		htP[i].ctP = ctP_h;
		err = pthread_create(&hardware_hariak[i], NULL, hardware_haria, (void *) &htP[i]);
		if(err > 0){
        	fprintf(stderr, "Errore bat gertatu da hardware hariak sortzean.\n");
        	exit(1);
	    }
	}
	
	for(i = 0; i < pm.h_kop; i++) // Ume guztiak amaitu arte itxaron
        pthread_join(hardware_hariak[i], NULL);*/
}

/*void *hardware_haria(void *param){
	struct hari_thread_parameters htP = *(struct hari_thread_parameters *)param;
	// Hari honen ID
	int zenbHari = htP.id;
	// Hari honen core objektua
	struct core *core_p = htP.ctP->core_p;
	int zenbCore = htP.ctP->id;
	int zenbCPU = htP.ctP->cpu_id;
	
	while(1){

		// Itxaron corearen ilaran prozesuren bat egon arte
		while(core_p->ilara == NULL);
		printf("BAI\n");

		pthread_mutex_lock(&core_p->mutex_ilara);
		struct Node_pcb *core_ilara = core_p->ilara;
		struct pcb *pcb_exek = &core_ilara->data;
		core_ilara = core_ilara->next;
		if(core_ilara == NULL)
			break;
		pthread_mutex_unlock(&core_p->mutex_ilara);
		
		if(pcb_exek->egoera == 0){ // Ilarako aukeratutako prozesua exekuzioan dago.
			pcb_exek->egoera = 1; // Egoera: exekutatzen
			printf("CPU: %d Corea: %d Haria: %d Prozesua: %d exekutatzen...\n", zenbCPU, zenbCore, zenbHari, pcb_exek->pid);
			
			while(pcb_exek->quantum > pcb_exek->denbora);
			pcb_exek->denbora = 0;

			pcb_exek->egoera = 0; // Egoera: itxaroten
			printf("CPU: %d Corea: %d Haria: %d Prozesua: %d amaitu da.\n", zenbCPU, zenbCore, zenbHari, pcb_exek->pid);


			pthread_mutex_lock(&core_p->mutex_ilara);
			// Core_ilararen amaieran sartu
			struct Node_pcb *last = core_ilara;
		    while (last->next != NULL) {
		        last = last->next;
		    }
		    last->next = core_ilara;
		    core_p->ilara = core_ilara->next;
		    core_ilara->next = NULL; //Azken prozesua izango denez next = NULL
		    //printf("Next eginda\n");
			pthread_mutex_unlock(&core_p->mutex_ilara);
		}
		else{
			core_p->ilara = core_ilara->next;
		}
	}
}*/