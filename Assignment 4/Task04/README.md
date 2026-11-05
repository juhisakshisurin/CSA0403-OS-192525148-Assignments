# Assignment 4 – Task 4: Disk Scheduling Algorithms

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 4 – CO4 |
| **Task** | Task 4 – Disk Scheduling Algorithms |
| **Marks / Bloom's Level** | 7 Marks / K5 – Evaluate |

---

## 1. Task Overview

The records-server disk has cylinders **0–199**, an initial head position of **53**, and the request queue:

```text
98, 183, 37, 122, 14, 124, 65, 67
```

Task 4 requires:

- Calculate service order and total head movement for FCFS, SSTF, SCAN, C-SCAN, LOOK and C-LOOK.
- Simulate all six algorithms with 10,000 random requests at light, medium and heavy load.
- Measure average head movement and maximum waiting time.
- Evaluate starvation and explain the fairness advantage of C-SCAN over SCAN.
- Add requests near cylinder 60 every millisecond and evaluate the request at cylinder 183 under SSTF.
- Explain Linux `none` and `mq-deadline` schedulers for SSDs and inspect the current scheduler.
- Complete TODO 1 in the supplied SSTF C program and execute it.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment4_Task04_Report.pdf` | Complete report without student personal details. Includes calculations, algorithms, simulation results, fairness analysis, SSD scheduler explanation and complexity analysis. |
| `README.md` | Student details, compiler/environment information, compile commands and run procedure. |
| `sstf_complete.c` | The supplied C program with only TODO 1 completed. It preserves the original FCFS, SCAN and C-SCAN code and prints the expected output. |
| `disk_scheduler_sim.c` | Independent simulation of FCFS, SSTF, SCAN, C-SCAN, LOOK and C-LOOK for 10,000 random requests at three defined loads, plus an SSTF starvation demonstration. |
| `check_ssd_scheduler.sh` | Read-only inspection of `/sys/block/<device>/queue/scheduler`. |
| `run_task4.sh` | Compiles and runs the sample SSTF program and the 10,000-request simulator, then captures disk-scheduler information. |
| `sample_output.txt` | Actual output of the completed supplied program. |
| `simulation_output.txt` | Actual simulation results and SSTF starvation demonstration. |
| `scheduler_simulation.csv` | Machine-readable result table for the 18 algorithm/load combinations. |
| `average_head_movement.png` | Plot of average head movement by algorithm and load. |
| `maximum_waiting_time.png` | Plot of maximum waiting time by algorithm and load. |
| `calculation_reference.txt` | Hand-calculation routes and movements for all six algorithms. |
| `measurement_worksheet.txt` | Worksheet for local device and evidence details. |
| `ssd_scheduler_output.txt` | Scheduler information visible in the artifact-preparation environment. This is not the student's physical disk result. |
| `environment.txt` | Compiler, kernel and visible block-device information from the preparation environment. |
| `calculation_reference.txt` | Hand-calculation order and movement values for all six sample algorithms. |
| `measurement_worksheet.txt` | Worksheet to record a local scheduler check and simulation settings. |

## 3. Environment Information

The files were compiled and tested in a Linux x86_64 environment with GCC. The package's `environment.txt` records the preparation environment; it is not claimed to be the student's own computer.

Record the actual environment used for submission:

```bash
gcc --version
cat /etc/os-release
uname -a
lsblk -d -o NAME,TYPE,ROTA,MODEL,SIZE
```

## 4. Complete TODO 1 – SSTF

The original question provides a `done[N]` array and asks for the pending request with the smallest distance from the current head.

Completed TODO block:

```c
int best = -1;
int best_distance = MAXCYL + 1;

for (int i = 0; i < N; i++) {
    if (done[i] == 0) {
        int distance = abs(req[i] - pos);

        if (distance < best_distance) {
            best_distance = distance;
            best = i;
        }
    }
}

if (best < 0) {
    fprintf(stderr, "SSTF selection error.\\n");
    return EXIT_FAILURE;
}

