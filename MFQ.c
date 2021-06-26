#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUEUE 4
#define IFLAG 0
#define NFLAG 1
#define MAX 1000


typedef struct process {         
	/* ID of each process */         
	int pid;          
	
	/* Arrival Time of each process */         
	int arrivalT;//first
	int varrivalT;//variation
	
	/* Terminated Time of each process */         
	int terminateT;          
	
	/* Turnaround Time */         
	int tt;          
	
	/* Waiting Time */         
	int wt;          
	
	/* Which Queue to be located */         
	//includes initial Queue of each process         
	int locQ;          
	
	/* # of cycles */         
	int numCycle;          
	
	/* cycles (arrival -> CPU BT - IO BT - CPU BT -... */         
	//specify remained Burst Time
	int* arrCycle;
	int currentArrIndex;

	/* total CPU/IO burst time */
	int cpuBT;
	int ioBT;	
} Process;

typedef struct node* Node;
typedef struct node {
	int data;
	Node nextptr;
};

typedef struct queue {
	/* Scheduling Scheme */
	// FCFS :0, RR :1, SPN :2, SRTN :3, HRRN :4
	int scheduling;

	/* Time Quantum of each Qs */
	int timeQuantum;

	/* Linked List for each Ready Qs */
	//including ready processes
	Node front;
	Node rear;

	/* current available index of readyQ */
	int index;

	/* pid of current process in the CPU */
	int currentPID;
} Queue;

typedef struct mfq {
	/* time */
	int time;

	/* Included Queues */
	Queue* Q;
} MFQ;


Process* process;
MFQ mfq;
int numProcess;

int setProcess(char* string);
int initProcess(int num, int a, int b, int c, int d, int* e);
int setQueue(void);
int initQueue(void);
void enqueue(Queue* q, int pid, int flag);
int dequeue(Queue* q);
Node createNode(int key);
int startScheduling(void);
int returnProcessInfo(int q);
void printGanttChartForm(void);
void printGanttChart(int pid);
void printResult(void);
void delete(void);
void checkInput();//^
void checkQueue();//^


int main(int argc, char* argv[]){

	// Set Process Information from Input File
	if(setProcess(argv[1])<0) {
		perror("> Error in Process Setting");
		return 0;
	}
	
	checkInput();//^
	
	// Set MFQ
	if(setQueue()<0) {
		perror("> Error in Queue Setting.");
		return 0;
	}

	checkQueue();//^

	// Start MFQ scheduling
	if(startScheduling()<0) {
		perror("> Error in MFQ Scheduling.");
		return 0;
	}

	// Print Output
	printResult();

	// Delete ALL Usage of Memory
	delete();

	return 0;	
}


