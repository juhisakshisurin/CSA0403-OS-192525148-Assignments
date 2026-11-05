# ASSIGNMENT 3 – OPERATING SYSTEMS (CO3)
## MEMORY MANAGEMENT

## Student Details

| **Field** | **Details** |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 3 – CO3 |
| **Nature** | Individual |
| **Duration** | One Week |
| **Total Marks** | 50 |
| **Bloom's Level** | K5 – Evaluate |

---

# 1. Assignment Overview

This assignment evaluates memory-management mechanisms such as linking, loading, paging, segmentation, virtual memory, demand paging, page-replacement techniques and disk management.

The assignment focuses on determining the effectiveness of different memory-management mechanisms for different system requirements.

The expected engineering workflow is:

**DESIGN → IMPLEMENTATION → ANALYSIS → EVALUATION**

Every task requires technical reasoning, implementation where specified, experimental investigation, alternative-concept analysis and a justified conclusion.

All C programs must compile using:

```bash
gcc -std=c11 -Wall -Wextra
```

The assignment evaluates all seven tasks at Bloom's Taxonomy Level K5 – Evaluate.

---

# 2. Marks Distribution

| **Task** | **Assessed Area** | **Marks** | **Bloom's Level** |
|---|---|---:|---|
| Q1 | Linking, Loading and Address Binding | 6 | K5 – Evaluate |
| Q2 | Paging | 8 | K5 – Evaluate |
| Q3 | Segmentation | 6 | K5 – Evaluate |
| Q4 | Virtual Memory and Demand Paging | 8 | K5 – Evaluate |
| Q5 | Page-Replacement Techniques | 8 | K5 – Evaluate |
| Q6 | Disk Management | 4 | K5 – Evaluate |
| Q7 | Memory-Management Plan from First Principles | 10 | K5 – Evaluate |
| | **TOTAL** | **50** | |

---

# 3. Q1 – Linking, Loading and Address Binding (6 Marks)

## Problem Context – Updating a Banking Application

A bank's branch software is built from many modules and libraries. When a security fix is made to the encryption library, every branch currently receives a new 200 MB executable.

## Implementation Tasks

- Build the same program using static linking and dynamic (shared) libraries.
- Compare executable sizes.
- List shared libraries using `ldd`.
- Load a library at run time using `dlopen()` and `dlsym()`.
- Display the addresses at which the program and its libraries are loaded using `/proc/self/maps`.

## Evaluation

Evaluate and explain:

- Compile-time address binding.
- Load-time address binding.
- Execution-time address binding.
- Roles of the linker and loader.
- Static versus dynamic linking for memory use.
- Start-up time.
- Ease of updating shared libraries.
- Position-independent code (PIC).

## Experimental Investigation

- Measure start-up time and memory use of static and dynamic versions.
- Replace the shared library without recompiling the program and demonstrate the change.
- Run two copies of the program and identify which memory pages are shared.
- Test what happens when the shared library is missing.

## Conclusion

Recommend which linking method the bank should use and justify the decision.

## Alternative Concept and Complexity Analysis

Evaluate loading the encryption library at run time using `dlopen()` only when needed instead of linking it dynamically at start-up.

Compare:

- Start-up time.
- Memory used when encryption is not required.
- Error handling when the library is missing.
- Advantages and disadvantages of run-time loading.

## Expected Deliverables

- C source code for static linking.
- C source code for dynamic linking.
- Shared-library source code.
- Run-time loading program using `dlopen()` and `dlsym()`.
- Executable-size comparison.
- `ldd` output.
- `/proc/self/maps` output.
- Start-up time and memory measurements.
- Shared-library replacement evidence.
- Missing-library test.
- Alternative-concept evaluation.
- Final recommendation.

---

# 4. Q2 – Paging (8 Marks)

## Problem Context – Memory for a Multi-User College Server

