# Assignment 3 – Task 2: Paging

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

## 1. Task Overview

**Q2 – Paging (8 Marks, K5 – Evaluate)** is based on a multi-user college server. Contiguous allocation creates external fragmentation, leaving free memory in scattered gaps even when enough total memory exists to load a program.

The task requires:

- Explain external fragmentation using a contiguous-allocation diagram.
- Translate logical addresses to physical addresses using a page table.
- Calculate page-table size for 32-bit and 48-bit address spaces with 4 KB pages.
- Calculate effective memory-access time with and without a TLB.
- Explain why paging removes external fragmentation but retains internal fragmentation.
- Evaluate page sizes of 1 KB, 4 KB and 2 MB.
- Explain hierarchical, hashed and inverted page tables.
- Explain protection and shared pages.
- Calculate internal fragmentation for 100 random-size processes at each page size.
- Calculate TLB EAT for hit ratios of 80%, 95% and 99%.
- Record Linux native page size with `getconf PAGESIZE` and program page faults with `/usr/bin/time -v`.
- Test addresses at the start and end of pages and beyond the final page.
- Evaluate an inverted page table as the alternative concept.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment3_Task02_Report.pdf` | Complete report without student identity details. |
| `paging_translation.c` | Completed version of the assignment's TODO 1. It handles invalid addresses, non-resident pages and physical-address translation. It also checks page boundaries. |
| `paging_experiments.c` | Reproducible 100-process internal-fragmentation experiment and TLB EAT calculation. |
| `inverted_page_table_demo.c` | Small hash-based inverted-page-table alternative demonstration. |
| `paging_memory_probe.c` | Allocates and touches 64 MiB, one byte per native page, for page-fault measurement. |
| `run_task2.sh` | Compiles and executes all experiments and saves the results/environment details. |
| `measurement_results_template.txt` | Template for recording native Linux page size and page-fault data from your own machine. |

## 3. System Requirements

| Component | Requirement |
|---|---|
| Programming Language | C |
| Compiler | GCC |
| Standard | C11 |
| Operating System | Linux / Ubuntu / WSL 2 |
| System utilities | `getconf`, `/usr/bin/time`, `lsb_release` (if installed) |

The execution script records the actual compiler and Linux environment in `environment.txt`. Verify the version used for your submission with:

```bash
gcc --version
lsb_release -a
uname -a
getconf PAGESIZE
```

Do not report an environment version copied from another computer.

## 4. Compile and Run All Experiments

Keep the following files in one `Task02` folder:

```text
Task02/
├── Assignment3_Task02_Report.pdf
├── README.md
├── paging_translation.c
├── paging_experiments.c
├── inverted_page_table_demo.c
├── paging_memory_probe.c
└── run_task2.sh
```

Open Ubuntu/WSL terminal in that folder and execute:

```bash
chmod +x run_task2.sh
./run_task2.sh
```

## 5. Exact Compilation Commands

### Page-table address translation

```bash
gcc -std=c11 -Wall -Wextra paging_translation.c -o paging_translation
```

### Internal fragmentation and TLB experiment

```bash
gcc -std=c11 -Wall -Wextra paging_experiments.c -o paging_experiments
```

### Inverted page-table demonstration

```bash
gcc -std=c11 -Wall -Wextra inverted_page_table_demo.c -o inverted_page_table_demo
```

### Page-fault memory probe

```bash
gcc -std=c11 -O2 -Wall -Wextra paging_memory_probe.c -o paging_memory_probe
```

## 6. Complete-the-Code Program

The supplied `paging_translation.c` preserves the assignment's process page table:

```c
#define PAGE_SIZE 1024
#define PAGES 8

int page_table[PAGES] = {5, 2, -1, 7, 0, -1, 3, -1};
```

For a logical address `a`:

```c
p = a / PAGE_SIZE;
d = a % PAGE_SIZE;
```

- If `p >= PAGES`, print `invalid address`.
- If `page_table[p] == -1`, print `page fault`.
- Otherwise calculate `frame * PAGE_SIZE + d`.

Run the default 1 KB page-size version:

```bash
./paging_translation
```

The six expected results from the question paper are:

```text
0    -> page 0 offset 0   : physical 5120
1500 -> page 1 offset 476 : physical 2524
3100 -> page 3 offset 28  : physical 7196
6200 -> page 6 offset 56  : physical 3128
9000 -> page 8 offset 808 : invalid address
2049 -> page 2 offset 1   : page fault
```

## 7. Recalculate with 2 KB Pages

The script recompiles the same program with `PAGE_SIZE=2048`:

```bash
gcc -std=c11 -Wall -Wextra -DPAGE_SIZE=2048 \
    paging_translation.c -o paging_translation_2kb