int setProcess(char* string){
	
	/* Set input file */

	char* input_file_name=string;
	FILE* input=fopen(input_file_name, "r");
	if(!input){
		perror("Failed to open input file. Terminate");
		exit(1);
	}


	/* read input file to get process information */
	
	//# of Processes
	numProcess=0;
	if(fscanf(input, "%d", &numProcess)!=EOF){
		if(numProcess<0) {
			puts("Input number of Process is wrong.");
			return -1;
		}
		else if(numProcess==0) {
			puts("No Process.");
			return;
		}
	}
	else return -1;

	//Processes
	process=(Process*)malloc(sizeof(Process)*numProcess);
	//for(int i=0;i<numProcess;i++) process[i]=(Process*)malloc(sizeof(Process));
	int pid=0;
	int arrivalT=0;
    	int initialQ=0;
	int numCycle=0;
	int tmp=0;
	int* tmpCycle;

	char line[MAX];
	char errCheck[3];

	for(int i=0; i<numProcess;i++){	
		if(fscanf(input, "%d %d %d %d", &pid, &arrivalT, &initialQ, &numCycle)==EOF){
			puts("Insufficient Input.");
			free(process);
			return -1;
		}

		if(pid<=0 || arrivalT <0 || (initialQ<0 || initialQ>4) || numCycle<=0){
			puts("Wrong Input.");
			free(process);
			return -1;
		}

		if(fgets(line, sizeof(line), input)==NULL){
			puts("Insufficient Input.");
			free(process);
			return -1;
		}
		tmp=2*numCycle-1;
		tmpCycle=(int*)malloc(sizeof(int)*tmp);
		for(int j=0;j<tmp;j++) tmpCycle[j]=-1;

		char* ptr;
		for(int j=0; j<tmp; j++){
			if(j==0){
				if((ptr=strtok(line, " "))==NULL){
					puts("Insufficient Input.");
					free(tmpCycle);
					free(process);
					return -1;
				}
			}
			else{
				if((ptr=strtok(NULL, " "))==NULL){
					puts("Insufficient Input.");
					free(tmpCycle);
					free(process);
					return -1;
				}
			}
			
			tmpCycle[j]=atoi(ptr);
			//printf("%d", tmpCycle[j]);

			if(tmpCycle[j]<=0){
				puts("Wrong Input.");
				free(tmpCycle);
				free(process);
				return -1;
			}
			else if(j==0 && tmpCycle[j]==-1){
				puts("Wrong Input : No CPU BT");
				free(tmpCycle);
				free(process);
				return -1;
			}
		}

		if((ptr=strtok(NULL, " "))!=NULL){              
		     	//printf("%s**\n",errCheck);//^
			puts("Too much Input.");
			free(tmpCycle);
			free(process);
			return -1;  
	 	}

		if(initProcess(i, pid, arrivalT, initialQ, numCycle, tmpCycle)<0) {
			free(tmpCycle);
			free(process);		
			return -1;
		}

		free(tmpCycle);

	}

	
	if(fscanf(input, "%c", &errCheck[0])!=EOF){        
       		puts("Too much Processes.");
		return -1;                 
	}

	return 0;
}

int initProcess(int num, int a, int b, int c, int d, int* e){         
	process[num].pid=a;         
	process[num].arrivalT=b;
	process[num].varrivalT=b;	
	process[num].terminateT=0;         
	process[num].tt=0;         
	process[num].wt=0;         
	process[num].locQ=c;         
	process[num].numCycle=d;
	process[num].cpuBT=0;
	process[num].ioBT=0;	
	process[num].arrCycle=(int*)malloc(sizeof(int)*(2*d-1));         
	process[num].currentArrIndex=0;
	for(int i=0;i<2*d-1;i++){                 
		process[num].arrCycle[i]=e[i];
		
		if(i%2!=0){
			process[num].cpuBT+=e[i];
		}
		else{
			process[num].ioBT+=e[i];
		}		
	}	
	
	return 0; 
}


int setQueue(void){

	if(initQueue()==-1) {
		free(mfq.Q);
		return -1;
	}

	/* Set processes in order to Initial Q */
	int initialQ;
	for(int i=0; i<numProcess; i++){
		initialQ=process[i].locQ;
		
		switch(initialQ){
			case 0:
				enqueue(&mfq.Q[0], process[i].pid, IFLAG);
				break;

			case 1:
				enqueue(&mfq.Q[1], process[i].pid, IFLAG);
				break;

			case 2:
				enqueue(&mfq.Q[2], process[i].pid, IFLAG);
				break;

			case 3:
				enqueue(&mfq.Q[3], process[i].pid, IFLAG);
				break;
		}
	}

	return 0;
}

int initQueue(void){
	mfq.time=0;

	mfq.Q=(Queue*) malloc(sizeof(Queue)*QUEUE);

	mfq.Q[0].scheduling=1;//RR
	mfq.Q[1].scheduling=1;
	mfq.Q[2].scheduling=1;
	mfq.Q[3].scheduling=0;//FCFS

	mfq.Q[0].timeQuantum=1;
	mfq.Q[1].timeQuantum=2;
	mfq.Q[2].timeQuantum=4;
	mfq.Q[3].timeQuantum=-1;//null

	mfq.Q[0].front=NULL;
	mfq.Q[1].front=NULL;
	mfq.Q[2].front=NULL;
	mfq.Q[3].front=NULL;
	mfq.Q[0].rear=NULL;
	mfq.Q[1].rear=NULL;
	mfq.Q[2].rear=NULL;
	mfq.Q[3].rear=NULL;

	mfq.Q[0].index=0;
	mfq.Q[1].index=0;
	mfq.Q[2].index=0;
	mfq.Q[3].index=0;

	// 0 means nothing FROM THAT QUEUEin the CPU
	mfq.Q[0].currentPID=0;         
	mfq.Q[1].currentPID=0;
	mfq.Q[2].currentPID=0;         
	mfq.Q[3].currentPID=0;

	return 0;
}


