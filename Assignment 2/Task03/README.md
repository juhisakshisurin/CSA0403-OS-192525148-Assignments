# Assignment 2 – Task 3: Threads

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 – Operating Systems |
| **Assignment** | Assignment 2 – CO2 |
| **Task** | Task 3 – Threads |
| **Date** | 03-11-2026 |
| **Under the Supervision of** | Dr. Velmurugan J |

---

## 1. Introduction

This task focuses on multithreading and its application in a city traffic-camera number-plate recognition system.

The system processes frames from 64 cameras. Sequential processing can cause delays during peak hours. POSIX threads are used to process camera frames concurrently and evaluate the performance of different threading configurations.

## 2. Aim

To implement and evaluate multithreaded processing using POSIX threads, compare different thread configurations, understand thread resource sharing and mapping models, and analyse the performance and limitations of threads compared with processes.

## 3. Objectives

- Understand the concept of multithreading.
- Process camera frames using POSIX threads.
- Compare one thread per camera with thread pools of 4, 8 and 16 threads.
- Identify shared and private thread resources.
- Differentiate user-level and kernel-level threads.
- Understand many-to-one, one-to-one and many-to-many thread models.
- Measure execution time and calculate speed-up.
- Analyse the effect of increasing the number of threads.
- Understand race conditions and shared-counter synchronisation.
- Compare thread-based and process-based solutions.

## 4. Problem Description

A city traffic system reads number plates from 64 cameras. The initial implementation processes camera frames sequentially, resulting in delays during peak hours.

Multithreading allows multiple camera-processing tasks to execute concurrently. This task evaluates different threading configurations to identify an efficient approach for processing the camera workload.

## 5. Threading Configurations

The following configurations are considered:

| Configuration | Description |
|---|---|
| Sequential Processing | Processes camera frames one after another. |
| 64 Threads | Creates one thread for each camera. |
| 4-Thread Pool | Uses four worker threads to process camera frames. |
| 8-Thread Pool | Uses eight worker threads to process camera frames. |
| 16-Thread Pool | Uses sixteen worker threads to process camera frames. |

## 6. Concepts Covered

| Concept | Description |
|---|---|
| POSIX Threads | Thread creation and management using the pthread library. |
| Shared Resources | Resources accessible by multiple threads within a process. |
| Private Resources | Resources maintained separately for each thread. |
| User-Level Threads | Threads managed by a user-space thread library. |
| Kernel-Level Threads | Threads managed and scheduled by the operating system kernel. |
| Many-to-One Model | Multiple user threads mapped to one kernel thread. |
| One-to-One Model | Each user thread mapped to a separate kernel thread. |
| Many-to-Many Model | Multiple user threads mapped to a smaller or equal number of kernel threads. |
| Race Condition | Incorrect behaviour caused by unsynchronised concurrent access to shared data. |
| Mutex | A mutual-exclusion mechanism used to protect critical sections. |

## 7. Performance Evaluation

The performance of different threading configurations is evaluated using execution time and speed-up.

**Speed-up Formula:**

Speed-up = Sequential Execution Time / Multithreaded Execution Time

The configurations compared are:

- Sequential processing.
- One thread per camera (64 threads).
- Thread pool of 4 threads.
- Thread pool of 8 threads.
- Thread pool of 16 threads.

The analysis considers thread-management overhead, context switching, CPU-core availability, synchronisation overhead and shared-resource contention.

Actual execution measurements should be recorded from the system used for testing.

## 8. Synchronisation

A shared counter accessed by multiple threads without protection may result in a race condition and lost updates.

A mutex lock protects the critical section and ensures that only one thread updates the shared counter at a time.

The pthread mutex functions used for synchronisation include:

- `pthread_mutex_init()`
- `pthread_mutex_lock()`
- `pthread_mutex_unlock()`
- `pthread_mutex_destroy()`

## 9. Alternative Concept

The alternative approach uses multiple processes instead of threads to process camera frames.

The comparison considers:

- Process creation and switching overhead.
- Thread creation and switching overhead.
- Separate address spaces versus shared address space.
- Memory consumption.
- Inter-process communication versus shared variables.
- Result collection.
- Synchronisation and fault isolation.

## 10. Technologies and System Requirements

| Component | Requirement |
|---|---|
| Programming Language | C |
| Threading Library | POSIX Threads (pthreads) |
| Compiler | GCC |
| Operating System | Ubuntu Linux / WSL 2 |
| Compilation Option | `-pthread` |

## 11. Compiler and Operating System Information

Record the actual environment used for execution.

| Field | Details |
|---|---|
| Compiler | GCC |
| GCC Version | Obtain using `gcc --version` |
| Linux Distribution | Obtain using `lsb_release -a` |
| Kernel Version | Obtain using `uname -r` |
| Threading Library | POSIX pthreads |

## 12. Compilation and Execution

Navigate to the Task03 project directory.

### Compile

```bash
gcc -std=c11 -Wall -Wextra -pthread thread_camera.c -o thread_camera
```

### Execute

```bash
./thread_camera
```

**Note:** Update the source filename and executable name if the actual implementation uses different names.

## 13. Performance Measurement

Record the execution time for each configuration using the same workload.

| Configuration | Execution Time | Speed-up |
|---|---|---|
| Sequential | Record measured value | 1.00 |
| 64 Threads | Record measured value | Calculate |
| 4 Threads | Record measured value | Calculate |
| 8 Threads | Record measured value | Calculate |
| 16 Threads | Record measured value | Calculate |

Speed-up is calculated by dividing the sequential execution time by the execution time of the corresponding multithreaded configuration.

Do not enter estimated values as actual measurements.

## 14. Files Included

| File Name | Description |
|---|---|
| `Assignment2_Task03_Report.pdf` | Complete academic report covering multithreading, pthreads, thread models, performance analysis, synchronisation and process comparison. |
| `README.md` | Project documentation, student details, concepts, environment and execution instructions. |
| `thread_camera.c` | POSIX multithreaded camera-processing implementation, if submitted separately. |
| `execution_output.txt` | Captured program output, if generated. |
| `performance_results.txt` | Actual performance measurements, if generated. |
| `execution_evidence.png` | Screenshot of successful program execution, if generated. |

## 15. Expected Outcome

The task demonstrates how POSIX threads can be used to process multiple camera workloads concurrently. It also evaluates the impact of thread-pool size on performance, explains thread resource sharing and synchronisation, and compares multithreading with process-based execution.

## 16. Result

The multithreading approaches are evaluated for the 64-camera processing scenario. Their performance, resource requirements and synchronisation behaviour are analysed to identify a suitable threading strategy for the given workload.
