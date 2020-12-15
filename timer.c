#include "timer.h"

void *timer_s(void *m){
	int maizt = pm.maiztasuna;

	while(1){		
		if(tick_kop >= maizt){
			pthread_mutex_lock(&mutex_tic);
			tick_kop = 0;
			pthread_mutex_unlock(&mutex_tic);
			//printf("Timer!\n");
			sem_post(&sch);
		}
	}
	
}