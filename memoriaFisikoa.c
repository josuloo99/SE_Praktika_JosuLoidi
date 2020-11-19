#include "memoriaFisikoa.h"

#define ORRI_TAULA_TAM 50

//struct memoriaFisikoa mf = malloc(1048576 * sizeof(struct memoriaFisikoa));
//struct memoriaFisikoa *mf;

void *memoriaFisikoa (void* m){
	mf = malloc(sizeof(struct memoriaFisikoa));
	er = malloc(1048576 * sizeof(struct eremua));
	mf->eremuak = er;
	mf->azkena = 0;	
}

void orri_taula(struct pcb * proz){
	int i;
	int azkena = mf->azkena;
	proz->pMemoria.pgb = azkena;

	char * prozesua1 = "prozesua1.txt";
	FILE* file = fopen(prozesua1, "r"); /* should check the result */
    char line[4];

    while (fgets(line, sizeof(line), file)) {
        struct eremua er = mf->eremuak[i];
		er.helbidea = malloc((24/8) + 1);
		sprintf(er.helbidea, "%d", azkena);
		er.data = malloc(4);
        sprintf(er.helbidea, "%s", line);
        azkena++;
    }

    fclose(file);
}