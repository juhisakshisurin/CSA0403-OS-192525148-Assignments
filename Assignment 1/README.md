# ASSIGNMENT 1 – OPERATING SYSTEMS (CO1)
## EVALUATE AND JUSTIFY THE OPERATING-SYSTEM DESIGN OF A SMART HOSPITAL

## Student Details

| **Field** | **Details** |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 – Operating Systems |
| **Assignment** | Assignment 1 – CO1 |
| **Assignment Title** | Evaluate and Justify the Operating-System Design of a Smart Hospital |
| **Under the Supervision of** | Dr. Velmurugan J. |
| **Nature of Assignment** | Individual |
| **Duration** | One Week |
| **Total Marks** | 50 |
| **Bloom's Level** | K5 – Evaluate |

---

# 1. Assignment Overview

This assignment focuses on evaluating and justifying the operating-system design required for a smart hospital.

The hospital contains several computing environments with different requirements:

- Bedside patient-monitoring devices.
- Nurses' handheld tablets.
- Doctors' desktop workstations.
- A central database server for patient records.
- An MRI imaging server with high processing requirements.
- Communication systems connecting partner hospitals.

The main objective is to determine whether one general-purpose operating system can support all these environments effectively or whether different operating-system types, structures and configurations are required.

The assignment evaluates operating-system concepts including:

- Types and history of operating systems.
- Operating-system structures.
- Operating-system services.
- System calls and their execution.
- Process-control system calls.
- CPU states and protection.
- Memory hierarchy and I/O systems.
- Performance measurement and comparison.
- Complete operating-system design and evaluation.

Every task is assessed at Bloom's Taxonomy Level K5 – Evaluate.

The expected engineering workflow is:

**MODEL → DESIGN → IMPLEMENT → MEASURE → COMPARE → EVALUATE → RECOMMEND**

---

# 2. Smart Hospital Scenario

A 500-bed hospital is replacing its existing computer systems.

The system must support the following requirements:

| **Hospital Environment** | **Main Requirement** |
|---|---|
| Bedside patient monitors | Raise an alarm within 10 milliseconds of an abnormal reading. |
| Nurses' handheld tablets | Provide mobility, reliable access and battery efficiency. |
| Doctors' desktop workstations | Support clinical applications, multitasking and secure access to patient records. |
| Central database server | Maintain reliable and continuous access to patient records. |
| MRI imaging server | Process large medical images using multiple processors. |
| Partner-hospital communication | Allow secure sharing of patient records between hospitals. |

The system is expected to grow through three stages:

1. One ward: 40 monitors and 10 tablets.
2. One hospital: 500 monitors, 120 tablets and 80 workstations.
3. Three hospitals: Shared patient records and communication across locations.

The design must balance patient safety, response time, reliability, security, throughput, storage and scalability.

---

# 3. Marks Distribution

| **Task** | **Assessed Area** | **Marks** | **Bloom's Level** |
|---|---|---:|---|
| Task 1 | Requirements of Each Computing Environment | 4 | K5 – Evaluate |
| Task 2 | Types of Operating Systems | 3 | K5 – Evaluate |
| Task 3 | Operating-System Structures | 4 | K5 – Evaluate |
| Task 4 | Operating-System Services | 4 | K5 – Evaluate |
| Task 5 | System Calls | 4 | K5 – Evaluate |
| Task 6 | Cost of System Calls | 4 | K5 – Evaluate |
| Task 7 | Process Control System Calls | 4 | K5 – Evaluate |
| Task 8 | CPU States and Protection | 3 | K5 – Evaluate |
| Task 9 | Memory Hierarchy and I/O Systems | 4 | K5 – Evaluate |
| Task 10 | Measurement and Comparison | 4 | K5 – Evaluate |
| Task 11 | Final Operating-System Recommendation | 4 | K5 – Evaluate |
| **Subtotal** | **Tasks 1–11** | **42** | |
| Task 12 | Day-4 Design Challenge – Re-evaluate and Adapt the Design | 8 | K5 – Evaluate |
| **TOTAL** | | **50** | |

