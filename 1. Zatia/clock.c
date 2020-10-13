#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 

#include "globals.h"

void *clock(){
	while(1){
		pthread_mutex_lock(&mutex_tic);
		tick_kop++;
		pthread_mutex_unlock(&mutex_tic);
	}
}