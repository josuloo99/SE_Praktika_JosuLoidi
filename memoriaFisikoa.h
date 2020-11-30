#ifndef MEMORYF_H
#define MEMORYF_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 
#include "globals.h"

// Helbide kopurua: 2^24 = 16777216 / 4KB = 512 frame

#define KERNEL_HASIERA 0 	// Kernela hasten den frame helbidea (kopurua)
#define KERNEL_AMAIERA 256 	// Kernela amaitzen den frame helbidea
#define ORRRI_TAULA 200		// Orri taula frame honetan dago

struct hitza {
	int data;					// Frame bateko hitzak izango duen datua (helbidea edo agindua izan daiteke adibidez), -1 libre badago
	//int libre;					// Hitza libre dagoen (0) ala ez (1) adierazteko
};

struct memoriaFisikoa { 				// Memoriaren arrayko elementu bakoitza Frame bat izango da
	int * hitza;						// Frame bateko hitzak izango duen datua (helbidea edo agindua izan daiteke adibidez), ondoren desplazamendua izanfo da. Bakoitza -1 libre badago
	int libre;							// Framea libre dagoen (0) ala ez (1) adierazteko
};


struct TBL_taula {
	int birtuala;
	int fisikoa;
};

struct TBL {
	struct TBL_taula *taula;
};


/*struct orri_taula {
	int helbidea;
	int desplazamendua;
	int birtuala;
	int fisikoa;
};*/



void *memoriaFisikoa (void* m);
void orriTaulaEsleitu(struct pcb * proz);


#endif