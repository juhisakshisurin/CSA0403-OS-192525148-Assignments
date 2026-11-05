# ASSIGNMENT 2 – OPERATING SYSTEMS (CO2)
## PROCESS MANAGEMENT, CPU SCHEDULING, SYNCHRONISATION AND DEADLOCK HANDLING

## Student Details

| **Field** | **Details** |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 – Operating Systems |
| **Assignment** | Assignment 2 – CO2 |
| **Nature of Assignment** | Individual |
| **Duration** | One Week |
| **Total Marks** | 50 |
| **Bloom's Level** | K5 – Evaluate |

---

# 1. Assignment Overview

Assignment 2 focuses on analysing process management and deadlock-handling techniques to improve CPU utilisation and overall system performance.

The assignment covers the following major operating-system concepts:

- Processes and process states.
- CPU scheduling algorithms.
- Threads and multithreading.
- Inter-process communication (IPC).
- Synchronisation using critical sections, semaphores and monitors.
- Deadlock handling and the Banker's algorithm.

The assignment uses practical scenarios such as online examination servers, cloud video-encoding services, traffic-camera processing, weather-station pipelines, railway ticket booking systems and shared laboratory equipment.

Each task requires technical understanding, implementation, experimental investigation, alternative-concept analysis and a justified engineering decision.

Every task is assessed at Bloom's Taxonomy Level K5 – Evaluate.

The expected engineering workflow is:

**MODEL → DESIGN → IMPLEMENT → MEASURE → COMPARE → EVALUATE → RECOMMEND**

---

# 2. Assignment Objectives

The main objectives are:

1. Understand process creation, execution and termination.
2. Evaluate CPU scheduling algorithms using waiting time, turnaround time and response time.
3. Analyse multithreading and its effect on parallel processing.
4. Implement and compare inter-process communication techniques.
5. Apply synchronisation mechanisms to protect shared resources.
6. Understand the necessary conditions for deadlock.
7. Implement the Banker's safety and resource-request algorithms.
8. Measure the performance of operating-system mechanisms.
9. Compare alternative approaches using time and space complexity.
10. Recommend appropriate process-management and deadlock-handling strategies.

---

# 3. Marks Distribution

| **Task** | **Assessed Area** | **Marks** | **Bloom's Level** |
|---|---|---:|---|
| Q1 | Processes and Process States | 6 | K5 – Evaluate |
| Q2 | CPU Scheduling | 8 | K5 – Evaluate |
| Q3 | Threads | 6 | K5 – Evaluate |
| Q4 | Inter-Process Communication | 6 | K5 – Evaluate |
| Q5 | Synchronisation: Critical Sections, Semaphores and Monitors | 11 | K5 – Evaluate |
| Q6 | Deadlock Handling and the Banker's Algorithm | 13 | K5 – Evaluate |
| | **TOTAL** | **50** | |

---

# 4. Q1 – Processes and Process States (6 Marks)

## Problem Context – Online Examination Server

An online examination server runs a separate process for every student's session, along with processes for automatically saving answers and recording proctoring videos.

When 2,000 students log in simultaneously, the server becomes slow and some sessions appear to hang.

## Implementation Tasks

- Draw the process-state diagram.
- Explain the following process states:
  - New.
  - Ready.
  - Running.
  - Waiting.
  - Terminated.
- Draw and explain the Process Control Block (PCB).
- Write a C program that creates child processes using `fork()`.
- Observe the processes using `ps` and `/proc`.

## Evaluation

Evaluate where the system spends time when 2,000 processes are active.

Consider:

- Context-switching cost.
- Memory required per process.
- Time spent waiting for I/O.
- CPU utilisation.
- Process-management overhead.
- Scalability and system performance.

## Design Decision

Compare the following designs:

1. One process per student.
2. One thread per student.
3. A fixed pool of worker processes.

Recommend the most suitable design for the examination server and justify the decision.

## Alternative Concept and Complexity Analysis

Evaluate using one thread per student inside a single server process instead of creating separate processes.

Compare:

- Context-switching time.
- Memory required per session.
- Separate process address spaces versus shared address spaces.
- Effect of one session crashing.
- Isolation and resource sharing.

## Expected Evidence

- Process-state diagram.
- PCB diagram and explanation.
- Working C program using `fork()`.
- Process IDs and parent process IDs.
- `ps` command output.
- `/proc` process information.
- Process-versus-thread comparison.
- Defended design decision.

