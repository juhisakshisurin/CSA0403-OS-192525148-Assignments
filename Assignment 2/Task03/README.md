# Assignment 2 – Task 3: Threads

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 – Operating Systems |

## 1. Task Overview

**Q3 – Threads (6 Marks, K5 – Evaluate)** is based on a traffic-camera number-plate reader that processes frames from 64 cameras. The assignment asks for POSIX-thread processing using one thread per camera and thread pools of 4, 8 and 16 threads.

The task also requires an explanation of shared and private thread resources, user-level and kernel-level threads, the many-to-one / one-to-one / many-to-many mapping models, measured speed-up on the executing machine, shared-counter race conditions, and an alternative process-based design.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment2_Task03_Report.pdf` | Task 3 report. It intentionally excludes student identity details. |
| `camera_threads.c` | POSIX-thread benchmark for sequential, 64-thread, 4-thread, 8-thread and 16-thread configurations. |
| `run_task3.sh` | Compiles and runs the program, saving output to `benchmark_results.txt`. |
| `benchmark_results_template.txt` | Template for recording the actual machine-specific measurements. |
| `environment_info.txt` | Commands for recording compiler, Linux and CPU details. |

## 3. Workload and Important Limitation

The question does not provide camera images, a number-plate recognition model, or a fixed frame workload. Therefore, the supplied program uses a repeatable synthetic CPU workload as a stand-in for frame analysis: 64 cameras, 4 frames per camera and 200,000 deterministic CPU iterations per frame.

**This is a threading and workload-distribution benchmark, not actual image decoding or number-plate recognition.** It uses the same workload for each configuration, verifies identical checksums, runs five trials and reports median elapsed time. Thread creation and joining are included in the threaded timings.

Do not describe the synthetic benchmark as a real camera-recognition deployment.

## 4. Requirements

| Component | Requirement |
|---|---|
| Language | C11 |
| Thread library | POSIX Threads (`pthread`) |
| Compiler | GCC |
| Operating system | Linux / Ubuntu / WSL 2 |
| Compile option | `-pthread` |

Record your actual environment:

```bash
gcc --version
lsb_release -a
uname -r
nproc
lscpu
```

## 5. Compile and Run

From the Task03 directory in Ubuntu/WSL:

```bash
chmod +x run_task3.sh
./run_task3.sh
```

The script runs the following compile command:

```bash
gcc -std=c11 -O2 -Wall -Wextra -pthread camera_threads.c -o camera_threads
```

It executes the program and writes the benchmark table to:

```text
benchmark_results.txt
```

Alternatively, compile and run manually:

```bash
gcc -std=c11 -O2 -Wall -Wextra -pthread camera_threads.c -o camera_threads
./camera_threads
```

## 6. Configurations Measured

| Configuration | Description |
|---|---|
| Sequential | Main thread processes all 64 cameras in order. |
| One thread per camera | Creates 64 POSIX threads. |
| Pool of 4 | Four worker threads dynamically claim camera tasks. |
| Pool of 8 | Eight worker threads dynamically claim camera tasks. |
| Pool of 16 | Sixteen worker threads dynamically claim camera tasks. |

## 7. Speed-Up Formula

```text
Speed-up = Sequential median execution time / Threaded median execution time
```

The program runs five trials per configuration and uses the median. It checks that each configuration produces the same per-camera checksums as the sequential baseline.

Use the actual timings printed on your computer. Do not copy timing values from another computer or treat illustrative values as your own experiment.

## 8. Shared Counter Synchronisation

The program includes a mutex-protected counter verification using four threads with 100,000 increments each. The expected result is 400,000. The program reports PASS when the actual result matches.

An unprotected shared increment (`counter++`) can cause lost updates because it is a read-modify-write operation. In C, unsynchronised concurrent access to a non-atomic shared object is a data race and has undefined behaviour. A mutex or atomic operation is required.

## 9. Result

The program compares thread-per-camera and bounded worker-pool configurations on the machine where it is executed. The measured results should be used to explain the effect of parallelism, thread creation/join overhead, CPU core availability, context switching, synchronisation and workload balance.

## 10. Submission Evidence

Include the following in the Task03 folder before submission:

- `Assignment2_Task03_Report.pdf`
- `README.md`
- `camera_threads.c`
- `run_task3.sh`
- `benchmark_results.txt` generated on your own Ubuntu/WSL system
- Screenshot of the completed run
- Compiler, Linux distribution and CPU details

The complete Assignment 2 repository name specified in the assignment is:

`CSA04-192525148-Assignment2`

Commit Task 3 progressively and share the working repository link with the course faculty.