A college server runs hundreds of student programs of different sizes. Contiguous memory allocation creates many small unusable gaps, and large programs cannot be loaded even when sufficient total memory is available.

## Implementation Tasks

- Explain external fragmentation using a diagram of contiguous allocation.
- Translate logical addresses to physical addresses using a page table.
- Calculate page-table size for 32-bit and 48-bit address spaces using 4 KB pages.
- Calculate effective memory-access time with and without a TLB.

## Evaluation

Explain and evaluate:

- How paging removes external fragmentation.
- Why internal fragmentation remains.
- Page sizes of 1 KB, 4 KB and 2 MB.
- Hierarchical or multi-level page tables.
- Hashed page tables.
- Inverted page tables.
- Protection and shared pages using paging.

## Experimental Investigation

- Calculate internal fragmentation for 100 processes of random sizes using each page size.
- Calculate effective memory-access time for TLB hit ratios of 80%, 95% and 99%.
- Find the page size of the Linux system using `getconf PAGESIZE`.
- Find page faults using `/usr/bin/time -v`.
- Test addresses at the beginning and end of a page and beyond the last page.

## Complete the Code – TODO 1

Complete the supplied C program to perform logical-to-physical address translation.

The implementation must:

1. Calculate the page number:

```c
p = a / PAGE_SIZE;
```

2. Calculate the offset:

```c
d = a % PAGE_SIZE;
```

3. Check whether the page number is outside the page table.
4. Print `invalid address` if the page is outside the table.
5. Print `page fault` if the corresponding frame number is -1.
6. Otherwise calculate the physical address:

```c
physical_address = frame * PAGE_SIZE + d;
```

## Expected Output

```text
0 -> page 0 offset      0 : physical 5120
1500 -> page 1 offset  476 : physical 2524
3100 -> page 3 offset   28 : physical 7196
6200 -> page 6 offset   56 : physical 3128
9000 -> page 8 offset  808 : invalid address
2049 -> page 2 offset    1 : page fault
```

Change `PAGE_SIZE` to 2048 and recalculate the results by hand before executing the program again.

## Conclusion

Recommend a suitable page size for the college server and justify the decision.

## Alternative Concept and Complexity Analysis

Evaluate using an inverted page table instead of a per-process multi-level page table.

Compare:

- Space required: proportional to physical memory versus each process's address space.
- Address-translation time: hash lookup versus walking several page-table levels.
- Memory overhead.
- Translation performance.

## Expected Deliverables

- Contiguous allocation fragmentation diagram.
- Logical-to-physical address translation calculations.
- Page-table size calculations for 32-bit and 48-bit spaces.
- Effective access-time calculations.
- Page-size comparison.
- Multi-level, hashed and inverted page-table explanations.
- Internal-fragmentation experiment for 100 processes.
- TLB hit-ratio calculations.
- Linux page-size and page-fault evidence.
- Completed C program.
- 1024-byte and 2048-byte page-size results.
- Inverted page-table alternative analysis.

---

# 5. Q3 – Segmentation (6 Marks)

## Problem Context – Student Programs with Separate Code, Stack and Data

Student programs contain:

- Code that should be read-only and shareable.
- A stack that grows.
- Data of varying sizes.

Paging alone treats these regions in a similar manner, whereas segmentation provides a way to represent separate logical memory regions.

## Implementation Tasks

- Translate `(segment, offset)` addresses using a segment table containing base and limit values.
- Show what happens when an offset exceeds the segment limit.
- Draw how segmentation with paging, as in Intel x86 architecture, translates an address.
- Show the memory segments of a running Linux program using `/proc/<pid>/maps`.

## Evaluation

Explain and evaluate:

- How segmentation matches the programmer's view of memory.
- How segments are protected and shared.
- External fragmentation caused by segmentation.
- Pure paging versus pure segmentation.
- Segmented paging.

## Experimental Investigation

