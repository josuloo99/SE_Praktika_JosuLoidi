#include "timer.h"

void *timer_s(void *m){
	struct parametroak pm = *(struct parametroak*) m;
	int maizt = pm.maiztasuna;

	int balioa;	
	while(1){
		pthread_mutex_lock(&mutex_tic);
		if(tick_kop > maizt){
			tick_kop = 0;
			printf("Timer!\n");
			sem_post(&sch);
		}
		pthread_mutex_unlock(&mutex_tic);
	}
	
}