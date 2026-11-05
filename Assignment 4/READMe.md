# ASSIGNMENT 4 – OPERATING SYSTEMS (CO4)
## OPTIMISING FILE STORAGE AND DISK ACCESS

## Student Details

| **Field** | **Details** |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 4 – CO4 |
| **Nature** | Individual |
| **Duration** | One Week |
| **Total Marks** | 50 |
| **Bloom's Level** | K5 – Evaluate |

---

# 1. Assignment Overview

This assignment focuses on optimising file storage and disk access by evaluating file-system organisation, file-allocation strategies, modern file systems, disk-scheduling algorithms, distributed and cloud-based storage, and device management.

The assignment evaluates the ability to choose appropriate file-storage and disk-access methods by analysing workloads, comparing techniques, calculating performance costs, implementing algorithms, measuring results and defending design decisions.

The expected engineering workflow is:

**INTERPRET THE WORKLOAD → IDENTIFY THE OPERATIONS → COMPARE THE METHODS → CALCULATE THE COST → IMPLEMENT → MEASURE → EVALUATE → RECOMMEND**

All code must compile using:

```bash
gcc -std=c11 -Wall -Wextra
```

All pseudocode must follow the SIMATS Engineering – Standard for Algorithmic Pseudocode.

---

# 2. Marks Distribution

| **Task** | **Competency Assessed** | **Marks** | **Bloom's Level** |
|---|---|---:|---|
| Task 1 | Evaluate and improve a file-system organisation | 6 | K5 – Evaluate |
| Task 2 | Choose a file-allocation method from a workload | 8 | K5 – Evaluate |
| Task 3 | Evaluate modern file systems | 9 | K5 – Evaluate |
| Task 4 | Optimise disk scheduling | 7 | K5 – Evaluate |
| Task 5 | Distributed and cloud storage, device management | 7 | K5 – Evaluate |
| Task 6 | Independently design a storage system | 13 | K5 – Evaluate |
| | **TOTAL** | **50** | |

---

# TASK 1 – FILE-SYSTEM DESIGN REVIEW (6 MARKS)

## 3. Evaluate a Proposed File Organisation

### Scenario

A college proposes to store every student's files on a shared server using:

- One flat directory for all 12,000 students.
- Files named only by the student's choice.
- Contiguous allocation for every file.
- No access control because students are expected not to access one another's files.

The proposed organisation must be evaluated and improved.

### Task 1.1 – Evaluate the Proposal (2 Marks)

Identify at least five weaknesses.

Examine:
- Name clashes in a single directory.
- Search time for a file among 12,000 students.
- File growth under contiguous allocation.
- Protection and privacy.
- Backup and recovery.
- Sharing files for group projects.

### Task 1.2 – Improve the Design (3 Marks)

Design a better file organisation covering:

- Directory structure using a tree or acyclic graph.
- Naming rules.
- File attributes.
- Access-control lists or permission bits.
- Links for shared project folders.

### Task 1.3 – Design Defence (1 Mark)

In not more than 100 words, explain how the proposed design protects privacy while still allowing sharing.

### Alternative Concept

Evaluate storing student files in a cloud service such as Google Drive or OneDrive instead of a shared server with its own directory structure.

Compare:
- Time required to find and open a file.
- Storage used, including copies.
- Control the college retains over privacy.

### Expected Deliverables

- Evaluation of the proposed flat-directory structure.
- At least five identified weaknesses.
- Improved directory-tree or acyclic-graph design.
- Naming rules and file attributes.
- Access-control design.
- Shared project-folder organisation.
- Design defence within 100 words.
- Cloud-storage alternative comparison.

---

# TASK 2 – FROM WORKLOAD TO FILE-ALLOCATION METHOD (8 MARKS)

## 4. Storage for a Video-Surveillance Recorder

### Scenario

A network video recorder stores footage from 32 cameras.

The files:
- Grow continuously while recording.
- Are mainly read from start to end.
- Are sometimes searched by time using random access.
- Are deleted after 30 days.

### Task 2.1 – Identify the Operations (2 Marks)

Identify:
- File operations.
- Frequency of each operation.
- Access patterns.
- Sequential and direct-access requirements.

### Task 2.2 – Compare the Allocation Methods (2 Marks)

Compare:

1. Contiguous allocation.
2. Linked allocation, including FAT.
3. Indexed allocation.

