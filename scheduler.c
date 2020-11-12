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

        			// ------Hemen mutex-----

		        	if (cpu_core[j].ilara == NULL) { 
				       cpu_core[j].ilara = new; 
				    } else{
				    	struct Node_pcb *last = cpu_core[j].ilara;
					    while (last->next != NULL) 
					        last = last->next; 
		    			//printf("CPU: %d, CORE: %d, lastID: %d\n", i, j, last->data.pid);
					    last->next = new; 
					}
					// -------Honaino mutex -----

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
			ctP[id].core_p = &cpu_s[i].coreak[j];
			err = pthread_create(&core_hariak[id], NULL, core_haria, (void *) &ctP[id++]);
			if(err > 0){
            	fprintf(stderr, "Errore bat gertatu da hariak sortzean.\n");
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

		// Itxaron corearen ilaran prozesuren bat egon arte
		while(core_p->ilara == NULL);
		struct Node_pcb *core_ilara = core_p->ilara;

		struct pcb *pcb_exek = &core_ilara->data;
		pcb_exek->egoera = 1; // Egoera: exekutatzen
		printf("%d corea %d exekutatzen...\n", zenb, pcb_exek->pid);
		int q = pcb_exek->quantum;
		sleep(q);
		pcb_exek->egoera = 0; // Egoera: itxaroten
		printf("%d corea %d exekuzioa amaituta.\n", zenb, pcb_exek->pid);

		// -----Hemen mutex-----
		// Core_ilararen amaieran sartu
		if (core_p->ilara == NULL) { 
	       core_p->ilara = core_ilara; 
	    } else{
	    	struct Node_pcb *last = core_p->ilara;
		    while (last->next != NULL) 
		        last = last->next; 
		    last->next = core_ilara; 
		    core_p->ilara = core_p->ilara->next;
		    printf("Next eginda\n");
		    // Lehenengoa borratzeko ?
		}
		// ------Honaino mutex---
	}



	/*while(1){
		if(core_ilara != NULL){
			printf("Core: %d node data: %d\n", zenb, core_ilara->data.pid);
			if(core_ilara->next != NULL)
				core_ilara = core_ilara->next;
		}
	}*/
	/*while(1){
		if(core_ilara != NULL){
			printf("Core node data: %d\n",core_ilara->data.pid);
			printf("Nire corearen (%d) prozesuak: %d\n", zenb, core_ilara->data.pid);
			core_ilara = core_ilara->next;
		}
	}*/

	//printf("%d Haria naiz\n", zenb);
	
	
	/* Hemendik aurrerako lana:
		- Hemen, hari bakoitzak bere ilara izango du, bere prozesu ilara.
		- Scheduler nagusiak -> ProcessQueue-ko prozesuak esleitu banaka-banaka ->
			-> hari hauetako bakoitzak timer-aren aginduz, bere ilarako prozesua hartu eta quantum bat itxaron, eta bueltan gure ilarara sartu
		- Noizean behin, ilarak orekatu: proz hutsak sartu berdintzeko
	*/
}