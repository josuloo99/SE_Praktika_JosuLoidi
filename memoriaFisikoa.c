#include "memoriaFisikoa.h"

#define ORRI_TAULA_TAM 50

pthread_mutex_t mutex_memoria;

void *memoriaFisikoa (void* m) {
	mf = malloc(MEM_F_KOP * sizeof(struct memoriaFisikoa));
	pthread_mutex_init(&mutex_memoria, 0);

	int i;
	for (i = 0; i < MEM_F_KOP; i++) {
		mf[i].libre = 0; // Frame bakoitza libre dagoela adierazi
	}
	mf[ORRI_TAULA].libre = 1; // Orri taularen framea okupatuta dago (200. framea)

	mf[ORRI_TAULA].hitza = malloc(DESPL_KOP * sizeof(int)); // Frameko hitzak definitu, int tamainako 2^12 hitz
	for (i = 0; i < DESPL_KOP; i++) {
		mf[ORRI_TAULA].hitza[i] = -1; // Hitz bakoitza libre dagoela adierazteko
	}
}


int MMU (struct pcb * proz, int birtuala) {
	int pgb = proz->pMemoria->pgb; 		// Prozesuaren 1. orri taularen helbide fisikoa
	int frame = birtuala >> 12;			// Helbide birtuala zenbatgarren framean dagoen lortu
	// (frame bakoitzak 4096ko desplazamendua (2^12) duela jakinda)
	// Bilatu orri taulan helbide birtuala zein helbide fisiko den
	int fisikoa = mf[ORRI_TAULA].hitza[pgb + frame];
	return fisikoa;
}

/*
	Funtzio honetan PCB bat eta irakurriko den fitxategiaren izena pasatzen dira
	eta bi gauza egiten dira:
	1. Fitxategia irakurtzen da eta memoria fisikoan idazten da libre dauden
	frameetan (kontuan hartuta posible dela frame bat baino gehiago behar izatea
	prozesu batek). Gainera, frame horien zenbakiak gorde egiten dira gerorako.

	2. Ondoren, memoria fisikoko orri taulak gordetzen diren framean
	libre dagoen hitz bat aurkitzen du eta beharrezkoak erreserbatzen ditu PCBarentzat,
	dagokien helbide fisikoak esleituz.
	Amaitzeko, prozesuaren PGB esleitzen da orri taularen helbidearekin
*/
void irakurriFitx(struct pcb * proz, unsigned char izena[64]) {

	// ** FITXATEGIA IRAKURRI ETA MEMORIA FISIKOA KUDEATU** //

	int i; // Prozesua gordeko den frame zenbakia gordetzeko
	int fKont = 0; // Prozesuak memoria fisikoan zenbat frame behar dituen gordetzeko

	for (i = KERNEL_AMAIERA + 1; i < MEM_F_KOP; i++) {
		//printf("mf[%d].libre = %d\n", i, mf[i].libre);
		// Memoria fisikoan kernelari EZ dagokion zatian frame libre bat aurkitu
		if (mf[i].libre == 0) {
			mf[i].libre = 1;
			break;
		}
	}
	// Memoria fisikoan frame libre bat baldin badago
	if (i < MEM_F_KOP) {
		// i posizioa izango da prozesu hau gordetzeko frame zenbakia
		fKont++; // Prozesuak memoriako frame bat izango du gutxienez

		// Fitxategia ireki
		FILE* FP;
		int bufferLength = 255;
		char buffer[bufferLength];

		FP = fopen(izena, "r");

		// .text eta .data helbide fisikoa gordetzeko
		if (fgets(buffer, bufferLength, FP))
			proz->pMemoria->text = atoi(buffer);
		if (fgets(buffer, bufferLength, FP))
			proz->pMemoria->data = atoi(buffer);

		// Hemendik aurrerakoak aginduak dira
		// Desplazamendua: Frame baten barruan agindu bakoitzaren posizioa determinatzeko
		int despl = 0;
		mf[i].hitza = malloc(DESPL_KOP * sizeof(int));



		while (fgets(buffer, bufferLength, FP)) {
			// Frame horretako dagokion desplazamenduan idatzi informazioa
			mf[i].hitza[despl] = (int)strtol(buffer, NULL, 16);
			despl++;
			if (despl >= DESPL_KOP) {
				i++; 		// Hurrengo framera pasa
				fKont++; 	// Beste frame bat gehiago beharko dela adierazi
				despl = 0;	// Desplazamendua hasieratu (frame berri batean gaudelako)


				mf[i].libre = 1; // Framea libre ez dagoela adierazi
				mf[i].hitza = malloc(DESPL_KOP * sizeof(int)); // Frrame berriko desplazamendua hasieratu
			}
		}
		fclose(FP);

		// ** PROZESUARI ORRI TAULA ESLEITU ETA HAU BETE DAGOKION HELBIDE FISIKOEKIN ** //

		// Prozesua gorde den lehen frame helbidea
		int frame1 = i - (fKont - 1);


		int t_kop, f_kop;
		// Orri taulako hitz bakoitzeko
		struct memoriaFisikoa * orriTaulak = &mf[ORRI_TAULA];
		for (t_kop = 0; t_kop < DESPL_KOP; t_kop++) {
			if (orriTaulak->hitza[t_kop] == -1) { 						// Libre baldin badago
				for (f_kop = 0; f_kop < fKont; f_kop++) {				// Behar diren frame bakoitzeko
					orriTaulak->hitza[t_kop + f_kop] = frame1 + f_kop; 	// Erreserbatu hitzak prozesuaren taularako. Dagokien helbide fisikoa jarri
				}
				proz->pMemoria->pgb = t_kop;							// PCBko PGB erregistroari lehen orri taularen helbidea (fisikoa) esleitu
				return;
			}
		}
		printf("Orri taula guztiak beteta daude. Errorea.\n");
		return;
	} else {
		printf("Memoria fisikoa beteta dago.\n");
	}
	return;
}