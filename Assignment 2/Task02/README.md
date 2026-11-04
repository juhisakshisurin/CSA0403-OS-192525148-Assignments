# Assignment 2 – Task 2: CPU Scheduling

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 – Operating Systems |
| **Assignment** | Assignment 2 – CO2 |
| **Task** | Task 2 – CPU Scheduling |
| **Date** | 03-11-2026 |
| **Under the Supervision of** | Dr. Velmurugan J |

---

## 1. Introduction

CPU Scheduling is an essential function of an operating system that determines which process gets access to the CPU and for how long.

This task focuses on analysing different CPU scheduling algorithms in the context of a cloud video-encoding service. It examines how scheduling decisions affect process waiting time, turnaround time, response time, fairness and context-switch overhead.

## 2. Aim

To study, analyse and compare different CPU scheduling algorithms, calculate their performance measures, implement Round Robin scheduling and identify a suitable scheduling approach for a cloud video-encoding workload.

## 3. Objectives

- Understand the working principles of CPU scheduling algorithms.
- Construct Gantt charts for process execution.
- Calculate completion time, turnaround time, waiting time and response time.
- Compare scheduling algorithms using their performance measures.
- Implement Round Robin scheduling using C.
- Analyse the effect of different time quantum values.
- Understand starvation and the ageing technique.
- Study Multilevel Feedback Queue scheduling.
- Recommend a suitable scheduling policy for the given workload.

## 4. Topics Covered

The report covers the following CPU scheduling algorithms and concepts:

| Topic | Description |
|---|---|
| First Come First Serve (FCFS) | Schedules processes according to their arrival order. |
| Shortest Job First (SJF) | Selects the available process with the shortest CPU burst time. |
| Shortest Remaining Time First (SRTF) | Selects the process with the shortest remaining execution time. |
| Priority Scheduling | Selects processes according to their assigned priorities. |
| Round Robin (RR) | Allocates CPU time using a fixed time quantum. |
| Starvation | Examines indefinite waiting of processes. |
| Ageing | Explains priority adjustment to reduce starvation. |
| Multilevel Feedback Queue (MLFQ) | Examines scheduling using multiple priority queues. |

## 5. Performance Measures

The following parameters are used to evaluate scheduling performance:

| Parameter | Formula / Meaning |
|---|---|
| Completion Time (CT) | Time at which a process finishes execution. |
| Turnaround Time (TAT) | Completion Time − Arrival Time |
| Waiting Time (WT) | Turnaround Time − Burst Time |
| Response Time (RT) | First CPU Start Time − Arrival Time |
| Average Waiting Time | Total Waiting Time / Number of Processes |
| Average Turnaround Time | Total Turnaround Time / Number of Processes |

## 6. Round Robin Implementation

The assignment includes a Round Robin scheduling implementation using the C programming language.

The implementation uses process arrival times, burst times, remaining burst times and a ready queue to simulate CPU allocation.

The program is analysed using the time quantum values specified in the assignment:

- Quantum = 1
- Quantum = 3
- Quantum = 5
- Quantum = 20

The results are compared using average waiting time and context switches.

## 7. System Requirements

| Component | Requirement |
|---|---|
| Programming Language | C |
| Compiler | GCC |
| Operating System | Ubuntu Linux / WSL 2 |
| Platform | Linux-compatible environment |
| Implementation | Round Robin CPU Scheduling |

## 8. Compilation and Execution

If the Round Robin source code is maintained as a separate file, use the following commands from its project directory.

### Compile

```bash
gcc -Wall -Wextra -std=c11 cpu_scheduling.c -o cpu_scheduling
```

### Execute

```bash
./cpu_scheduling
```

**Note:** Update the source filename or executable name if your actual implementation uses a different name.

## 9. Files Included

| File Name | Description |
|---|---|
| `Task2_Report.pdf` | Complete academic report containing scheduling explanations, calculations, Gantt charts, comparisons and conclusions. |
| `README.md` | Documentation describing the task, objectives, concepts and report contents. |

**Note:** If the Round Robin implementation is submitted separately, include its actual C source code and execution evidence in the Task 2 folder as required by the assignment.

## 10. Expected Outcome

The task aims to demonstrate an understanding of CPU scheduling algorithms and their impact on operating system performance.

It also develops the ability to calculate scheduling metrics, implement Round Robin scheduling, analyse time quantum values and evaluate starvation-prevention techniques.

## 11. Result

The CPU scheduling algorithms and their performance characteristics are analysed through scheduling calculations, Gantt charts and comparative evaluation. Round Robin scheduling is studied through C implementation, and a suitable scheduling approach is recommended for the given cloud video-encoding workload based on the analysis.

---

**Course:** CSA0403 – Operating Systems  
**Assignment:** Assignment 2 – CO2  
**Task:** Task 2 – CPU Scheduling