void enqueue(Queue* q, int pid, int flag){
	Node new=createNode(pid);

	/* If Empty Queue, Add Node */
	if((q->front==NULL && q->rear==NULL) || q->index==0){
		q->front=new;
		q->rear=new;
	}

	/* Otherwise, Insert to Linked List by priority(Arrival Time) */
	else {		
		// enqueue from given processes in DESC order (arrival time)
		if(flag==IFLAG){
			int srcPID=pid;
			int targetPID;
			int srcIndex, targetIndex;
			Node tmp=q->front;
			Node tmpBefore=NULL;

			//find source process's array index
			for(int i=0;i<numProcess;i++){
				if(process[i].pid==srcPID){
					srcIndex=i;
					break;
				}
			}

			while(1){
				targetPID=tmp->data;
				
				//find target process's array index
				for(int i=0; i<numProcess;i++){
					if(process[i].pid==targetPID){
						targetIndex=i;
						break;
					}
				}

				//insert among the linked list
				if(process[srcIndex].varrivalT < process[targetIndex].varrivalT){
					new->nextptr=tmp;
					if(tmpBefore!=NULL) {
						tmpBefore->nextptr=new;
					}
					else q->front=new;
					break;
				}
				//move on to next node
				else {
					tmpBefore=tmp;
					
					if(tmp!=q->rear) {
						tmp=tmp->nextptr;
					}

					else {
						q->rear->nextptr=new;
						q->rear=new;
						break;
					}
				}
			}
		}

		else if(flag==NFLAG){
			q->rear->nextptr=new;
			q->rear=new;
		}
	}

	q->index++;
	//printf("%d %d \n", q->front->data, q->rear->data);//^
}

int dequeue(Queue* q){
	int tmp;

	if((q->front==NULL && q->rear==NULL) || q->index==0){
		if(q->index==0){         
			q->front=NULL;        
		       	q->rear=NULL; 
		}

		puts("Queue is Empty");
		exit(1);
	}
	else{
		Node del=(Node)malloc(sizeof(struct node));
		
		del=q->front;
		q->front=q->front->nextptr;
		tmp=del->data;

		free(del);
		
		q->index--;
	}

	//return dequeued element(process)
	return tmp;
}


Node createNode(int key){
	Node new=(Node)malloc(sizeof(struct node));
	new->data=key;
	new->nextptr=NULL;

	return new;
}


