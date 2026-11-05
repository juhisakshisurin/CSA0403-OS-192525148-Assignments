# Assignment 3 – Task 5: Page-Replacement Techniques

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

---

## 1. Task Overview

**Q5 – Page-Replacement Techniques (8 Marks, K5 – Evaluate)** is based on a web server that keeps frequently used data pages in memory. When memory is full, the operating system must choose a victim page.

The task requires:

- Implement FIFO, LRU and Optimal page replacement.
- Complete TODO 1 in the given LRU code.
- Count page faults for the supplied reference string with 3 and 4 frames.
- Demonstrate Belady's anomaly using FIFO.
- Explain why Optimal is best but cannot be implemented online.
- Explain the LRU stack property and why LRU cannot suffer Belady's anomaly.
- Explain exact-LRU hardware/bookkeeping needs and the Clock (second-chance) approximation.
- Generate strong-locality and random reference strings.
- Count FIFO, LRU, Optimal and Clock faults for 3–10 frames.
- Plot page faults against frame count for each algorithm.
- Measure each algorithm's running time for 100,000 references.
- Verify one supplied string by hand.
- Recommend a policy for the web server using the measurements.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment3_Task05_Report.pdf` | Report without student details. Includes hand calculations, algorithm comparison, result tables, Clock analysis and engineering decision. |
| `page_replacement.c` | Complete C program for FIFO, LRU, Optimal and Clock/second-chance. Completes the assignment's TODO 1 and performs the reference-string experiments and 100,000-reference timing tests. |
| `plot_page_faults.py` | Reads the two CSV result files and generates the page-fault comparison graph. |
| `run_task5.sh` | Compiles and runs the C program, writes output CSV/text files and creates the graph when matplotlib is installed. |
| `faults_locality.csv` | Page-fault counts for 3–10 frames using the strong-locality workload. |
| `faults_random.csv` | Page-fault counts for 3–10 frames using the random workload. |
| `runtime_100k.csv` | Median runtime for each algorithm, workload and frame count. |
| `page_fault_comparison.png` | Generated plot comparing FIFO, LRU, Optimal and Clock. |

## 3. System Requirements

| Component | Requirement |
|---|---|
| Language | C |
| Standard | C11 |
| Compiler | GCC |
| Operating System | Linux / Ubuntu / WSL 2 |
| Plotting | Python 3 and matplotlib |

Record your own environment:

```bash
gcc --version
uname -a
nproc
```

## 4. Compile and Run All Experiments

Keep these files in one `Task05` folder:

```text
Task05/
├── Assignment3_Task05_Report.pdf
├── README.md
├── page_replacement.c
├── plot_page_faults.py
└── run_task5.sh
```

Run in Ubuntu/WSL:

```bash
chmod +x run_task5.sh
./run_task5.sh
```

The script compiles and executes the fixed reference string, generates both 100,000-reference workloads, calculates page faults for 3–10 frames, measures runtimes and creates the plot.

If matplotlib is missing:

```bash
sudo apt update
sudo apt install python3-matplotlib
```

Then regenerate the graph:

```bash
python3 plot_page_faults.py
```

## 5. Complete TODO 1 – LRU

The supplied LRU function maintains `last[]`, the index of the most recent use of the page in each occupied frame.

The completed replacement block is:

```c
int victim;

if (used < frames) {
    victim = used++;
    f[victim] = ref[i];
} else {
    victim = 0;

    for (int j = 1; j < frames; j++) {
        if (last[j] < last[victim])
            victim = j;
    }

    f[victim] = ref[i];
}

