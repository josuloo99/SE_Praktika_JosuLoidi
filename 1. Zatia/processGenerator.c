#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 

#include "gobals.h"

struct pcb {
	int pid;
};



void processGenerator(void *m, void){
	int maizt = *(int*)m;
	int i = 0;

	
	srand((unsigned) time(&t));
	while(1){
		sleep(rand() % maizt);
		prozesua_sortu(i);
		i++;
	}
}

void generateProcess(int i, struct processQueue *queue){
	struct pcb prozesua;
	prozesua->pid = i;
	ilara[i] = prozesua;
}