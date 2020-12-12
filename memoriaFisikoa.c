#include "memoriaFisikoa.h"

#define ORRI_TAULA_TAM 50

struct memoriaFisikoa *mf; // Aldagai hau .h-n definitu ezkero errorea

void *memoriaFisikoa (void* m){
	mf = malloc(MEM_F_KOP * sizeof(struct memoriaFisikoa));

	int i;
	for (i = 0; i < MEM_F_KOP; i++){
		mf[i].libre = 0; // Frame bakoitza libre dagoela adierazi
	}
	mf[ORRRI_TAULA].libre = 1; // Orri taularen framea okupatuta dago (200. framea)

	mf[ORRRI_TAULA].hitza = malloc(DESPL_KOP * sizeof(int)); // Frameko hitzak definitu, int tamainako 2^12 hitz
	for (i = 0; i < DESPL_KOP; i++){
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
	for (i = 0; i < DESPL_KOP; i++){
		if (orriTaulak->hitza[i] == -1){ 							// Libre baldin badago
			for(j = 0; j<2; j++){
				orriTaulak->hitza[i+j] = 0; 						// Erreserbatu hitzak prozesuaren taularako. 0an hasieratu. Gero ikusiko da zer egin
			}
			proz->pMemoria->pgb = i;								// PCBko PGB erregistroari orri taularen helbidea esleitu
			return;
		}
	}
	printf("Orri taula guztiak beteta daude. Errorea.\n");
	return;
}

int MMU (struct pcb * proz, int birtuala){ // Birtualak 0 edo 1 balioa izan behar du ?¿?¿? .data eta .text
	int pgb = proz->pMemoria->pgb;
	// Bilatu orri taulan helbide birtuala zein helbide fisiko den
	int fisikoa = mf[ORRRI_TAULA].hitza[pgb+birtuala].data;
	return fisikoa;
}

void irakurriFitx(struct pcb * proz, char * izena){
	FILE* FP;
	int bufferLength = 255;
	char buffer[bufferLength];

	FP = fopen(izena "r");

	int i;
	for(i = KERNEL_AMAIERA + 1; i < MEM_F_KOP){
		// Memoria fisikoan kernelari EZ dagokion zatian frame libre bat aurkitu
		if(mf[i].libre = 0)
			break;
	}
	// Memoria fisikoan frame libre bat baldin badago
	if (i < MEM_F_KOP){
		int despl = 0;
		// .text eta .data helbide fisikoa gordetzeko
		if(fgets(buffer, bufferLength, filePointer))
			proz->pMemoria->text = (int) buffer;
		if(fgets(buffer, bufferLength, filePointer))
			proz->pMemoria->data = (int) buffer;

		// Beste guztiak gordetzeko
		while(fgets(buffer, bufferLength, filePointer)) {
		    //printf("%s\n", buffer);
		    // Frame horretako dagokion desplazamenduan idatzi informazioa
		    mf[i].hitza[despl].data = (int) buffer;
		}
		// Orri taulan helbide fisikoak esleitzea falta da. Baina zein dira helbide fisiko horiek ?¿?¿?¿?
		fclose(filePointer);
	} else {
		printf("Memoria fisikoa beteta dago.\n");
	}
}