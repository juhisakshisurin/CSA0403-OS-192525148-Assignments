
# Task 6 – Evaluate the Cost of System Calls

## Student Details

| Field | Details |
|:---|:---|
| Name | Juhi Surin |
| Register Number | 192525148 |
| Course | CSA0403 – Operating Systems |
| Assignment | Assignment 1 – CO1 |
| Task | Task 6 – Evaluate the Cost of System Calls |
| Under the Supervision of | Dr. Velmurugan J. |

## Objective

To evaluate the cost of system calls, analyse the effect of buffer size on `read()` and `write()` operations, and measure system-call activity using `strace`.

## 1. Introduction

A system call is a request made by a user program to the operating-system kernel for performing a particular operation. System calls are more expensive than ordinary function calls because they involve switching from user mode to kernel mode.

In the smart hospital, the database server frequently reads and writes patient records. Therefore, reducing unnecessary system calls during file operations can improve backup performance.

## 2. Why Does Each System Call Cost More Than an Ordinary Function Call?

An ordinary function call executes within the same process and normally remains in user mode. However, a system call requires the CPU to enter kernel mode to perform a protected operation.

Reasons for additional cost:

- **Mode Switching:** The CPU switches from user mode to kernel mode.
- **Parameter Validation:** The kernel checks the parameters and permissions.
- **Kernel Processing:** The operating system performs the requested operation.
- **Context Preservation:** The CPU maintains the required execution state.
- **Return to User Mode:** The kernel returns the result to the application.

**Evaluation:** Frequent system calls increase execution overhead. Therefore, using larger buffers can reduce the number of system calls and improve file-copying performance.

## 3. Effect of Buffer Size on `read()` and `write()` Calls

A buffer is a temporary memory area used to store data during file operations.

When a database copies a file, the buffer size determines how much data is transferred in each read and write operation.

**Formula:**

Number of read operations, including the final EOF read:

`N = ceil(n / b) + 1`

Where:

- `n` = Total file size in bytes.
- `b` = Buffer size in bytes.
- `ceil()` = Rounds the result up to the next integer.

**Example:** Consider a file of 10,000 bytes.

| Buffer Size | Number of `read()` Calls |
|:---:|---:|
| 1 byte | 10001 |
| 512 bytes | 21 |
| 4096 bytes | 4 |

**Evaluation:**

- A 1-byte buffer requires 10,001 read calls, creating high system-call overhead.
- A 512-byte buffer reduces the number of calls to 21.
- A 4096-byte buffer requires only 4 read calls.

Therefore, a larger buffer reduces repeated transitions between user mode and kernel mode.

## 4. How Library Buffering Reduces System Calls

C library functions such as `fread()` and `fwrite()` use internal buffers to reduce the number of system calls.

**Without library buffering:**

- The program directly uses `read()` and `write()`.
- Small buffers can cause frequent system calls.
- More kernel transitions increase overhead.

**With library buffering:**

- `fread()` collects data into a library-managed buffer.
- `fwrite()` collects output data before writing it.
- The library transfers larger blocks using fewer underlying system calls.

**Evaluation:** Library buffering is useful for large backup operations because it reduces system-call overhead and simplifies file handling. However, it requires additional memory for the library buffers.

## 5. C Program Using `read()` and `write()`

**Aim:** To copy a file using the `read()` and `write()` system calls and compare the number of read operations for different buffer sizes.

The program creates a file containing 10,000 bytes and copies it using three different buffer sizes.

**Compilation:**

```bash
gcc task6.c -o task6
```

**Execution:**

```bash
./task6
```

**Observed output:**

```text
buffer    1 bytes: 10001 read() calls
buffer  512 bytes:    21 read() calls
buffer 4096 bytes:     4 read() calls
```

**Explanation:**

1. The program creates a file containing 10,000 bytes.
2. It copies the file using three different buffer sizes.
3. It counts every `read()` call, including the final call that detects the end of the file.
4. The final output compares the number of read operations.

## 6. Measure System Calls Using `strace`

The `strace` command is used to observe the system calls made by a program.

**Commands:**

```bash
strace -c ./task6
```

To display only read and write system calls:

```bash
strace -c -e trace=read,write ./task6
```

The output provides information such as:

- Number of system calls.
- Total time spent in system calls.
- Average time per call.
- Percentage of time spent in each operation.

**Evaluation:** The program output and `strace` results can be compared to understand how buffer size affects system-call frequency. The exact `strace` count may include additional calls made by the runtime environment, so the program's own counter is useful for identifying its copy-loop read calls.

### Actual Screenshot Evidence

The supplied terminal screenshot shows the following program results:

| Buffer Size | Program Counter |
|:---:|---:|
| 1 byte | 10001 |
| 512 bytes | 21 |
| 4096 bytes | 4 |

The supplied `strace` summary reports **10,027 total `read` calls** for the traced execution.

![Task 6 – Buffer Size and strace Output](Task06_Buffer_and_strace.png)

## 7. Alternative Concept – Memory-Mapped Files (`mmap`)

Instead of using `read()` and `write()` with a buffer, the hospital backup program can use memory-mapped files through the `mmap()` system call.

Memory mapping maps a file into a process's virtual address space. The program can then access file contents using memory operations.

### Comparison and Complexity Analysis

**Using `read()` and `write()`:**

- Number of read calls is approximately `O(n/b)`.
- Additional buffer space is `O(b)`.
- Total data-processing time is `O(n)`.

**Using `mmap()`:**

- Mapping requires a small number of explicit system calls.
- Accessing all file contents still requires processing `n` bytes, so the work is `O(n)`.
- The virtual mapping requires address space proportional to the mapped file size, `O(n)`.
- Physical memory use depends on how many mapped pages are resident.

### Justification

Memory mapping can reduce explicit read/write system calls and provide convenient file access. However, mapping very large files can consume substantial virtual address space and introduce page-fault overhead.

For the hospital's backup program, buffered `read()` and `write()` operations are a practical choice because they provide predictable, controlled memory usage. A 4096-byte buffer performs better than the smaller buffers in the given experiment.

## 8. Conclusion

System-call overhead affects the performance of file operations. Small buffer sizes increase the number of read and write operations, while larger buffers reduce system-call frequency.

Library buffering also improves efficiency by reducing repeated kernel transitions. Based on the given 10,000-byte experiment, the 4096-byte buffer is the best choice among the three tested sizes. Memory mapping is an alternative, but its memory requirements must be considered when processing large hospital backup files.

