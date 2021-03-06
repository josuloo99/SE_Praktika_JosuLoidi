#ifndef MEMORYF_H
#define MEMORYF_H

#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 
#include "globals.h"

// Helbide kopurua (24 bit): 2^24 = 16777216

// 4KBeko orriak/frameak (2^12) -> desplazamendua orri bakoitzeko (offset): 12 bit
// Helbideratze tarte logikoa: 24-12 = 12bit -> 2^12B -> 4KB ----> 12 bit orriak + 12 bit desplazamendua --> 2^12 orri = 4096 orri (fisikoan berdin)
// Helbideratze tarte fisikoa: 24-12 = 12bit -> 2^12B -> 4KB 

#define KERNEL_HASIERA 0 	// Kernela hasten den frame helbidea (kopurua)
#define KERNEL_AMAIERA 256 	// Kernela amaitzen den frame helbidea
#define ORRI_TAULA 200		// Orri taulak frame honetan daude
#define MEM_F_KOP 4096      // Memoria fisikoak 4096 frame egongo dira -> 2^12
#define DESPL_KOP 4096      // Frame / orri bakoitzean dagoen hitz (desplazamendu) kopurua -> 2^12
#define TBL_KOP 512			// TBLko taulak izango duen tamaina

struct memoriaFisikoa { 				// Memoriaren arrayko elementu bakoitza Frame bat izango da
	int * hitza;						// Frame bateko hitzak izango duen datua (helbidea edo agindua izan daiteke adibidez), ondoren desplazamendua izanfo da. Bakoitza -1 libre badago
	int libre;							// Framea libre dagoen (0) ala ez (1) adierazteko
};

struct TBL {							// TBL gordetzeko egitura. Array bat eta posizio bakoitzean:
	int proz;							// Zein prozesuri dagokion
	int frameB;							// Helbide birtuala
	int fisikoa;						// Helbide birtual horri dagokion helbide fisikoa
};


void *memoriaFisikoa (void* m);
int MMU (struct pcb * proz, int birtuala);
void irakurriFitx(struct pcb * proz, unsigned char izena[64]);

int TBL (int proz, int frame);
void addTBL(int proz, int frame, int fisikoa);

extern pthread_mutex_t mutex_memoria;		// Memoriako atzipenetarako mutex-a.
struct memoriaFisikoa *mf; 					// Portatilean .h-n definitu ezkero errorea
struct TBL * TBL_taula;

#endif