---

# 5. Q2 – CPU Scheduling (8 Marks)

## Problem Context – Cloud Video-Encoding Service

A video service receives two categories of jobs:

- Short jobs such as thumbnail generation, taking approximately 2 seconds.
- Long jobs such as full-film encoding, taking approximately 20 minutes.

Customers complain that short jobs wait behind long jobs, while the company wants to keep processors fully utilised.

## Implementation Tasks

Calculate and compare the following CPU scheduling algorithms:

1. First Come First Serve (FCFS).
2. Shortest Job First (SJF).
3. Shortest Remaining Time First (SRTF).
4. Priority Scheduling.
5. Round Robin (RR).

For each algorithm, calculate:

- Gantt chart.
- Waiting time.
- Turnaround time.
- Response time.
- Average waiting time.
- Average turnaround time.
- Average response time.

Implement the Round Robin algorithm in C.

## Given Round Robin Test Data

The supplied program uses the following process information:

| **Process** | **Arrival Time** | **Burst Time** |
|---|---:|---:|
| P1 | 0 | 8 |
| P2 | 1 | 4 |
| P3 | 2 | 9 |
| P4 | 3 | 5 |

Default time quantum:

```c
int quantum = 3;
```

Priority values for the separate Priority Scheduling calculation must be taken from the applicable problem data. If no priority values are supplied, clearly state any illustrative assumptions instead of presenting them as given values.

## Evaluation

### A. Time Quantum Investigation

Evaluate Round Robin using:

- Quantum = 1.
- Quantum = 3.
- Quantum = 5.
- Quantum = 20.

Compare:

- Average waiting time.
- Average turnaround time.
- Response time.
- Number of context switches.
- Scheduling overhead.

### B. Starvation and Ageing

Explain:

- Why starvation can occur in SJF and Priority Scheduling.
- How ageing gradually increases the priority of waiting processes.
- How ageing helps prevent indefinite waiting.

### C. Multilevel Feedback Queue

Explain how a Multilevel Feedback Queue (MLFQ) handles short and long jobs.

Evaluate the use of:

- Shorter time quantum for new jobs.
- Longer time quantum for CPU-intensive jobs.
- Movement between priority queues.
- Starvation prevention.

## Design Decision

Recommend the most suitable CPU scheduling policy for the cloud video-encoding service using calculated averages and performance comparisons.

## Complete the Code – TODO 1

Complete the supplied Round Robin C program.

The implementation must:

1. Select the next process from the ready queue.
2. Execute it for `min(quantum, left[p])` time units.
3. Print the corresponding Gantt-chart segment.
4. Advance the simulation time.
5. Reduce the process's remaining burst time.
6. Add newly arrived processes to the queue.
7. Reinsert the current process if it has remaining execution time.
8. Record its finishing time when completed.
9. Continue until every process finishes.

## Expected Output – Quantum 3

```text
Gantt: P1(0-3) P2(3-6) P3(6-9) P4(9-12) P1(12-15) P2(15-16) P3(16-19) P4(19-21)
P1(21-23) P3(23-26)

Process TAT WT
P1        23 15
P2        15 11
P3        24 15
P4        18 13

Average TAT = 20.00, average WT = 13.50
```

Run the program again with quantum 1 and quantum 20 and compare the results.

## Alternative Concept and Complexity Analysis

Evaluate using a Multilevel Feedback Queue instead of a single Round Robin queue.

Compare:

- Average waiting time for short jobs.
- Average waiting time for long jobs.
- Scheduling overhead.
- Starvation risk.
- Scheduling cost per decision.

The assignment highlights the comparison between O(1) queue operations and O(n) selection for SJF.

## Expected Evidence

- Gantt charts for all five scheduling algorithms.
- Waiting-time calculations.
- Turnaround-time calculations.
- Response-time calculations.
- Completed Round Robin C program.
- Quantum 1, 3, 5 and 20 comparison.
- Context-switch analysis.
- Starvation and ageing explanation.
- MLFQ explanation.
- Final scheduling recommendation.
- Alternative-concept and complexity analysis.

---

# 6. Q3 – Threads (6 Marks)

## Problem Context – Traffic-Camera Number-Plate Reader

A city traffic system processes number-plate information from 64 cameras.

The initial version processes the cameras sequentially and cannot keep up during peak hours.

## Implementation Tasks

Use POSIX threads to process camera frames using:

