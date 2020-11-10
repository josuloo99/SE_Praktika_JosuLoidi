#ifndef PROCESSGENERATOR_H
#define PROCESSGENERATOR_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 
#include <time.h>

#include "globals.h"

void *processGenerator(void *m);
void generateProcess(int i);

//struct Node_pcb* linkedQueue;
struct Node_pcb* linkedQueue;


#endif