---

# 4. TASK 1 – Evaluate the Requirements of Each Computing Environment (4 Marks)

## Objective

Evaluate what each part of the smart hospital requires from its operating system.

## Areas to Analyse

- Response-time requirements.
- Reliability and availability.
- Throughput.
- Battery and power efficiency.
- Security and patient-data protection.
- Conflicting system requirements.
- Patient-safety-critical operations.
- Historical evolution of operating systems, including batch, time-sharing, personal and mobile systems.

## Required Evaluation

Prepare a ranked requirement table for every hospital environment.

The ranking must justify why a particular requirement is more important for one environment than another.

For example:

- Bedside monitors prioritise predictable alarm response and patient safety.
- Handheld tablets prioritise power efficiency and secure mobility.
- Database servers prioritise reliability and availability.
- MRI servers prioritise processing performance and data transfer.

## Alternative Concept and Complexity Analysis

Evaluate using one general-purpose operating system throughout the hospital instead of different operating-system types for different environments.

Compare:

- Worst-case alarm response time.
- Unbounded scheduling delay versus a guaranteed deadline.
- Memory requirements on different devices.
- System-administration effort.
- Performance and reliability limitations.

## Expected Evidence

- Ranked requirement table for every environment.
- Justification for each ranking.
- Patient-safety analysis.
- Alternative-design evaluation.

---

# 5. TASK 2 – Judge the Type of Operating System for Each Environment (3 Marks)

## Objective

Compare different operating-system types and select the most suitable type for each hospital environment.

## Operating-System Types to Evaluate

1. Mainframe Operating System.
2. Desktop Operating System.
3. Multiprocessor Operating System.
4. Distributed Operating System.
5. Clustered Operating System.
6. Real-Time Operating System.
7. Handheld Operating System.

## Required Evaluation

Determine which operating-system type is appropriate for:

- Bedside patient monitors.
- Nurses' handheld tablets.
- Doctors' workstations.
- Central database server.
- MRI imaging server.
- Partner-hospital communication.

Also evaluate:

- Hard real-time versus soft real-time systems.
- Symmetric multiprocessing (SMP).
- Asymmetric multiprocessing (AMP).
- Clustered systems and database fail-over.

## Alternative Concept and Complexity Analysis

Evaluate using one large multiprocessor machine for the database server instead of a cluster of smaller machines with fail-over.

Compare:

- Availability.
- Single point of failure.
- Fail-over time.
- Hardware and maintenance cost.
- Throughput as processor count increases.
- Memory availability and scalability.

## Expected Evidence

- Operating-system type comparison.
- Selection for all six environments.
- Justification for selected types.
- Rejected alternatives with reasons.
- Database clustering and fail-over evaluation.

---

# 6. TASK 3 – Evaluate Operating-System Structures (4 Marks)

## Objective

Evaluate operating-system structures based on reliability, performance, maintenance and fault isolation.

## Structures to Compare

- Simple or Monolithic Structure.
- Layered Structure.
- Microkernel Structure.
- Modular Structure.
- Hybrid Structure.

## Required Evaluation

Discuss:

- Reliability of each structure.
- Performance and service-request overhead.
- Ease of updating and maintenance.
- Kernel size and modularity.
- Fault isolation.
- Suitability for patient monitors and database servers.

Also examine the structures used by:

- Linux.
- Windows.
- Android.
- Real-time operating systems such as FreeRTOS or QNX.

## Alternative Concept and Complexity Analysis

Evaluate using a monolithic kernel for bedside monitors instead of a microkernel.

Compare:

- Service-request time: direct system-call execution versus message passing.
- Memory requirements.
- Driver failure impact.
- Reliability and fault isolation.
- Performance trade-offs.

## Expected Evidence

- Comparison table of operating-system structures.
- Reliability and performance evaluation.
- Kernel-structure examples.
- Defended structure choice for patient monitors.
- Defended structure choice for the database server.
- Alternative-concept analysis.

---

# 7. TASK 4 – Evaluate the Services an Operating System Must Provide (4 Marks)

