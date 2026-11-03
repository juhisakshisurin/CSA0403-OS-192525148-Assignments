
# Task 10 – Measure and Compare

## Student Details

| Field | Details |
|:---|:---|
| Name | Juhi Surin |
| Register Number | 192525148 |
| Course | CSA0403 – Operating Systems |
| Assignment | Assignment 1 – CO1 |
| Task | Task 10 – Measure and Compare |
| Under the Supervision of | Dr. Velmurugan J. |

## Objective

To measure and compare system-call counts, CPU execution time and the time required to create processes and threads in Linux.

## 1. Introduction

Performance measurement helps evaluate how efficiently an operating system manages system calls, processes and threads.

In a smart hospital, efficient resource management is important because patient monitoring, database operations and MRI image processing must run reliably.

This task measures system calls, CPU execution time and the time required to create processes and threads in Linux.

## 2. Number of System Calls for Different Buffer Sizes

The `strace -c` command is used to count system calls made by a program.

The copy program from Task 6 is executed using different buffer sizes.

**Commands:**

```bash
strace -c ./copy
```

To focus on read and write system calls:

```bash
strace -c -e trace=read,write ./copy
```

### Comparison

| Buffer Size | Expected `read()` Calls |
|:---:|---:|
| 1 byte | 10001 |
| 512 bytes | 21 |
| 4096 bytes | 4 |

**Evaluation:**

- A 1-byte buffer requires a large number of read operations.
- A 512-byte buffer reduces the number of system calls.
- A 4096-byte buffer requires fewer read operations.
- Larger buffers reduce the overhead of repeated system calls.

Therefore, a 4096-byte buffer is more efficient among the three tested sizes in terms of read-call count.

**Note:** The program's own counter and `strace` may show different totals because `strace` can also count system calls made during program startup and termination.

## 3. Time Spent in User Mode and Kernel Mode

The Linux `time` command measures the execution time of a program.

**Command:**

```bash
time ./copy
```

It provides three important values:

| Measurement | Meaning |
|:---|:---|
| Real Time | Total elapsed time. |
| User Time | CPU time spent executing user-level instructions. |
| System Time | CPU time spent executing kernel operations. |

**Evaluation:**

User time represents the time spent executing the program's instructions.

System time represents the time spent performing operating-system operations, such as reading and writing files.

A program that performs many system calls may spend more time in kernel mode.

## 4. Measuring the Time to Create 1000 Processes

The following C program creates 1000 child processes using `fork()`.

### C Program – Process Creation

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define N 1000

int main()
{
    pid_t pid[N];
    int i;

    for (i = 0; i < N; i++)
    {
        pid[i] = fork();

        if (pid[i] < 0)
        {
            perror("fork");
            break;
        }

        if (pid[i] == 0)
        {
            _exit(0);
        }
    }

    for (int j = 0; j < i; j++)
    {
        waitpid(pid[j], NULL, 0);
    }

    printf("Processes created: %d\n", i);
    return 0;
}
```

### Compilation and Execution

```bash
gcc process.c -o process
time ./process
```

### Explanation

1. The program initialises an array to store process IDs.
2. The `fork()` system call creates a child process.
3. Each child terminates using `_exit(0)`.
4. The parent stores the process IDs.
5. The parent uses `waitpid()` to collect the child processes.
6. The program displays the number of processes created.

## 5. Measuring the Time to Create 1000 Threads

The following C program creates 1000 threads using `pthread_create()`.

### C Program – Thread Creation

```c
#include <stdio.h>
#include <pthread.h>

#define N 1000

void *task(void *arg)
{
    return NULL;
}

int main()
{
    pthread_t threads[N];
    int i;

    for (i = 0; i < N; i++)
    {
        if (pthread_create(&threads[i], NULL, task, NULL) != 0)
        {
            printf("Thread creation failed at %d\n", i);
            break;
        }
    }

    for (int j = 0; j < i; j++)
    {
        pthread_join(threads[j], NULL);
    }

    printf("Threads created: %d\n", i);
    return 0;
}
```

### Compilation and Execution

```bash
gcc thread.c -o thread -pthread
time ./thread
```

### Explanation

1. The program declares an array to store thread IDs.
2. `pthread_create()` creates a new thread.
3. Each thread executes the task function.
4. `pthread_join()` waits for the created threads to finish.
5. The program displays the number of threads created.

## 6. Measurement Comparison

Record the actual values obtained from your Ubuntu terminal.

| Measurement | 1000 Processes | 1000 Threads |
|:---|:---:|:---:|
| Real Time | ______ | ______ |
| User Time | ______ | ______ |
| System Time | ______ | ______ |
| Number Created | 1000 | 1000 |

**Evaluation:**

Processes generally require more resources because each process has its own address space and process-management information.

Threads share the address space and resources of their parent process. Therefore, thread creation generally requires less memory and overhead than process creation.

However, actual execution time depends on system resources, scheduling, CPU load and implementation.

## 7. Alternative Concept and Complexity Analysis

**Alternative:** Creating Threads Instead of Processes.

If threads are created instead of processes, the operating system can manage multiple execution paths within a single process.

### Time Complexity

For creating `n` processes:

`Time Complexity = O(n)`

For creating `n` threads:

`Time Complexity = O(n)`

Both operations involve creating `n` execution units. However, the actual execution time differs because process creation and thread creation have different overheads.

### Space Complexity

**Processes:**

Each process requires its own virtual address space and process-management information.

**Threads:**

Threads share code, data and heap memory, but each thread requires its own stack and thread-management information.

Therefore, threads generally consume less additional memory than processes.

### Comparison Between Processes and Threads

| Feature | Processes | Threads |
|:---|:---|:---|
| Creation | Uses `fork()` | Uses `pthread_create()` |
| Memory | Separate virtual address spaces | Share process address space |
| Communication | Requires IPC mechanisms | Can communicate using shared variables |
| Creation Overhead | Generally higher | Generally lower |
| Context Switching | Generally more expensive | Generally less expensive |
| Isolation | Stronger isolation | Weaker isolation |

## 8. How Measurements Support Earlier Judgements

The measurements help evaluate the earlier design decisions.

- Task 6 showed that increasing the buffer size can reduce the number of read system calls.
- The `time` command helps identify the CPU time spent in user mode and kernel mode.
- Process and thread measurements help compare their creation overhead.
- Threads generally use fewer additional resources because they share the process address space.

The results help select suitable execution methods for hospital applications.

For the hospital database server, threads can be useful for handling multiple requests efficiently. Separate processes are suitable when stronger isolation between applications is required.

## 9. Conclusion

Performance measurement helps identify the efficiency of operating-system operations.

Larger I/O buffers reduce repeated system calls, while threads generally require fewer resources than separate processes.

The `strace` and `time` commands help measure system-call behaviour and CPU execution time.

Therefore, selecting suitable buffer sizes and process-management techniques improves the overall efficiency of the hospital system.

