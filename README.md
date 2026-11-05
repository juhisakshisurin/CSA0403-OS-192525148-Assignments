# OPERATING SYSTEMS – ASSIGNMENTS CO1 TO CO5
## CONSOLIDATED ASSIGNMENT REPOSITORY

## Student Details

| **Field** | **Details** |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 – Operating Systems |
| **Course Outcomes** | CO1, CO2, CO3, CO4 and CO5 |
| **Total Assignments** | 5 |
| **Nature** | Individual |
| **Assessment Level** | K5 – Evaluate |
| **Total Marks** | 250 |

---

# 1. Repository Overview

This repository contains the implementation, analysis, experimental investigations and engineering recommendations for all five Operating Systems assignments covering CO1 to CO5.

The assignments are designed to develop practical and analytical understanding of operating-system concepts through real-world scenarios, programming exercises, system-level experiments, performance measurements and design decisions.

Each assignment addresses a separate course outcome.

| **Assignment** | **Course Outcome** | **Main Coverage** | **Marks** |
|---|---|---|---:|
| Assignment 1 | CO1 | Operating-System Design of a Smart Hospital | 50 |
| Assignment 2 | CO2 | Process Management and Deadlock Handling | 50 |
| Assignment 3 | CO3 | Memory Management | 50 |
| Assignment 4 | CO4 | File Storage and Disk Access Optimisation | 50 |
| Assignment 5 | CO5 | Linux, Virtualisation, Containers and Cloud Deployment | 50 |
| | | **TOTAL** | **250** |

The five assignments collectively cover:

- Operating-system types, structures and services.
- System calls and process management.
- CPU scheduling and multithreading.
- Inter-process communication and synchronisation.
- Deadlock handling and resource allocation.
- Linking, loading and address binding.
- Paging, segmentation and virtual memory.
- Page-replacement algorithms.
- File systems and file-allocation methods.
- Disk scheduling and storage management.
- Linux administration.
- Virtualisation and containerisation.
- Cloud deployment and orchestration.

The common assessment expectation is to understand a problem, develop a suitable solution, implement it where required, measure its behaviour, compare alternatives and defend the final decision.

---

# 2. Assignment 1 – CO1
## Evaluate and Justify the Operating-System Design of a Smart Hospital

<badge>50 Marks</badge>

### Objective

Evaluate the operating-system requirements of a smart hospital and recommend suitable operating-system types, structures, services and configurations for different computing environments.

The scenario includes bedside patient monitors, nurses' handheld tablets, doctors' desktop workstations, a central database server, an MRI imaging server and communication between partner hospitals.

### Task Breakdown

| **Task** | **Topic** | **Marks** |
|---|---|---:|
| Task 1 | Requirements of Each Computing Environment | 4 |
| Task 2 | Types of Operating Systems | 3 |
| Task 3 | Operating-System Structures | 4 |
| Task 4 | Operating-System Services | 4 |
| Task 5 | System Calls | 4 |
| Task 6 | Cost of System Calls | 4 |
| Task 7 | Process Control System Calls | 4 |
| Task 8 | CPU States and Protection | 3 |
| Task 9 | Memory Hierarchy and I/O Systems | 4 |
| Task 10 | Measurement and Comparison | 4 |
| Task 11 | Recommend and Defend the Operating-System Plan | 4 |
| Task 12 | Re-evaluate and Adapt the Design | 8 |
| | **TOTAL** | **50** |

### Work Covered

**Tasks 1–4: Operating-System Design**

- Analyse the requirements of each hospital environment.
- Compare mainframe, desktop, multiprocessor, distributed, clustered, real-time and handheld operating systems.
- Evaluate monolithic, layered, microkernel, modular and hybrid structures.
- Identify essential and optional operating-system services.

**Tasks 5–7: System Calls and Process Control**

- Explain system-call categories and execution.
- Investigate system-call costs and file buffering.
- Implement file-copy operations using `read()` and `write()`.
- Demonstrate process creation, execution and termination using `fork()`, `exec()` and `waitpid()`.

**Tasks 8–10: Hardware Protection and Performance**

