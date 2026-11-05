# Assignment 3 – Task 7: Design a Memory-Management Plan from First Principles

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

## 1. Task Overview

**Q7 – Design a Memory-Management Plan from First Principles (10 Marks, K5 – Evaluate)** describes a smart-factory edge device with 2 GB RAM and no swap. It runs:

- A real-time control task that must not be delayed by a page fault.
- A machine-learning task requiring 1.5 GB.
- A logging task.
- Future tasks may be added.

The work is organised according to the four stages in the question:

1. Interpret the problem and convert it to measurable requirements.
2. Compare contiguous allocation, paging, segmentation and demand paging.
3. Implement and verify memory locking and simulate the three tasks with a page-replacement simulator.
4. Defend the design, explain a fourth 500 MB task and identify one limitation.

The alternative concept evaluates a real-time operating system with no virtual memory.

## 2. Design Assumptions

The question does not specify the control task's working-set size, logger's buffer size, Linux kernel reserve or control-loop deadline. The report therefore makes explicit initial assumptions that must be validated on the target edge device.

| Memory area | Budget |
|---|---:|
| OS, kernel and services | 320 MiB |
| Locked real-time control working set | 64 MiB |
| Machine-learning working set | 1,536 MiB (1.5 GiB) |
| Logger active ring buffer | 64 MiB |
| Safety/headroom reserve | 64 MiB |
| **Total** | **2,048 MiB (2 GiB)** |

The memory budget is exactly 2 GiB, leaving the stated 64 MiB as headroom after the task and OS reservations. This is a tight initial design and must be validated using actual boot-time and peak-load measurements. The OS reserve, real-time set and logging buffer are design assumptions, not figures stated in the question.

## 3. Files Included

| File | Description |
|---|---|
| `Assignment3_Task07_Report.pdf` | Full report without student identity details. |
| `README.md` | Student details, build environment, commands, execution instructions and results. |
| `realtime_lock_bench.c` | Compares an unlocked, demand-faulting memory loop with a prefaulted `mlock()`-locked loop. Reports average and maximum observed per-page-touch latency plus minor/major fault deltas. |
| `task_memory_sim.c` | LRU page-replacement simulation for the control, ML and logging working sets, using the stated 2 GiB budget. Also checks admission of the additional 500 MiB task. |
| `run_task7.sh` | Compiles and runs both C programs and saves environment and output files. |
| `submission_measurements_template.txt` | Worksheet for recording the student's own local measurements. |

## 4. System Requirements

| Component | Requirement |
|---|---|
| Language | C |
| Standard | C11 |
| Compiler | GCC |
| OS | Linux / Ubuntu / WSL 2 |
| Linux interfaces | `mlock()`, `mmap()`, `madvise()`, `getrusage()`, `fork` is not required in this task |
| Permission | The process must be allowed to lock the requested amount of memory |

Reference build environment used while preparing and testing these files:

- GCC: Debian 14.2.0-19
- OS: Debian GNU/Linux 13 (trixie)
- Kernel: Linux x86_64 6.18.57

This is the artifact preparation environment, not the user's own system. Run the following commands on the submission machine and replace the README's environment information if it differs:

```bash
gcc --version
cat /etc/os-release
uname -a
ulimit -l
getconf PAGESIZE
```

## 5. Build and Run All Experiments

Keep these files in one `Task07` folder:

```text
Task07/
├── Assignment3_Task07_Report.pdf
├── README.md
├── realtime_lock_bench.c
├── task_memory_sim.c
├── run_task7.sh
└── submission_measurements_template.txt
```

Open Ubuntu/WSL terminal in this folder and run:

```bash
chmod +x run_task7.sh
./run_task7.sh
```

The script records:
- GCC version and Linux kernel/distribution.
- Native page size and RAM/swap details.
- Current memlock limit.
- Unlocked versus locked control-loop measurements.
- Page-replacement and memory-budget simulation results.

## 6. Exact Compilation Commands

### Real-time lock benchmark

```bash
gcc -std=c11 -O2 -Wall -Wextra realtime_lock_bench.c -o realtime_lock_bench
```

Run with the default 4 MiB test area:

```bash
./realtime_lock_bench
```

Or specify a working set from 1 to 64 MiB:

```bash
./realtime_lock_bench 4
```

The default is deliberately small to fit common Linux `ulimit -l` settings. The program first measures an unlocked region after discarding its pages between passes, then attempts `mlock()` and pre-faults all pages before measuring the locked loop.

