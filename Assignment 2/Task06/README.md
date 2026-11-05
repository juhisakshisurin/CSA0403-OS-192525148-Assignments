# Assignment 2 – Task 6: Deadlock Handling and the Banker’s Algorithm

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

---

## 1. Task Overview

**Q6 – Deadlock Handling and the Banker’s Algorithm (13 Marks, K5 – Evaluate)** is based on a shared laboratory equipment scheduler. Research jobs need GPUs, simulation-software licences and high-speed storage. Jobs can wait for resources held by other jobs, causing a deadlock that stops the cluster.

The task covers:

- The four necessary conditions for deadlock and a resource-allocation graph.
- The Banker’s safety algorithm and resource-request algorithm.
- Manual explanation of why P0’s request `(0, 2, 0)` produces an unsafe state.
- Comparison of prevention, avoidance, detection and recovery, and the ostrich approach.
- Prediction of Banker’s algorithm running-time growth in processes `n` and resource types `m`.
- Measurement of the safety algorithm for 10, 100 and 1,000 processes.
- A final recommendation for the research centre.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment2_Task06_Report.pdf` | Complete report without student identity details. |
| `bankers_algorithm.c` | Original assignment Banker’s program with TODO 1 completed. |
| `bankers_benchmark.c` | Safety-algorithm scaling benchmark for 10, 100 and 1,000 processes. |
| `run_task6.sh` | Compiles and executes both programs and saves output/environment evidence. |
| `bankers_output_expected.txt` | Expected output from the supplied five-process example. |
| `benchmark_results_template.txt` | Template for recording local benchmark timings. |

## 3. System Requirements

| Component | Requirement |
|---|---|
| Programming Language | C |
| Compiler | GCC |
| Standard | C11 |
| Operating System | Linux / Ubuntu / WSL 2 |

Record the compiler and operating system used for your own run:

```bash
gcc --version
lsb_release -a
uname -a
nproc
```

The execution script automatically stores this information in `environment.txt`.

## 4. Compile and Run

Keep `bankers_algorithm.c`, `bankers_benchmark.c` and `run_task6.sh` in the same `Task06` folder.

Open Ubuntu/WSL terminal in that folder and run:

```bash
chmod +x run_task6.sh
./run_task6.sh
```

The script performs these compilation commands:

```bash
gcc -std=c11 -Wall -Wextra bankers_algorithm.c -o bankers_algorithm
gcc -std=c11 -O2 -Wall -Wextra bankers_benchmark.c -o bankers_benchmark
```

You can also compile and execute each program separately.

### Banker’s algorithm

```bash
gcc -std=c11 -Wall -Wextra bankers_algorithm.c -o bankers_algorithm
./bankers_algorithm
```

### Scaling benchmark

```bash
gcc -std=c11 -O2 -Wall -Wextra bankers_benchmark.c -o bankers_benchmark
./bankers_benchmark
```

## 5. Expected Output – Given Banker’s State

The supplied values are:

- Available = `(3, 3, 2)`
- Processes = `P0` to `P4`
- Resource types = `R1`, `R2`, `R3`

The completed program should produce:

```text
initial state safe: P1 P3 P4 P0 P2
P1 request: granted, safe sequence P1 P3 P4 P0 P2
P4 request: must wait (not available)
P0 request: denied (unsafe)
```

The resource-request cases are executed in the exact order given in the assignment.

## 6. Hand Analysis of P0 Request

After P1’s request is granted, Available becomes `(2, 3, 0)`. P0 requests `(0, 2, 0)`. The request is within P0’s remaining need and the currently available resources, so the program tentatively grants it.

The tentative Available vector becomes `(2, 1, 0)`. No unfinished process has a Need vector less than or equal to this Work vector for all three resource types. In particular, every unfinished process either needs more R1, more R2 or at least one R3, while Work for R3 is zero. No process can finish and release resources. The tentative state is unsafe, so the request is rolled back and denied.

## 7. Complexity Prediction

For `n` processes and `m` resource types, the safety algorithm may scan up to `n` processes on each of up to `n` passes. Each process check examines up to `m` resource types.

Worst-case safety-check time:

```text
O(m × n²)
```

The Maximum, Allocation and Need matrices require `O(mn)` storage. Work and Available require `O(m)`, while Finish and the safe sequence require `O(n)`.

For fixed `m`, a tenfold increase in `n` gives an approximately 100-fold increase in the worst-case operation count.

## 8. Measurement for 10, 100 and 1,000 Processes

`bankers_benchmark.c` creates a safe input ordering that forces repeated safety scans and measures the median safety-check time for:

- 10 processes.
- 100 processes.
- 1,000 processes.

It uses three resource types and five timing trials per configuration. The program reports median milliseconds per safety check and verifies that every generated state is safe.

The script saves the results to:

```text
bankers_benchmark_results.txt
```

**Important:** Run the benchmark on your own Ubuntu/WSL system and use those timings in your submission. Do not copy timings from another computer. Actual elapsed times depend on CPU, compiler, operating system, system load and timer resolution.

## 9. Generated Evidence Files

After running `./run_task6.sh`, the directory contains:

| File | Description |
|---|---|
| `bankers_output.txt` | Safe sequence and all three resource-request decisions. |
| `bankers_benchmark_results.txt` | Measured safety-check times for 10, 100 and 1,000 processes. |
| `environment.txt` | GCC version, Linux distribution, kernel and CPU count. |

Capture screenshots showing the expected safe sequence/request decisions and the benchmark results table.

## 10. Result

The implementation evaluates safe-state detection and request granting using Banker’s algorithm. The scaling benchmark measures the growth of the safety check as the process count increases, and the report compares prevention, avoidance, detection/recovery and ignoring deadlock for the shared laboratory cluster.

## 11. GitHub Submission Reminder

The repository name specified by the assignment is:

`CSA04-192525148-Assignment2`

Keep Task 6 in a separate `Task06` folder. Include the report, both C source files, run script, actual output files, environment details and screenshots. Commit Task 6 separately and share the working repository link with the course faculty.
