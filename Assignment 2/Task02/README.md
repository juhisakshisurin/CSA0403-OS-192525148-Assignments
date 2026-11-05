# Assignment 2 – Task 2: CPU Scheduling

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 – Operating Systems |

## 1. Task Overview

Task 2 (Q2 – CPU Scheduling, 8 Marks, K5 – Evaluate) analyses CPU scheduling for a cloud video-encoding service that handles short thumbnail jobs and long full-film jobs.

The task requires Gantt charts and performance calculations for FCFS, SJF, SRTF, Priority Scheduling and Round Robin; a working Round Robin implementation; a time-quantum investigation; starvation and ageing analysis; an MLFQ explanation; and a justified scheduling recommendation.

## 2. Given Process Data

| Process | Arrival Time | Burst Time |
|---|---:|---:|
| P1 | 0 | 8 |
| P2 | 1 | 4 |
| P3 | 2 | 9 |
| P4 | 3 | 5 |

**Priority note:** The assignment does not specify numeric priorities. The report uses an explicitly labelled illustrative assumption (lower number = higher priority): P1=3, P2=1, P3=4, P4=2. Replace these values if your instructor provides a priority table.

## 3. Files Included

| File | Description |
|---|---|
| `Assignment2_Task02_Report.pdf` | Complete report with algorithms, Gantt charts, calculations, Round Robin quantum analysis, starvation, ageing, MLFQ, recommendation and complexity analysis. |
| `round_robin.c` | Completed C implementation of the assignment's Round Robin TODO. Default quantum is 3. |
| `program_output_q3.txt` | Output produced by running the program with quantum 3. |
| `README.md` | Task documentation, student details and execution instructions. |

## 4. Requirements

| Component | Requirement |
|---|---|
| Language | C |
| Compiler | GCC |
| Operating System | Linux / Ubuntu / WSL 2 |
| Standard | C11 |

Record the actual environment used for your own execution:

```bash
gcc --version
lsb_release -a
uname -r
```

## 5. Compilation and Execution

Open Ubuntu/WSL terminal and navigate to the folder containing `round_robin.c`.

### Compile

```bash
gcc -std=c11 -Wall -Wextra round_robin.c -o round_robin
```

### Run

```bash
./round_robin
```

The program uses `quantum = 3` by default. Its TAT and WT values should match the assignment's expected output:

- P1: TAT 23, WT 15
- P2: TAT 15, WT 11
- P3: TAT 24, WT 15
- P4: TAT 18, WT 13
- Average TAT: 20.00
- Average WT: 13.50

## 6. Testing Quantum 1 and Quantum 20

The assignment specifically asks for quantum 1 and quantum 20 in addition to the supplied quantum 3. Change this line in `round_robin.c`:

```c
int quantum = 3;
```

For quantum 1, change it to:

```c
int quantum = 1;
```

Then compile and execute again:

```bash
gcc -std=c11 -Wall -Wextra round_robin.c -o round_robin
./round_robin
```

Repeat with `int quantum = 20;`. Save the outputs separately, for example `output_q1.txt` and `output_q20.txt`. Keep the quantum-3 output as `program_output_q3.txt`.

## 7. Performance Measures

- Completion Time (CT): time when a process finishes.
- Turnaround Time (TAT): CT − Arrival Time.
- Waiting Time (WT): TAT − Burst Time.
- Response Time (RT): First CPU Start Time − Arrival Time.
- Speed/quantum comparison should use the same arrival and burst data for every run.

The report counts a context switch as a transition between two different processes in consecutive CPU execution segments. The initial dispatch is not counted.

## 8. Result

The task compares five CPU scheduling algorithms and evaluates Round Robin with different time quantum values. It also analyses starvation, ageing and MLFQ to recommend a scheduling policy for a mixed short-job and long-job video-encoding workload.

## 9. Submission Reminder

The assignment requires a GitHub repository named:

`CSA04-192525148-Assignment2`

Maintain a separate folder for each task. Include the report, C source, actual program outputs and screenshots as required. Record your own GCC version and Linux distribution/version. Commit your work progressively and add the working GitHub repository link to the report cover page if required by your faculty.

