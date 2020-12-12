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
#define ORRRI_TAULA 200		// Orri taula frame honetan dago
#define MEM_F_KOP 4096      // Memoria fisikoak 4096 frame egongo dira -> 2^12
#define DESPL_KOP 4096      // Frame / orri bakoitzean dagoen hitz (desplazamendu) kopurua -> 2^12

struct hitza {
	int data;						// Frame bateko hitzak izango duen datua (helbidea edo agindua izan daiteke adibidez), -1 libre badago
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


void *memoriaFisikoa (void* m);
void orriTaulaEsleitu(struct pcb * proz);
int MMU (struct pcb * proz, int birtuala);


#endif