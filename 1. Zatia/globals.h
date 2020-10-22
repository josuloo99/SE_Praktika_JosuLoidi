#ifndef GLOBALS_H
#define GLOBALS_H

extern pthread_mutex_t mutex_tic;
extern volatile int tick_kop;

extern sem_t sch;


struct parametroak{
	int maiztasuna;
	int proz_kop;
	int proz_t;
};

struct pcb {
	int pid;
};

struct processQueue{
	struct pcb *ilara;
};

extern struct processQueue pqueue;

#endif