## Objective

Evaluate the operating-system services required by each hospital environment.

## Services to Analyse

1. User Interface.
2. Program Execution.
3. I/O Operations.
4. File-System Management.
5. Communication.
6. Error Detection.
7. Resource Allocation.
8. Accounting.
9. Protection.
10. Security.

## Required Evaluation

Determine:

- Which services can be removed or minimised on bedside monitors.
- Which services are essential for database servers.
- Which services are required by handheld tablets.
- Which services are required by doctors' workstations.
- How the operating system protects one patient's records from another user's programs.

## Alternative Concept and Complexity Analysis

Evaluate using a full desktop operating system on handheld tablets instead of a reduced handheld operating system.

Compare:

- Additional memory consumption.
- Additional storage requirements.
- Start-up time.
- Battery consumption.
- Performance and resource overhead.

## Expected Evidence

- Service comparison table for each environment.
- Essential and optional service identification.
- Justifications for including or excluding services.
- Patient-data protection explanation.
- Alternative-concept analysis.

---

# 8. TASK 5 – Examine System Calls (4 Marks)

## Objective

Examine how applications request operating-system services through system calls.

## Required Concepts

Explain the six major system-call categories:

| **Category** | **Purpose** | **Examples** |
|---|---|---|
| Process Control | Creates, executes and terminates processes. | `fork()`, `exec()`, `exit()`, `wait()` |
| File Management | Manages file operations. | `open()`, `read()`, `write()`, `close()` |
| Device Management | Controls device operations. | `ioctl()`, `read()`, `write()` |
| Information Maintenance | Retrieves or updates system information. | `getpid()`, `time()`, `uname()` |
| Communication | Exchanges data between processes or systems. | `pipe()`, `socket()`, `send()`, `recv()` |
| Protection | Controls access to system resources. | `chmod()`, `chown()`, `access()` |

## Required Evaluation

Explain the complete working of the `read()` system call:

1. Library call.
2. Trap.
3. Mode switch.
4. Kernel processing.
5. Return to the application.

Also explain how system-call parameters are passed using:

- Registers.
- A block in memory.
- The stack.

## Experimental Investigation

Use `strace` to observe system calls made by a program.

Example:

```bash
strace ./readfile
```

To trace file-related operations:

```bash
strace -e trace=openat,read,write,close ./readfile
```

## Alternative Concept and Complexity Analysis

Evaluate using C library functions:

- `fopen()`
- `fread()`
- `fwrite()`

instead of directly using:

- `open()`
- `read()`
- `write()`

Compare:

- Number of system calls for a file of n bytes.
- Library buffering.
- Additional buffer-space requirements.
- Performance and ease of implementation.

For an example 4096-byte library buffer, the approximate number of underlying read operations is proportional to `n / 4096`.

## Expected Evidence

- System-call category table.
- Step-by-step `read()` explanation.
- System-call flow diagram.
- Parameter-passing explanation.
- Actual `strace` output.
- Alternative-concept and complexity analysis.

---

# 9. TASK 6 – Evaluate the Cost of System Calls (4 Marks)

## Objective

Evaluate how system-call frequency affects file-copying and backup performance.

## Required Evaluation

Explain:

- Why a system call is more expensive than an ordinary function call.
- The effect of buffer size on `read()` and `write()` calls.
- How library buffering reduces system calls.
- How to select a suitable backup-program buffer size using measurements.

## Complete the Code – TODO 1

Complete the supplied C program using the `read()` and `write()` system calls.

The program must:

1. Open the source file.
2. Open or create the destination file.
3. Read up to `bufsize` bytes into the buffer.
4. Count every `read()` call.
5. Stop when `read()` returns 0 or an error.
6. Write all bytes read to the destination.
7. Close both files and return the number of read calls.

The implementation must handle successful partial writes correctly.

## Expected Output

For a 10,000-byte test file:

```text
buffer    1 bytes: 10001 read() calls
buffer  512 bytes:    21 read() calls
buffer 4096 bytes:     4 read() calls
```

