#include "kernel.h"

int main(int argc, char const *argv[])
{
	char *p;
	
	struct parametroak pm;
	pm.maiztasuna = strtol(argv[1], &p, 10);
	pm.proz_kop = strtol(argv[2], &p, 10);
	pm.proz_t = strtol(argv[3], &p, 10);

	hasi(pm);
	return 0;
}

void hasi(struct parametroak pm){
	pthread_t clock;
	pthread_t timer;
	pthread_t processG;

    sem_init(&tick, 0, 1);
    sem_init(&begiratu, 0, 1);  
	pthread_mutex_init(&mutex_tic, 0);

	pthread_create(&clock, NULL, clock_s, (void *)&pm); // Haria sortu
	pthread_create(&timer, NULL, timer_s, NULL);
	pthread_create(&processG, NULL, processGenerator, (void *)&pm);

	pthread_join(clock, NULL);
	pthread_join(timer, NULL);
	pthread_join(processG, NULL);
}