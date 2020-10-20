#ifndef KERNEL_H
#define KERNEL_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 

#include "globals.h"
#include "clock.h"
#include "timer.h"
#include "processGenerator.h"


void hasi(struct parametroak pm);

pthread_mutex_t mutex_tic;
volatile int tick_kop = 0;

sem_t tick;
sem_t begiratu;

#endif