int startScheduling(void){
	printGanttChartForm();

	int turn=0;
	int currentState[4]={-1,-1,-1,-1};//If not -1 -> specific arrivalT at Queue of I/O state Process
	int stateflag=0;//If positive value, then change value
	int minState=0;

	int endProcess=0;
	int timeQuantum=0;
	int currentQueue;
	int currentProcessIndex;
	int tmpArrIndex;
	int cpuBT;
	int ioBT;

	/* Scheduling Process according to Time */
	while(1){

		if(stateflag==0) {
			currentQueue=0;

			//Q0 -> Q1 -> Q2 -> Q3
			while(mfq.Q[currentQueue].index==0){
				currentQueue++;
	
				if(currentQueue>3) {
					perror("What's wrong in Scheduling");
					return -1;	
					break;
				}
			}
		}
		else{
			stateflag=0;
			currentState[0]=currentState[1]=currentState[2]=currentState[3]=-1;
		}


		timeQuantum=mfq.Q[currentQueue].timeQuantum;
		currentProcessIndex=returnProcessInfo(currentQueue);

		//Set MFQ Time
		//first-CPU-entered process 
		if(turn==0) {         
			mfq.time=process[currentProcessIndex].varrivalT;	
		}


		//puts("a");
		/* Check if it is while I/O Burst Time */
		if(process[currentProcessIndex].varrivalT > mfq.time){
			
			int t0, t1, t2, t3;
			if(mfq.Q[0].index!=0) {
				t0=returnProcessInfo(0);
				currentState[0]=process[t0].varrivalT;
			}
			if(mfq.Q[1].index!=0) {
				t1=returnProcessInfo(1);
				currentState[1]=process[t1].varrivalT;
			}
			if(mfq.Q[2].index!=0) {
				t2=returnProcessInfo(2);
				currentState[2]=process[t2].varrivalT;
			}
			if(mfq.Q[3].index!=0) {
				t3=returnProcessInfo(3);
				currentState[3]=process[t3].varrivalT;
			}

			for(int i=0;i<QUEUE;i++){
				//Other processes which will consecutely follow exists
				if(currentState[i]!=-1 && currentState[i]<=mfq.time) {
					stateflag=-1;
					currentQueue=i;
					break;
				}
			}
			if(stateflag==-1) continue;

			else{
				//Find early-wakeup-process
				minState=-1;
				for(int i=0;i<QUEUE;i++){
					if(currentState[i]!=-1 && (minState == -1 || minState > currentState[i])){  
 						minState=currentState[i]; 
						currentQueue=i;     
    					}         
				}           
	 			
				if(minState!=-1){    
    					mfq.time=minState;
					stateflag=1;	
				}
				else{
					perror("What's wrong with Sleep Process");
					return -1;
				}

				continue;
			}
		}

		//dequeue from Q(i)        
		dequeue(&mfq.Q[currentQueue]);
		
		//print entry of new process       
		printGanttChart(process[currentProcessIndex].pid);
			
		//Get Current Process CPU Burst Time
		tmpArrIndex=process[currentProcessIndex].currentArrIndex;
		cpuBT=process[currentProcessIndex].arrCycle[tmpArrIndex];

		switch(currentQueue){
			case 0:
			case 1:
			case 2:
				/* To next Queue */
				if(cpuBT > timeQuantum){
					//print runout time of current process
					mfq.time+=timeQuantum;
					printGanttChart(process[currentProcessIndex].pid);
				
					//update CPUBT to remaining amount
					process[currentProcessIndex].arrCycle[tmpArrIndex]-=timeQuantum;
			
					process[currentProcessIndex].varrivalT+=timeQuantum;

					//Q(i) -> Q(i+1)
					//enqueue to Q(i+1)
					if(currentQueue!=3){
						enqueue(&mfq.Q[currentQueue+1], process[currentProcessIndex].pid, IFLAG);
						puts("--------------------");
					}

					//Q(3) -> Q(3)
					else{
						enqueue(&mfq.Q[currentQueue], process[currentProcessIndex].pid, IFLAG);
						puts("--------------------");
					}
				}

				/* To I/O Burst OR Process-Run ends */
				else {
					//set time according to process-used time then print
					mfq.time+=process[currentProcessIndex].arrCycle[tmpArrIndex];
					printGanttChart(process[currentProcessIndex].pid);
					process[currentProcessIndex].arrCycle[tmpArrIndex]=0;

					// To I/O Bursti
					if(process[currentProcessIndex].currentArrIndex+1 < (2*process[currentProcessIndex].numCycle-1)){
						process[currentProcessIndex].currentArrIndex++;
						tmpArrIndex=process[currentProcessIndex].currentArrIndex;
						ioBT=process[currentProcessIndex].arrCycle[tmpArrIndex];
						process[currentProcessIndex].varrivalT=mfq.time+ioBT;
						process[currentProcessIndex].currentArrIndex++;

						//Q(i) -> Q(i-1)
						//enqueue to Q(i-1)
						if(currentQueue!=0){
							enqueue(&mfq.Q[currentQueue-1], process[currentProcessIndex].pid, IFLAG);
							puts("--------------------");
						}
						//Q(0) -> Q(0)
						else{
							enqueue(&mfq.Q[currentQueue], process[currentProcessIndex].pid, IFLAG);
							puts("--------------------");
						}
					}

					// Process-Run End
					else if(process[currentProcessIndex].currentArrIndex+1==(2*process[currentProcessIndex].numCycle-1)){
						printf("-------------------- -> PROCESS %d END\n", process[currentProcessIndex].pid);
						process[currentProcessIndex].terminateT=mfq.time;
						endProcess++;
					}
				}

				turn++;

				break;

			case 3:
				//set time according to process-used time then print
				mfq.time+=process[currentProcessIndex].arrCycle[tmpArrIndex];
				printGanttChart(process[currentProcessIndex].pid); 
				process[currentProcessIndex].arrCycle[tmpArrIndex]=0;

				// To I/O Burst            
				if(process[currentProcessIndex].currentArrIndex +1< (2*process[currentProcessIndex].numCycle-1)){
					process[currentProcessIndex].currentArrIndex++;
					tmpArrIndex=process[currentProcessIndex].currentArrIndex;
					ioBT=process[currentProcessIndex].arrCycle[tmpArrIndex]; 
					process[currentProcessIndex].varrivalT=mfq.time+ioBT;
					process[currentProcessIndex].currentArrIndex++;  
					
  					enqueue(&mfq.Q[currentQueue], process[currentProcessIndex].pid, IFLAG);
					puts("--------------------");
				}
		 		
				//Process-Run Ends		
   				else if(process[currentProcessIndex].currentArrIndex+1==(2*process[currentProcessIndex].numCycle-1)){
					printf("-------------------- -> PROCESS %d END\n", process[currentProcessIndex].pid);		
					process[currentProcessIndex].terminateT=mfq.time;
					endProcess++;                                               
				}

				turn++;

				break;
			
			default:
				perror("Scheduling Error.");
				return -1;
		}


		//scheduling end condition
		if(endProcess==numProcess) break;
	}

	return 0;
}

