#ifndef GLOBALS_H
#define GLOBALS_H

extern pthread_mutex_t mutex_tic;
extern volatile int tick_kop;

extern sem_t tick;
extern sem_t begiratu;


struct parametroak{
	int maiztasuna;
	int proz_kop;
	int proz_t;
};

//extern struct parametroak pm;

//extern struct parametroak;
//extern struct pcb;
//extern struct processQueue;

#endif