The final read returning 0 is included in these counts.

Run the program using:

```bash
strace -c ./task6
```

Compare the program's read-call counter with the system-call count reported by `strace`.

## Alternative Concept and Complexity Analysis

Evaluate memory-mapped file access using `mmap()` instead of buffered `read()` and `write()`.

Compare:

- Number of explicit system calls.
- Kernel and user-space data copying.
- Page-fault overhead.
- Memory requirements for very large files.
- Time and space complexity.

## Expected Evidence

- Completed C program.
- Results for all three buffer sizes.
- `strace -c` output.
- Performance comparison.
- Alternative `mmap()` analysis.
- Justified buffer-size recommendation.

---

# 10. TASK 7 – Evaluate Process Control System Calls (4 Marks)

## Objective

Evaluate how a command shell creates, executes, monitors and terminates processes.

## Required Concepts

Explain the functions of:

- `fork()`
- `exec()`
- `wait()`
- `waitpid()`
- `exit()`
- `_exit()`

Also explain:

- How a shell runs a command.
- How the parent retrieves the child's exit status.
- Zombie processes.
- Orphan processes.
- Why `fork()` followed by `exec()` is used.

## Complete the Code – TODO 1

Complete the supplied program using:

- `fork()`
- `execlp()`
- `waitpid()`
- `WEXITSTATUS`

The child must execute:

```c
execlp("expr", "expr", "6", "*", "7", (char *)NULL);
```

If execution fails, the child must terminate using exit status 127.

The parent must wait for the child and print the returned exit status.

## Expected Output

```text
shell: running job 1
42
job 1 exit status 0
shell: running job 2
job 2 exit status 3
```

## Additional Investigation

Explain:

- Why `stdout` must be flushed before `fork()`.
- Why removing the second `fflush(stdout)` and using `exit(3)` can duplicate buffered output when redirected to a file.
- The difference between `exit()` and `_exit()`.

## Alternative Concept and Complexity Analysis

Evaluate using `posix_spawn()` or threads instead of `fork()` followed by `exec()`.

Compare:

- Time to launch 1,000 commands.
- Memory copied or shared.
- Copy-on-write behaviour.
- Process isolation.
- Time and space complexity.

## Expected Evidence

- Process execution program.
- Successful compilation and execution.
- Expected output comparison.
- Exit-status explanation.
- Output-buffering investigation.
- Process diagrams.
- Alternative-concept analysis.

---

# 11. TASK 8 – Evaluate CPU States and Protection (3 Marks)

## Objective

Evaluate how the hardware protects the smart-hospital system.

## Required Concepts

Explain:

- User mode and kernel mode.
- Privileged instructions.
- CPU mode bit.
- Hardware interrupts.
- Traps.
- Timer interrupts.
- Protection against unauthorised memory access.

## Required Evaluation

Explain:

- What happens when a user program attempts a privileged instruction.
- Why user programs must not be allowed to disable interrupts.
- How the hardware prevents one application from reading another application's memory.
- The role of memory-protection mechanisms in patient-data security.

## Alternative Concept and Complexity Analysis

Evaluate polling instead of interrupt-driven sensor monitoring.

Compare:

- Worst-case delay before an abnormal reading is detected.
- Polling interval.
- Interrupt latency.
- CPU time consumed by polling.
- Power consumption.

For the bedside monitoring system, the selected mechanism must be evaluated against the 10 ms alarm-response requirement.

## Expected Evidence

- User-mode versus kernel-mode comparison.
- Privileged-instruction explanation.
- Interrupt and trap explanation.
- CPU and memory protection analysis.
- Polling versus interrupt evaluation.
- Justified monitoring approach.

---

# 12. TASK 9 – Evaluate the Memory Hierarchy and I/O Systems (4 Marks)

## Objective

Evaluate the storage hierarchy and I/O methods required by the smart hospital.

## Required Concepts

Explain the levels of the memory hierarchy:

1. Registers.
2. Cache.
3. Main Memory (RAM).
4. SSD.
5. Hard Disk (HDD).
6. Network Storage.

