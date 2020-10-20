#include "clock.h"

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