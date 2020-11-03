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
void generateProcess(int i, struct processQueue queue);
struct processQueue pqueue;

#endif