Compare their typical:

- Access times.
- Storage capacities.
- Speed.
- Cost.
- Applications.

## Required Evaluation

Determine where the following data should be stored:

- MRI images.
- Patient records.
- Bedside-monitor readings.

Evaluate the following I/O methods:

- Programmed I/O.
- Interrupt-driven I/O.
- Direct Memory Access (DMA).

Also explain the effect of caching on patient-record lookup speed.

## Alternative Concept and Complexity Analysis

Evaluate interrupt-driven I/O with one interrupt per word instead of DMA for a 500 MB MRI image.

Given:

- Image size = 500 MB.
- Word size = 4 bytes.
- One interrupt per word.

Calculate:

```text
Number of words = (500 × 1024 × 1024) / 4

Number of interrupts = 131,072,000
```

If an illustrative interrupt-handling time of 1 microsecond is assumed:

```text
Total CPU time = 131,072,000 × 1 microsecond

               = 131.072 seconds
```

This is an illustrative calculation. Actual interrupt-handling time depends on hardware and the operating system.

Compare DMA and interrupt-driven I/O in terms of CPU involvement, transfer overhead and buffer requirements.

## Expected Evidence

- Memory-hierarchy comparison table.
- Data-placement recommendation.
- I/O-method comparison.
- DMA justification for MRI image transfer.
- Cache-hit and cache-miss explanation.
- 500 MB interrupt calculation.
- Alternative-concept and complexity analysis.

---

# 13. TASK 10 – Measure and Compare (4 Marks)

## Objective

Measure the behaviour of programs on a Linux system and compare the results with earlier theoretical evaluations.

## Required Measurements

### A. System-Call Counts

Use:

```bash
strace -c ./copy
```

Measure system calls for different buffer sizes and compare the results with Task 6.

### B. User and Kernel Execution Time

Use:

```bash
time ./copy
```

Record:

- Real time.
- User time.
- System time.

### C. Process Creation

Write and execute a C program that creates 1,000 processes using `fork()`.

Compile and execute:

```bash
gcc process.c -o process

time ./process
```

### D. Thread Creation

Write and execute a C program that creates 1,000 threads using `pthread_create()`.

Compile and execute:

```bash
gcc thread.c -o thread -pthread

time ./thread
```

## Required Evaluation

Compare:

- Process creation time.
- Thread creation time.
- Memory requirements.
- User-mode and kernel-mode time.
- System-call overhead.
- Actual performance against theoretical expectations.

Record actual measurements obtained from the Linux environment.

## Alternative Concept and Complexity Analysis

Evaluate creating threads instead of processes.

Compare:

- Time required for 1,000 processes versus 1,000 threads.
- Separate process address spaces versus shared thread address spaces.
- Stack and management overhead.
- Isolation and resource sharing.

## Expected Evidence

- `strace -c` output.
- `time` command output.
- Process-creation C program.
- Thread-creation C program.
- Measurements for 1,000 processes and threads.
- Comparison table.
- Explanation of observed differences.

---

# 14. TASK 11 – Recommend and Defend the Operating-System Plan (4 Marks)

## Objective

Recommend a complete operating-system architecture for the smart hospital using the analysis and evidence developed in Tasks 1–10.

## Required Evaluation

The final recommendation must include:

- Suitable operating-system type for every environment.
- Suitable operating-system structure.
- Essential operating-system services.
- System-call requirements.
- Process-management requirements.
- CPU and memory protection.
- Storage placement.
- I/O methods.
- Reliability and availability.
- Security and scalability.

## Suggested Operating-System Plan

| **Hospital Environment** | **Recommended Approach** | **Main Justification** |
|---|---|---|
| Bedside Patient Monitors | Real-Time OS with a suitable minimal or microkernel-based design | Predictable response and patient safety. |
| Nurses' Handheld Tablets | Handheld OS | Mobility and power efficiency. |
| Doctors' Workstations | Desktop OS | User interface and multitasking. |
| Central Database Server | Clustered system with fail-over | Availability and reliable patient-record access. |
| MRI Imaging Server | Multiprocessor OS using suitable parallel processing and DMA | High computational performance and efficient image transfer. |
| Partner-Hospital Communication | Distributed system | Secure data sharing across locations. |

