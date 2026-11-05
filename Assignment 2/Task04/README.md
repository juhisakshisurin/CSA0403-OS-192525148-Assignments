# Assignment 2 – Task 4: Inter-Process Communication

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

## 1. Task Overview

**Q4 – Inter-Process Communication (6 Marks, K5 – Evaluate)** is based on a weather-station data pipeline. One process reads sensor values and another analyses them. They must exchange data continuously without losing readings.

The task requires:
- An implementation using a pipe.
- A second implementation using shared memory or a message queue.
- An explanation of how a pipe signals end-of-data.
- A comparison of shared-memory and message-passing models.
- An explanation of payload copying and synchronisation.
- Transfer-time measurements for 1 MB, 10 MB and 100 MB.
- A justified choice of communication method.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment2_Task04_Report.pdf` | Report without student identity details. Covers pipe IPC, shared memory, semaphores, EOF, copying, synchronization, complexity and measurement method. |
| `pipe_readings.c` | Completed version of the assignment's supplied pipe program. |
| `ipc_benchmark.c` | Compares pipe and shared-memory + semaphore transfers at 1, 10 and 100 MiB. |
| `run_task4.sh` | Compiles and runs both programs and saves output/environment evidence. |

## 3. System Requirements

| Component | Requirement |
|---|---|
| Language | C |
| Compiler | GCC |
| Operating System | Linux / Ubuntu / WSL 2 |
| IPC methods | POSIX pipe, POSIX shared memory and process-shared POSIX semaphores |

Record the actual machine details using:

```bash
gcc --version
lsb_release -a
uname -a
```

The run script automatically saves compiler and Linux information to `environment.txt`.

## 4. Compile and Run

Keep all four files in the same directory. Open Ubuntu/WSL terminal in that directory and run:

```bash
chmod +x run_task4.sh
./run_task4.sh
```

The script executes the following commands:

### Compile the supplied pipe example

```bash
gcc -std=c11 -Wall -Wextra pipe_readings.c -o pipe_readings
```

### Compile the pipe/shared-memory benchmark

```bash
gcc -std=c11 -Wall -Wextra ipc_benchmark.c -o ipc_benchmark -pthread -lrt
```

### Run the pipe example

```bash
./pipe_readings
```

Expected output:

```text
analyser: 6 readings, average 35.5, max 41
reader: done
```

The six readings are supplied in the assignment: 31, 33, 36, 34, 41 and 38.

## 5. Pipe: End-of-Data Behaviour

The parent writes the sensor readings into the pipe and then closes its write descriptor (`fd[1]`). The child continues reading all buffered data. When the pipe buffer becomes empty and all write descriptors have been closed, `read()` returns 0, indicating EOF.

If the parent does not close `fd[1]`, the child cannot observe EOF because a writer is still open. The child can remain blocked in `read()`, while the parent waits for it with `waitpid()`.

## 6. Shared-Memory Implementation

The benchmark uses POSIX shared memory created with `shm_open()` and `ftruncate()`, then mapped with `mmap()`.

Two process-shared semaphores coordinate a single 256 KiB shared buffer:
- `empty`: producer may write to the slot.
- `full`: consumer may read the slot.

The producer writes a chunk and posts `full`. The consumer processes it and posts `empty`. A final flag signals end-of-transfer.

## 7. Benchmark Method

The benchmark transfers 1 MiB, 10 MiB and 100 MiB using both a pipe and shared memory. The program uses the same deterministic payload in both cases and validates the received data with a checksum.

Each case runs three trials and reports the median elapsed time. Timing includes payload generation/transfer and consumer completion; process and IPC-object setup is outside the timed interval.

The benchmark labels the requested sizes as MiB, where 1 MiB = 1,048,576 bytes. Use the measured output from your own computer.

## 8. Results to Record

The script creates `ipc_benchmark_results.txt`. Copy the timings from that file into your report or a results table.

| Data size | Pipe median time | Shared-memory median time | Validation |
|---|---|---|---|
| 1 MiB | Record local measurement | Record local measurement | PASS |
| 10 MiB | Record local measurement | Record local measurement | PASS |
| 100 MiB | Record local measurement | Record local measurement | PASS |

Do not use timing values from another computer as your own. Transfer times depend on the CPU, Linux kernel, memory subsystem, scheduler and system load.

## 9. Generated Evidence Files

After running `./run_task4.sh`, the directory will contain:

| Generated file | Contents |
|---|---|
| `pipe_readings_output.txt` | Output of the six-reading pipe example. |
| `ipc_benchmark_results.txt` | Pipe and shared-memory transfer timings and checksum validation. |
| `environment.txt` | GCC version, Linux distribution and kernel details. |

Capture a screenshot of the terminal showing the pipe output and the six successful benchmark rows.

## 10. Result

The pipe implementation demonstrates kernel-managed stream communication and EOF signalling. The shared-memory implementation demonstrates direct access to a common mapped region coordinated by process-shared semaphores. The benchmark measures both methods at increasing payload sizes and validates the data with checksums so that a justified IPC choice can be made from local measurements.

## 11. Repository Submission Reminder

The Assignment 2 repository name specified in the question paper is:

`CSA04-192525148-Assignment2`

Keep this Task04 folder separate from the other task folders. Include the report, both C programs, the shell script, generated output files, environment details and screenshots. Commit Task 4 separately and share the working repository link with the course faculty.