1. One thread per camera (64 threads).
2. A pool of 4 threads.
3. A pool of 8 threads.
4. A pool of 16 threads.

Determine:

- Which resources are shared between threads.
- Which resources are private to each thread.
- Differences between user-level and kernel-level threads.
- Many-to-one thread model.
- One-to-one thread model.
- Many-to-many thread model.
- Speed-up obtained on the executing machine.
- Why speed-up stops increasing.
- What happens when multiple threads update a shared counter without protection.

## Evaluation

Analyse:

- Thread creation overhead.
- Context-switching cost.
- CPU-core availability.
- Memory usage.
- Synchronisation overhead.
- Workload distribution.
- Scalability and parallel processing.

## Alternative Concept and Complexity Analysis

Evaluate processing camera frames using several processes instead of threads.

Compare:

- Process creation time.
- Thread creation time.
- Context-switching time.
- Separate address spaces versus shared memory.
- Inter-process communication versus shared variables.
- Memory requirements.
- Performance and isolation.

## Expected Evidence

- Working POSIX multithreaded C program.
- One-thread-per-camera implementation.
- 4-thread pool.
- 8-thread pool.
- 16-thread pool.
- Measured execution times.
- Speed-up calculations.
- Shared and private thread-resource comparison.
- User-level and kernel-level thread models.
- Shared-counter synchronisation demonstration.
- Process-versus-thread evaluation.

**Measurement note:** The actual camera images and recognition model are not supplied by the assignment. Where a synthetic workload is used for benchmarking, identify it clearly as a simulation rather than actual number-plate recognition.

---

# 7. Q4 – Inter-Process Communication (6 Marks)

## Problem Context – Weather Station Data Pipeline

A weather station contains:

- One process that reads sensor measurements.
- Another process that analyses the readings.

The processes must exchange data continuously without losing readings.

## Implementation Tasks

Implement communication using:

1. A pipe.
2. Shared memory or a message queue.

Determine:

- How a pipe signals the end of data.
- Difference between shared-memory and message-passing models.
- Which method copies the data.
- Which method avoids repeated payload copying.
- Which method requires additional synchronisation and why.

## Complete the Code – TODO 1

Complete the supplied C program using a pipe.

The parent process sends sensor readings to the child process.

The child must:

1. Read one integer at a time from the pipe.
2. Continue reading until `read()` returns 0.
3. Maintain the total number of readings.
4. Calculate the sum.
5. Find the maximum reading.
6. Calculate the average.
7. Display the final result.

The parent must close its write descriptor after sending all readings.

## Given Sensor Readings

```c
int readings[] = {31, 33, 36, 34, 41, 38};
```

## Expected Output

```text
analyser: 6 readings, average 35.5, max 41
reader: done
```

## Additional Investigation – End of Data

Remove:

```c
close(fd[1]);
```

from the parent process.

Explain why the child does not receive EOF and may remain blocked in `read()` while the parent waits for it.

## Growth Investigation

Measure transfer times for:

- 1 MB.
- 10 MB.
- 100 MB.

Compare the pipe and shared-memory/message-queue implementations.

## Evaluation

Discuss:

- Data-transfer overhead.
- Kernel involvement.
- Payload copying.
- Synchronisation.
- Transfer performance.
- Data integrity.
- Scalability.

## Alternative Concept and Complexity Analysis

Evaluate exchanging readings through shared memory with a semaphore instead of a pipe.

Compare:

- Data-transfer time.
- Two kernel-mediated copies in a conventional pipe path versus direct shared-memory access to the payload.
- Synchronisation requirements.
- Time complexity for transferring n readings.
- Space required for the communication buffer.

## Expected Evidence

- Completed pipe C program.
- Expected output verification.
- Shared-memory or message-queue implementation.
- EOF explanation.
- 1 MB, 10 MB and 100 MB measurements.
- Pipe versus shared-memory comparison.
- Alternative-concept and complexity analysis.
- Justified IPC recommendation.

---

# 8. Q5 – Synchronisation: Critical Sections, Semaphores and Monitors (11 Marks)

## Problem Context – Railway Ticket Booking

Thousands of booking agents reserve seats on the same trains simultaneously.

Without synchronisation, the same seat may be sold twice. Using one global lock can make the booking system very slow.

## Design Starting Principle

A correct solution to the critical-section problem must guarantee:

1. Mutual Exclusion.
2. Progress.
3. Bounded Waiting.