If `mlock()` fails, the program prints the reason and marks the locked measurement as skipped. Do not report that as a successful locked-memory result. Check the allowed locked-memory limit with:

```bash
ulimit -l
```

If a larger test is required, increase the locked-memory allowance only on a controlled test system with administrator approval. Do not disable system safety limits on a production machine.

## 7. What the Lock Benchmark Measures

The program reports:

- `avg_touch`: average time for one measured page touch.
- `max_observed_touch`: maximum single page-touch duration observed in the run.
- `minor_faults` and `major_faults`: resource-usage deltas during the measured loop.

The unlocked case uses `madvise(..., MADV_DONTNEED)` between passes so that the next pass has to fault pages back in. The locked case uses `mlock()` and touches all pages before timing the control loop.

The maximum reported value is an observed maximum for this run, not a hard-real-time worst-case execution-time proof. `mlock()` prevents the locked region from being paged out, but Linux scheduling, interrupts, drivers and other kernel activity can still affect timing. For a strict hard-real-time guarantee, use a real-time kernel and validate end-to-end deadlines on the target hardware.

## 8. Three-Task Memory and Page-Replacement Simulation

Compile and run:

```bash
gcc -std=c11 -O2 -Wall -Wextra task_memory_sim.c -o task_memory_sim
./task_memory_sim
```

The memory-budget simulator uses a 16 MiB abstract model unit to keep the simulation compact:

- 128 model units represent 2,048 MiB of physical RAM.
- 20 units (320 MiB) are reserved for OS/kernel/services.
- 4 units (64 MiB) are pinned for the real-time control task.
- 96 units (1,536 MiB) are assigned to the ML task.
- 4 units (64 MiB) are assigned to the logger.
- 4 units (64 MiB) are kept as headroom.

**Important:** These 16 MiB model units are not the hardware page size. The simulation is an aggregate memory-budget model; the real Linux system uses its native page size (typically much smaller).

The real-time pages are preloaded/pinned before the simulated control phase. The ML task incurs demand faults on first touch and then reuses its resident 1.5 GiB working set. The logger's four-unit ring buffer faults on initial fill and is then reused. The simulator verifies zero page-replacement faults for the real-time phase.

## 9. Fourth Task of 500 MiB

A new task requiring 500 MiB changes the planned total from 2,048 MiB to 2,548 MiB, exceeding the current 2 GiB device by 500 MiB before any additional safety margin.

The simulator therefore rejects the fourth task on the existing device. The report recommends at least 3 GiB as a mathematical minimum and 4 GiB as a more practical target with operating-system and burst headroom. Alternatively, move the fourth task to another edge node or reduce another task's validated working set.

## 10. Alternative: RTOS with No Virtual Memory

With an RTOS and no virtual memory:
- Allocate the control task's code, stack and data statically at boot; do not page or swap it.
- Use deterministic fixed memory pools for ML buffers and logger buffers.
- The control loop avoids VM page faults and can have a more predictable worst-case delay, subject to interrupt, scheduler and driver WCET.
- ML can use the remaining physical memory after the RTOS and control/logging reservations. Dynamic growth is constrained by fixed memory pools.
- Adding tasks requires a static memory-budget review, partition/pool changes and often a firmware rebuild/revalidation; this is less flexible than Linux demand paging.

The report includes the trade-off and complexity comparison.

## 11. Generated Evidence Files

After running `./run_task7.sh`, the folder contains:

| Output file | Purpose |
|---|---|
| `environment.txt` | Compiler, OS, kernel, native page size, memory and memlock limit. |
| `realtime_measurements.txt` | Unlocked/locked average and maximum observed access latency and fault counts. |
| `task_memory_simulation.txt` | Memory budget, per-task frame allocation, simulated fault counts and fourth-task admission decision. |

Capture a terminal screenshot showing both measurements and the simulator's PASS result.

## 12. Result

The locked-memory benchmark compares page-fault behaviour and observed access latency for an unlocked and prefaulted `mlock()` region. The LRU simulator models the three tasks in the proposed 2 GiB budget, pins the real-time working set and checks the additional 500 MiB task. The design protects the control working set from paging while allowing the ML and logging tasks to use managed memory.

## 13. GitHub Submission Reminder

The repository name required by the assignment is:

`CSA04-192525148-Assignment3`

Keep Task 7 in its own `Task07` folder. Include the report, README, C source files, shell script, actual output files and screenshots. Commit Task 7 separately and include the working repository link in the main assignment submission.
