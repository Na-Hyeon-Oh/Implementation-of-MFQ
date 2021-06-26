#include <stdio.h>
#include <stdlib.h>

typedef struct process {
	/* ID of each process */
	int pid;

	/* Arrival Time of each process */
	int arrivalT;

	/* Terminated Time of each process */
	int termT;

	/* Turnaround Time */
	int tt;

	/* Waiting Time */
	int wt;

	/* Which Queue to be located */
	//includes initial Queue of each process
	int locQ;

	/* # of cycles */
	int numCycle;

	/* cycles (arrival - CPU BT - IO BT - CPU BT -... */
	int* arrCycle;

} Process;


int initProcess(int a, int b, int c, int d, int* e){
	process[a].pid=a+1;
	process[a].arrivalt=b;
	process[a].termT=0;
	process[a].tt=0;
	process[a].wt=0;
	process[a].locQ=c;
	process[a].numCycle=d;
	process[a].arrCycle=(int*)malloc(sizeof(int)*(2*d-1));
	for(int i=0;i<2*d-1;i++){
		process[a].arrCycle[i]=e[i];
	}

	return 0;
}