done[best] = 1;
order[k] = req[best];
pos = req[best];
```

The full completed source is `sstf_complete.c`.

Compile with the assignment's required C11 warning flags:

```bash
gcc -std=c11 -Wall -Wextra sstf_complete.c -o sstf_complete
```

Run:

```bash
./sstf_complete
```

Expected output:

```text
FCFS   53 98 183 37 122 14 124 65 67 -> 640 cylinders
SSTF   53 65 67 37 14 98 122 124 183 -> 236 cylinders
SCAN   53 37 14 0 65 67 98 122 124 183 -> 236 cylinders
C-SCAN 53 ... 199, jump to 0 ... 37 -> 183 cylinders (+199 for the jump)
```

The supplied C-SCAN program prints the service-path subtotal (183) and the circular jump separately (+199). Counting that jump, the full physical head travel is 382 cylinders.

## 5. Additional Sample Calculations – LOOK and C-LOOK

The source program in the question does not print LOOK or C-LOOK, but Task 4.1 explicitly asks for both.

The directions are kept consistent with the source:
- LOOK initially moves toward cylinder 0, then reverses at the last request.
- C-LOOK initially moves upward, then jumps from the highest request to the lowest pending request.

| Algorithm | Service order after start 53 | Total head movement |
|---|---|---:|
| LOOK | 37, 14, 65, 67, 98, 122, 124, 183 | 208 cylinders |
| C-LOOK | 65, 67, 98, 122, 124, 183, jump to 14, 37 | 322 cylinders, including the 169-cylinder circular jump |

## 6. 10,000-Request Simulation

The simulator is deterministic and reproducible. It uses the same generated request stream for all six algorithms within each load.

### Explicit model assumptions

| Parameter | Assumption |
|---|---|
| Disk cylinders | 0–199 |
| Initial head | 53 |
| Requests per load | 10,000 |
| Cylinder selection | Uniform random over 0–199 |
| Arrival intervals | Uniform from 0.5 to 1.5 times the mean interval |
| Light load | Mean inter-arrival 8.0 ms |
| Medium load | Mean inter-arrival 2.0 ms |
| Heavy load | Mean inter-arrival 0.6 ms |
| Modelled request service time | 0.20 + 0.020 × head movement (ms) |
| Waiting-time definition | Dispatch start time minus request arrival time |
| Head-movement metric | Total cylinder movement divided by completed requests; SCAN/C-SCAN/LOOK/C-LOOK travel and wraps are counted |

These are simulation parameters because the question does not specify a physical drive's seek-time curve, rotational latency or arrival rate. The values are not claimed as measurements from a physical records server.

### Measured simulation output

The full-precision values are in `scheduler_simulation.csv`; console output is in `simulation_output.txt`.

| Load | Algorithm | Avg. movement (cyl/request) | Avg. wait (ms) | Max wait (ms) |
|---|---|---:|---:|---:|
| Light | FCFS | 66.257 | 0.000 | 0.001 |
| Light | SSTF | 66.257 | 0.000 | 0.001 |
| Light | SCAN | 132.696 | 0.699 | 3.860 |
| Light | C-SCAN | 189.803 | 2.764 | 9.962 |
| Light | LOOK | 66.257 | 0.000 | 0.001 |
| Light | C-LOOK | 66.257 | 0.663 | 3.960 |
| Medium | FCFS | 66.758 | 0.936 | 10.851 |
| Medium | SSTF | 64.334 | 0.551 | 7.489 |
| Medium | SCAN | 85.527 | 2.328 | 8.558 |
| Medium | C-SCAN | 88.130 | 4.247 | 11.207 |
| Medium | LOOK | 64.392 | 0.560 | 7.309 |
| Medium | C-LOOK | 65.540 | 1.356 | 10.464 |
| Heavy | FCFS | 66.552 | 4,672.836 | 9,305.147 |
| Heavy | SSTF | 20.041 | 3.798 | 27.960 |
| Heavy | SCAN | 20.056 | 4.020 | 12.721 |
| Heavy | C-SCAN | 20.067 | 5.873 | 14.171 |
| Heavy | LOOK | 20.042 | 3.669 | 12.091 |
| Heavy | C-LOOK | 20.053 | 5.401 | 15.414 |

Interpretation:
- Under light load, requests often arrive after the previous request has completed; therefore FCFS and SSTF can have nearly zero queue waiting.
- At medium load, SSTF and LOOK reduce movement and waiting in this generated stream.
- At heavy load, FCFS accumulates a large queue and high maximum wait. SSTF has low movement but can be unfair to distant requests; the separate near-stream test demonstrates why.
- C-SCAN/C-LOOK can incur additional wrap movement. C-SCAN's fairness benefit is a more uniform sweep order, not a guarantee of a lower maximum wait for every finite workload.

## 7. SSTF Starvation Demonstration

The simulator adds:
- One request at cylinder 183 arriving at time 0.
- A stream of 1,000 requests at cylinders 59, 60 and 61, arriving once every millisecond.
- A busy-device service-time assumption of 1.20 + 0.030 × movement (ms), so the near requests remain queued.

The simulator reports that SSTF serves all 1,000 nearer requests before the cylinder-183 request. Its modelled wait is 1,201.320 ms for the finite stream. If the stream continues indefinitely, the far request can starve because each new near request is closer to the current head.

## 8. Fairness: SCAN vs C-SCAN

SCAN sweeps in both directions. Requests near one end can receive service sooner during one pass, while requests in the opposite region may wait until the head reaches the end and reverses.

C-SCAN services requests in one direction only and then wraps to the start. Every cylinder waits for the next one-way sweep, reducing directional preference and making waiting time more uniform across cylinder positions over repeated cycles. The wrap consumes head movement, so C-SCAN can increase total movement or absolute waiting in a particular finite test. Uniformity is a fairness property, not a promise of a smaller average or maximum wait in every simulation.

## 9. Complexity Analysis

Let N be the number of pending requests.

| Algorithm | Naive selection cost | Total cost for N requests | Extra space |
|---|---|---|---|
| FCFS | O(N) to find oldest pending request in array | O(N²) | O(N) request/done arrays |
| SSTF | O(N) scan for nearest request per dispatch | O(N²) | O(N) |
| SCAN / C-SCAN | O(N) scan for nearest request in direction per dispatch | O(N²) | O(N) |
| LOOK / C-LOOK | O(N) scan to find next request / wrap target | O(N²) | O(N) |

A sorted queue can reduce repeated selection work: inserting requests is O(log N) per request with a balanced tree, and selecting a next request by cylinder/direction can be O(log N) or O(1) from maintained endpoints. Maintaining multiple orderings and arrival-time ordering increases memory and implementation complexity. The supplied 8-request program uses simple array scans because N is small.

## 10. Solid-State Drive Scheduler Inspection

An HDD has mechanical seek movement, so reordering requests by location can reduce head movement. SSD/NVMe devices have no moving head and can serve many parallel requests; a host-side elevator ordering may add CPU/queueing overhead without the same mechanical benefit.

- `none` submits requests without block-layer reordering. It is a low-overhead choice for fast, parallel storage.
- `mq-deadline` provides deadline-oriented dispatch and can help control read latency/fairness when that trade-off is needed. It may be selected for a virtual guest or a device/workload where bounded waiting is preferred.
- The active scheduler varies by kernel, device and distribution. Do not assume every SSD uses `none`.

Check a particular device:

```bash
lsblk -d -o NAME,TYPE,ROTA,MODEL,SIZE
cat /sys/block/<disk>/queue/scheduler
```

The active choice appears in square brackets. For example, `[none] mq-deadline` means `none` is active.

Run the supplied read-only check:

```bash
chmod +x check_ssd_scheduler.sh
./check_ssd_scheduler.sh
```

Or specify the device name:

```bash
./check_ssd_scheduler.sh nvme0n1
```

The provided `ssd_scheduler_output.txt` is from the artifact-preparation environment and does not identify the student's physical SSD. Run the script locally and retain its actual output.

## 11. Run Everything

Keep the following files in one `Task04` folder:

```text
Task04/
├── Assignment4_Task04_Report.pdf
├── README.md
├── sstf_complete.c
├── disk_scheduler_sim.c
├── check_ssd_scheduler.sh
└── run_task4.sh
```

Run in Ubuntu/WSL:

```bash
chmod +x run_task4.sh check_ssd_scheduler.sh
./run_task4.sh
```

To check one named device:

```bash
DISK_DEVICE=nvme0n1 ./run_task4.sh
```

The script compiles both C files, executes the supplied SSTF program, runs all 18 simulation cases and records the scheduler that is visible in `/sys`.

Exact compilation commands:

```bash
gcc -std=c11 -Wall -Wextra sstf_complete.c -o sstf_complete
gcc -std=c11 -O2 -Wall -Wextra disk_scheduler_sim.c -o disk_scheduler_sim
```

## 12. Result

The completed SSTF implementation reproduces the expected sample output. The extended simulator compares six scheduling algorithms using 10,000 arrivals for each of three load levels, records average movement and maximum waiting time, and demonstrates the starvation risk of SSTF under a sustained near-head stream. The live scheduler-inspection script reads the active scheduler without changing it.
