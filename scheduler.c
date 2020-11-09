#include "scheduler.h"

void *scheduler_t(void *m){
	struct Node_pcb *current_node = linkedQueue;
	initialize();
	while(1){
		sem_wait(&sch);

		/*printf("Scheduler!\n");
	    struct Node_pcb *current_node = n;
	    printf("Nodes: ");
	   	while ( current_node->next != NULL) {
	        printf("%d ", current_node->data.pid);
	        current_node = current_node->next;
	    }
	    printf("\n");*/

	    //Prozesuak coreetako harietan sartu eta originaletik kendu
	    int i,j;
	    //struct Node_pcb *current_node = n;
	   	while ( current_node != NULL) {

		    for (i = 0; i < pm.cpu_kop; i++){
		    	struct core *cpu_core = cpu_s[i].coreak;

		    	for (j = 0; j < pm.core_kop; j++){
		    		struct Node_pcb *last = cpu_core[j].ilara;

		    		while (last->next != NULL) 
        				last = last->next;

        			struct Node_pcb *new = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
        			new->data = current_node->data;
        			new->next = NULL;
        			last = new;
        			last = last->next;

		    		current_node = current_node->next;
		    	}
		    }

		}

	}
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
	struct core_thread_parameters *ctP_h = (struct core_thread_parameters *)param;
	int zenb = ctP_h->id;
	struct core core_p = *ctP_h->core_p;	

	//printf("%d Haria naiz\n", zenb);
	
	
	/* Hemendik aurrerako lana:
		- Hemen, hari bakoitzak bere ilara izango du, bere prozesu ilara.
		- Scheduler nagusiak -> ProcessQueue-ko prozesuak esleitu banaka-banaka ->
			-> hari hauetako bakoitzak timer-aren aginduz, bere ilarako prozesua hartu eta quantum bat itxaron, eta bueltan gure ilarara sartu
		- Noizean behin, ilarak orekatu: proz hutsak sartu berdintzeko
	*/
}