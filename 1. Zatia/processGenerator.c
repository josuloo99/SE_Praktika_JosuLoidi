#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <pthread.h>
#include <semaphore.h> 

struct pcb {
	int pid;
};

int main(int argc, char const *argv[])
{
	return 0;
}

void processGenerator(){
	srand((unsigned) time(&t));
	int z = rand() % 0.1;
	while(1){
		//prozesua sortu
		sleep(z);
	}
}

void generateProcess(){
	
}