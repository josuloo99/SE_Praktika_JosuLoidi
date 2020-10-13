#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h>

void *timer(void *m){
	while(1){
		if(tick_kop == m){
			pthread_mutex_lock(&mutex_tic);
			tick_kop = 0;
			printf("Kaixo! Ziklo bat naiz.\n");
			pthread_mutex_unlock(&mutex_tic);
		}
	}
}