## Implementation and Evaluation Tasks

### A. Critical-Section Problem

Explain:

- Mutual exclusion.
- Progress.
- Bounded waiting.

Evaluate why these properties are required for a railway ticket-booking system.

### B. Peterson's Solution

Explain:

- How Peterson's solution works for two processes.
- How it satisfies the three critical-section requirements under its assumptions.
- Why the classical implementation may fail on modern processors without appropriate memory-ordering guarantees.

### C. Semaphores

Explain:

- Binary semaphores.
- Counting semaphores.
- `wait()`.
- `signal()`.

Evaluate how semaphores protect a shared railway seat counter.

### D. Classic Synchronisation Problems

Explain how semaphores solve:

1. Bounded-Buffer (Producer–Consumer) Problem.
2. Readers–Writers Problem.
3. Dining-Philosophers Problem.

### E. Monitors

Explain how monitors and condition variables help protect shared resources and simplify synchronisation.

## Bounded-Buffer Implementation

The supplied C program must be completed using semaphores and a mutex.

### Complete the Code – TODO 1: Producer

The producer must:

1. Wait on the `empty` semaphore.
2. Lock the mutex.
3. Insert an item at `buffer[in]`.
4. Advance the `in` index circularly.
5. Unlock the mutex.
6. Post the `full` semaphore.

### Complete the Code – TODO 2: Consumer

The consumer must:

1. Wait on the `full` semaphore.
2. Lock the mutex.
3. Retrieve an item from `buffer[out]`.
4. Advance the `out` index circularly.
5. Unlock the mutex.
6. Post the `empty` semaphore.
7. Add the consumed item to `consumed_sum`.

Compile the program with `-pthread`.

## Expected Output

For the original 20-item test:

```text
items consumed: 20, sum = 210 (expected 210)
```

## Test Your Design

Run the bounded-buffer program with:

- 1 producer and 1 consumer.
- 4 producers and 4 consumers.

Demonstrate that every produced item is consumed exactly once.

Run the original 20-item program 100 times and verify that the sum is always 210.

Then remove the mutex and evaluate whether the result remains correct with multiple producers and consumers.

## Deployment Decision – Lock Granularity

Compare:

1. One global lock for the entire booking system.
2. One lock per train.
3. One lock per coach.

Measure the throughput of each approach and defend the most suitable design.

Evaluate:

- Contention.
- Parallelism.
- Locking overhead.
- Throughput.
- Correctness.
- Resource sharing.

## Alternative Concept and Complexity Analysis

Evaluate protecting the shared counter using:

- C11 atomic operations (`atomic_fetch_add`).
- A spin lock.
- A semaphore and mutex.

Measure using:

- 1 thread.
- 4 threads.
- 16 threads.

Compare:

- Time per update.
- CPU time wasted while spinning.
- Synchronisation overhead.
- Extra space.
- Performance for long critical sections.

## Expected Evidence

- Critical-section properties explanation.
- Peterson's algorithm.
- Binary and counting semaphore explanation.
- Producer–consumer implementation.
- Readers–writers explanation.
- Dining-philosophers explanation.
- Monitor and condition-variable explanation.
- Completed TODO 1 and TODO 2.
- 1P/1C and 4P/4C tests.
- Every-item-exactly-once verification.
- 100-run verification.
- Mutex-removal investigation.
- Global, per-train and per-coach throughput comparison.
- Atomic, spin-lock and mutex benchmark.
- Final synchronisation recommendation.

---

# 9. Q6 – Deadlock Handling and the Banker's Algorithm (13 Marks)

## Day-4 Design Challenge

### Problem Context – Shared Laboratory Equipment Scheduler

A research centre operates jobs that require multiple shared resources:

- GPUs.
- Simulation-software licences.
- High-speed storage.

Jobs sometimes wait for resources held by other jobs, resulting in deadlock and causing the cluster to stop until an administrator intervenes.

The objective is to evaluate deadlock-handling techniques and implement the Banker's algorithm.

## A. Characterise Deadlock (2 Marks)

State and explain the four necessary conditions for deadlock:

1. Mutual Exclusion.
2. Hold and Wait.
3. No Preemption.
4. Circular Wait.

Draw a resource-allocation graph for the research-centre cluster.

**Expected Evidence:** A correct resource-allocation graph identifying all four deadlock conditions.

## Complete the Code – TODO 1

Complete the supplied `is_safe()` function.