- Translate eight addresses manually and verify them using a small program.
- Trigger a segmentation fault and explain the operating-system response.
- Calculate fragmentation after loading and removing 20 segments of random sizes.
- Compare the size of tables required in each memory-management scheme.

## Conclusion

Explain why modern operating systems rely mainly on paging.

## Alternative Concept and Complexity Analysis

Evaluate pure paging instead of segmentation.

Compare:

- Internal versus external fragmentation.
- Page-table and segment-table sizes.
- Address-translation time.
- Ease of code sharing and protection.

## Expected Deliverables

- Segment-table design.
- Base-and-limit address translation program.
- Eight manual address calculations.
- Segmentation with paging diagram.
- `/proc/<pid>/maps` output.
- Segmentation-fault demonstration.
- 20-segment fragmentation experiment.
- Comparison of pure paging, pure segmentation and segmented paging.
- Table-size calculations.
- Pure-paging alternative analysis.
- Justified conclusion.

---

# 6. Q4 – Virtual Memory and Demand Paging (8 Marks)

## Problem Context – Running Large Medical-Imaging Software on a Laptop

A radiologist needs to run an image-processing program that uses 24 GB of memory on a laptop with 16 GB of RAM.

## Implementation Tasks

Explain:

- Steps involved in handling a page fault.
- Effective memory-access time for page-fault rates of:
  - 1 in 1,000.
  - 1 in 100,000.
  - 1 in 1,000,000.
- Assume memory access time of 100 ns and page-fault service time of 8 ms.
- Copy-on-write and how it makes `fork()` fast.
- Thrashing and the working-set model.

## Evaluation

Evaluate:

- How demand paging allows a 24 GB program to run on a machine with 16 GB RAM.
- Maximum acceptable page-fault rate for a 10% slowdown.
- How the number of frames per process should be decided.
- Equal frame allocation.
- Proportional frame allocation.
- How the operating system detects and stops thrashing.

## Experimental Investigation

- Write a program that accesses memory sequentially and randomly and counts page faults.
- Measure execution time as memory usage grows beyond available free RAM.
- Use a virtual machine or memory limit for controlled testing.
- Demonstrate copy-on-write with `fork()` by measuring memory before and after writing.
- Plot page faults against the number of frames for a reference string.

## Conclusion

Determine whether buying more RAM or improving the program's locality is the better solution for the radiologist.

Justify the decision using measurements.

## Alternative Concept and Complexity Analysis

Evaluate running the program on a machine with 32 GB RAM instead of relying on demand paging.

Compare:

- Effective access time in both cases using the measured page-fault rate.
- Cost of additional RAM.
- Cost of slower execution.
- Performance and resource-utilisation trade-offs.

## Expected Deliverables

- Page-fault handling explanation.
- Effective access-time calculations.
- 10% slowdown page-fault-rate calculation.
- Copy-on-write demonstration.
- Thrashing and working-set explanation.
- Sequential and random memory-access program.
- Page-fault and execution-time measurements.
- Memory-before-and-after fork measurements.
- Page-fault versus frame-count graph.
- 16 GB versus 32 GB memory comparison.
- Final recommendation.

---

# 7. Q5 – Page-Replacement Techniques (8 Marks)

## Problem Context – Choosing a Policy for a Web Server

A web server keeps frequently used data pages in memory. When memory becomes full, the operating system must decide which page to remove.

An unsuitable page-replacement policy can increase page faults and reduce response performance.

## Implementation Tasks

Implement:

1. FIFO page replacement.
2. LRU page replacement.
3. Optimal page replacement.

Count page faults for the given reference string using:

- 3 frames.
- 4 frames.

Also:

- Demonstrate Belady's anomaly using FIFO.
- Explain the Clock (Second-Chance) algorithm as a practical approximation of LRU.

## Evaluation

Explain and evaluate:

- Why Optimal is the theoretical best but cannot be implemented in practice.
- Why LRU does not suffer from Belady's anomaly because of the stack property.
- Hardware support required by exact LRU.
- How Clock avoids the need for exact LRU bookkeeping.
- Performance for a workload with strong locality.

## Experimental Investigation

- Generate reference strings with strong locality.
- Generate reference strings with random access.
- Count faults for FIFO, LRU and Optimal using 3 to 10 frames.
- Plot page faults against the number of frames for each algorithm.
- Measure the running time of each algorithm for 100,000 references.
- Verify the program's results by manually calculating one reference string.

## Engineering Decision

Recommend a page-replacement policy for the web server using the measured results.

## Complete the Code – TODO 1

Complete the supplied LRU C program.

The implementation must:

1. Use an available free frame if one exists.
2. Otherwise identify the frame whose `last[]` value is the smallest.
3. Replace the least recently used page.
4. Record `last[] = i` for the frame containing the current page.

## Expected Output

```text
3 frames: FIFO 15, LRU 12, Optimal 9
4 frames: FIFO 10, LRU 8, Optimal 8
Belady string, FIFO: 3 frames 9 faults, 4 frames 10 faults
```

Explain:

- Why FIFO has more faults with four frames than with three for the Belady reference string.
- Why LRU does not exhibit this behaviour.

## Alternative Concept and Complexity Analysis

Evaluate the Clock (Second-Chance) algorithm instead of exact LRU.

Compare:

- Time per reference.
- Updating timestamps or a list versus setting one reference bit.
- Extra space per frame.
- Page faults compared with LRU in the simulation.

## Expected Deliverables

- FIFO implementation.
- LRU implementation.
- Optimal implementation.
- Completed LRU TODO.
- 3-frame and 4-frame results.
- Belady's anomaly demonstration.
- Clock algorithm explanation or simulation.
- Strong-locality and random-reference experiments.
- 3-to-10-frame comparison.
- Page-fault graphs.
- Running-time measurements for 100,000 references.
- Manual verification.
- Engineering recommendation.

---

# 8. Q6 – Disk Management (4 Marks)

## Problem Context – Storage for a Hospital Records Server

The hospital records server must:

- Start reliably.
- Survive disk failure.
- Have adequate swap space for peak load.

## Implementation Tasks

Explain:

- Low-level formatting.
- Partitioning.
- Logical formatting.
- Boot block.
- Boot process.
- Bad-block handling.
- Suitable swap-space size and location.

## Evaluation

Compare RAID 0, RAID 1, RAID 5 and RAID 10 for:

- Reliability.
- Usable capacity.
- Read speed.
- Write speed.

Also:

- Choose a RAID level for the records server.
- Evaluate swap on a separate disk versus a swap file.
- Explain how Linux partitions are viewed using `lsblk` and `fdisk -l`.

## Experimental Investigation

- Show partitions of the Linux system.
- Show existing swap space.
- Calculate usable capacity for six disks of 4 TB for each RAID level.
- Calculate data-loss probability for RAID 1 and RAID 5 using a stated disk-failure rate.
- Measure file-read speed on SSD and HDD if both are available.

## Conclusion

Recommend a storage configuration for the hospital records server and justify it.

## Alternative Concept and Complexity Analysis

Evaluate RAID 6 instead of RAID 5 or RAID 10.

Compare:

- Usable capacity of six 4-TB disks.
- Write time and additional parity calculation.
- Number of disk failures that can be survived.

## Expected Deliverables

- Disk-formatting explanation.
- Boot-block and boot-process explanation.
- Bad-block handling explanation.
- Swap-space design.
- RAID 0, RAID 1, RAID 5 and RAID 10 comparison.
- RAID capacity calculations.
- Data-loss probability calculations.
- Linux partition and swap outputs.
- SSD/HDD read-speed measurements where available.
- RAID 6 alternative analysis.
- Final storage recommendation.

---

# 9. Q7 – Design a Memory-Management Plan from First Principles (10 Marks)