- Evaluate user mode, kernel mode, interrupts, traps and memory protection.
- Analyse memory hierarchy and I/O methods such as programmed I/O, interrupt-driven I/O and DMA.
- Measure system-call counts, execution times, process creation and thread creation.

**Tasks 11–12: Engineering Design**

- Recommend the complete operating-system architecture for the hospital.
- Re-evaluate the architecture after introducing remote patient monitoring and cloud-based patient records.

### Major Implementation and Evidence

- C programs for system calls and process control.
- Shell commands and `strace` output.
- Process and CPU-protection diagrams.
- System-call cost calculations.
- Process and thread measurements.
- Memory hierarchy and I/O comparisons.
- Final hospital architecture and revised design.

---

# 3. Assignment 2 – CO2
## Process Management, CPU Scheduling, Synchronisation and Deadlock Handling

<badge>50 Marks</badge>

### Objective

Analyse process-management and deadlock-handling techniques to improve CPU utilisation and system performance.

The assignment uses real-world scenarios involving online examination servers, cloud video encoding, traffic cameras, weather stations, railway ticket booking and shared laboratory equipment.

### Task Breakdown

| **Task** | **Topic** | **Marks** |
|---|---|---:|
| Task 1 | Processes and Process States | 6 |
| Task 2 | CPU Scheduling | 8 |
| Task 3 | Threads | 6 |
| Task 4 | Inter-Process Communication | 6 |
| Task 5 | Synchronisation: Critical Sections, Semaphores and Monitors | 11 |
| Task 6 | Deadlock Handling and the Banker's Algorithm | 13 |
| | **TOTAL** | **50** |

### Work Covered

**Task 1: Processes and Process States**

- Process-state diagram.
- Process Control Block (PCB).
- Process creation using `fork()`.
- Process observation using `ps` and `/proc`.
- Process-per-student versus thread-per-student design.

**Task 2: CPU Scheduling**

- FCFS.
- SJF.
- SRTF.
- Priority Scheduling.
- Round Robin.
- Gantt charts and scheduling calculations.
- Waiting time, turnaround time and response time.
- Time-quantum investigation.
- Starvation, ageing and Multilevel Feedback Queue.

**Task 3: Threads**

- POSIX threads.
- One thread per camera.
- Thread pools of 4, 8 and 16.
- User-level and kernel-level threads.
- Thread models.
- Speed-up and shared-counter synchronisation.

**Task 4: Inter-Process Communication**

- Pipes.
- Shared memory or message queues.
- End-of-file signalling.
- Data transfer and synchronisation.
- Transfer-time measurements for 1 MB, 10 MB and 100 MB.

**Task 5: Synchronisation**

- Mutual exclusion.
- Progress.
- Bounded waiting.
- Peterson's solution.
- Binary and counting semaphores.
- Producer–consumer problem.
- Readers–writers problem.
- Dining-philosophers problem.
- Monitors and condition variables.
- Lock granularity and throughput.
- Atomic operations and spin locks.

**Task 6: Deadlock Handling**

- Four necessary deadlock conditions.
- Resource-allocation graph.
- Deadlock prevention.
- Deadlock avoidance.
- Deadlock detection and recovery.
- Banker's safety algorithm.
- Resource-request algorithm.
- Safe-sequence calculation.
- Complexity prediction and benchmarking.

### Major Implementation and Evidence

- C process-management programs.
- Round Robin scheduling implementation.
- POSIX multithreading benchmarks.
- Pipe and shared-memory programs.
- Semaphore and mutex-based bounded-buffer program.
- Peterson's algorithm demonstration.
- Banker's algorithm implementation.
- Performance and complexity measurements.

---

# 4. Assignment 3 – CO3
## Memory Management

<badge>50 Marks</badge>

### Objective

Evaluate memory-management mechanisms such as linking, loading, paging, segmentation, virtual memory, demand paging, page-replacement techniques and disk management.

The assignment focuses on determining which memory-management mechanism is most effective for different system requirements.

### Task Breakdown

| **Task** | **Topic** | **Marks** |
|---|---|---:|
| Task 1 | Linking, Loading and Address Binding | 6 |
| Task 2 | Paging | 8 |
| Task 3 | Segmentation | 6 |
| Task 4 | Virtual Memory and Demand Paging | 8 |
| Task 5 | Page-Replacement Techniques | 8 |
| Task 6 | Disk Management | 4 |
| Task 7 | Memory-Management Plan from First Principles | 10 |
| | **TOTAL** | **50** |