The program uses:

```c
#define P 5
#define R 3

int avail[R] = {3, 3, 2};
```

The implementation must:

1. Calculate the Need matrix:

```text
Need = Maximum - Allocation
```

2. Initialise the Work vector using Available.
3. Find an unfinished process whose remaining Need is less than or equal to Work for every resource type.
4. Allow the selected process to finish.
5. Add its allocated resources back to Work.
6. Mark the process as finished.
7. Store the process in the safe sequence.
8. Continue until all processes finish or no eligible process can be found.
9. Return unsafe when no eligible process can finish.

## Expected Output

```text
initial state safe: P1 P3 P4 P0 P2
P1 request: granted, safe sequence P1 P3 P4 P0 P2
P4 request: must wait (not available)
P0 request: denied (unsafe)
```

Show manually why P0's request:

```text
(0, 2, 0)
```

leads to an unsafe state.

## B. Compare the Strategies (3 Marks)

Evaluate the following deadlock-handling strategies:

1. Deadlock Prevention.
2. Deadlock Avoidance.
3. Deadlock Detection and Recovery.
4. Ignoring Deadlock (Ostrich Approach).

Compare their:

- Advantages.
- Limitations.
- Runtime overhead.
- Resource utilisation.
- Suitability for the shared research-centre environment.

**Expected Evidence:** A reasoned comparison of the four deadlock-handling strategies.

## C. Apply the Banker's Algorithm (4 Marks)

Implement:

- Safety algorithm.
- Resource-request algorithm.

Find a safe sequence for the given resource state and determine whether each request can be granted.

Evaluate the following requests in the order specified by the assignment:

- P1 requests `(1, 0, 2)`.
- P4 requests `(3, 3, 0)`.
- P0 requests `(0, 2, 0)`.

**Expected Evidence:** A working implementation with correct safe sequences and resource-request decisions.

## D. Predict Before Implementing (1 Mark)

Predict how the running time of the Banker's algorithm grows with:

- Number of processes `n`.
- Number of resource types `m`.

The safety algorithm has the worst-case time complexity:

```text
O(m × n²)
```

State the prediction clearly and explain its basis.

**Expected Evidence:** A clear and testable prediction.

## E. Measure (1 Mark)

Measure the running time of the safety algorithm for:

- 10 processes.
- 100 processes.
- 1,000 processes.

Compare measured timings with the theoretical complexity prediction.

**Expected Evidence:** Measured times presented in a comparison table.

## F. Defend the Final Design (2 Marks)

Recommend the most suitable deadlock-handling strategy for the research centre.

Justify the recommendation by evaluating:

- Resource-request overhead.
- Safety-check cost.
- Availability of maximum resource demands in advance.
- Resource utilisation.
- Risk of deadlock.
- Recovery cost.

**Expected Evidence:** A critical, evidence-based final recommendation.

## Alternative Concept and Complexity Analysis

Evaluate deadlock detection and recovery instead of the Banker's algorithm.

Compare:

- Safety-check cost for every resource request.
- Cost of periodically running a detection algorithm.
- Work lost when a job is aborted.
- Recovery overhead.
- Time complexity.

The supplied assignment identifies the comparison of approximately `O(m × n²)` for every Banker's check against avoiding a safety check on every request.

## Expected Evidence

- Four deadlock conditions.
- Resource-allocation graph.
- Completed Banker's algorithm.
- Safe-sequence calculation.
- Resource-request decisions.
- Hand calculation for P0's unsafe request.
- Prevention, avoidance, detection and recovery comparison.
- Complexity prediction.
- Measurements for 10, 100 and 1,000 processes.
- Final deadlock-handling recommendation.

---

# 10. Suggested Overall Folder Organisation

Maintain six separate folders, one for each task.