The exact final selection must be defended against the requirements and trade-offs discussed in the earlier tasks.

## Required Evaluation

Discuss:

- Limitations and risks of the proposed architecture.
- Hardware and software failures.
- Network interruptions.
- Data protection.
- Availability and recovery.
- Support for a group of three hospitals sharing patient records.

## Alternative Concept and Complexity Analysis

Evaluate moving the complete system to virtual machines on two large servers instead of using separate physical machines for each environment.

Compare:

- Monitor response time.
- Processor and memory overhead.
- Virtualisation effects.
- Reliability and failure recovery.
- Isolation between workloads.
- Resource allocation.

## Expected Evidence

- Complete architecture diagram.
- Final OS-selection table.
- Protection and I/O design.
- Limitations and risk analysis.
- Multi-hospital scalability evaluation.
- Virtual-machine alternative analysis.
- Evidence-based final recommendation.

---

# 15. TASK 12 – Re-evaluate and Adapt the Design (8 Marks)

## Day-4 Design Challenge

### Changed Requirement

The hospital introduces:

1. Remote patient-monitoring devices in patients' homes that send readings through the internet.
2. A cloud-based patient-record service shared with partner hospitals.

The original design must be re-evaluated.

You may:

**RETAIN → MODIFY → or REDESIGN** the original solution.

## A. Impact Analysis (2 Marks)

Identify which operating-system types, services and protection mechanisms are affected.

Analyse changes to:

- Remote-device operating systems.
- Communication systems.
- Security mechanisms.
- Cloud storage.
- Availability.
- Resource management.

**Expected Evidence:** Clear analysis of how the new requirements affect the original design.

## B. Evaluation of the Original Design (2 Marks)

Evaluate whether the original architecture can support:

- Home-based monitoring.
- Internet communication.
- Cloud-based records.
- Shared hospital records.
- Required response time.
- Reliability and security.

**Expected Evidence:** Technically supported evaluation of the existing design.

## C. Adaptation or Redesign (2 Marks)

Develop a revised architecture.

Possible design considerations:

- Embedded real-time operating system for home-monitoring devices.
- Secure internet communication.
- Distributed architecture for shared records.
- Cloud-based storage.
- Authentication and access control.
- Encryption and secure data transfer.
- Backup and recovery mechanisms.

**Expected Evidence:** Appropriate revised operating-system design.

## D. Evidence-Based Defence (2 Marks)

Defend whether the original architecture should be retained, modified or redesigned.

Justify the decision using:

- Technical feasibility.
- Response time.
- Reliability.
- Security.
- Scalability.
- Resource requirements.
- Performance trade-offs.

**Expected Evidence:** Final decision supported by technical reasoning and evidence.

## Expected Deliverables

- Impact-analysis table.
- Evaluation of the original architecture.
- Revised architecture diagram.
- Remote-monitoring design.
- Cloud-records design.
- Security and availability analysis.
- Evidence-based final defence.

---

# 16. Suggested Folder Organisation

The assignment work is organised into 12 separate task folders.