### Work Covered

**Task 1: Linking, Loading and Address Binding**

- Static linking.
- Dynamic linking.
- Shared libraries.
- `dlopen()` and `dlsym()`.
- Linker and loader.
- Address binding.
- Position-independent code.
- Memory and start-up comparisons.

**Task 2: Paging**

- Logical-to-physical address translation.
- Page tables.
- External and internal fragmentation.
- TLB and effective access time.
- Multi-level page tables.
- Hashed page tables.
- Inverted page tables.
- Page-size comparison.

**Task 3: Segmentation**

- Segment tables.
- Base and limit registers.
- Segment-offset address translation.
- Segmentation with paging.
- Linux process memory maps.
- Segmentation faults.
- External fragmentation.

**Task 4: Virtual Memory and Demand Paging**

- Page-fault handling.
- Effective access time.
- Demand paging.
- Copy-on-write.
- Thrashing.
- Working-set model.
- Frame allocation.
- Sequential and random memory access.

**Task 5: Page-Replacement Techniques**

- FIFO.
- LRU.
- Optimal.
- Belady's anomaly.
- Clock or Second-Chance algorithm.
- Page-fault comparison.
- Page-replacement performance measurements.

**Task 6: Disk Management**

- Low-level formatting.
- Partitioning.
- Logical formatting.
- Boot block.
- Bad-block handling.
- Swap-space design.
- RAID 0, RAID 1, RAID 5 and RAID 10.
- RAID capacity and reliability calculations.

**Task 7: Memory-Management Design Challenge**

- Memory requirements of real-time, machine-learning and logging tasks.
- Comparison of contiguous allocation, paging, segmentation and demand paging.
- Memory locking using `mlock()`.
- Worst-case delay measurement.
- Page-replacement simulation.
- Real-time operating-system alternative.

### Major Implementation and Evidence

- Static and dynamic linking programs.
- Page-address translation program.
- Segmentation programs.
- Memory-access and copy-on-write experiments.
- FIFO, LRU and Optimal page-replacement implementations.
- RAID capacity calculations.
- Memory-locking programs.
- Page-fault and performance graphs.

---

# 5. Assignment 4 – CO4
## Optimising File Storage and Disk Access

<badge>50 Marks</badge>

### Objective

Evaluate file-system organisation, file-allocation strategies, modern file systems, disk-scheduling algorithms, distributed and cloud-based storage, and device management.

The assignment focuses on selecting file-storage and disk-access methods according to workload requirements and defending the choices using calculations and measurements.

### Task Breakdown

| **Task** | **Topic** | **Marks** |
|---|---|---:|
| Task 1 | File-System Design Review | 6 |
| Task 2 | File-Allocation Method | 8 |
| Task 3 | Modern File Systems | 9 |
| Task 4 | Disk-Scheduling Algorithms | 7 |
| Task 5 | Distributed and Cloud-Based Storage | 7 |
| Task 6 | Storage Design Challenge | 13 |
| | **TOTAL** | **50** |

### Work Covered

**Task 1: File-System Design Review**

- Flat-directory limitations.
- Directory-tree and acyclic-graph organisation.
- File naming.
- File attributes.
- Access-control lists and permissions.
- Shared project folders.
- Cloud-storage alternative.

**Task 2: File-Allocation Methods**

- Contiguous allocation.
- Linked allocation.
- File Allocation Table (FAT).
- Indexed allocation.
- File-growth and fragmentation analysis.
- Disk-read calculations.
- Inode direct and indirect pointers.
- Free-space management using bit vectors, linked lists, grouping and counting.
- First-fit allocation implementation.

**Task 3: Modern File Systems**

- FAT32.
- NTFS.
- ext4.
- XFS.
- Btrfs.
- ZFS.
- APFS.
- Journaling.
- Copy-on-write.
- File-system consistency.
- Real Linux file-system inspection.

**Task 4: Disk Scheduling**

