#include "loader.h"


struct Node_pcb* linkedQueue;
void *processGenerator(void *pmt) {
	struct parametroak pm = *(struct parametroak*) pmt;
	int proz_kop = pm.proz_kop;
	int proz_t = pm.proz_t;
	int i = 20, k;
	time_t t;

	linkedQueue = NULL;
	struct Node_pcb *aux = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));

	srand((unsigned) time(&t));
	while (1) {
		int r = (rand() % proz_t) + 1;
		if (r != 0) {
			sleep(r);
			struct Node_pcb *aux = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
			struct Node_pcb *last = linkedQueue;

			struct pcb prozesua;
			prozesua.pid = i; // pid esleitu
			//prozesua.quantum = (rand() % 10000000) + 1; // Quantuma esleitu, random batekin
			prozesua.quantum = (rand() % 100) + 1; // Quantuma esleitu, random batekin (Balio hauekin batzuetan Quamtuma azkarragoa da programaren exekuzioa baino)
			prozesua.martxan = EG_ZAIN;	// Prozesua ez dago exekuzioan

			struct mm * pMem = malloc(sizeof(struct mm));
			pMem->R = malloc(16 * sizeof(int)); // Erregistroen array-a hasieratu
			for (k = 0; k < 16; k++)
				pMem->R[k] = 0;
			pMem->PC = 0;
			pMem->IR = 1;
			prozesua.pMemoria = pMem;

			kargatuPrograma(&prozesua);

			aux->data = prozesua;
			aux->next = NULL;

			// Prozesuen ilaran sartu prozesu berria
			if (linkedQueue == NULL)
			{
				linkedQueue = aux;
			} else {

				while (last->next != NULL)
					last = last->next;

				last->next = aux;
			}

			printf("Idatzi da, %d zenbakia\n", aux->data.pid);

			i++;
		}
	}
}

void kargatuPrograma(struct pcb * prozesua) {

	// Ausaz aukeratutako fitxategi batetik programa kargatu eta pgb, data (aldagaiak) eta text (kodea) helbideak esleitu
	srand(time(0));
	int zenbk = (rand() % 50);
	unsigned char fitxIzena[64];

	char str[20];
	if (zenbk < 10)
		sprintf(fitxIzena, "Programak/prog00%d.elf", zenbk);
	else
		sprintf(fitxIzena, "Programak/prog0%d.elf", zenbk);

	irakurriFitx(prozesua, fitxIzena); 	// Hemen lortutakoa: prozesuaren lehen orri taulako helbide fisikoa (PGB),
	// memoria fisikoan datuak sartuta, zenbat frame erabili diren programa gordetzeko (fKop)
	return;
}