Evaluate each method for:
- Sequential access.
- Direct access.
- File growth.
- Fragmentation.
- Reliability.

### Task 2.3 – Calculate the Cost (3 Marks)

For a 4 GB file with 4 KB blocks:

Calculate:
- Number of disk reads required to reach the last block using each allocation method.
- Maximum file size supported by an inode containing:
  - 12 direct pointers.
  - 1 single-indirect pointer.
  - 1 double-indirect pointer.
  - 1 triple-indirect pointer.
- Each block address uses 4 bytes.

### Task 2.4 – Free-Space Management (1 Mark)

Compare:
- Bit vector.
- Linked free list.
- Grouping and counting.

For a 4 TB disk, calculate the size of the bit vector.

### Complete the Code

Complete TODO 1 in the supplied C program using **first-fit contiguous allocation**.

The function must:
- Scan the disk for the first run of consecutive free blocks.
- Check whether the requested file size can fit.
- Mark the selected blocks with the file letter.
- Return the starting block.
- Return -1 if no suitable continuous run exists.

Compile using:

```bash
gcc -std=c11 -Wall -Wextra task2.c -o task2
```

Run:

```bash
./task2
```

Expected output:

```text
AAAAAA........CCCCC.............
E (10 blocks) contiguous: start 19
E (10 blocks) indexed: index block 6, data blocks 7 8 9 10 11 12 13 29 30 31
AAAAAAeEEEEEEECCCCCEEEEEEEEEEEEE
```

Explain why file E, which requires ten blocks, cannot be stored contiguously even though 21 blocks are free, and how indexed allocation solves the problem.

### Alternative Concept

Evaluate linked allocation with a File Allocation Table (FAT) held in memory instead of indexed allocation.

Compare:
- Time to reach block k of a file: O(k) FAT look-ups versus O(1) or a few index reads.
- Memory required for the FAT on a 4 TB disk.
- Reliability if the FAT is damaged.

### Expected Deliverables

- File-operation analysis.
- Allocation-method comparison.
- Disk-read calculations.
- Maximum inode file-size calculation.
- Free-space management comparison.
- Bit-vector size calculation.
- Completed C program.
- Expected-output verification.
- FAT alternative analysis.

---

# TASK 3 – MODERN FILE SYSTEMS (9 MARKS)

## 5. Choosing a File System for Different Devices

### Task 3.1 – Compare Modern File Systems (3 Marks)

Compare:

- FAT32.
- NTFS.
- ext4.
- XFS.
- Btrfs.
- ZFS.
- APFS.

Evaluate:
- Maximum file size.
- Maximum volume size.
- Journaling.
- Snapshots.
- Checksums.
- Copy-on-write.

### Task 3.2 – Journaling and Consistency (2 Marks)

Explain what happens to a file system when power fails during a write.

Discuss:
- How journaling maintains consistency.
- How copy-on-write maintains consistency.
- Difference between ext4 journaling modes:
  - Journal.
  - Ordered.
  - Writeback.

### Task 3.3 – Choose for Each Device (3 Marks)

Select and justify a suitable file system for:

1. A pen drive used on both Windows and Linux.
2. A Linux database server.
3. A laptop.
4. A backup server requiring snapshots.

### Task 3.4 – Inspect a Real File System (1 Mark)

Use the following Linux commands:

```bash
stat
df -i
ls -i
tune2fs -l
dumpe2fs -h
```

Demonstrate:
- Inodes.
- Block size.
- Free space.
- File-system information.

### Alternative Concept

Evaluate using a copy-on-write file system such as Btrfs or ZFS instead of ext4 with journaling for the database server.

Compare:
- Write time for small random updates.
- Extra storage used by snapshots and checksums.
- Recovery speed after a power failure.

### Expected Deliverables

- Modern file-system comparison table.
- Explanation of journaling and copy-on-write.
- Comparison of ext4 journaling modes.
- Justified file-system selection for four devices.
- Real Linux inspection command outputs.
- Btrfs/ZFS alternative analysis.

---

# TASK 4 – DISK SCHEDULING ALGORITHMS (7 MARKS)

## 6. Optimising Requests on a Records Server

### Scenario

The records server has:

- Disk cylinders: 0–199.
- Initial head position: 53.
- Request queue: 98, 183, 37, 122, 14, 124, 65, 67.

### Task 4.1 – Calculate (2 Marks)

Calculate the order of service and total head movement for:

