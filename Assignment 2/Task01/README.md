# Assignment 2 – Task 1: Processes and Process States

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Date** | 03-11-2026 |
| **Course** | CSA0403 – Computer Vision |
| **Under the Supervision of** | Dr. Velmurugan J |
| **Assignment** | Assignment 2 – CO2 |
| **Task** | Task 1 / Q1 – Processes and Process States |
| **Maximum Marks** | 6 |

> **Course-code verification required:** The uploaded assignment PDF identifies Assignment 2 as **Operating Systems – CO2**, whereas the course supplied for this cover information is **CSA0403 – Computer Vision**. The supplied detail is reproduced exactly; confirm the correct course name/code with faculty before submission.

## Objective
Model process states and the Process Control Block (PCB), implement process creation using `fork()`, observe processes using `ps` and `/proc`, and evaluate process-per-student, thread-per-student and fixed worker-process-pool designs for 2,000 concurrent examination sessions.

## Files
```text
CSA04-192525148-Assignment2/
├── README.md
└── Task01/
    ├── README.md
    ├── Task01_Report.pdf
    ├── process_states.c
    ├── run.sh
    └── sample_execution_output.txt
```

## Tested Environment
The program was compiled and executed in GCC 14.2.0 on Debian GNU/Linux 13 (trixie), Linux kernel 6.18.57. Verify your own environment with `gcc --version`, `cat /etc/os-release`, and `uname -sr`; update this table if your environment differs.

## Compile and Run
```bash
gcc -std=c11 -Wall -Wextra -Wpedantic process_states.c -o process_states
./process_states
```
Or run `chmod +x run.sh && ./run.sh`.

The program creates three child processes. Each sleeps for 12 seconds, during which the parent reads selected fields from `/proc/<PID>/status`. The parent then calls `waitpid()` for each child and reports its exit status.

## Observe with ps and /proc
While the children are sleeping, use the actual PIDs printed by your run:
```bash
ps -o pid,ppid,stat,comm -p <PID1>,<PID2>,<PID3>
cat /proc/<PID1>/status
```
`S` in the process state indicates interruptible sleeping. Capture your own screenshots while the children are alive and add them to this folder. The PIDs change on each execution.

## Process-State Model
- **New:** OS is creating the process and its kernel data structures.
- **Ready:** Process is eligible for CPU time but waiting in the ready queue.
- **Running:** CPU is executing the process.
- **Waiting/Blocked:** Process awaits an event such as I/O completion.
- **Terminated:** Execution has ended; the parent collects status with `wait()`/`waitpid()`.

```text
New --admitted--> Ready --dispatch--> Running --exit--> Terminated
                       ^             |
                       |             | I/O/event wait
                  preemption          v
                       +---------- Waiting
                                    | event completion
                                    v
                                  Ready
```

## Process Control Block (PCB)
| PCB field | Purpose |
|---|---|
| PID and parent PID | Identify process and parent |
| Process state | Record ready/running/waiting/terminated status |
| Program counter | Address of next instruction |
| CPU registers | Preserve execution context across context switches |
| Scheduling information | Priority, scheduling parameters and queue links |
| Memory information | Address-space and page-table references |
| I/O and file information | Open files, devices and pending I/O |
| Accounting/security | CPU usage, user identity and permissions |

## Evaluation and Design Decision
A process per student provides strong address-space isolation, but 2,000 processes require many process records, separate virtual address spaces and scheduling operations. Context switches can reduce useful CPU time, while I/O-blocked processes still consume process resources. A thread per student generally uses less memory because threads share code, data and open resources, but each has its own stack/context; shared data needs synchronization, and a process-wide failure can affect all its threads. A fixed worker-process pool bounds the number of worker processes and queues incoming sessions, balancing overhead and worker-level isolation. Its queue latency and pool size must be load-tested.

**Recommendation:** Use a bounded worker-process pool as the baseline, potentially with asynchronous I/O or a modest number of threads per worker. Measure response latency, throughput, memory, CPU utilization and failure recovery before selecting pool size.

## Alternative Concept and Complexity Analysis
| Design | Approximate scaling for n sessions | Trade-off |
|---|---|---|
| One process/student | O(n) process records and separate address spaces | Strong isolation; highest memory/kernel overhead |
| One thread/student | O(n) thread records/stacks; shared address space | Lower per-session overhead; synchronization and shared-failure risks |
| Fixed pool of k workers | O(k) worker processes plus O(n) session/queue metadata | Bounded worker overhead; queueing and sizing trade-offs |

Creating n processes or threads requires O(n) creation operations. A worker pool creates k workers (O(k)); queue operations depend on the data structure (a suitable queue can provide O(1) enqueue/dequeue). Process context switches may involve address-space and TLB effects; same-process thread switches can avoid address-space changes. Actual timings depend on hardware, OS and workload, so do not claim fixed timings without measurements.

## Result
The program creates child processes with `fork()`, observes their parent relationship and sleeping state through `/proc`, and reaps them using `waitpid()`. The tested run showed `State: S (sleeping)` for each child, followed by exit status 0 when reaped.

## Evidence to Add Before Submission
- [ ] Your screenshot of successful compilation and execution.
- [ ] Your `ps` screenshot while children are alive.
- [ ] Your `/proc/<PID>/status` screenshot.
- [ ] Your own measurements if required by the instructor.
- [ ] Actual GitHub URL in the report cover page and repository README.
- [ ] Confirm the course code/name with faculty.

## References
1. Assignment 2 – CO2, Q1: Processes and Process States, supplied assignment PDF, page 12.
2. Linux manual pages: `fork(2)`, `waitpid(2)`, `ps(1)`, and `proc(5)`.