```text
Assignment 2/
│
├── README.md
│
├── Task01/
│   ├── README.md
│   ├── Task1_Report.pdf
│   ├── process_states.c
│   ├── run_task1.sh
│   ├── program_output.txt
│   └── process_observation.txt
│
├── Task02/
│   ├── README.md
│   ├── Assignment2_Task02_Report.pdf
│   ├── round_robin.c
│   ├── program_output_q3.txt
│   ├── output_q1.txt
│   └── output_q20.txt
│
├── Task03/
│   ├── README.md
│   ├── Assignment2_Task03_Report.pdf
│   ├── camera_threads.c
│   ├── run_task3.sh
│   ├── benchmark_results.txt
│   └── environment_info.txt
│
├── Task04/
│   ├── README.md
│   ├── Assignment2_Task04_Report.pdf
│   ├── pipe_readings.c
│   ├── ipc_benchmark.c
│   ├── run_task4.sh
│   ├── pipe_readings_output.txt
│   └── ipc_benchmark_results.txt
│
├── Task05/
│   ├── README.md
│   ├── Assignment2_Task05_Report.pdf
│   ├── bounded_buffer.c
│   ├── peterson_demo.c
│   ├── booking_locks.c
│   ├── atomic_spin_benchmark.c
│   ├── run_task5.sh
│   └── Generated Evidence
│
└── Task06/
    ├── README.md
    ├── Assignment2_Task06_Report.pdf
    ├── bankers_algorithm.c
    ├── bankers_benchmark.c
    ├── run_task6.sh
    └── Generated Evidence
```

The filenames are based on the existing task documentation and suggested generated outputs. Keep the actual filenames consistent with the files in each folder.

---

# 11. Software and Environment Details

Record the actual compiler and operating-system environment used for the experiments.

| **Environment Item** | **Details** |
|---|---|
| Operating System | Linux / Ubuntu |
| Linux Distribution | __________________________ |
| Distribution Version | __________________________ |
| Linux Kernel | __________________________ |
| GCC Compiler | __________________________ |
| GCC Version | __________________________ |
| C Standard | C11 |
| POSIX Thread Library | pthread |
| IPC Mechanisms | Pipes, Shared Memory / Message Queues |
| Other Tools | `ps`, `/proc`, `time`, GCC |

Useful commands:

```bash
gcc --version
cat /etc/os-release
uname -a
nproc
lscpu
```

Record the environment used for each experiment rather than assuming all measurements were obtained on identical hardware.

---

# 12. Compilation and Execution Guidelines

## General C Program

```bash
gcc -std=c11 -Wall -Wextra source_file.c -o output_program
```

## POSIX Thread Programs

```bash
gcc -std=c11 -Wall -Wextra -pthread source_file.c -o output_program
```

## Optimised Thread Benchmark

```bash
gcc -std=c11 -O2 -Wall -Wextra -pthread source_file.c -o output_program
```

## Run a Program

```bash
./output_program
```

## Execute Shell Scripts

```bash
chmod +x run_task.sh
./run_task.sh
```

## Measure Execution Time

```bash
time ./output_program
```

Always use the exact source filename included in the task folder.

---

# 13. Overall Evidence and Submission Checklist

## Q1 – Processes and Process States

- [ ] Process-state diagram.
- [ ] Process Control Block diagram.
- [ ] C program using `fork()`.
- [ ] Child-process creation evidence.
- [ ] Parent and child PID verification.
- [ ] `ps` output.
- [ ] `/proc` output.
- [ ] Process-per-student versus thread-per-student comparison.
- [ ] Fixed worker-pool evaluation.
- [ ] Final design recommendation.

## Q2 – CPU Scheduling

- [ ] FCFS Gantt chart.
- [ ] SJF Gantt chart.
- [ ] SRTF Gantt chart.
- [ ] Priority Scheduling Gantt chart.
- [ ] Round Robin Gantt chart.
- [ ] Waiting-time calculations.
- [ ] Turnaround-time calculations.
- [ ] Response-time calculations.
- [ ] Completed Round Robin TODO.
- [ ] Quantum 1, 3, 5 and 20 results.
- [ ] Context-switch comparison.
- [ ] Starvation and ageing explanation.
- [ ] MLFQ explanation.
- [ ] Final scheduling recommendation.

## Q3 – Threads

- [ ] POSIX-thread implementation.
- [ ] 64-thread configuration.
- [ ] 4-thread pool.
- [ ] 8-thread pool.
- [ ] 16-thread pool.
- [ ] Execution-time measurements.
- [ ] Speed-up calculations.
- [ ] Shared and private thread-resource explanation.
- [ ] User-level and kernel-level thread models.
- [ ] Shared-counter race-condition evaluation.
- [ ] Process-versus-thread alternative analysis.

## Q4 – Inter-Process Communication

- [ ] Pipe implementation.
- [ ] Completed pipe TODO.
- [ ] Expected output verification.
- [ ] EOF explanation.
- [ ] Shared-memory or message-queue implementation.
- [ ] 1 MB transfer measurement.
- [ ] 10 MB transfer measurement.
- [ ] 100 MB transfer measurement.
- [ ] Pipe versus shared-memory comparison.
- [ ] Data-copying and synchronisation explanation.
- [ ] Final IPC recommendation.

