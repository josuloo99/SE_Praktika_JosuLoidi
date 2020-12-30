#include "processGenerator.h"
struct Node_pcb* linkedQueue;
void *processGenerator(void *pmt){
	struct parametroak pm = *(struct parametroak*) pmt;
	int proz_kop = pm.proz_kop;
	int proz_t = pm.proz_t;
	int i = 20;
	time_t t;

	linkedQueue = NULL;
	struct Node_pcb *aux = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
	
	srand((unsigned) time(&t));
	while(1){
		int r = rand() % proz_t;
		if(r != 0){
			sleep(r);
			struct Node_pcb *aux = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
			struct Node_pcb *last = linkedQueue;

			struct pcb prozesua;
			prozesua.pid = i; // pid esleitu
			prozesua.quantum = (rand() % 10000000) + 1; // Quantuma esleitu, random batekin
			prozesua.denbora = 0; // Zenbat denbora pasa duen exekuzioan, hasieran 0
			//prozesua.kop = (rand() % 5) + 1; // Prozesua amaitzeko beharko den ziklo kopuru osoa (1-5 ausazko zenbakia)
			//prozesua.kont = 0; // Egindako ziklo kopurua
			prozesua.martxan = -1;	// Prozesua ez dago exekuzioan

			struct mm * pMem = malloc(sizeof(struct mm));
			prozesua.pMemoria = pMem;

			kargatuPrograma(&prozesua);

			aux->data = prozesua;
			aux->next = NULL;

			// Prozesuen ilaran sartu prozesu berria
		    if (linkedQueue == NULL) 
		    { 
		       linkedQueue = aux; 
		    } else{
		    	
			    while (last->next != NULL) 
			        last = last->next; 
			  
			    last->next = aux; 
			}

			printf("Idatzi da, %d zenbakia\n", aux->data.pid);

			i++;
		}
	}
}

void kargatuPrograma(struct pcb * prozesua){
	
	// Emandako fitxategitik programa kargatu eta pgb, data (aldagaiak) eta text (kodea) helbideak esleitu
	char * fitxIzena;
	fitxIzena = "prog000.elf"; //PROBA
	irakurriFitx(prozesua, fitxIzena); 	// Hemen lortutakoa: prozesuaren lehen orri taulako helbide fisikoa (PGB), 
										// memoria fisikoan datuak sartuta, zenbat frame erabili diren programa gordetzeko (fKop)
	//printf("Prozesuaren 1. gordetako datua: %d\n", mf[mf[ORRI_TAULA].hitza[prozesua->pMemoria->pgb]].hitza[0]); //ONDO ITEU -> Bueno 09000028 ordez 9000028 gordetzea
	return;
}