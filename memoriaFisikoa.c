#include "memoriaFisikoa.h"

#define ORRI_TAULA_TAM 50

struct memoriaFisikoa *mf; // Aldagai hau .h-n definitu ezkero errorea

void *memoriaFisikoa (void* m){
	mf = malloc(512 * sizeof(struct memoriaFisikoa));

	int i;
	for (i = 0; i < 512; i++){
		mf[i].libre = 0; // Frame bakoitza libre dagoela adierazi
	}
	mf[ORRRI_TAULA].libre = 1; // Orri taularen framea okupatuta dago

	mf[ORRRI_TAULA].hitza = malloc(32768 * sizeof(int)); // Frameko hitzak definitu, int tamainako 2^15 hitz
	for (i = 0; i < 32768; i++){
		mf[ORRRI_TAULA].hitza[i] = -1; // Hitz bakoitza libre dagoela adierazteko
	}
}

/*
	Funtzio honek memoria fisikoko orri taulak gordetzen diren framean 
	libre dagoen hitz bat aurkitzen du eta ondorengo biak erreserbatzen
	ditu parametrotzat enmaten den prozesuarentzat. Amaitzeko, prozesuaren
	PGB esleitzen da orri taularen helbidearekin
*/
void orriTaulaEsleitu(struct pcb * proz){
	int i, j;
	// Orri taulako hitz bakoitzeko
	struct memoriaFisikoa * orriTaulak = &mf[ORRRI_TAULA];
	for (i = 0; i < 32768; i++){
		if (orriTaulak->hitza[i] == -1){ 							// Libre baldin badago
			for(j = 0; j<2; j++){
				orriTaulak->hitza[i+j] = 0; 						// Erreserbatu hitzak prozesuaren taularako. 0an hasieratu. Gero ikusiko da zer egin
			}
			proz->pMemoria->pgb = i;								// PCBko PGB erregistroari orri taularen helbidea esleitu
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