- FCFS.
- SSTF.
- SCAN.
- C-SCAN.
- LOOK.
- C-LOOK.
- Head movement calculations.
- Fairness and starvation.
- Simulation of 10,000 requests.
- SSD scheduler inspection.

**Task 5: Distributed and Cloud Storage**

- NFS.
- HDFS.
- Ceph.
- Cloud object storage.
- Replication.
- Strong and eventual consistency.
- Device drivers.
- Buffering.
- Caching.
- Spooling.
- Block-device interface.

**Task 6: Video-Streaming Storage Design**

- Workload interpretation.
- Storage architecture.
- SSD, HDD and cloud-storage selection.
- Block-size selection.
- Disk-scheduler selection.
- Popular-video caching.
- Replication strategy.
- Storage capacity and throughput calculations.
- Disk-read calculations.
- Simulation of two alternative designs.
- Cloud object storage and CDN evaluation.

### Major Implementation and Evidence

- File-allocation C program.
- SSTF disk-scheduling program.
- Disk-scheduling simulation.
- File-system comparison tables.
- Linux storage inspection commands.
- Storage-capacity and performance calculations.
- Video-streaming storage architecture.
- Experimental results for two alternative designs.

---

# 6. Assignment 5 – CO5
## Linux, Containers and Virtualisation for Efficient Operations

<badge>50 Marks</badge>

### Objective

Evaluate how Linux operating systems, virtual machines, containers and cloud orchestration can be used to deploy services efficiently, support changing workloads, recover from failures and manage operational costs.

### Case Study – University Result Portal

The university operates a Python web application connected to a database.

The portal supports a few hundred users on ordinary days, but 150,000 students log in within a two-hour period on result day.

The existing deployment uses one physical Linux server and fails under peak demand.

The assignment develops a deployment and scaling solution using Linux administration, virtualisation, containerisation and cloud orchestration.

## Part A – Design (25 Marks)

| **Task** | **Topic** | **Marks** |
|---|---|---:|
| A1 | Evaluate Linux System Components | 7 |
| A2 | Design System Administration | 5 |
| A3 | Evaluate Virtualisation | 5 |
| A4 | Evaluate Containerisation | 4 |
| A5 | Design Cloud Deployment and Orchestration | 4 |
| | **PART A TOTAL** | **25** |

### A1: Linux System Components

- Linux architecture.
- Kernel, system libraries, shell and utilities.
- Process and memory management.
- Completely Fair Scheduler (CFS).
- Page cache and swapping.
- `/proc` and `/sys`.
- Virtual File System (VFS).
- Linux permissions and `sudo`.

### A2: System Administration

- User accounts and groups.
- systemd services.
- Log monitoring.
- IP addressing and DNS.
- Nginx reverse proxy.
- Firewall configuration.
- SSH keys.
- Shell scripting and cron.
- Configuration-management alternative using Ansible.

### A3: Virtualisation

- Type-1 hypervisors.
- Type-2 hypervisors.
- KVM, VMware ESXi, Xen and Hyper-V.
- VirtualBox and VMware Workstation.
- Full virtualisation.
- Para-virtualisation.
- Intel VT-x and AMD-V.
- Isolation, consolidation, snapshots and live migration.
- Virtualisation overhead and cost.

### A4: Containerisation

- Linux namespaces.
- Control groups (cgroups).
- Container image layers.
- Containers versus virtual machines.
- Start-up time.
- Memory consumption.
- Isolation and security.

### A5: Cloud Deployment and Orchestration

- IaaS, PaaS and SaaS.
- Cloud infrastructure design.
- Load balancers.
- Managed databases.
- Kubernetes Pods.
- Deployments.
- Services.
- Replicas.
- Resource requests and limits.
- Horizontal Pod Autoscaler (HPA).

## Part B – Implementation and Experimental Investigation (25 Marks)

| **Task** | **Topic** | **Marks** |
|---|---|---:|
| B1 | Administer the Linux Server | 7 |
| B2 | Containerise the Portal | 5 |
| B3 | Compare Bare Metal, Virtual Machine and Container | 8 |
| B4 | Final Engineering Decision | 5 |
| | **PART B TOTAL** | **25** |

### B1: Linux Server Administration

