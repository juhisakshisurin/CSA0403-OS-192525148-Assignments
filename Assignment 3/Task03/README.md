# Assignment 3 – Task 3: Segmentation

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

---

## 1. Task Overview

**Q3 – Segmentation (6 Marks, K5 – Evaluate)** concerns student programs with separate code, stack and data regions. The code should be read-only and shareable, the stack grows, and data varies in size.

The assignment requires:

- Translate `(segment, offset)` addresses using a segment table with base and limit.
- Show what happens when an offset exceeds the segment limit.
- Draw how segmentation with paging (as in Intel x86) translates an address.
- Show the segments/mappings of a running Linux program using `/proc/<pid>/maps`.
- Explain how segmentation matches the programmer’s view of memory.
- Explain how segments are protected and shared.
- Evaluate external fragmentation caused by segmentation.
- Compare pure paging, pure segmentation and segmented paging.
- Translate eight addresses by hand and verify them with a small program.
- Trigger a segmentation fault and explain the OS response.
- Calculate fragmentation after loading and removing 20 random-size segments.
- Compare the size of the tables needed in each scheme.
- Evaluate pure paging as the alternative concept and explain why modern OSs rely mainly on paging.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment3_Task03_Report.pdf` | Complete report without student identity details. Includes the x86-style translation diagram, eight calculations, fragmentation experiment and table-size comparison. |
| `segment_translation.c` | Segment table base/limit translation program. Verifies eight addresses, including limit violations. |
| `segmentation_paging_demo.c` | Educational two-stage segmentation-plus-paging address translation model. |
| `segment_maps.c` | Prints process mappings from `/proc/self/maps`; accepts `--hold` to stay alive for `/proc/<pid>/maps` inspection. |
| `segfault_demo.c` | Forks a child process, triggers a deliberate invalid access in the child and reports the SIGSEGV from the parent. |
| `segment_fragmentation.c` | Loads 20 deterministic random-size segments into simulated contiguous memory, removes 10, then measures free holes and external fragmentation. |
| `run_task3.sh` | Compiles and runs all programs and saves the outputs. |

## 3. System Requirements

| Component | Requirement |
|---|---|
| Language | C |
| Compiler | GCC |
| Standard | C11 |
| Operating System | Linux / Ubuntu / WSL 2 |
| Linux proc filesystem | `/proc` mounted |

Record your own environment:

```bash
gcc --version
uname -a
```

The memory-map test uses `/proc`, which is available on Linux and Ubuntu under WSL.

## 4. Compile and Run All Experiments

Keep the following files in one `Task03` folder:

```text
Task03/
├── Assignment3_Task03_Report.pdf
├── README.md
├── segment_translation.c
├── segmentation_paging_demo.c
├── segment_maps.c
├── segfault_demo.c
├── segment_fragmentation.c
└── run_task3.sh
```

Open Ubuntu/WSL terminal in that folder:

```bash
chmod +x run_task3.sh
./run_task3.sh
```

The script compiles all five C programs and runs the experiments in sequence.

## 5. Compile and Run Individually

### Segment-offset translation

```bash
gcc -std=c11 -Wall -Wextra segment_translation.c -o segment_translation
./segment_translation
```

### Segmentation with paging

```bash
gcc -std=c11 -Wall -Wextra segmentation_paging_demo.c -o segmentation_paging_demo
./segmentation_paging_demo
```

### Linux process memory maps

```bash
gcc -std=c11 -Wall -Wextra segment_maps.c -o segment_maps
./segment_maps
```

For a live process map, use:

```bash
./segment_maps --hold &
PID=$!
cat /proc/$PID/maps
wait $PID
```

### Controlled segmentation-fault demonstration

```bash
gcc -std=c11 -Wall -Wextra segfault_demo.c -o segfault_demo
ulimit -c 0
./segfault_demo
```

The invalid access takes place in a child process only. The parent waits and reports whether the child terminated with SIGSEGV.

### External-fragmentation experiment

```bash
gcc -std=c11 -Wall -Wextra segment_fragmentation.c -o segment_fragmentation
./segment_fragmentation
```

## 6. Segment Table and Eight Address Calculations

The translation program uses this illustrative segment table:

| Segment | Base | Inclusive limit | Permissions |
|---|---:|---:|---|
| CODE | `0x10000` | `0x0FFF` | R-X |
| DATA | `0x30000` | `0x07FF` | RW- |
| STACK | `0x70000` | `0x03FF` | RW- |

For a valid address:

```text
Address = Base + Offset
```

The offset is valid when:

```text
0 <= Offset <= Limit
```

The eight tests are:

| No. | Segment:offset | Result |
|---:|---|---|
| 1 | CODE:0000 | `0x10000` |
| 2 | CODE:0123 | `0x10123` |
| 3 | CODE:0FFF | `0x10FFF` |
| 4 | CODE:1000 | Segment limit fault |
| 5 | DATA:0040 | `0x30040` |
| 6 | DATA:0800 | Segment limit fault |
| 7 | STACK:03FF | `0x703FF` |
| 8 | STACK:0400 | Segment limit fault |

The base/limit values are the explicit demonstration assumptions used by the implementation; the assignment does not supply a specific segment table.

In pure segmentation without paging, the base-plus-offset value is treated as the physical address for the teaching calculation. With paging enabled, it is the linear address that is translated again by the page tables.

## 7. Segmentation with Paging

The `segmentation_paging_demo.c` program models the classic two-stage x86 protected-mode flow:

1. Segment selector identifies a descriptor.
2. Descriptor base and limit are checked.
3. Base is added to offset to produce a linear address.
4. Linear page number and page offset are extracted.
5. Page mapping gives a physical frame.
6. Physical frame base plus page offset gives the physical address.

The small mapping array is an educational model, not an implementation of the hardware page-directory/page-table structures.

The report diagram describes the classic IA-32 protected-mode translation path. Modern x86-64 long mode uses a mostly flat segmentation model, with limited segment-base behaviour such as FS/GS still relevant.

## 8. Linux `/proc/<pid>/maps`

The script starts `segment_maps --hold`, reads its PID, and captures:

```bash
cat /proc/$PID/maps
```

The output shows virtual memory areas such as the executable's read-only and executable mappings, writable data, shared libraries, anonymous mappings, heap and stack where present.

Linux VMAs in `/proc/<pid>/maps` should not be confused with classic hardware segment descriptors. They are the operating system's virtual-memory mapping view.

## 9. Segmentation Fault Demonstration

`segfault_demo.c` forks a child and performs a deliberate null-pointer write only in that child. The parent waits and checks the terminating signal.

Expected output is similar to:

```text
Child <pid> terminated by SIGSEGV (signal 11).
The OS detected an invalid virtual-memory access and delivered SIGSEGV;
the parent process continued.
```

On modern x86 systems, a null-pointer access generally triggers a page-fault exception because virtual page zero is not mapped. Linux then delivers SIGSEGV. This is commonly called a segmentation fault, but it is not necessarily a classic segment-limit violation.

## 10. External Fragmentation – 20 Segments

The fragmentation program uses:

- Simulated physical memory: 1,024 KiB.
- 20 deterministic pseudo-random segment sizes between 12 KiB and 72 KiB.
- First-fit contiguous allocation.
- Removal of every second segment (odd IDs).
- Total free space, largest hole, number of holes and `total free − largest hole` calculation.

With the included fixed random seed, the run produces:

| Measurement after removal | Result |
|---|---:|
| Total free memory | 647 KiB |
| Largest free hole | 297 KiB |
| Number of holes | 10 |
| External-fragmentation indicator | 350 KiB |
| Test request | 298 KiB |

The 298 KiB request is smaller than total free space but larger than the largest hole, so it cannot fit contiguously. This demonstrates external fragmentation. The deterministic seed makes the experiment reproducible.

## 11. Table-Size Comparison Assumptions

The assignment does not specify table-entry sizes or an address width for this comparison. The report uses an illustrative 32-bit virtual address space, 4 KiB pages, 4-byte page-table entries and 8-byte segment descriptors (32-bit base plus 32-bit limit).

It assumes three segments: code 12 KiB, data 8 KiB and stack 4 KiB. These occupy six 4 KiB pages.

| Scheme | Illustrative metadata |
|---|---:|
| Pure segmentation | 3 × 8-byte descriptors = 24 bytes |
| Pure paging, flat 32-bit table | 2^20 × 4-byte PTE = 4 MiB |
| Pure paging, six mapped leaf entries only | 6 × 4-byte PTE = 24 bytes, excluding hierarchy |
| Segmented paging | 24-byte descriptors + 24-byte leaf PTEs = 48 bytes, excluding upper-level tables |

These are comparison assumptions, not exact measurements of an operating system's implementation. Real multi-level page tables require directory/table overhead and allocate lower levels according to mapped address-space regions.

## 12. Generated Evidence Files

After running `./run_task3.sh`, the directory contains:

| Output file | Purpose |
|---|---|
| `environment.txt` | GCC, Linux distribution and kernel information. |
| `segment_translation_output.txt` | Eight address translations and limit faults. |
| `segmentation_paging_output.txt` | Two-stage address translation demonstration. |
| `segment_maps_process_output.txt` | Program's PID and its own `/proc/self/maps`. |
| `segment_maps_pid_maps.txt` | Live `/proc/<pid>/maps` capture. |
| `segfault_output.txt` | Child-process SIGSEGV result. |
| `segment_fragmentation_output.txt` | 20-segment load/remove experiment and external-fragmentation calculation. |

Capture screenshots showing the eight translations, `/proc/<pid>/maps`, SIGSEGV report and fragmentation result.

## 13. Result

The programs demonstrate segment base/limit translation, limit faults, the classic x86-style segmentation-to-linear-to-physical paging path, Linux process memory mappings, controlled SIGSEGV handling and external fragmentation after segment removal. The report compares pure paging, pure segmentation and segmented paging, including the table-size assumptions.

## 14. GitHub Submission Reminder

The repository name specified in the assignment is:

`CSA04-192525148-Assignment3`

Keep Task 3 in a separate `Task03` folder. Include the report, README, all five C source files, run script, generated output files and screenshots. Commit Task 3 separately and share the working repository link with the course faculty.
