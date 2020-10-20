#ifndef TIMER_H
#define TIMER_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

#include "globals.h"


void *timer_s();

#endif