```text
Assignment 1/
│
├── README.md
│
├── Task01/
│   ├── README.md
│   └── OS Task 1 Juhi Surin (192525148).docx
│
├── Task02/
│   ├── README.md
│   └── OS Task 2 Juhi Surin (192525148).docx
│
├── Task03/
│   ├── README.md
│   └── OS Task 3 Juhi Surin (192525148).docx
│
├── Task04/
│   ├── README.md
│   └── OS Task 4 Juhi Surin (192525148).docx
│
├── Task05/
│   ├── README.md
│   └── OS Task 5 Juhi Surin (192525148).docx
│
├── Task06/
│   ├── README.md
│   ├── OS Task 6 Juhi Surin (192525148).docx
│   └── Execution Evidence
│
├── Task07/
│   ├── README.md
│   ├── OS Task 7 Juhi Surin (192525148).docx
│   ├── task7(original).c
│   ├── task7(modified).c
│   └── Execution Evidence
│
├── Task08/
│   ├── README.md
│   └── OS Task 8 Juhi Surin (192525148).docx
│
├── Task09/
│   ├── README.md
│   └── OS Task 9 Juhi Surin (192525148).docx
│
├── Task10/
│   ├── README.md
│   └── OS Task 10 Juhi Surin (192525148).docx
│
├── Task11/
│   ├── README.md
│   └── OS Task 11 Juhi Surin (192525148).docx
│
└── Task12/
    ├── README.md
    └── OS Task 12 Juhi Surin (192525148).docx
```

The folder structure above reflects the organisation of the current repository. Add any missing source files, command outputs, diagrams and screenshots required by the assignment before final submission.

---

# 17. Software and Environment Details

Record the actual environment used to compile and execute the programs.

| **Environment Item** | **Version / Details** |
|---|---|
| Operating System | Linux / Ubuntu |
| Linux Distribution | __________________________ |
| Distribution Version | __________________________ |
| Linux Kernel | __________________________ |
| GCC Compiler | __________________________ |
| GCC Version | __________________________ |
| C Standard | C11 |
| Architecture | __________________________ |
| Other Tools | `strace`, `time`, `ps`, Linux `/proc`, pthreads |

Useful commands:

```bash
gcc --version
cat /etc/os-release
uname -a
```

Do not assume that every task was executed on the same environment. Record the actual environment used for each experiment.

---

# 18. Compilation and Execution Guidelines

## General C Compilation

```bash
gcc -std=c11 -Wall -Wextra source_file.c -o output_program
```

## Programs Using POSIX Threads

```bash
gcc -std=c11 -Wall -Wextra -pthread source_file.c -o output_program
```

## Run a Program

```bash
./output_program
```

## Measure Execution Time

```bash
time ./output_program
```

## Trace System Calls

```bash
strace ./output_program
```

## Count System Calls

```bash
strace -c ./output_program
```

## Inspect Processes

```bash
ps -ef
```

All programs must be compiled and executed on Linux. Use the exact source filenames contained in the respective task folders when running the commands.

---

# 19. Overall Submission and Evidence Checklist

## Task 1 – Requirements

- [ ] Ranked requirement table for all environments.
- [ ] Response-time and patient-safety evaluation.
- [ ] Reliability and security analysis.
- [ ] Single-general-purpose-OS alternative.

## Task 2 – OS Types

- [ ] Comparison of all specified OS types.
- [ ] Selection for every hospital environment.
- [ ] SMP and AMP evaluation.
- [ ] Database clustering and fail-over.
- [ ] Multiprocessor alternative analysis.

## Task 3 – OS Structures

- [ ] Monolithic, layered, microkernel, modular and hybrid comparison.
- [ ] Linux, Windows, Android and RTOS examples.
- [ ] Structure selection for monitors and database server.
- [ ] Driver-failure and service-request evaluation.

## Task 4 – OS Services

- [ ] All ten OS services evaluated.
- [ ] Service table for each environment.
- [ ] Essential and optional services identified.
- [ ] Tablet desktop-OS alternative evaluated.

## Task 5 – System Calls

- [ ] Six system-call categories.
- [ ] `read()` execution flow.
- [ ] Parameter-passing methods.
- [ ] Actual `strace` output.
- [ ] Library-buffering alternative analysis.

## Task 6 – Cost of System Calls

- [ ] Completed TODO 1 program.
- [ ] 1-byte buffer result.
- [ ] 512-byte buffer result.
- [ ] 4096-byte buffer result.
- [ ] `strace -c` output.
- [ ] `mmap()` alternative evaluation.

## Task 7 – Process Control

- [ ] Completed TODO 1 program.
- [ ] `fork()`, `execlp()` and `waitpid()` implementation.
- [ ] Expected output verification.
- [ ] Exit-status explanation.
- [ ] Zombie and orphan process explanation.
- [ ] `stdout` flushing investigation.
- [ ] Alternative-concept analysis.

