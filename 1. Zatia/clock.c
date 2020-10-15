#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 

#include "globals.h"

void *clock_s(void *m){
	struct parametroak pm = *(struct parametroak*) m;
	int maizt = pm.maiztasuna;
	while(1){
		pthread_mutex_lock(&mutex_tic);
		tick_kop++;
		pthread_mutex_unlock(&mutex_tic);	
		if(tick_kop % maizt == 0){
			sem_post(&tick);
		}
	}
}