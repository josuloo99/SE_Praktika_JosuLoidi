#include "scheduler.h"

void *scheduler_t(void *m) {

	initialize();
	while (1) {
		sem_wait(&sch);
		addToCores();
	}
}

void addToCores() {
	struct Node_pcb *current_node = linkedQueue;

	//Prozesuak coreetako harietan sartu eta originaletik kendu
	int i, j;
	while ( current_node != NULL) {
		for (i = 0; i < pm.cpu_kop; i++) {
			struct core *cpu_core = cpu_s[i].coreak;

			for (j = 0; j < pm.core_kop; j++) {

				struct Node_pcb *new = (struct Node_pcb*)malloc(sizeof(struct Node_pcb));
				new->data = current_node->data;
				new->next = NULL;

				pthread_mutex_lock(&cpu_core->mutex_ilara);

				if (cpu_core[j].ilara == NULL) {
					cpu_core[j].ilara = new;
				} else {
					struct Node_pcb *last = cpu_core[j].ilara;
					while (last->next != NULL)
						last = last->next;
					last->next = new;
				}
				pthread_mutex_unlock(&cpu_core->mutex_ilara);

				if (current_node->next == NULL) {
					linkedQueue = NULL;
					return;
				} else {
					current_node = current_node->next;
				}
			}
		}
	}
	//Suposatuz, beti kabituko direla prozesu guztiak coreetako harietan
	linkedQueue = NULL;
}

static int *th_queue_kop;

void initialize() {
	int cpuK = pm.cpu_kop;
	int core = pm.core_kop;

	int core_hari_kop = cpuK * core;

	th_queue_kop = malloc(core * sizeof(int));
	pthread_t *core_hariak;

	core_hariak = malloc(core_hari_kop * sizeof(pthread_t));

	int i = 0;
	int j = 0;
	int id = 0;
	int err;

	struct core_thread_parameters *ctP;
	ctP = malloc(core_hari_kop * sizeof(struct core_thread_parameters));

	for (i = 0; i < cpuK; i++) {
		for (j = 0; j < core; j++) {
			ctP[id].id = id;
			ctP[id].cpu_id = i;
			ctP[id].core_p = &cpu_s[i].coreak[j];
			err = pthread_create(&core_hariak[id], NULL, core_haria, (void *) &ctP[id++]);
			if (err > 0) {
				fprintf(stderr, "Errore bat gertatu da core hariak sortzean.\n");
				exit(1);
			}
		}
	}
}

void *core_haria(void *param) {
	// Core honen parametroak
	struct core_thread_parameters *ctP_h = (struct core_thread_parameters *)param;
	// Core honen ID
	int zenb = ctP_h->id;
	// Core honen core objektua
	struct core *core_p = ctP_h->core_p;

	// Hariei parametroak pasatzeko aldagaia
	int h_kop = pm.h_kop;
	struct hari_thread_parameters * htP;
	htP = malloc(h_kop * sizeof(struct hari_thread_parameters));

	pthread_t *hardware_thread;
	hardware_thread = malloc(h_kop * sizeof(pthread_t));

	int j, err;
	for (j = 0; j < h_kop; j++) {
		htP[j].id = j;
		//htP[j].haria = core_p->hariak; //htP[j].haria = core_p->hariak[j];
		htP[j].ctP = ctP_h;
		err = pthread_create(&hardware_thread[j], NULL, hardware_exekuzioa, (void *) &htP[j]);
		if (err > 0) {
			fprintf(stderr, "Errore bat gertatu da hardware hari threadak sortzean.\n");
			exit(1);
		}
	}
}