int returnProcessInfo(int q){
	int retVal;
	int tmpData=mfq.Q[q].front->data;//pid
	for(int i=0; i<numProcess;i++){
		if(process[i].pid==tmpData){
			retVal=i;    
			break;    
		}    
	}

	return retVal;
}


void printGanttChartForm(void){
	/* output by OStream*/
	printf("================================================================\n\n");

	//* Gantt chart *//
	// current CPU Burst Process information : when and what
	printf("*** Gantt Chart ***\n");
	
	//print format
	puts("--------------------");
	printf("  (time) ||  (pid)  \n");
	puts("-------------------- -> MFQ SCHEDULING START");
}

void printGanttChart(int pid){
	printf("%9d||%9d\n", mfq.time, pid);
}

void printResult(void){
	//*TT, WT*//
	printf("\n*** Turnaround Time & Waiting Time of each Process ***\n");
	int sumTT=0;
	int sumWT=0;
	float avgTT, avgWT;
	for(int i=0;i<numProcess;i++){                 
		process[i].tt=process[i].terminateT-process[i].arrivalT; 
     		sumTT+=process[i].tt;

		process[i].wt=process[i].tt-process[i].cpuBT-process[i].ioBT;
		sumWT+=process[i].wt;	
		
		printf("Process %d : %d %d\n", process[i].pid, process[i].tt, process[i].wt);
	}
	puts("");
	
	printf("*** Average Turnaround Time & Average Waiting Time ***\n");
	avgTT=(float)sumTT/numProcess;
	avgWT=(float)sumWT/numProcess;
	printf("%.3f %.3f\n", avgTT, avgWT);
}


void delete(void){
	
	//delete process
	for(int i=0;i<numProcess;i++) free(process[i].arrCycle);
	free(process);

	//delete Queue
	free(mfq.Q);
}


void checkInput(){
	printf("*** INPUT ***\n");
	printf("# of process : %d\n", numProcess);
	for(int i=0;i<numProcess;i++){
		printf("pid : %d,   arrival time : %d,    initialQ : %d,   # of cycles : %d\n", process[i].pid, process[i].arrivalT, process[i].locQ, process[i].numCycle);
	}
	puts("");
}

void checkQueue(){

	puts("*** Initial Q States ***");
	puts("front<------------->rear");

	Node tmp=NULL;

	for(int i=0;i<QUEUE;i++){

		printf("Q%d :  ", i);
		tmp=mfq.Q[i].front;
		for(int j=0;j<mfq.Q[i].index;j++){
			printf("%d ", tmp->data);
			tmp=tmp->nextptr;
		}
		puts("");
	}
	puts("");
}
