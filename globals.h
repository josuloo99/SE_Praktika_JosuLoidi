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

struct mm {					//Memory management
	int pgb; 				//orri-taularen helbide fisikoa
	int text;				//kodearen segmentuaren helbide birtuala
	int data;				//datuen segmentuaren helbide birtula

	int IR;					// Instruction Register
	int PC;					// Program Counter
	int PTDR; 				// Page Table Base Register
	int * R; 				// Erregistroak gordetzeko
};

// Prozesu bakar baten PCB datu egitura
struct pcb {
	int pid;
	int quantum;	
	int denbora; 				// Clock-ak kontrolatuko du denbora, beti hemen 1 gehituz
	int egoera;	
	
	int martxan;				// Hariren bat prozesua exekutatzen ari den ala ez adierazten du 
								// (-1 ez badago exekuzioan, bestela hariaren zenbakia)

	struct mm * pMemoria;		// Prozesu bakoitzaren memoria kudeatzeko aldagaiak
};

// PCBak dituen linked list
struct Node_pcb { 
    struct pcb data;
    struct Node_pcb* next;
};


// Core bakoitzaren hardware hariaren datu egitura
struct h {
	int id;
	//struct Node_pcb *prozesua; 	// Hariaren prozesuaren nodoa /*ALDATU? Prozesuaren node_pcb eman ordez, corea-ren ilarako punteroa pasa*/
	struct pcb * prozesua;	// Hau bai pcb-aren punteroa	

	int IR;					// Instruction Register
	int PC;					// Program Counter
	int PTDR; 				// Page Table Base Register
	int * R; 				// Erregistroak gordetzeko

	int denbora;			// Prozesuaren exekuzio denbora kontrolatzeko
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