- Inspect processes, memory, disk and network.
- Use `ps`, `top`, `htop`, `free`, `vmstat`, `iostat`, `df` and `ss`.
- Create shell scripts for log analysis and disk-space alerts.
- Schedule scripts using cron.
- Complete the supplied Bash TODOs using `awk`, `sort` and `uniq`.
- Run the portal as a systemd service.
- Configure Nginx and firewall rules.

### B2: Containerise the Portal

- Create a Dockerfile.
- Build and run a Docker image.
- Use Docker Compose for the portal and database.
- Configure persistent database volumes.
- Apply CPU and memory limits.
- Inspect image layers using `docker history`.
- Optimise image size using slim images or multi-stage builds.
- Complete all three Dockerfile TODOs.

### B3: Deployment Comparison

Deploy the same application using:

1. Direct Linux deployment (bare metal).
2. Virtual machine.
3. Docker container.

Measure:

- Start-up time.
- Memory usage.
- Image or disk size.
- Requests per second.

Also:

- Deploy using Kubernetes on minikube or kind.
- Start with three replicas.
- Delete a Pod and demonstrate automatic recreation.
- Scale to six replicas under load.
- Configure CPU and memory requests and limits.
- Complete the two Kubernetes YAML TODOs.

### B4: Final Engineering Decision

- Deploy a second service alongside the result portal.
- Give both services separate resource limits.
- Estimate infrastructure capacity for 150,000 students in two hours.
- Use measured requests per second from B3.
- Compare private servers, containers, Kubernetes, public cloud and hybrid deployment.
- Justify the final decision using measurements and cost estimates.

### Major Implementation and Evidence

- Linux administration commands.
- Bash scripts and cron configuration.
- systemd and Nginx configuration.
- Dockerfile and Compose files.
- Container resource-limit evidence.
- Bare-metal, VM and Docker comparison.
- Kubernetes Deployment and Service YAML.
- Pod recovery and scaling evidence.
- Capacity estimation and production recommendation.

---

# 7. Consolidated Repository Structure

The repository is organised into five assignment folders.

```text
CSA0403-OS-192525148-Assignments/
│
├── README.md
│
├── Assignment 1/
│   ├── README.md
│   ├── Task01/
│   ├── Task02/
│   ├── Task03/
│   ├── Task04/
│   ├── Task05/
│   ├── Task06/
│   ├── Task07/
│   ├── Task08/
│   ├── Task09/
│   ├── Task10/
│   ├── Task11/
│   └── Task12/
│
├── Assignment 2/
│   ├── README.md
│   ├── Task01/
│   ├── Task02/
│   ├── Task03/
│   ├── Task04/
│   ├── Task05/
│   └── Task06/
│
├── Assignment 3/
│   ├── README.md
│   ├── Task01/
│   ├── Task02/
│   ├── Task03/
│   ├── Task04/
│   ├── Task05/
│   ├── Task06/
│   └── Task07/
│
├── Assignment 4/
│   ├── README.md
│   ├── Task01/
│   ├── Task02/
│   ├── Task03/
│   ├── Task04/
│   ├── Task05/
│   └── Task06/
│
└── Assignment 5/
    ├── README.md
    ├── PART A/
    │   ├── A1_Linux_System_Components/
    │   ├── A2_System_Administration/
    │   ├── A3_Virtualisation/
    │   ├── A4_Containerisation/
    │   └── A5_Cloud_Orchestration/
    │
    └── PART B/
        ├── B1_Linux_Administration/
        ├── B2_Containerisation/
        ├── B3_Deployment_Comparison/
        └── B4_Final_Engineering_Decision/
```

Each assignment folder contains its own detailed README, task-specific reports, source files, scripts, configurations and execution evidence.

The folder names above describe the intended organisation. Retain the actual file and folder names used in the repository unless you deliberately rename them.

---

# 8. Technologies and Tools Used Across the Assignments

