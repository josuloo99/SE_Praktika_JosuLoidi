#include "kernel.h"


/*
	Programa konpilatzeko: gcc -o kernel *.c -lpthread
*/
int main(int argc, char const *argv[])
{
	char *p;

	if (argc == 7) {
		pm.maiztasuna = strtol(argv[1], &p, 10) * 1000000;
		pm.proz_kop = strtol(argv[2], &p, 10);
		pm.proz_t = strtol(argv[3], &p, 10);
		pm.cpu_kop = strtol(argv[4], &p, 10);
		pm.core_kop = strtol(argv[5], &p, 10);
		pm.h_kop = strtol(argv[6], &p, 10);

		cpu_sortu(pm.cpu_kop, pm.core_kop, pm.h_kop);
	} else {
		fprintf(stderr, "Parametro kopuru okerra\n\nFormatua: ./kernel \n\tmaiztasuna_(1000000 aldiz txikiagoa)\n\tilarako_prozesu_kopurua\n\tprozesuak_sortzeko_maiztasuna\n\tCPU_kopurua\n\tcore_kopurua_(CPU bakoitzeko)\n\thardware_hari_kopurua_(core bakoitzeko)\n");
		exit(1);
	}

	hasi(pm);
	return 0;
}

void hasi(struct parametroak pm) {
	pthread_t clock;
	pthread_t timer;
	pthread_t processG;
	pthread_t scheduler;
	pthread_t memFisikoa;

	sem_init(&sch, 0, 0);
	pthread_mutex_init(&mutex_tic, 0);

	tick_kop = 0;

	pthread_create(&clock, NULL, clock_s, (void *)&pm); // Haria sortu
	pthread_create(&timer, NULL, timer_s, (void *)&pm);
	pthread_create(&processG, NULL, processGenerator, (void *)&pm);
	pthread_create(&scheduler, NULL, scheduler_t, (void *)&pm);
	pthread_create(&memFisikoa, NULL, memoriaFisikoa, NULL);

	pthread_join(clock, NULL);
	pthread_join(timer, NULL);
	pthread_join(processG, NULL);
	pthread_join(scheduler, NULL);
	pthread_join(memFisikoa, NULL);
}

void cpu_sortu(int cpu, int core, int hhari) {
	int i = 0;
	int j = 0;
	int k = 0;
	int id = 0;

	// cpu_struct hasieratu
	cpu_s = malloc(cpu * sizeof(struct cpu));

	// cpu bakoitzari id esleitu
	for (i = 0; i < cpu; i++) {
		cpu_s[i].id = i;
	}

	// cpu bakoitzeko coreak sortu
	for (i = 0; i < cpu; i++) {
		core_s = malloc(core * sizeof(struct core));
		// core bakoitzeko
		for (j = 0; j < core; j++) {
			struct Node_pcb *coreQueue = malloc(sizeof(struct Node_pcb));
			coreQueue->next = NULL;

			// Hardware hari guztiak gordetzeko espazioa erreserbatu
			h_s = malloc(hhari * sizeof(struct h));
			for (k = 0; k < hhari; k++) {
				// Sortutako hariei id esleitu
				h_s[k].id = k;
				h_s[k].prozesua = NULL;
				h_s[k].R = malloc(16 * sizeof(int)); // Erregistroen array-a hasieratu
				// Hariaren parametro berriekin zerbait egin behar bada:
			}
			// Coreari bere ilara esleitu
			core_s[j].ilara = coreQueue;
			pthread_mutex_init(&core_s[j].mutex_ilara, 0);
			// Coreari hari hauek esleitu
			core_s[j].hariak = h_s;
			// Coreari id esleitu
			core_s[j].id = id;
			id++;
		}
		// cpu-ari corea esleitu
		cpu_s[i].coreak = core_s;
	}
}