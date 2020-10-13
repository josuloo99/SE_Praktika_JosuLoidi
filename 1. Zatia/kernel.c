#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 

#include "globals.h"
#include "clock.h"
#include "timer.h"

pthread_mutex_t mutex_tic;
int tick_kop = 0;

int main(int argc, char const *argv[])
{
	char *p;
	
	hasi(strtol(argv[1], &p, 10));
	return 0;
}

void hasi(int m){
	pthread_t clock;
	pthread_t timer;

	err = pthread_create(&clock, NULL, clock, NULL); // Haria sortu
	err = pthread_create(&hariak[i], NULL, lortu_zenbakiak, (void *)&m);

	pthread_join(clock, NULL);
	pthread_join(timer, NULL);
}