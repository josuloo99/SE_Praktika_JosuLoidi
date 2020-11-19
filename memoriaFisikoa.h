#ifndef MEMORYF_H
#define MEMORYF_H
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 
#include "globals.h"

struct eremua{
	char* helbidea;
	char* data;
};

struct memoriaFisikoa{
	struct eremua * eremuak;
	int azkena;
};

struct memoriaFisikoa *mf;
struct eremua *er;

#endif