## Task 8 – CPU Protection

- [ ] User mode and kernel mode.
- [ ] Privileged instructions.
- [ ] Interrupts and traps.
- [ ] Memory protection.
- [ ] Polling versus interrupts.
- [ ] 10 ms alarm-response evaluation.

## Task 9 – Memory Hierarchy and I/O

- [ ] Memory hierarchy table.
- [ ] Data-placement recommendation.
- [ ] Programmed, interrupt-driven and DMA comparison.
- [ ] Cache-hit and cache-miss explanation.
- [ ] 500 MB interrupt calculation.
- [ ] DMA alternative analysis.

## Task 10 – Measurement

- [ ] System-call count measurements.
- [ ] User and kernel execution-time measurements.
- [ ] 1,000-process creation program.
- [ ] 1,000-thread creation program.
- [ ] Actual timing results.
- [ ] Process versus thread comparison.

## Task 11 – Final Recommendation

- [ ] Complete OS selection table.
- [ ] Architecture diagram.
- [ ] Protection and I/O design.
- [ ] Risk and limitation analysis.
- [ ] Three-hospital scalability analysis.
- [ ] Virtual-machine alternative evaluation.

## Task 12 – Design Challenge

- [ ] Impact analysis.
- [ ] Original-design evaluation.
- [ ] Revised architecture.
- [ ] Remote-monitoring design.
- [ ] Cloud-records design.
- [ ] Security and reliability evaluation.
- [ ] Evidence-based final defence.

---

# 20. Important Reporting and Evaluation Guidelines

1. Answer every question and all its subdivisions.
2. Follow the prescribed marks distribution.
3. Include proper technical explanations and comparisons.
4. Support recommendations with measurable evidence wherever required.
5. Complete every supplied TODO without unnecessarily modifying the remaining code.
6. Compile and execute the programs on Linux.
7. Compare actual outputs with the assignment's expected outputs.
8. Include screenshots and command outputs as evidence.
9. Do not fabricate system-call counts, execution times or memory measurements.
10. Clearly state assumptions used in calculations.
11. Include time and space complexity analysis wherever required.
12. Explain the advantages and limitations of alternative concepts.
13. Ensure all diagrams, calculations, reports and programs are consistent.
14. Include a justified conclusion for each task.
15. Be prepared to explain and defend the submitted work during assessment verification.

---

# 21. GitHub Submission Requirements

The assignment requires a GitHub repository containing the executed work for all 12 tasks.

The repository must contain:

- Separate folders for each task.
- C source files for the supplied coding exercises.
- Completed TODO programs.
- Scripts and configuration files, where applicable.
- Input files and generated outputs.
- Screenshots and command outputs.
- Task-wise README files.
- Overall Assignment 1 README.
- The assignment report containing evidence for Tasks 1–12.

The assignment also requires progressive commits, with at least one commit for every task. The commit history will be checked.

The prescribed standalone repository naming format in the question paper is:

```text
CSA04-192525148-Assignment1
```

The current combined repository containing the Assignment 1 folder is:

```text
CSA0403-OS-192525148-Assignments
```

**Assignment 1 folder:**  
https://github.com/juhisakshisurin/CSA0403-OS-192525148-Assignments/tree/main/Assignment%201

---

# 22. Final Outcome

Assignment 1 demonstrates the ability to evaluate operating-system concepts and apply them to a real-world smart-hospital environment.

The work progresses from identifying computing requirements and selecting operating-system types to evaluating kernel structures, services, system calls, process control, CPU protection, memory hierarchy and I/O systems.

The final tasks integrate these concepts into a complete operating-system design and re-evaluate the architecture when remote monitoring and cloud-based patient records are introduced.

The completed assignment must demonstrate technical understanding through clear comparisons, justified design choices, working programs, calculations, actual measurements, diagrams and evidence-based recommendations.

**END OF ASSIGNMENT 1 – CO1**
