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
#include "scheduler.h"
#include "memoriaFisikoa.h"

struct parametroak pm;

void hasi(struct parametroak pm);
void cpu_sortu(int cpu, int core);

pthread_mutex_t mutex_tic;
volatile int tick_kop = 0;

sem_t sch;

struct timer_p *timerq;
struct cpu *cpu_s;
struct core *core_s;
struct h *h_s;
#endif