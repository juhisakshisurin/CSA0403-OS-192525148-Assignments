# Assignment 4 – Task 2: From Workload to File-Allocation Method

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 4 – CO4 |
| **Task** | Task 2 – From Workload to File-Allocation Method |
| **Marks / Bloom's Level** | 8 Marks / K5 – Evaluate |

## 1. Task Overview

The question describes a network video-surveillance recorder that stores footage from **32 cameras**. Files grow continuously while recording, are read mainly from start to end, are sometimes searched by time (random access), and are deleted after 30 days.

Task 2 requires:

- Identify file operations, their relative frequencies and sequential/direct access patterns.
- Compare contiguous, linked (including FAT) and indexed allocation for sequential access, direct access, file growth, fragmentation and reliability.
- For a 4 GB file with 4 KB blocks, calculate reads to reach the last block for each method.
- Calculate the maximum file size supported by an inode with 12 direct, one single-indirect, one double-indirect and one triple-indirect pointer, using 4-byte block addresses.
- Compare bit vector, linked free list and grouping/counting for a 4 TB disk, and calculate bit-vector size.
- Complete TODO 1 using first-fit contiguous allocation, compile and execute the supplied program.
- Explain why the 10-block file cannot fit contiguously even though 21 blocks are free, and how indexed allocation stores it.
- Evaluate the FAT-in-memory alternative, including lookup complexity, memory use and reliability.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment4_Task02_Report.pdf` | Complete report without student identity details. Includes all task answers, calculations, comparisons, algorithm analysis and recommendation. |
| `README.md` | Student details, GCC/environment information, compile command and run instructions. |
| `file_allocation.c` | Assignment's given C program with only TODO 1 completed. The rest of the given program is preserved. |
| `run_task2.sh` | Compiles the C program, executes it and checks its output against the expected output. |
| `file_allocation_output.txt` | Actual output from the executed program. |
| `expected_output.txt` | Expected output from the question paper for comparison. |
| `calculations_reference.txt` | Separate summary of the numerical calculations and assumptions. |
| `environment.txt` | Compiler, Linux distribution and kernel information from the preparation environment. |

## 3. Workload Summary

The recorder has 32 continuous recording streams. The operations are classified as:

| Operation | Relative frequency | Access pattern |
|---|---|---|
| Append/write camera footage | Continuous and high frequency; 32 streams operate concurrently. | Sequential append |
| Playback/read recorded footage | Main read operation. | Mainly sequential |
| Search by timestamp | Occasional compared with continuous recording/playback. | Direct/random access to the relevant time/block |
| Retention deletion | Periodic; footage older than 30 days is removed. | Metadata lookup and block deallocation |
| Create/close recording files | At recording-window boundaries or stream start/stop. | Directory/metadata operation |

The question does not provide camera bit rate, video resolution, clip duration or the number of simultaneous viewers. Therefore, an exact throughput or total 30-day storage requirement cannot be calculated from the supplied data alone.

## 4. Allocation-Method Recommendation

The report recommends indexed allocation (preferably a multi-level index for large files) for the stated mixed workload:

- It supports continuous file growth without requiring one long contiguous free run.
- It supports direct access by time/block through an index.
- It supports sequential playback with reasonable block traversal.
- It avoids external fragmentation of the file's data blocks.
- It has index metadata overhead and index-block damage must be protected by file-system consistency and backup mechanisms.

Contiguous allocation is fast but a growing recording may not have adjacent free blocks. Linked allocation suits sequential streams but needs a chain traversal for direct time access. FAT keeps the chain table in memory but incurs O(k) table lookups to reach block k and uses substantial memory for a 4 TB disk with 4-byte entries.

## 5. Numerical Calculation Assumptions

For the worked calculations, binary units are used:

- 1 KB = 2^10 bytes.
- 1 GB = 2^30 bytes.
- 1 TB = 2^40 bytes.
- Allocation block size = 4 KB.
- Address size = 4 bytes.

A 4 GB file therefore contains:

```text
4 × 2^30 / 2^12 = 2^20 = 1,048,576 blocks
```

A 4 TB disk contains 2^30 4-KB blocks, so its bit vector requires:

```text
2^30 bits / 8 = 2^27 bytes = 128 MiB
```

If the question intends decimal drive units (1 TB = 10^12 bytes), state that alternative separately; the reported main result follows binary OS textbook convention.

### Disk-read assumptions for the last block

- Contiguous: file extent metadata is already available; calculate the offset and read the last data block = 1 data-block read.
- Basic linked allocation: follow the chain from the first block, reading each block to obtain the next pointer, including the final block = 1,048,576 block reads.
- FAT in memory: follow 1,048,575 FAT entries in RAM, then read the target data block = one data-block read plus O(k) RAM lookups.
- Indexed: one 4-KB index block holds only 1,024 four-byte addresses. A 4-GB file therefore needs a two-level index: root index + second-level index + target data block = 3 reads if none is cached (2 data/index reads if the root is cached).

The indexed-read answer explicitly accounts for the pointer capacity of a 4-KB index block; a single index block cannot contain 1,048,576 pointers.

## 6. Complete TODO 1 – First-Fit Contiguous Allocation

The completed `alloc_contiguous()` scans from block 0, counts consecutive free blocks, marks the first run with the file letter and returns its starting block. It returns -1 if no sufficiently long run exists.

Exact compilation command required by the assignment:

```bash
gcc -std=c11 -Wall -Wextra file_allocation.c -o file_allocation
```

Run:

```bash
./file_allocation
```

Or execute the complete validation script:

```bash
chmod +x run_task2.sh
./run_task2.sh
```

The script compares the actual output byte-for-byte with `expected_output.txt` and prints PASS when it matches.

Expected output:

```text
AAAAAA........CCCCC.............
E (10 blocks) contiguous: start 19
E (10 blocks) indexed: index block 6, data blocks 7 8 9 10 11 12 13 29 30 31
AAAAAAeEEEEEEECCCCCEEEEEEEEEEEEE
```

## 7. Clarification: E Can Fit Contiguously in the Given Initial State

After releasing files B and D, 21 blocks are free, but they are split into two separated runs:

- Blocks 6–13: 8 consecutive free blocks.
- Blocks 19–31: 13 consecutive free blocks.

The first-fit allocator places the 10-block contiguous E file at block 19, because the second run is long enough. In the exercise's actual call order, that is the successful contiguous allocation. The indexed allocation then uses the remaining 11 free blocks: block 6 for the index and blocks 7–13 and 29–31 for the ten data blocks.

The initial free-space state has 21 free blocks; it does contain a 13-block contiguous run. Therefore, it is not correct to say E cannot be stored contiguously in the initial state. The expected program output shows that E **can** be stored contiguously at start block 19. Indexed allocation also succeeds afterward using the remaining scattered free blocks. This distinction is important when explaining the supplied expected output.

## 8. Folder Organisation and Submission Contents

Keep all Task 2 materials together in a folder named `Task02`:

```text
Task02/
├── Assignment4_Task02_Report.pdf
├── README.md
├── file_allocation.c
├── run_task2.sh
├── file_allocation_output.txt
├── expected_output.txt
├── calculations_reference.txt
└── environment.txt
```

Before submission, run the C program on your Ubuntu/WSL system, confirm that the output matches the expected output, and include the generated execution output and environment information with the report.