./paging_translation_2kb
```

Expected recalculated results:

| Logical address | Page | Offset | Frame | Physical address |
|---:|---:|---:|---:|---:|
| 0 | 0 | 0 | 5 | 10240 |
| 1500 | 0 | 1500 | 5 | 11740 |
| 3100 | 1 | 1052 | 2 | 5148 |
| 6200 | 3 | 56 | 7 | 14392 |
| 9000 | 4 | 808 | 0 | 808 |
| 2049 | 1 | 1 | 2 | 4097 |

The same eight-entry page table is used for the recalculation.

## 8. Page-Table Size Assumption

The question specifies page size but does not specify page-table-entry width. The report therefore states the assumption used in the calculation: a 4-byte page-table entry for both address widths. It also presents the result if an 8-byte entry is used.

For 4 KB pages, the offset uses 12 bits.

- 32-bit address: 2^20 virtual pages; at 4 bytes per PTE, 4 MiB.
- 48-bit address: 2^36 virtual pages; at 4 bytes per PTE, 256 GiB.

The large theoretical 48-bit single-level table motivates multi-level or inverted page-table structures.

## 9. TLB Effective Access Time Assumptions

The problem does not provide memory or TLB access times. The report uses an explicit illustrative assumption:

- TLB lookup: 10 ns.
- Main-memory access: 100 ns.
- No page fault.
- One-level page table for the miss calculation.

Formula:

```text
EAT = h(TLB + memory) + (1-h)(TLB + 2 × memory)
```

The program reports EAT for 80%, 95% and 99% hit ratios. Replace the illustrative timing assumptions if your faculty supplies different values.

## 10. Internal Fragmentation Experiment

`paging_experiments.c` generates 100 deterministic pseudo-random process sizes from 1 byte to 10 MiB using a fixed random seed.

For each page size:

```text
Allocated size = ceil(process size / page size) × page size
Internal waste = allocated size − process size
```

The program compares 1 KB, 4 KB and 2 MB pages. Since the random seed is fixed, the same process sizes and fragmentation results are generated when the program is rerun.

Run:

```bash
./paging_experiments
```

## 11. Linux Native Page Size and Page Faults

The script runs:

```bash
getconf PAGESIZE
```

It also compiles `paging_memory_probe.c`, which allocates 64 MiB and touches one byte in each native page. Page-fault statistics are collected with:

```bash
/usr/bin/time -v ./paging_memory_probe
```

The script saves the actual output in `page_faults_time.txt` and selected summary lines in `page_fault_summary.txt`.

Record the values from your own Ubuntu/WSL environment. Page-fault counts and memory measurements are machine- and run-dependent.

## 12. Inverted Page Table Alternative

`inverted_page_table_demo.c` demonstrates a hash-based lookup using process ID and virtual page number as the key. It models the core idea of one entry per physical frame.

- Per-process multi-level page tables use space according to the mapped virtual pages of processes; a TLB miss requires a walk across the configured levels.
- An inverted page table uses O(F) entries for F physical frames. A hash lookup is expected O(1), with worst-case O(F) search in a simple implementation.

Run:

```bash
./inverted_page_table_demo
```

The demonstration is a simplified educational hash table, not a replacement for the kernel's hardware page-table implementation.

## 13. Generated Evidence Files

After running `./run_task2.sh`, the folder contains:

| Output file | Purpose |
|---|---|
| `environment.txt` | GCC, Linux distribution, kernel and native page size. |
| `translation_1kb.txt` | Six supplied logical-address results and page-boundary tests for 1 KB pages. |
| `translation_2kb.txt` | Recalculated results and boundary tests for 2 KB pages. |
| `paging_experiment_results.txt` | 100-process internal fragmentation and TLB EAT calculations. |
| `inverted_page_table_output.txt` | Inverted page-table lookup demonstration. |
| `linux_page_size.txt` | Native Linux page size. |
| `paging_memory_probe_output.txt` | 64 MiB allocation/touch program output. |
| `page_faults_time.txt` | Full `/usr/bin/time -v` output. |
| `page_fault_summary.txt` | Page-fault, RSS and elapsed-time summary. |

Capture terminal screenshots showing the translation results, experiment table, native page size and page-fault output.

## 14. Result

The programs demonstrate page-table address translation, invalid-address/page-fault distinction, address-boundary handling, internal-fragmentation trade-offs, TLB effective access time and an inverted-page-table alternative. Use the actual Linux page size and page-fault data recorded on your own system in the final submission.

## 15. GitHub Submission Reminder

The repository name specified in the assignment is:

`CSA04-192525148-Assignment3`

Keep Task 2 in its own `Task02` folder. Include the report, all C source files, run script, generated outputs and screenshots. Commit Task 2 separately and add the actual repository link to the report cover page and repository-level README before submitting.