## Problem Context – Memory Management for an Edge Device in a Smart Factory

A smart factory installs edge devices with:

- 2 GB RAM.
- No swap disk.
- A real-time control task that must never be delayed by a page fault.
- A machine-learning task requiring 1.5 GB.
- A logging task.

The vendor plans to add more tasks in the future.

You must develop a complete memory-management design from the requirements.

## Stage 1 – Interpret the Problem (2 Marks)

- Identify memory requirements of each task.
- Identify timing constraints of each task.
- Identify which tasks can tolerate page faults.
- Identify which tasks cannot tolerate page faults.
- State requirements as measurable statements.

## Stage 2 – Compare Candidate Mechanisms (3 Marks)

Compare:

- Contiguous allocation.
- Paging.
- Segmentation.
- Demand paging.

Decide how memory should be locked for the real-time task, for example using `mlock()`.

Present a comparison table with justification.

## Stage 3 – Implement and Verify (3 Marks)

Implement:

- A program that locks memory for a time-critical loop.
- A measurement of worst-case delay with memory locking.
- A measurement of worst-case delay without memory locking.
- A page-replacement simulation for the memory usage of the three tasks.
- A verification approach demonstrating that the real-time task is not delayed by page faults.

## Stage 4 – Defend the Design (2 Marks)

- Justify the proposed memory-management plan using measurements.
- Explain what changes if a fourth task requiring 500 MB is added.
- Identify one remaining limitation.

## Alternative Concept and Complexity Analysis

Evaluate using a real-time operating system without virtual memory instead of Linux with locked memory for the control task.

Compare:

- Worst-case delay of the control task.
- Memory available to the machine-learning task.
- Effort of adding new tasks.
- Advantages and limitations of both approaches.

## Expected Deliverables

- Task memory-requirement analysis.
- Timing constraints.
- Paging and segmentation comparison.
- Memory-locking design using `mlock()` or an appropriate mechanism.
- C implementation of memory locking.
- Worst-case delay measurements with and without locking.
- Page-replacement simulation for the three tasks.
- Fourth-task capacity analysis.
- Remaining limitation.
- Real-time OS alternative analysis.
- Final memory-management recommendation.

---

# 10. Suggested Overall Folder Organisation

Keep each task in a separate folder containing its report, source code, supporting scripts, input files, generated outputs and evidence.

```text
Assignment3/
│
├── README.md
│
├── Task01/
│   ├── Report.pdf
│   ├── README.md
│   ├── static_linking.c
│   ├── dynamic_linking.c
│   ├── shared_library.c
│   ├── runtime_loading.c
│   └── evidence/
│
├── Task02/
│   ├── Report.pdf
│   ├── README.md
│   ├── paging.c
│   ├── fragmentation_simulation.c
│   └── evidence/
│
├── Task03/
│   ├── Report.pdf
│   ├── README.md
│   ├── segment_translation.c
│   ├── segmentation_paging_demo.c
│   ├── segment_maps.c
│   ├── segfault_demo.c
│   ├── segment_fragmentation.c
│   └── evidence/
│
├── Task04/
│   ├── Report.pdf
│   ├── README.md
│   ├── memory_access.c
│   ├── cow_demo.c
│   ├── page_fault_measurement.c
│   └── evidence/
│
├── Task05/
│   ├── Report.pdf
│   ├── README.md
│   ├── page_replacement.c
│   ├── page_replacement_simulation.c
│   ├── graphs/
│   └── evidence/
│
├── Task06/
│   ├── Report.pdf
│   ├── README.md
│   ├── raid_calculator.py
│   ├── run_task6.sh
│   ├── measurement_worksheet.txt
│   └── evidence/
│
└── Task07/
    ├── Report.pdf
    ├── README.md
    ├── memory_locking.c
    ├── memory_simulation.c
    ├── page_replacement_simulator.c
    └── evidence/
```

