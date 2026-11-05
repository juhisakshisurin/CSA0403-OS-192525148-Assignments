# Assignment 3 – Task 4: Virtual Memory and Demand Paging

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

---

## 1. Task Overview

**Q4 – Virtual Memory and Demand Paging (8 Marks, K5 – Evaluate)** uses the example of a radiologist running image-processing software requiring 24 GB of memory on a laptop with 16 GB of RAM.

The question requires:

- Explain the steps of handling a page fault.
- Calculate effective access time for page-fault rates of 1/1,000, 1/100,000 and 1/1,000,000, using 100 ns memory access and 8 ms fault service.
- Explain copy-on-write and why it makes `fork()` fast.
- Explain thrashing and the working-set model.
- Explain how demand paging allows a 24 GB program to run with 16 GB RAM.
- Calculate the maximum page-fault rate for a 10% slow-down.
- Explain equal and proportional frame allocation.
- Explain how the OS detects and stops thrashing.
- Measure sequential and random page-touch behaviour and page faults.
- Measure runtime as memory grows beyond available RAM, using a VM or memory limit.
- Demonstrate copy-on-write using `fork()` and memory measurements.
- Plot page faults against the number of frames for a reference string.
- Recommend whether buying more RAM or improving locality is better.
- Evaluate the alternative of running on a 32 GB RAM machine.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment3_Task04_Report.pdf` | Complete report without personal identity details. Includes EAT calculations, page-fault flow, working-set model, results methodology, 32 GB alternative and recommendation. |
| `memory_access_bench.c` | Touches a memory region in sequential and randomized page order, revisits the working set for three passes, and records minor/major page faults and execution time using `getrusage()`. |
| `cow_fork_demo.c` | Allocates and touches memory, forks a child, and compares `/proc/self/smaps_rollup` before and after child writes to demonstrate copy-on-write. |
| `frame_faults.c` | Calculates FIFO page faults for a fixed reference string with 1–10 frames and writes `frame_faults.csv`. |
| `plot_faults.py` | Plots frame count against FIFO page faults and saves `frame_faults.png`. |
| `run_task4.sh` | Compiles all programs, runs safe default tests and creates evidence files. |

## 3. System Requirements

| Component | Requirement |
|---|---|
| Language | C |
| Compiler | GCC |
| Standard | C11 |
| Operating System | Linux / Ubuntu / WSL 2 |
| Python plot | Python 3 and matplotlib |
| System interfaces | `getrusage()`, `fork()`, `/proc/self/smaps_rollup`, `/proc/meminfo` |

Record your environment:

```bash
gcc --version
uname -a
getconf PAGESIZE
free -h
swapon --show
```

The script saves GCC, kernel, native page size and memory/swap details in `environment.txt`.

## 4. Compile and Run the Experiments

Keep the following files in one `Task04` folder:

```text
Task04/
├── Assignment3_Task04_Report.pdf
├── README.md
├── memory_access_bench.c
├── cow_fork_demo.c
├── frame_faults.c
├── plot_faults.py
└── run_task4.sh
```

Open Ubuntu/WSL terminal inside this folder and run:

```bash
chmod +x run_task4.sh
./run_task4.sh
```

The script compiles all three C programs, runs sequential/random access tests, runs the copy-on-write experiment, creates a frame/fault CSV and attempts to generate the plot.

If matplotlib is unavailable, install it with:

```bash
sudo apt update
sudo apt install python3-matplotlib
```

Then run:

```bash
python3 plot_faults.py
```

## 5. Sequential and Random Memory Access

Run one memory size directly, for example 64 MiB:

```bash
gcc -std=c11 -O2 -Wall -Wextra memory_access_bench.c -o memory_access_bench
./memory_access_bench 64
```

The program:
1. Obtains the native page size using `sysconf(_SC_PAGESIZE)`.
2. Maps a fresh anonymous memory region.
3. Visits one byte in every page in sequential order.
4. Repeats the experiment using a deterministic shuffled page order.
5. Revisits the working set for three passes.
6. Uses `getrusage()` to report minor and major page-fault deltas and a monotonic clock to report elapsed time.

A fresh anonymous mapping normally produces minor faults when pages are first touched. Sequential and random tests may show similar fault counts because each page is visited; the order can still affect locality and runtime. If the working set exceeds the VM's physical-memory allowance and pages are evicted to swap, later passes may produce additional faults and longer times. The actual result depends on the kernel, swap and memory policy.

## 6. Memory Growth Beyond Available RAM – Safe Procedure

The default script tests 32, 64 and 128 MiB, subject to a conservative safety check. It will skip a size that exceeds half of current `MemAvailable`, unless `ALLOW_MEMORY_PRESSURE=1` is set.

**Do not intentionally exhaust the RAM of your everyday Windows/WSL host.** The question explicitly permits using a virtual machine or a memory limit. For a pressure experiment:

1. Use a dedicated Ubuntu VM, not your main Windows/WSL environment.
2. Set a fixed RAM limit in the VM settings (for example, 4 GB).
3. Ensure the VM has an appropriate swap allocation and enough disk space.
4. Observe `free -h` and `swapon --show` inside the VM.
5. Begin with small allocations and increase them gradually, staying within the VM's RAM-plus-swap capacity.
6. Compare measured times and page faults as the test working set approaches and exceeds the VM's available RAM.

Inside that dedicated VM, an example command is:

```bash
MEM_SIZES_MB="256 512 1024 2048 3072" ALLOW_MEMORY_PRESSURE=1 ./run_task4.sh
```

Use that example only after assigning suitable RAM and swap to the VM. Reduce the maximum size if the VM has less capacity. `ALLOW_MEMORY_PRESSURE=1` bypasses the script's conservative check; it does not provide a separate hard memory cap. If no VM is available, report the safe local sweep and clearly state that the over-available-RAM condition was not tested rather than risk an OOM event on the host.

Generated output: `memory_access_results.txt`.

## 7. Copy-on-Write Demonstration

Run:

```bash
gcc -std=c11 -O2 -Wall -Wextra cow_fork_demo.c -o cow_fork_demo
./cow_fork_demo 64
```

The program:
- Maps and touches 64 MiB before calling `fork()`.
- Reads RSS, PSS, Shared_Dirty and Private_Dirty from `/proc/self/smaps_rollup`.
- Shows the child's measurements immediately after `fork()`, before it writes.
- Writes one byte per page in the child.
- Reads the child's memory statistics again after writing.
- Shows the parent's memory stats after the child exits.

Expected interpretation: immediately after `fork()`, the parent and child map the same physical pages read-only under copy-on-write. PSS is divided across processes and the mapping is reported as shared. When the child writes, the kernel allocates private copies for modified pages; the child's `Private_Dirty` increases. RSS alone may not show the sharing benefit because the same shared pages are counted in each process's RSS.

Output: `cow_fork_results.txt`.

## 8. Page Faults Against the Number of Frames

Run:

```bash
gcc -std=c11 -O2 -Wall -Wextra frame_faults.c -o frame_faults
./frame_faults
python3 plot_faults.py
```

The program uses a fixed 60-reference string with repeated locality around a small group of page numbers and calculates FIFO faults for 1 through 10 frames.

Files created:
- `frame_faults.csv`
- `frame_faults_output.txt`
- `frame_faults.png`

The plot is a FIFO reference-string experiment. It does not represent measured faults from the laptop application; it demonstrates how the number of available frames changes faults for the given reference string.

## 9. Effective Access Time Calculations

Given:
- Memory access time = 100 ns.
- Page-fault service time = 8 ms = 8,000,000 ns.
- A faulted instruction must be restarted, so include one additional 100 ns memory access after page-in.

Use:

```text
EAT = (1 − p) × 100 + p × (8,000,000 + 100) ns
```

| Page-fault rate | EAT |
|---|---:|
| 1 in 1,000 | 8,100 ns = 8.1 µs |
| 1 in 100,000 | 180 ns |
| 1 in 1,000,000 | 108 ns |

For a maximum 10% slow-down, EAT must be no greater than 110 ns:

```text
100 + p × 8,000,000 <= 110
p <= 0.00000125
```

So the maximum approximate page-fault rate is 1.25 × 10^-6, or about one fault per 800,000 memory references.

## 10. Working Set, Thrashing and Frame Allocation

The working set of process i, `WSS_i`, is the set of distinct pages referenced in the most recent window of Δ references/time. The total demand is:

```text
D = Σ WSS_i
```

If total working-set demand exceeds the frames available to active processes, processes repeatedly fault and page in pages while other useful pages are evicted. This is thrashing.

- Equal allocation: if m frames are available to n processes, each gets approximately m/n frames, subject to minimum needs and integer rounding.
- Proportional allocation: process i with size s_i gets approximately `(s_i / Σs_j) × m` frames.
- Working-set allocation: allocate enough frames to cover each process's current working set where possible.

The OS can detect thrashing from a high page-fault rate, high paging I/O and low useful CPU utilisation. It can increase a process's frames, reduce multiprogramming by suspending/swapping out a process, or use page-fault-frequency thresholds to adjust frames.

## 11. Alternative: 32 GB RAM Instead of 16 GB

A 32 GB laptop may keep more of the 24 GB image-processing working set resident, reducing page faults if the workload actually touches most of those pages. It still needs memory for the OS and other applications, so 32 GB does not guarantee that all 24 GB are simultaneously resident under every workload.

Use the measured page-fault rate `p` from the application's representative workload in:

```text
EAT_16GB = (1 − p16) × 100 + p16 × 8,000,100 ns
EAT_32GB = (1 − p32) × 100 + p32 × 8,000,100 ns
```

The report does not invent a purchase price or claim that the memory benchmark equals the radiology application's fault rate. Compare the actual extra-memory cost with the measured time saved. Improving locality is usually the first software optimisation because it can reduce the working set and page faults without hardware cost. Additional RAM is justified when the application's unavoidable active working set remains larger than the available memory.

## 12. Generated Evidence Files

After `./run_task4.sh`, the folder contains:

| Output | Purpose |
|---|---|
| `environment.txt` | GCC, kernel, page size, memory and swap details. |
| `memory_access_results.txt` | Sequential/random run times and minor/major page faults. |
| `cow_fork_results.txt` | Memory statistics before and after fork and child writes. |
| `frame_faults.csv` | Frame-count/page-fault data. |
| `frame_faults_output.txt` | Console output of the frame simulation. |
| `frame_faults.png` | Page-fault versus frame-count plot. |

Capture screenshots of the memory tests, COW memory statistics and plot. Include the actual generated files in the GitHub Task04 folder.

## 13. Result

The programs demonstrate page faults from sequential and randomized page access, copy-on-write behaviour across `fork()`, and the relationship between frame count and FIFO page faults. The report applies the supplied memory and fault-service times to EAT, explains working sets and frame allocation, and evaluates the 16 GB versus 32 GB design choice. Use your own VM/laptop results for the runtime and page-fault evidence.

## 14. GitHub Submission Reminder

The repository name specified in the assignment is:

`CSA04-192525148-Assignment3`

Keep Task 4 in a separate `Task04` folder. Include the report, README, all C/Python source files, run script, generated output files, plot and screenshots. Commit Task 4 separately and share the working repository link with the course faculty.
