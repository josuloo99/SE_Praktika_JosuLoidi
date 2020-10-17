struct pcb {
	int pid;
};

struct processQueue{
	struct pcb *ilara;
};

void *processGenerator(void *m);
void generateProcess(int i, struct processQueue queue);