1. FCFS.
2. SSTF.
3. SCAN.
4. C-SCAN.
5. LOOK.
6. C-LOOK.

### Task 4.2 – Simulate (2 Marks)

Implement the disk-scheduling algorithms and measure:

- Average head movement.
- Maximum waiting time.

Use 10,000 random requests under:
- Light load.
- Medium load.
- Heavy load.

### Task 4.3 – Evaluate Fairness (2 Marks)

Evaluate:
- Which algorithms can starve requests.
- Why C-SCAN provides more uniform waiting time than SCAN.

Add a stream of requests near cylinder 60 arriving every millisecond and evaluate what happens to the request at cylinder 183 under SSTF.

### Task 4.4 – Solid-State Drives (1 Mark)

Explain why Linux uses the `none` or `mq-deadline` scheduler for SSDs.

Check the scheduler of your disk using:

```bash
cat /sys/block/<disk>/queue/scheduler
```

### Complete the Code

Complete TODO 1 in the supplied C program to implement SSTF.

The implementation must:
- Find the pending request with the smallest distance from the current head.
- Mark the selected request as completed.
- Store it in the service order.
- Move the current head position to the selected request.

Compile:

```bash
gcc -std=c11 -Wall -Wextra task4.c -o task4
```

Run:

```bash
./task4
```

Expected output:

```text
FCFS   53 98 183 37 122 14 124 65 67 -> 640 cylinders
SSTF   53 65 67 37 14 98 122 124 183 -> 236 cylinders
SCAN   53 37 14 0 65 67 98 122 124 183 -> 236 cylinders
C-SCAN 53 ... 199, jump to 0 ... 37 -> 183 cylinders (+199 for the jump)
```

### Alternative Concept

Evaluate LOOK or C-LOOK instead of SCAN and C-SCAN.

Compare:
- Total head movement for the given queue.
- Average waiting time.
- Maximum waiting time.
- Extra work required to find the last request using O(n) scanning or a sorted queue.

### Expected Deliverables

- Hand calculations for all six algorithms.
- C implementation of SSTF.
- Simulation of 10,000 requests.
- Light, medium and heavy load measurements.
- Average head movement and maximum waiting-time results.
- SSTF starvation demonstration.
- Fairness comparison.
- SSD scheduler inspection output.
- LOOK/C-LOOK alternative analysis.

---

# TASK 5 – DISTRIBUTED AND CLOUD-BASED STORAGE (7 MARKS)

## 7. Storage for a University

### Scenario

A university wants all student and staff files to be accessible from any campus and from home.

### Task 5.1 – Compare the Options (3 Marks)

Compare:

1. NFS.
2. Distributed file systems such as HDFS or Ceph.
3. Cloud object storage such as Amazon S3 or Google Cloud Storage.

Evaluate:
- Consistency.
- Availability.
- Performance.
- Cost.

### Task 5.2 – Replication and Consistency (2 Marks)

Explain:
- How replication protects stored data.
- Three-copy replication in HDFS.
- Strong consistency.
- Eventual consistency.
- Trade-offs between strong and eventual consistency.

### Task 5.3 – Device Management (2 Marks)

Explain how an operating system manages storage devices through:

- Device drivers.
- Buffering.
- Caching.
- Spooling.
- Block-device interface.

### Alternative Concept

Evaluate using one large NFS server instead of a replicated distributed file system.

Compare:
- Read time as the number of users increases.
- Write time as the number of users increases.
- Storage usage: one copy versus three replicas.
- Availability when one server fails.

### Expected Deliverables

- Comparison of NFS, distributed file systems and cloud object storage.
- Replication explanation.
- Strong versus eventual consistency comparison.
- Operating-system device-management explanation.
- Single NFS server alternative analysis.
- Justified storage recommendation.

---

# TASK 6 – DESIGN CHALLENGE (13 MARKS)

## 8. Storage Design for a Video-Streaming Start-up

### Scenario

A start-up streams lecture videos to 200,000 students.

Each video:
- Is between 300 MB and 2 GB.
- Is uploaded once.
- Is read thousands of times.
- Is mainly accessed during the first week.
- Allows students to jump to any minute of the video.

The company has a budget for a few servers with hard disks and SSDs and may also use cloud storage.

The design is not provided. You must develop a complete storage solution.

### Stage 1 – Interpret the Workload (2 Marks)

