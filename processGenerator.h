#ifndef PROCESSGENERATOR_H
#define PROCESSGENERATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 
#include <time.h>
#include <string.h>

#include "globals.h"
#include "memoriaFisikoa.h"
#include "scheduler.h"

void *processGenerator(void *m);
void generateProcess(int i);
void kargatuPrograma(struct pcb * prozesua);
#endif