| **Area** | **Technologies / Tools** |
|---|---|
| Programming | C, Python, Bash |
| Compiler | GCC |
| Operating System | Linux / Ubuntu / WSL 2 |
| Process Management | `fork()`, `exec()`, `waitpid()`, `ps`, `/proc` |
| CPU Scheduling | FCFS, SJF, SRTF, Priority, Round Robin |
| Multithreading | POSIX Threads (`pthread`) |
| Synchronisation | Semaphores, mutexes, monitors, C11 atomics |
| IPC | Pipes, Shared Memory, Message Queues |
| Deadlock Handling | Banker's Algorithm |
| Memory Management | Paging, Segmentation, Virtual Memory |
| Page Replacement | FIFO, LRU, Optimal, Clock |
| File Systems | FAT32, NTFS, ext4, XFS, Btrfs, ZFS, APFS |
| Disk Management | RAID, Disk Scheduling, File Allocation |
| Linux Inspection | `strace`, `time`, `stat`, `df`, `lsblk`, `iostat` |
| Virtualisation | KVM, VirtualBox, VMware and related hypervisors |
| Containers | Docker, Docker Compose |
| Cloud Orchestration | Kubernetes, minikube, kind |
| Networking | Nginx, DNS, SSH, firewall tools |
| Performance Testing | `time`, `strace`, `fio`, `ab`, `wrk` or `hey` |

The exact tools required differ by assignment. Use only the technologies relevant to the particular task and record the actual versions used.

---

# 9. General Compilation and Execution Guidelines

## C Programs

Compile with:

```bash
gcc -std=c11 -Wall -Wextra source_file.c -o output_program
```

Run with:

```bash
./output_program
```

## POSIX Thread Programs

Compile with:

```bash
gcc -std=c11 -Wall -Wextra -pthread source_file.c -o output_program
```

## Shell Scripts

```bash
chmod +x script.sh
./script.sh
```

## Python Programs

```bash
python3 program.py
```

## System Performance Measurements

```bash
time ./program
strace -c ./program
```

## Docker

```bash
docker build -t application-name .
docker run -p 8000:8000 application-name
docker ps
docker images
docker stats
```

## Docker Compose

```bash
docker compose up -d
docker compose ps
docker compose down
```

## Kubernetes

```bash
kubectl apply -f deployment.yaml
kubectl get pods
kubectl get deployments
kubectl get services
kubectl describe pod <pod-name>
```

Use the precise commands and filenames given in each task-specific README.

---

# 10. Consolidated Environment Information

Record the actual environments used across the assignments.

| **Environment Item** | **Details** |
|---|---|
| Linux distribution | ______________________________ |
| Distribution version | ______________________________ |
| Linux kernel | ______________________________ |
| GCC version | ______________________________ |
| Python version | ______________________________ |
| Docker version | ______________________________ |
| Docker Compose version | ______________________________ |
| Kubernetes version | ______________________________ |
| Virtualisation platform | ______________________________ |
| CPU architecture | ______________________________ |
| RAM | ______________________________ |
| Storage type | ______________________________ |

Useful commands:

```bash
cat /etc/os-release
uname -a
gcc --version
python3 --version
docker --version
docker compose version
kubectl version
lscpu
free -h
lsblk
```

Do not enter assumed version information. Record what is actually installed on the machine used for execution.

---

# 11. Consolidated Submission Checklist

## Assignment 1 – CO1

- [ ] All 12 task reports.
- [ ] Operating-system requirements and comparison tables.
- [ ] System-call programs and evidence.
- [ ] Process-control implementation.
- [ ] CPU protection and I/O analysis.
- [ ] Performance measurements.
- [ ] Final smart-hospital architecture.
- [ ] Remote-monitoring and cloud redesign.

## Assignment 2 – CO2

- [ ] All six task reports.
- [ ] Process creation and process-state evidence.
- [ ] CPU scheduling calculations and Gantt charts.
- [ ] Round Robin implementation and quantum tests.
- [ ] POSIX thread implementation and speed-up measurements.
- [ ] Pipe and shared-memory IPC implementations.
- [ ] Semaphore and mutex-based synchronisation programs.
- [ ] Banker's algorithm and safe-sequence evidence.
- [ ] Deadlock analysis and final recommendation.

## Assignment 3 – CO3

- [ ] All seven task reports.
- [ ] Static and dynamic linking programs.
- [ ] Paging and address-translation implementation.
- [ ] Segmentation experiments.
- [ ] Virtual-memory and copy-on-write experiments.
- [ ] FIFO, LRU and Optimal page-replacement programs.
- [ ] Disk-management and RAID calculations.
- [ ] Memory-locking implementation.
- [ ] Experimental measurements and graphs.

