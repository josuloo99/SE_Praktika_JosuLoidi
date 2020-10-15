#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

#include "globals.h"

void *timer_s(){
	while(1){
		sem_wait(&tick);
		printf("Kaixo, tick bat pasa da\n");
		sem_post(&begiratu);
	}
}