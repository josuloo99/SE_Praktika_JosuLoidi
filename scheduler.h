#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 

#include "globals.h"

void *scheduler_t(void *m);
void initialize();
void *core_haria(void *param);
void *hardware_exekuzioa(void *param);
void addToCores();

struct core_thread_parameters{
	int id;
	int cpu_id;
	struct core *core_p;
};

struct hari_thread_parameters {
	int id;
	//struct h * haria;
	struct core_thread_parameters *ctP;
};

#endif