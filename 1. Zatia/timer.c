#include "timer.h"

void *timer_s(){
	while(1){
		sem_wait(&tick);
		printf("Kaixo, tick bat pasa da\n");
		sem_post(&begiratu);
	}
}