## Q5 – Synchronisation

- [ ] Mutual exclusion, progress and bounded waiting.
- [ ] Peterson's solution.
- [ ] Binary and counting semaphore explanation.
- [ ] Bounded-buffer implementation.
- [ ] Completed TODO 1 and TODO 2.
- [ ] Readers-writers explanation.
- [ ] Dining-philosophers explanation.
- [ ] Monitor and condition-variable explanation.
- [ ] 1P/1C verification.
- [ ] 4P/4C verification.
- [ ] Every-item-exactly-once verification.
- [ ] 100-run test.
- [ ] Mutex-removal investigation.
- [ ] Global-lock throughput.
- [ ] Per-train-lock throughput.
- [ ] Per-coach-lock throughput.
- [ ] Atomic, spin-lock and mutex comparison.
- [ ] 1, 4 and 16 thread measurements.
- [ ] Final synchronisation recommendation.

## Q6 – Deadlock Handling and Banker's Algorithm

- [ ] Four necessary deadlock conditions.
- [ ] Resource-allocation graph.
- [ ] Completed Banker's safety algorithm.
- [ ] Resource-request algorithm.
- [ ] Initial safe-sequence verification.
- [ ] P1 request verification.
- [ ] P4 request verification.
- [ ] P0 unsafe-request hand calculation.
- [ ] Deadlock prevention comparison.
- [ ] Deadlock avoidance comparison.
- [ ] Detection and recovery comparison.
- [ ] Ostrich approach evaluation.
- [ ] Complexity prediction.
- [ ] 10-process timing.
- [ ] 100-process timing.
- [ ] 1,000-process timing.
- [ ] Final deadlock-handling recommendation.

---

# 14. Important Reporting and Evaluation Guidelines

1. Answer every question and all its subdivisions.
2. Follow the prescribed marks distribution.
3. Include the required diagrams, calculations and technical explanations.
4. Complete every supplied "Complete the Code" program without unnecessarily changing the remaining code.
5. Compile the C programs using appropriate GCC warning flags.
6. Use `-pthread` for POSIX thread programs.
7. Execute every required program on Linux.
8. Compare actual outputs with the expected outputs in the assignment.
9. Include actual terminal outputs and screenshots.
10. Record real execution-time and performance measurements.
11. Do not fabricate timings, speed-ups or benchmark results.
12. Clearly state assumptions wherever the assignment does not supply specific values.
13. Include time and space complexity using Big-O notation wherever required.
14. Explain the advantages and limitations of alternative concepts.
15. Connect experimental results to the theoretical evaluation.
16. Include a justified recommendation for every design decision.
17. Ensure that source files, reports, outputs and README instructions are consistent.
18. Be prepared to explain and defend the submitted implementation and results during assessment verification.

---

# 15. GitHub Submission Requirements

The assignment requires a GitHub repository containing the executed work for all six tasks.

The repository must include:

- A separate folder for each task.
- All C source files.
- Completed "Complete the Code" programs.
- Shell scripts and configuration files, wherever applicable.
- Input files and generated outputs.
- Screenshots and command outputs.
- Task-wise README files.
- Overall Assignment 2 README.
- Assignment report containing evidence for Q1–Q6.

The prescribed repository naming format is:

```text
CSA04-192525148-Assignment2
```

The assignment requires the repository to be shared with the course faculty. It may be public or private with the faculty added as a collaborator.

Progressive commits are mandatory:

- At least one commit for each task.
- The commit history will be checked.
- Uploading all files only on the final day will not be accepted.

The working GitHub repository link must be included in the assignment report cover page and submitted as required by the faculty.

---

# 16. Final Outcome

Assignment 2 demonstrates the ability to analyse, implement and evaluate essential operating-system mechanisms related to process management, CPU scheduling, multithreading, inter-process communication, synchronisation and deadlock handling.

The six tasks progress from process creation and scheduling to concurrent execution, communication between processes, safe access to shared resources and deadlock prevention or avoidance.

The final submission integrates theoretical concepts with practical C implementations, experimental measurements, algorithm comparisons, complexity analysis and evidence-based engineering recommendations.

**END OF ASSIGNMENT 2 – CO2**
