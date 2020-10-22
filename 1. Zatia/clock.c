#include "clock.h"

void *clock_s(void *m){
	while(1){
		pthread_mutex_lock(&mutex_tic);
		tick_kop++;
		pthread_mutex_unlock(&mutex_tic);
	}
}