Identify:
- Storage operations.
- Access patterns.
- Performance targets.
- Sequential and random-access requirements.

State the requirements as testable statements.

### Stage 2 – Propose the Storage Architecture (2 Marks)

Propose:
- Where videos should be stored.
- Use of SSDs.
- Use of hard disks.
- Use of cloud storage.
- Suitable file system.
- File-allocation method.

### Stage 3 – Identify the Critical Design Decisions (2 Marks)

Decide and justify:
- Block size.
- Disk scheduler.
- Caching of popular videos.
- Replication factor.

### Stage 4 – Design and Calculate (4 Marks)

Present the complete design with calculations covering:

- Storage capacity.
- Throughput.
- Number of disk reads required to begin playback at any minute.
- Storage requirements for the expected video collection.
- Data access and caching considerations.

### Stage 5 – Implement and Demonstrate (3 Marks)

Simulate the workload using:

- Your file-allocation program.
- Your disk-scheduling program.
- `fio`, where appropriate.

Report measured results for two alternative designs.

Compare the results and explain which design performs better for the specified workload.

### Alternative Concept

Evaluate storing all videos in cloud object storage and delivering them through a Content Delivery Network (CDN) instead of using the company's own servers.

Compare:
- Time to start playback for students in different cities.
- Monthly storage cost.
- Monthly data-transfer cost.
- Operational and maintenance effort.

### Expected Deliverables

- Workload analysis.
- Complete storage architecture.
- File-system and allocation-method selection.
- Block-size and scheduler justification.
- Caching and replication decisions.
- Storage-capacity calculations.
- Throughput calculations.
- Disk-read calculations.
- Simulation of two alternative designs.
- Measured result comparison.
- Cloud object storage and CDN alternative analysis.
- Final evidence-based recommendation.

---

# 9. Suggested Overall Folder Organisation

Organise each task into a separate folder containing the report, source code, scripts, input files, output files and supporting evidence.

```text
Assignment4/
│
├── README.md
│
├── Task01/
│   ├── Report.pdf
│   ├── README.md
│   ├── proposed_directory_structure.txt
│   └── evidence/
│
├── Task02/
│   ├── Report.pdf
│   ├── README.md
│   ├── allocation.c
│   ├── input_files/
│   ├── output_files/
│   └── evidence/
│
├── Task03/
│   ├── Report.pdf
│   ├── README.md
│   ├── filesystem_comparison.txt
│   └── evidence/
│
├── Task04/
│   ├── Report.pdf
│   ├── README.md
│   ├── sstf_complete.c
│   ├── disk_scheduler_sim.c
│   ├── check_ssd_scheduler.sh
│   ├── scheduler_simulation.csv
│   └── evidence/
│
├── Task05/
│   ├── Report.pdf
│   ├── README.md
│   ├── storage_architecture.txt
│   └── comparison_tables/
│
└── Task06/
    ├── Report.pdf
    ├── README.md
    ├── storage_simulation.c
    ├── input_files/
    ├── output_files/
    ├── simulation_results.csv
    └── evidence/
```

---

# 10. Software and Environment Details

Record the actual development and execution environment used.

| **Environment Item** | **Version / Details Used** |
|---|---|
| Linux distribution | ______________________________ |
| Linux distribution version | ______________________________ |
| Linux kernel | ______________________________ |
| GCC compiler | ______________________________ |
| GCC version | ______________________________ |
| Disk type (HDD/SSD/NVMe) | ______________________________ |
| Disk model | ______________________________ |
| Block size | ______________________________ |
| File system | ______________________________ |
| Simulation tools | ______________________________ |
| `fio` version, if used | ______________________________ |

Useful commands:

```bash
gcc --version
cat /etc/os-release
uname -a
lsblk -o NAME,TYPE,ROTA,MODEL,SIZE,FSTYPE
df -hT
df -i
```

---

# 11. Compilation and Execution Guidelines

All C programs must compile with:

```bash
gcc -std=c11 -Wall -Wextra source_file.c -o output_program
```

For individual tasks, replace the source filename and output-program name with the actual filenames.

Example:

```bash
gcc -std=c11 -Wall -Wextra allocation.c -o allocation
./allocation
```

For shell scripts:

```bash
chmod +x script.sh
./script.sh
```

For filesystem inspection:

```bash
stat filename
df -i
ls -i
```

For block-device scheduler inspection:

```bash
cat /sys/block/<disk>/queue/scheduler
```

