# MFQ Scheduling

✅ OS_PA1

<br>

## Index
+ [Overview](#ov)
+ [Tools](#with)
+ [Prerequisites](#pre)
+ [Details](#details)

<br><br>

## Overview <a name = "ov"></a>

Implementation of MFQ scheduling policy with 4 Ready Queue.

- Q0 : RR Scheduling Policy (tq = 1)
- Q1 : RR Scheduling Policy (tq = 2)
- Q2 : RR Scheduling Policy (tq = 4)
- Q3 : FCFS Scheduling Policy
<br>
▶️

  ▪️ priority : Q0 > Q1 > Q2 > Q3
  
  ▪️ Scheduling sequence is from high priorty to low priority
  
  ▪️ If time quantum is consumed by process, Qi -> Qi+1
  
  ▪️ If process finishes IO burst and wakeup, Qi -> Qi-1
  
  ▪️ re-entering, Q3 -> Q3


<br>

## Tools <a name = "with"></a>

<a href="https://github.com/search?q=user%3ADenverCoder1+is%3Arepo+language%3Ac"><img alt="C" src="https://img.shields.io/badge/C-2370ED.svg?logo=c&logoColor=white"></a>

✔️ WSL Ubuntu

✔️ cmder


<br>

## Prerequisites <a name = "pre"></a>

|Policy|Descripton|
|------|---|
|RR|main file|
|FCFS|main file|


<br>

## Details <a name = "details"></a>

### Description of Files

|File Name|Descripton|
|------|---|
|MFQ.c|main file|
|Makefile|bash script|
|MFQ|execution file|
|input files|test cases in following [format](#inputF)|

### Compile and Execute

```
make
./MFQ input_file_name
```

### Algorithm

|MFQ|Queues|CPU|
|---|------|---|
|{ time, Queue[] }|{ scheduling, timeQuantum, front/rearNode, index }|{ Process }|
||- Q0 <br>- Q1 <br>- Q2 <br>- Q3|{ pid, Arrival/Terminal Time, Turnaround Time, Waiting Time, locationQueue, # of cycles, cpu/IO Burst Time… }|

<br>
⚫ input file로 부터 필요한 변수들 입력받고 Process, Queue 변수에 기본 값 세팅

⚫ sleep 상태의 process들에 대해 따로 I/O Burst와 관련한 queue를 사용하지 아니하고 기존 ready queue들에 process의 varrivalT 변수를 이용하여 미리 넣어주었기 때문에, stateflag를 두어 그 값이 0인지 아닌지에 따라 탐색해야 할 Queue(currentQueue)를 0으로부터 시작할지 아니면 후에 I/O Burst와 관련하여 정해진 값으로
update하지 않은 채 진행할지 결정

⚫ process가 Q0, Q1, Q2로부터 왔을 때, 각 Queue에 따른 timequantum에 따라 해당 process가 runout이 될지 timequantum 내에 cpu burst를 끝낼지가 결정됨.

후자의 경우(wakeup_ timequantum 내에 IO Burst가 끝나는 것)에는 해당 process가 완전히 종료될 수 있는지를 확인하여 endProcess 값을 update하고 이 endProcess의 값이 만약 (# of processes)와 같아진다면 해당
scheduling이 종료

⚫ process가 Q3로부터 왔을 때, 위의 Q0~3와는 다르게 각 Queue에 따른 timequantum이 존재하지 않음. 단지 arrivalTime만 중요.

따라서 해당 process의 I/O Burst 혹은 process의 종료만 고려.





### Input Format <a name="inputF"></a>
  
  ```
  (# of process)
  (pid) (arrivalTime) (initialQ) (cycle #) (CPU Burst Time(BT)) (I/O Burst Time) ... (CPU BT)
  ...
  (pid) (arrivalTime) (initialQ) (cycle #) (CPU BT) (I/O BT) ... (CPU BT)
  
  ```
  

  
### Output Format
 
  ```
*** Input ***
...

*** Initial Q States ***
...

-----------------------------------------------------------------------------

*** Gantt Chart ***
---------------------
  (time)  ||  (pid)
--------------------- -> MFQ SCHEDULING START
       t0 ||  pid0
       t1 ||  pid1
       ...
       tn }}  pidn
--------------------- -> PROCESS PIDn END

*** Turnaround Time & Waiting Time of each Process ***
Process (pid0): (pid0's Turnaround Time(TT)) (pid0's Waiting Time(WT))
...
Process (pidn): (pidn's TT) (pid0's WT)

*** Average Turnaround Time & Average Waiting Time ***
(Avg TT) (Avg WT)
  ```

- Gantt Chart : Print time and pid when each process goes into CPU from any Queue or goes out from CPU
- TT = (Process Terminate Time) - (Process Initial Arrival Time)
- WT = (Process TT) - {(Sum of Process CPU BT) + (Sum of Process I/O BT)}
