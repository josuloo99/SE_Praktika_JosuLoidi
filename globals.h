#ifndef GLOBALS_H
#define GLOBALS_H

extern pthread_mutex_t mutex_tic;
extern volatile int tick_kop;

extern sem_t sch;

// Programa exekutatzean sartzen diren parametroak gordetzeko
struct parametroak{
	int maiztasuna;
	int proz_kop;
	int proz_t;

	int cpu_kop;
	int core_kop;
	int h_kop;
};

// Prozesu bakar baten PCB datu egitura
struct pcb {
	int pid;
	int quantum;
	int egoera;

	int kop; // Prozesua amaitu dadin behar diren ziklo kopurua
	int kont; // Prozesua exekutatu den ziklo kopurua
};

// PCBak dituen linked list
struct Node_pcb { 
    struct pcb data;
    struct Node_pcb* next;
};


// Core bakoitzaren hardware hariaren datu egitura
struct h {
	int id;
	struct pcb **prozesua; //Punteroen punteroa
};

// Core bakoitzaren datu egitura
struct core {
	int id;
	struct h *hariak;
	struct Node_pcb* ilara;
	pthread_mutex_t mutex_ilara;
};

// CPU bakoitzaren datu egitura
struct cpu {
	int id;
	struct core *coreak;
};



extern struct cpu *cpu_s;
extern struct core *core_s;
extern struct h *h_s;

extern struct Node_pcb* linkedQueue;

extern struct parametroak pm;
#endif