The filenames above are suggested organisation examples. Use the actual source filenames prepared for each task.

---

# 11. Software and Environment Details

Record the actual compiler, Linux distribution and tools used.

| **Environment Item** | **Version / Details Used** |
|---|---|
| Linux distribution | ______________________________ |
| Linux distribution version | ______________________________ |
| Linux kernel | ______________________________ |
| GCC compiler | ______________________________ |
| GCC version | ______________________________ |
| C standard | C11 |
| Memory page size | ______________________________ |
| System architecture | ______________________________ |
| Storage device type | ______________________________ |
| Storage file system | ______________________________ |
| Other tools used | ______________________________ |

Useful commands:

```bash
gcc --version
cat /etc/os-release
uname -a
getconf PAGESIZE
lsblk
df -hT
free -h
```

---

# 12. Compilation and Execution Guidelines

All C programs must be compiled with:

```bash
gcc -std=c11 -Wall -Wextra source_file.c -o output_program
```

For programs using POSIX threads, use the appropriate thread option:

```bash
gcc -std=c11 -Wall -Wextra -pthread source_file.c -o output_program
```

For programs using dynamic loading, link with the required dynamic-loading library where needed:

```bash
gcc -std=c11 -Wall -Wextra runtime_loading.c -o runtime_loading -ldl
```

Run a compiled program using:

```bash
./output_program
```

For shell scripts:

```bash
chmod +x script.sh
./script.sh
```

For memory and system measurements, use the commands specified by the relevant task, such as:

```bash
getconf PAGESIZE
/usr/bin/time -v ./program
strace -c ./program
cat /proc/<pid>/maps
```

Record the exact compilation command, execution steps and actual result for each program in its task-specific README.

---

# 13. Evidence and Submission Checklist

## Q1 – Linking, Loading and Address Binding

- [ ] Static-linked program.
- [ ] Dynamic-linked program.
- [ ] Shared library.
- [ ] `dlopen()` and `dlsym()` implementation.
- [ ] Executable-size comparison.
- [ ] `ldd` output.
- [ ] `/proc/self/maps` output.
- [ ] Start-up time measurements.
- [ ] Memory-use measurements.
- [ ] Shared-library replacement test.
- [ ] Shared-page demonstration.
- [ ] Missing-library test.
- [ ] Run-time loading alternative analysis.

## Q2 – Paging

- [ ] External-fragmentation diagram.
- [ ] Page-table address translation.
- [ ] 32-bit page-table size calculation.
- [ ] 48-bit page-table size calculation.
- [ ] TLB effective access-time calculations.
- [ ] Page-size comparison.
- [ ] Multi-level, hashed and inverted page-table explanation.
- [ ] Internal fragmentation for 100 processes.
- [ ] TLB hit ratios of 80%, 95% and 99%.
- [ ] Linux page-size output.
- [ ] Page-fault output.
- [ ] Completed address-translation program.
- [ ] 1024-byte and 2048-byte page-size test.
- [ ] Inverted page-table alternative analysis.

## Q3 – Segmentation

- [ ] Segment table with base and limit.
- [ ] Eight manual address translations.
- [ ] Address-translation program.
- [ ] Segmentation with paging diagram.
- [ ] `/proc/<pid>/maps` output.
- [ ] Segmentation-fault demonstration.
- [ ] 20-segment fragmentation experiment.
- [ ] Table-size comparison.
- [ ] Pure paging alternative analysis.

## Q4 – Virtual Memory and Demand Paging

- [ ] Page-fault handling explanation.
- [ ] Effective access-time calculations.
- [ ] 10% slowdown calculation.
- [ ] Copy-on-write explanation.
- [ ] Thrashing and working-set explanation.
- [ ] Sequential and random memory-access program.
- [ ] Page-fault measurements.
- [ ] Memory-before-and-after fork measurements.
- [ ] Page-fault versus frame-count graph.
- [ ] 16 GB versus 32 GB comparison.

