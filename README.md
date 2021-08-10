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

⚫ stack[]에 process 추적 과정을 저장

⚫ input file로부터 numProcess, numRsrcType, numRsrcUnit[], allocationMatrix[][], requestMatrix[][] 값들을 모두 받아옴

⚫ available[]에 할당이 가능한 unit 개수 저장

<br>

⚫ available[j]의 값과 Rij의 값을 반복해서 비교하여 만약 available[j]>=Rij라면 stack으로 해당 Pi (pid=i)를 push해준다.
그렇지 않다면 다음 process로 넘어가 available과 비교하는 과정을 반복한다.

⚫ 만약 stack의 크기가 N이 된다면, 이는 모든 process에 자원을 할당하는 순서가 존재한다는 뜻이므로 이는 NO-Deadlock state로 볼 수 있다.

⚫ 만약 numProcess=N이고 현재 stack의 크기가 N-3인데 더 이상 할당할 수 있는 process가 없다면, 해당 system은 deadlock인 state에 있다고 볼 수 있다.
이 때, deadlock 상태의 blocked process list는 stack의 크기가 최대일 때 stack에 포함되지 않은 process로 출력한다.


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
