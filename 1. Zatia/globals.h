#ifndef GLOBALS_H
#define GLOBALS_H

extern pthread_mutex_t mutex_tic;
extern volatile int tick_kop;

extern sem_t sch;


struct parametroak{
	int maiztasuna;
	int proz_kop;
	int proz_t;

	int cpu_kop;
	int core_kop;
	int h_kop;
};

struct pcb {
	int pid;
};

struct processQueue{
	struct pcb *ilara;
};

struct cpu {
	int cpu_kop;
	int core_kop;
	int h_kop;
};

extern struct cpu cpu;
extern struct processQueue pqueue;

#endif