void *hardware_exekuzioa(void *param) {
	// Parametroak lortu
	struct hari_thread_parameters *htP = (struct hari_thread_parameters *)param;

	pthread_mutex_t mutex_ilara = htP->ctP->core_p->mutex_ilara;

	// Parametroetatik hardware hari objektua lortu
	struct h * hh = &htP->ctP->core_p->hariak[htP->id];

	// Corearen pcb-en ilara lortu
	struct Node_pcb * ilara = htP->ctP->core_p->ilara;
	struct Node_pcb * ilaraN;
	// Corearen eta hardware hariaren ID lortu
	int c_id = htP->ctP->id;
	int h_id = hh->id;

	printf("Hari num: %d // Core num: %d\n", h_id, c_id);
	while (1) {
		// Hariak ez badauka inongo prozesurik esleituta ilaran bilatu
		if (hh->prozesua == NULL) {

			ilaraN = ilara;

			// Itxaron ilaran prozesuren bat egon arte
			while (ilaraN->next == NULL);
			ilaraN = ilaraN->next;

			// Mutex: ilarako atzipen eta irakurketek elkar eraginik izan ez dezaten
			pthread_mutex_lock(&mutex_ilara);
			// Prozesua ez badago beste hariren baten exekuzioan exekuziorako hartu
			if (ilaraN->data.martxan == EG_ZAIN) {
				ilaraN->data.martxan = EG_EXEK;

				// Beharrezko parametro guztiak lortu
				hh->prozesua = &ilaraN->data;
				hh->PC = hh->prozesua->pMemoria->PC;
				hh->IR = hh->prozesua->pMemoria->IR;
				//hh->R = malloc(16*sizeof(int));
				hh->R = hh->prozesua->pMemoria->R;
				hh->denbora = hh->prozesua->quantum;
				hh->PTDR = hh->prozesua->pMemoria->pgb;

				// Hariei esleipena eginda. PRINT
				// printf("%d//%d Prozesua: %d\n", c_id, h_id, ilaraN->data.pid);
			} else
				ilara->next = ilaraN->next;
			// Mutex askatu
			pthread_mutex_unlock(&mutex_ilara);
		}
		// Hariak prozesu bat esleituta baldin badauka exekutatu
		else {
			int h_fisikoa, despl, agindua, exit = 0;
			// Quantuma pasa ez den bitartean
			while (hh->denbora > 0) {
				// MMU funtzioari deitu helbide fisikoa lortzeko
				h_fisikoa = MMU(hh->prozesua, hh->PC);
				// Desplazamendua kalkulatu maskara bat eginez
				despl = hh->PC & 0x000FFF;

				// Agindua exekutatu
				exit = agindua_exekutatu(hh, h_fisikoa, despl);
				if (exit == 1){
					printf("AMAITU: %d %d//%d\n", hh->prozesua->pid, c_id, h_id);
					break;
				}
				hh->PC++;
				hh->IR++;
			}

			// Testuinguru aldaketa, quantuma edo prozesua amaitu da
			// Erregistro guztien balioak PCBan gorde
			hh->prozesua->pMemoria->PC = hh->PC;
			hh->prozesua->pMemoria->IR = hh->IR;
			hh->prozesua->pMemoria->R = hh->R;

			pthread_mutex_lock(&mutex_ilara);
			if (exit == 1)
				hh->prozesua->martxan = EG_AMAI;
			else if (exit == 0) {
				hh->prozesua->martxan = EG_ZAIN;
				atzeraBidali(ilara, hh->prozesua);
			}
			pthread_mutex_unlock(&mutex_ilara);
			hh->prozesua = NULL;
		}
	}
}

/*
	Ilara bat eta prozesu bat emanda, prozesuaren kopia bat egiten
	du eta amaieran gehitzen dio. Bidean zenbaki bereko prozesu bat
	baldin badago, ezabatu egiten du zerrendatik.
*/
void atzeraBidali(struct Node_pcb * ilara, struct pcb * element) {
	struct Node_pcb * list = ilara;

	struct Node_pcb * new;
	new = malloc(sizeof(struct Node_pcb));
	new->data = *element;
	new->next = NULL;

	while (list->next != NULL) {
		if (list->next->data.pid == element->pid) {
			list->next = list->next->next;
		} else {
			list = list->next;
		}
	}
	list->next = new;
}

/*
	Emandako agindua exekutatuko du.
	@return 0 Agindua ongi exekutatu bada
	@return 1 Agindua exit agindu bat baldin bada
*/
int agindua_exekutatu(struct h * hh, int h_fisikoa, int despl) {
	// Memoria fisikotik agindua lortu
	int agindua = mf[h_fisikoa].hitza[despl];

	if (agindua == 0xF0000000) {
		//printf("Exit %d\n", hh->prozesua->pid);
		return 1;
	}

	int * R = hh->R;
	int op, reg1, reg2, reg3, v_address;
	op = (agindua >> 28) & 0x0F;
	reg1 = (agindua >> 24) & 0x0F;
	reg2 = (agindua >> 20) & 0x0F;
	reg3 = (agindua >> 16) & 0x0F;
	v_address = agindua & 0x00FFFFFF;

	switch (op) {
	case 0: // ld
		//printf("0x%X -> ld r%d, 0x%X\n", agindua, reg1, v_address);
		pthread_mutex_lock(&mutex_memoria);
		R[reg1] = mf[h_fisikoa].hitza[v_address / 4];
		pthread_mutex_unlock(&mutex_memoria);
		break;
	case 1: // st
		//printf("0x%X -> st r%d, 0x%X\n", agindua, reg1, v_address);
		pthread_mutex_lock(&mutex_memoria);
		mf[h_fisikoa].hitza[v_address / 4] = R[reg1];
		pthread_mutex_unlock(&mutex_memoria);
		break;
	case 2: // add
		//printf("0x%X -> add r%d, r%d + r%d\n", agindua, reg1, reg2, reg3);
		pthread_mutex_lock(&mutex_memoria);
		R[reg1] = R[reg2] + R[reg3];
		pthread_mutex_unlock(&mutex_memoria);
		break;
	}
	return 0;
}

void erregistroakInprimatu(int * r) {
	printf("R0: %d R4: %d R8: %d R12: %d \nR1: %d R5: %d R9: %d R13: %d \nR2: %d R6: %d R10: %d R14: %d \nR3: %d R7: %d R11: %d R15: %d \n",
	       r[0], r[4], r[8], r[12], r[1], r[5], r[9], r[13], r[2], r[6], r[10], r[14], r[3], r[7], r[11], r[15]);
	printf("--------------------------------\n");
}

void ilaraInprimatu(struct Node_pcb * ilara) {
	struct Node_pcb * current_node = ilara;
	while ( current_node != NULL) {
		printf("%d ", current_node->data.pid);
		current_node = current_node->next;
	}
	printf("\n");
}