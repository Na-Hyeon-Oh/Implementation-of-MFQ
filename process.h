#include <stdio.h> 

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

int initProcess(int a, int b, int c, int d, int* e);
