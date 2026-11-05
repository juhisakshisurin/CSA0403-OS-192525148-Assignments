# Assignment 2 – Task 5: Synchronisation, Critical Sections, Semaphores and Monitors

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

---

## 1. Task Overview

**Q5 – Synchronisation: Critical Sections, Semaphores and Monitors (11 Marks, K5 – Evaluate)** is based on a railway ticket-booking system in which many agents may try to reserve seats at the same time.

The task covers:

- Mutual exclusion, progress and bounded waiting.
- Peterson's solution for two processes and its assumptions on modern processors.
- Binary and counting semaphores for protecting shared resources.
- Bounded-buffer (producer–consumer), readers–writers and dining-philosophers problems.
- Monitors and condition variables.
- Bounded-buffer verification with 1 producer/1 consumer and 4 producers/4 consumers.
- A 100-run validation of the original 20-item producer–consumer case.
- Throughput comparison of a global lock, per-train locks and per-coach locks.
- C11 atomic operations and spin locks compared at 1, 4 and 16 threads.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment2_Task05_Report.pdf` | Complete report; intentionally excludes student identity details. |
| `bounded_buffer.c` | Completed TODO 1 and TODO 2; supports 1P/1C and 4P/4C tests; verifies each item exactly once. |
| `peterson_demo.c` | Two-thread Peterson demonstration implemented with C11 sequentially-consistent atomics. |
| `booking_locks.c` | Measures throughput for one global lock, one lock per train and one lock per coach. |
| `atomic_spin_benchmark.c` | Compares `atomic_fetch_add`, an atomic-flag spin lock and a pthread mutex using 1, 4 and 16 threads. |
| `run_task5.sh` | Compiles and executes the experiments and saves output files. |

## 3. Requirements

| Component | Requirement |
|---|---|
| Language | C11 |
| Compiler | GCC |
| Operating System | Linux / Ubuntu / WSL 2 |
| Thread library | POSIX Threads (`pthread`) |
| Synchronisation | POSIX semaphores, pthread mutexes, C11 atomics |

Record your actual environment:

```bash
gcc --version
lsb_release -a
uname -a
nproc
```

The run script stores the environment details in `environment.txt`.

## 4. Compile and Execute

Place all `.c` files and `run_task5.sh` in the same `Task05` directory. Open Ubuntu/WSL in that directory:

```bash
chmod +x run_task5.sh
./run_task5.sh
```

The script compiles using:

```bash
gcc -std=c11 -Wall -Wextra -pthread bounded_buffer.c -o bounded_buffer
gcc -std=c11 -Wall -Wextra -pthread peterson_demo.c -o peterson_demo
gcc -std=c11 -O2 -Wall -Wextra -pthread booking_locks.c -o booking_locks
gcc -std=c11 -O2 -Wall -Wextra -pthread atomic_spin_benchmark.c -o atomic_spin_benchmark
```

## 5. Bounded-Buffer Tests

### 1 producer and 1 consumer

```bash
./bounded_buffer 1 1
```

Expected result:

```text
producers=1 consumers=1 items=20 sum=210 (expected 210) each item exactly once: PASS
```

### 4 producers and 4 consumers

```bash
./bounded_buffer 4 4
```

Each producer creates 20 uniquely numbered items, for a total of 80 items. The expected sum is 1 + 2 + ... + 80 = 3240.

```text
producers=4 consumers=4 items=80 sum=3240 (expected 3240) each item exactly once: PASS
```

The program checks the occurrence count of every expected item, not only the sum.

### 100-run check

The script runs the 1P/1C test 100 times and verifies that the result contains `sum=210` and `exactly once: PASS` on every run. Results are saved in `bounded_buffer_100_runs.txt`.

## 6. Removing the Mutex – Optional Unsafe Experiment

The semaphores `empty_slots` and `full_slots` count free and occupied slots. They do not make the shared circular-buffer indices `in` and `out` safe for simultaneous access by multiple producers or multiple consumers.

To compile the deliberately unsafe variant:

```bash
gcc -DUNSAFE_NO_MUTEX -std=c11 -Wall -Wextra -pthread bounded_buffer.c -o bounded_buffer_unsafe
```

Run:

```bash
./bounded_buffer_unsafe 4 4
```

This variant intentionally has unsynchronised accesses to shared buffer indices and is a data-race experiment only. In C, a data race on non-atomic objects causes undefined behaviour. It may occasionally appear to work; one successful run does not establish correctness. Do not use this variant for the final correct implementation.

## 7. Peterson's Algorithm

Run:

```bash
./peterson_demo
```

The demonstration uses C11 atomic variables with sequentially consistent ordering. `volatile` alone is not a replacement for atomic operations or memory-order guarantees. The classical Peterson algorithm is designed for two participants.

## 8. Railway Lock-Granularity Throughput

Run:

```bash
./booking_locks
```

The benchmark compares:

- One global lock for the whole booking system.
- One lock per train.
- One lock per coach.

The same number of booking attempts is made for each design. It reports median elapsed time, attempted requests per second and successful bookings. The benchmark uses 16 agent threads, 3 trials, 8 trains, 8 coaches per train and 512 seats per coach. These are controlled simulation parameters, not real railway capacity figures.

Output is saved as `booking_lock_results.txt`.

## 9. Atomic, Spin-Lock and Mutex Comparison

Run:

```bash
./atomic_spin_benchmark
```

The benchmark tests 1, 4 and 16 threads, each performing 200,000 increments, with three trials. It reports median elapsed time, nanoseconds per update, aggregate worker CPU time and failed spin attempts.

- `atomic_fetch_add` is suitable for a simple atomic counter update.
- A spin lock repeatedly checks for lock availability and can waste CPU under contention.
- A mutex can block waiting threads and is generally more appropriate for longer critical sections.

Output is saved as `atomic_spin_results.txt`.

## 10. Generated Evidence

After `./run_task5.sh`, the directory contains:

| Output file | Evidence |
|---|---|
| `bounded_buffer_1p1c.txt` | One producer/one consumer result. |
| `bounded_buffer_4p4c.txt` | Four producers/four consumers result. |
| `bounded_buffer_100_runs.txt` | Verification that all 100 original tests passed. |
| `peterson_output.txt` | Peterson two-thread demonstration. |
| `booking_lock_results.txt` | Lock-granularity throughput comparison. |
| `atomic_spin_results.txt` | Atomic/spin/mutex measurements. |
| `environment.txt` | Compiler, Linux and CPU details. |

Capture a screenshot of the terminal showing the bounded-buffer outputs, 100-run verification, lock throughput results and atomic/spin/mutex results.

**Important:** Timing results depend on the machine and system load. Use the measurements produced on your own Ubuntu/WSL system in your final submission; do not copy another computer's timing values.

## 11. Result

The experiments demonstrate semaphore-based producer–consumer coordination, Peterson's two-process mutual exclusion, lock-granularity trade-offs in a simulated railway booking workload, and the performance/CPU-cost differences between atomic increments, spin locks and mutexes.

## 12. Repository Submission Reminder

The assignment repository name specified in the question paper is:

`CSA04-192525148-Assignment2`

Keep Task 5 in its own `Task05` folder. Include the report, all C source files, run script, actual generated outputs and screenshots. Commit Task 5 separately and share the working repository link with the course faculty.
