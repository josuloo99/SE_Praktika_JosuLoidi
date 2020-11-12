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
void *core_haria(void *i);
void addToCores();

struct core_thread_parameters{
	int id;
	struct core *core_p;
};

//struct core_thread_parameters ctP;

#endif