Record the exact compile command, execution steps and actual result for every program.

---

# 12. Evidence and Submission Checklist

## Task 1 – File-System Design Review

- [ ] Weaknesses in the proposed flat directory identified.
- [ ] Improved directory structure designed.
- [ ] Naming rules defined.
- [ ] File attributes identified.
- [ ] Access-control policy designed.
- [ ] Shared-folder design explained.
- [ ] Cloud storage alternative evaluated.
- [ ] Design defence provided within 100 words.

## Task 2 – File-Allocation Method

- [ ] File operations and access patterns identified.
- [ ] Contiguous, linked and indexed allocation compared.
- [ ] Disk-read calculations completed.
- [ ] Maximum inode file-size calculation completed.
- [ ] Free-space methods compared.
- [ ] Bit-vector size calculated.
- [ ] First-fit C program TODO completed.
- [ ] Program compiled and executed.
- [ ] Expected output verified.
- [ ] FAT alternative evaluated.

## Task 3 – Modern File Systems

- [ ] FAT32, NTFS, ext4, XFS, Btrfs, ZFS and APFS compared.
- [ ] Journaling explained.
- [ ] Copy-on-write explained.
- [ ] ext4 journaling modes compared.
- [ ] File systems selected for four devices.
- [ ] Linux inspection commands executed.
- [ ] Btrfs/ZFS alternative evaluated.

## Task 4 – Disk Scheduling

- [ ] FCFS calculations completed.
- [ ] SSTF calculations completed.
- [ ] SCAN calculations completed.
- [ ] C-SCAN calculations completed.
- [ ] LOOK calculations completed.
- [ ] C-LOOK calculations completed.
- [ ] SSTF C program TODO completed.
- [ ] 10,000-request simulation completed.
- [ ] Light, medium and heavy loads evaluated.
- [ ] Average movement measured.
- [ ] Maximum waiting time measured.
- [ ] SSTF starvation demonstrated.
- [ ] SSD scheduler checked.
- [ ] LOOK/C-LOOK alternative evaluated.

## Task 5 – Distributed and Cloud Storage

- [ ] NFS evaluated.
- [ ] HDFS/Ceph evaluated.
- [ ] Cloud object storage evaluated.
- [ ] Replication explained.
- [ ] Strong and eventual consistency compared.
- [ ] Device drivers explained.
- [ ] Buffering and caching explained.
- [ ] Spooling and block-device interface explained.
- [ ] Single NFS server alternative evaluated.

## Task 6 – Video-Streaming Storage Design

- [ ] Workload analysed.
- [ ] Storage architecture designed.
- [ ] File system selected.
- [ ] Allocation method selected.
- [ ] Block size justified.
- [ ] Disk scheduler selected.
- [ ] Caching strategy designed.
- [ ] Replication factor selected.
- [ ] Storage capacity calculated.
- [ ] Throughput calculated.
- [ ] Disk reads calculated.
- [ ] Two alternative designs simulated.
- [ ] Measured results compared.
- [ ] Cloud object storage and CDN alternative evaluated.
- [ ] Final recommendation justified.

---

# 13. Important Reporting and Measurement Guidelines

1. Answer every question and its subdivisions.
2. Follow the marks distribution and evaluation requirements.
3. Include technical explanations, comparison tables, calculations and justified decisions.
4. Complete every supplied C program TODO without unnecessarily changing the remaining program.
5. Compile all C programs using the specified GCC warning flags.
6. Execute every required program on Linux and compare its output with the expected output.
7. Include actual command outputs and experimental results.
8. Do not fabricate disk performance, access time, simulation results or system measurements.
9. State assumptions clearly wherever the question does not provide exact values.
10. Analyse both time complexity and space complexity using Big-O notation where required.
11. Include the alternative-concept analysis specified in every task.
12. Explain differences between theoretical expectations and actual measurements.
13. Ensure that source code, input files, output files and reports are consistent.
14. Provide clear explanations for every design decision.

---

# 14. Final Outcome

Assignment 4 demonstrates the ability to optimise file storage and disk access through systematic evaluation and implementation.

It progresses from file-system design and file-allocation strategies to modern file systems, disk scheduling, distributed storage, device management and independent storage-architecture design.

The final recommendations must be supported by correct calculations, working C implementations, measured simulation results and reasoned technical comparisons.

**END OF ASSIGNMENT 4 – CO4**