## Assignment 4 – CO4

- [ ] All six task reports.
- [ ] File-system design and directory organisation.
- [ ] File-allocation C program.
- [ ] Maximum file-size and free-space calculations.
- [ ] Modern file-system comparison.
- [ ] SSTF disk-scheduling implementation.
- [ ] Disk-scheduling simulation.
- [ ] Distributed and cloud-storage comparison.
- [ ] Video-streaming storage architecture.
- [ ] Two-design simulation and recommendation.

## Assignment 5 – CO5

- [ ] Part A design documents.
- [ ] Part B implementation evidence.
- [ ] Linux administration command outputs.
- [ ] Bash scripts and cron evidence.
- [ ] systemd, Nginx and firewall configuration.
- [ ] Dockerfile and Docker Compose files.
- [ ] Container build and execution evidence.
- [ ] Bare-metal, VM and container measurements.
- [ ] Kubernetes deployment and scaling evidence.
- [ ] Pod failure and recovery evidence.
- [ ] Capacity calculation for 150,000 students.
- [ ] Final production architecture and cost analysis.

---

# 12. General Reporting and Evaluation Guidelines

1. Complete every task and all its subdivisions.
2. Follow the marks distribution and terminology in the respective assignment question papers.
3. Include diagrams, tables, calculations and technical explanations wherever required.
4. Complete all supplied "Complete the Code" TODOs.
5. Avoid unnecessary modifications to the remaining supplied program.
6. Compile and execute C programs using the specified GCC warning flags.
7. Run all required system-level experiments in the relevant Linux environment.
8. Include actual terminal outputs and screenshots as supporting evidence.
9. Do not fabricate performance measurements, execution times, memory usage or benchmark results.
10. Clearly identify assumptions in calculations.
11. Include time and space complexity using Big-O notation wherever required.
12. Discuss the alternative concept specified in each task.
13. Compare theoretical expectations with actual experimental results.
14. Justify every engineering recommendation.
15. Ensure the README, report, source files and generated outputs are consistent.
16. Be prepared to explain and defend the submitted implementation during assessment verification.

---

# 13. GitHub Repository Organisation and Submission

This repository consolidates the five Operating Systems assignments.

**Repository Name:**

```text
CSA0403-OS-192525148-Assignments
```

**Repository Link:**

https://github.com/juhisakshisurin/CSA0403-OS-192525148-Assignments

The assignment papers specify GitHub submission requirements and repository naming instructions. Where faculty requires separate repositories for individual assignments, follow those instructions even though the work is organised here in a consolidated repository.

The repository should maintain:

- One top-level folder for each assignment.
- Separate task folders.
- Task-specific README files.
- Source code and configuration files.
- Input and output files.
- Execution screenshots.
- Actual measurement evidence.
- Assignment reports.
- Progressive commit history.

At least one meaningful commit should be maintained for every task wherever required by the assignment instructions.

---

# 14. Overall Learning Outcome

The five Operating Systems assignments collectively demonstrate the application and evaluation of fundamental and advanced operating-system concepts.

The work progresses through five course outcomes:

| **Course Outcome** | **Learning Focus** |
|---|---|
| CO1 | Evaluate operating-system types, structures, services and system calls for different computing environments. |
| CO2 | Analyse process management, CPU scheduling, synchronisation and deadlock-handling techniques. |
| CO3 | Evaluate linking, loading, paging, segmentation, virtual memory and disk management. |
| CO4 | Optimise file allocation, file systems, disk scheduling and distributed storage. |
| CO5 | Evaluate Linux administration, virtualisation, containerisation and cloud orchestration. |

The complete repository demonstrates practical implementation, system-level observation, algorithmic analysis, performance measurement and evidence-based engineering decisions.

**FINAL REPOSITORY: OPERATING SYSTEMS – CO1 TO CO5**

**TOTAL ASSIGNMENTS: 5**

**TOTAL ASSESSMENT MARKS: 250**

**END OF CONSOLIDATED OPERATING SYSTEMS ASSIGNMENTS**