## Q5 – Page-Replacement Techniques

- [ ] FIFO implementation.
- [ ] LRU implementation.
- [ ] Optimal implementation.
- [ ] Completed LRU TODO.
- [ ] 3-frame and 4-frame results.
- [ ] Belady's anomaly demonstration.
- [ ] Clock/Second-Chance explanation.
- [ ] Strong-locality reference string.
- [ ] Random reference string.
- [ ] 3-to-10-frame comparison.
- [ ] Page-fault graphs.
- [ ] Running-time measurements for 100,000 references.
- [ ] Manual calculation verification.
- [ ] Engineering recommendation.

## Q6 – Disk Management

- [ ] Formatting and partitioning explanation.
- [ ] Boot-block and boot-process explanation.
- [ ] Bad-block handling.
- [ ] Swap-space decision.
- [ ] RAID 0, 1, 5 and 10 comparison.
- [ ] Six-disk capacity calculations.
- [ ] RAID 1 and RAID 5 data-loss probability calculations.
- [ ] Linux partition and swap output.
- [ ] SSD/HDD read-speed comparison where available.
- [ ] RAID 6 alternative analysis.
- [ ] Final storage recommendation.

## Q7 – Memory-Management Plan

- [ ] Real-time control task requirements.
- [ ] Machine-learning task requirements.
- [ ] Logging task requirements.
- [ ] Candidate memory-management mechanisms compared.
- [ ] Memory-locking implementation.
- [ ] Worst-case delay measurement with locking.
- [ ] Worst-case delay measurement without locking.
- [ ] Page-replacement simulation.
- [ ] Fourth-task memory-capacity calculation.
- [ ] Remaining limitation identified.
- [ ] Real-time OS alternative analysis.
- [ ] Final memory-management recommendation.

---

# 14. Important Reporting and Measurement Guidelines

1. Answer every question and all its subdivisions.
2. Follow the marks distribution and evaluation requirements.
3. Complete every supplied "Complete the Code" program without unnecessarily changing the remaining code.
4. Compile all C programs using the specified GCC warning flags.
5. Execute all programs on Linux and compare the results with the expected output.
6. Include actual command outputs, calculations, diagrams and experimental evidence.
7. Do not fabricate memory usage, page-fault counts, execution times or system measurements.
8. Clearly state assumptions wherever exact input data is not supplied.
9. Analyse time and space complexity using Big-O notation wherever required.
10. Include the alternative-concept and complexity analysis in every task.
11. Explain differences between theoretical expectations and actual experimental results.
12. Ensure source code, input data, output files and reports are consistent.
13. Keep a separate folder for every task.
14. Be prepared to explain and defend the submitted code, calculations and design choices during assessment verification.

---

# 15. General Submission Requirements

The assignment requires:

- Individual execution of every task.
- A separate folder for each task.
- All C source files and completed TODO programs.
- Scripts and configuration files, where applicable.
- Input files and generated output files.
- Command outputs and screenshots.
- A report containing the evaluations, calculations, graphs and measurements for Q1–Q7.
- A README containing student details, compiler and version, Linux distribution and version, exact compilation commands, execution steps and a short result for every task.

The prescribed repository name is:

```text
CSA04-192525148-Assignment3
```

The assignment requires the repository to be shared with the course faculty. It also requires progressive commits, with at least one commit for each task. The commit history will be checked.

---

# 16. Final Outcome

Assignment 3 demonstrates the ability to evaluate and implement memory-management mechanisms for different computing requirements.

It progresses from linking and loading to paging, segmentation, virtual memory, page-replacement policies, disk management and independent memory-management design.

The final solutions must be supported by correct calculations, completed C programs, experimental measurements, graphs, command outputs, complexity analysis and justified engineering recommendations.

**END OF ASSIGNMENT 3 – CO3**