last[victim] = i;
```

Compile and run the fixed supplied string:

```bash
gcc -std=c11 -O2 -Wall -Wextra page_replacement.c -o page_replacement
./page_replacement
```

Expected output:

```text
3 frames: FIFO 15, LRU 12, Optimal 9
4 frames: FIFO 10, LRU 8, Optimal 8
Belady string, FIFO: 3 frames 9 faults, 4 frames 10 faults
```

## 6. Page-Replacement Algorithms

### FIFO

FIFO removes the page that has been in memory longest, using a circular pointer. It is simple and has low replacement bookkeeping overhead, but it ignores how recently or how frequently a page was used. FIFO can suffer Belady's anomaly.

### LRU

LRU removes the page whose most recent reference is farthest in the past. The C implementation stores the latest reference index for each frame and selects the smallest timestamp. LRU uses recency and has the stack property, so its page-fault count cannot increase when the number of frames increases for the same reference string.

### Optimal

Optimal replaces the resident page whose next use is farthest in the future (or is never used again). It gives the minimum possible number of page faults for a known reference string. A real online operating system cannot know the future references, so Optimal is used as a comparison lower bound. The benchmark precomputes next-use positions from the generated, fully known simulation string.

### Clock / Second Chance

Clock stores one reference bit per frame and uses a circular hand. A hit sets the frame's bit. On a fault, the hand clears set bits while moving and replaces the first frame with bit 0. It approximates LRU without updating a full recency list or timestamp on every reference.

## 7. Belady's Anomaly

The second supplied string is:

```text
1 2 3 4 1 2 5 1 2 3 4 5
```

FIFO produces 9 faults with 3 frames and 10 faults with 4 frames. More frames can change the FIFO eviction order: with 4 frames the early pages 1, 2, 3 and 4 fill the memory, then the FIFO queue cycles through pages in a way that causes repeated faults at the end of the string.

LRU cannot show this anomaly because it has the stack property: for a fixed reference prefix, the set of pages resident with k frames is always a subset of the set resident with k+1 frames. Therefore, increasing the frame count cannot remove a page that would have been resident under the smaller LRU allocation, and the number of faults is non-increasing.

## 8. Experimental Workload Generation

The program creates two deterministic 100,000-reference strings:

- **Strong locality:** 90% of references select from a hot set of 8 pages; 10% select from a broader 64-page range.
- **Random:** every reference selects uniformly from 64 pages.

A fixed pseudo-random seed makes the generated workloads reproducible. The program compares FIFO, LRU, Optimal and Clock for 3 through 10 frames.

The full page-fault counts are stored in:

- `faults_locality.csv`
- `faults_random.csv`

The plot is stored in `page_fault_comparison.png`.

## 9. Runtime Measurement

For each workload, algorithm and frame count, the program executes the page-replacement simulation three times and reports the median elapsed time in milliseconds. The C implementation measures:

- FIFO: linear frame lookup plus a circular replacement pointer.
- LRU: linear frame lookup plus a scan of last-use timestamps to find the oldest resident page.
- Optimal: linear lookup plus a precomputed next-use array and a scan of resident pages.
- Clock: linear lookup plus reference-bit inspection and hand advancement on faults.

The precomputed Optimal version is an offline simulation technique. Its future-reference knowledge is not available to a live operating system.

The output is saved in `runtime_100k.csv`. Runtime values depend on CPU, compiler, operating system and system load. Use your own run's CSV if your faculty expects measurements from your computer.

## 10. Clock Compared with LRU

The benchmark records the Clock/LRU fault-count difference for both workloads. Clock requires one reference bit per frame and one hand pointer; LRU requires a per-frame timestamp or a recency structure that must be updated on hits.

In the included locality simulation, Clock is close to LRU at small frame counts and increasingly behind it at larger counts. In the random simulation, both have similar and high fault counts; a similar count there does not mean the workload has good locality. The exact percentage differences are recorded in the report and CSV results.

## 11. Engineering Decision

For this generated strong-locality web-server workload, LRU has fewer faults than FIFO and Clock, while remaining close in runtime in the measured C simulation. Optimal has the fewest faults but requires future knowledge and is not implementable as an online policy.

The report recommends LRU for the simulated workload when lower fault count is the primary objective and exact recency bookkeeping is affordable. Clock is a practical alternative for a real system that prefers low-cost reference-bit tracking and can tolerate a modest increase in faults. The final production choice should be validated with the web server's real access trace and hardware support.

## 12. Generated Evidence

After running `./run_task5.sh`, the folder contains:

| Output file | Evidence |
|---|---|
| `environment.txt` | GCC version, kernel and CPU count. |
| `page_replacement_output.txt` | Expected fixed-string output and runtime benchmark console results. |
| `faults_locality.csv` | Faults per algorithm for 3–10 frames, locality workload. |
| `faults_random.csv` | Faults per algorithm for 3–10 frames, random workload. |
| `runtime_100k.csv` | Runtime and fault count for all 100,000-reference runs. |
| `page_fault_comparison.png` | Page-fault plot for each algorithm and workload. |

Capture a terminal screenshot showing the supplied expected output, runtime benchmark and the plot.

## 13. Result

The completed program reproduces the expected FIFO/LRU/Optimal values, demonstrates Belady's anomaly, compares four replacement policies for strong-locality and random 100,000-reference workloads, plots fault counts for 3–10 frames and measures each algorithm's runtime. The measured results support an LRU recommendation for the simulated locality-heavy workload, with Clock as a lower-bookkeeping practical alternative.

## 14. GitHub Submission Reminder

The repository name specified in the assignment is:

`CSA04-192525148-Assignment3`

Keep Task 5 in a separate `Task05` folder. Include the report, README, C source, plotting script, run script, CSV files, graph and terminal screenshots. Commit Task 5 separately and share the working repository link with the course faculty.
