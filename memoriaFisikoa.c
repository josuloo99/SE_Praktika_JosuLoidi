#include "memoriaFisikoa.h"

#define ORRI_TAULA_TAM 50

void *memoriaFisikoa (void* m){
	mf = malloc(512 * sizeof(struct memoriaFisikoa));

	int i;
	for (i = 0; i < 512; i++){
		mf[i].libre = 0; // Frame bakoitza libre dagoela adierazi
	}
	mf[ORRRI_TAULA].libre = 1; // Orri taularen framea okupatuta dago
}

void orriTaulaEsleitu(struct pcb * proz){
	int i;
	// Orri taulako hitz bakoitzeko
	struct memoriaFisikoa * orriTaulak = mf[ORRRI_TAULA];
	for (i = 0; i < 32768; i++){
		if (mf[ORRRI_TAULA]->data[i].libre == 0){ 						// Libre baldin badago
			mf[ORRRI_TAULA]->data[i].libre = 1; 						// Adierazi ez dagoela libre
			proz->pMemoria->pgb = i;									// PCBko PGB erregistroari orri taularen helbidea esleitu
			return;
		}
	}
	return;
}

int MMU (int birtuala){
	int fisikoa;
	// Bilatu orri taulan helbide birtuala zein helbide fisiko den
	return fisikoa;
}