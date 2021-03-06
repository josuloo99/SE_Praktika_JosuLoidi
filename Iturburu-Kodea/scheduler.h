#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "globals.h"
#include "memoriaFisikoa.h"

void *scheduler_t(void *m);
void initialize();
void *core_haria(void *param);
void *hardware_exekuzioa(void *param);
void addToCores();
int agindua_exekutatu(struct h * hh, int h_fisikoa, int despl);

void erregistroakInprimatu(int * r);
void ilaraInprimatu(struct Node_pcb * ilara);
void atzeraBidali(struct Node_pcb * ilara, struct pcb * element);

struct core_thread_parameters {
	int id;
	int cpu_id;
	struct core *core_p;
};

struct hari_thread_parameters {
	int id;
	//struct h * haria;
	struct core_thread_parameters *ctP;
};


struct timespec t_ld_st;
struct timespec t_add;

#endif