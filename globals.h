#ifndef GLOBALS_H
#define GLOBALS_H

extern pthread_mutex_t mutex_tic;
extern volatile int tick_kop;

extern sem_t sch;

enum PROZ_EGOERA {
    waiting, executing, blocked
};

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
	int quantum;
	enum PROZ_EGOERA egoera;
};

struct processQueue{
	struct pcb *ilara;
	int indizea;
	int beteta;
};

struct h {
	int id;
	struct pcb **prozesua; //Punteroen punteroa
};

struct core {
	int id;
	struct h *hariak;
	struct processQueue ilara;
};

struct cpu {
	int id;
	struct core *coreak;
};


extern struct cpu *cpu_s;
extern struct core *core_s;
extern struct h *h_s;

extern struct processQueue pqueue;

extern struct parametroak pm;
#endif