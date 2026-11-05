# Assignment 3 – Task 6: Disk Management

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

---

## 1. Task Overview

**Q6 – Disk Management (4 Marks, K5 – Evaluate)** is based on a hospital records server that must start reliably, survive disk failure and provide enough swap space for peak load.

The assignment requires:

- Explain low-level formatting, partitioning and logical formatting.
- Explain the boot block and boot process.
- Explain bad-block handling.
- Decide the size and location of swap space.
- Compare RAID 0, RAID 1, RAID 5 and RAID 10 for reliability, capacity and speed.
- Choose a RAID level for the records server.
- Compare swap on a separate disk with a swap file.
- Explain how partitions are viewed using `lsblk` and `fdisk -l`.
- Show the partitions and swap of the Linux system.
- Calculate usable capacity for six 4-TB disks at each RAID level.
- Calculate data-loss probability for RAID 1 and RAID 5 using a stated disk-failure probability.
- Measure file-read speed on SSD and HDD if both are available.
- Evaluate RAID 6 as the alternative concept.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment3_Task06_Report.pdf` | Complete report without student identity details. |
| `README.md` | Instructions and student details in a GitHub-compatible table. |
| `raid_calculator.py` | Calculates RAID capacities and simplified data-loss probabilities. |
| `run_task6.sh` | Captures Linux partition/swap evidence, runs calculations and optionally measures SSD/HDD read speed. |
| `submission_evidence_template.txt` | Worksheet to record the actual Linux system and storage benchmark results. |

## 3. System Requirements

| Component | Requirement |
|---|---|
| Operating System | Linux / Ubuntu / WSL 2 |
| Tools | `lsblk`, `fdisk`, `swapon`, `df`, `dd`, Python 3 |
| Permissions | `sudo` may be required for full `fdisk -l` output |
| SSD/HDD test | Writable mount directories on the devices to compare |

Record your own environment:

```bash
uname -a
lsblk -o NAME,SIZE,TYPE,FSTYPE,MOUNTPOINTS,ROTA,MODEL
swapon --show
df -hT
```

`ROTA=0` generally indicates a non-rotational device (SSD/NVMe); `ROTA=1` indicates a rotational disk. Virtualised or USB storage may report device characteristics differently, so check the device model as well.

## 4. Run the Complete Evidence Script

Keep these files together in one `Task06` folder:

```text
Task06/
├── Assignment3_Task06_Report.pdf
├── README.md
├── raid_calculator.py
├── run_task6.sh
└── submission_evidence_template.txt
```

Open Ubuntu/WSL in that folder and run:

```bash
chmod +x run_task6.sh
./run_task6.sh
```

The script:
1. Captures the block-device layout with `lsblk`.
2. Captures swap using `swapon --show` and `/proc/swaps`.
3. Captures partition-table information using `fdisk -l` (may request sudo).
4. Captures filesystem usage with `df -hT`.
5. Calculates six-disk RAID capacities and the example failure probabilities.
6. Measures SSD/HDD file-read speed only when the corresponding directory is supplied.

**The script does not create or modify RAID arrays, partitions, filesystems or swap.** It is an inspection and benchmark script only.

## 5. RAID Capacity Calculator

Run the default assignment calculation:

```bash
python3 raid_calculator.py
```

Defaults:
- 6 disks
- 4 TB per disk
- Per-disk failure probability 0.02 (2%) over the selected assessment interval

To use a different per-disk failure probability, for example 1%:

```bash
python3 raid_calculator.py --disks 6 --size-tb 4 --failure-probability 0.01
```

### Capacity assumptions

| RAID | Capacity calculation | Usable capacity |
|---|---|---:|
| RAID 0 | 6 × 4 TB | 24 TB |
| RAID 1 | One six-way mirror: 4 TB | 4 TB |
| RAID 5 | (6 − 1) × 4 TB | 20 TB |
| RAID 10 | (6 / 2) × 4 TB | 12 TB |
| RAID 6 | (6 − 2) × 4 TB | 16 TB |

RAID 1 capacity depends on how disks are grouped. This assignment comparison treats RAID 1 as one six-way mirror. Three striped two-disk mirror pairs are RAID 10, not a single RAID 1 mirror, and provide 12 TB usable capacity.

These are decimal TB values before filesystem, metadata and manufacturer reserve overhead.

### Failure-probability assumption

The question asks for a disk-failure rate but does not supply a numerical value or assessment period. The report therefore uses an illustrative independent per-disk failure probability `p = 0.02` (2%) over one chosen assessment interval. This is not a measured annual failure rate.

Under this simplified model:
- RAID 1 (six-way mirror) loses data only if all six copies fail: `P(loss) = p^6`.
- RAID 5 loses data if two or more of the six drives fail in the interval: `P(loss) = 1 − (1−p)^6 − 6p(1−p)^5`.

The model assumes independent failures and does not model rebuild time, correlated failures, latent sector errors, controller faults or human error. Change `p` to the rate supplied by your faculty or actual reliability data.

## 6. Inspect Linux Partitions and Swap

The script saves:
- `disk_evidence/disk_layout.txt` – `lsblk`, swap and `/proc/swaps`.
- `disk_evidence/fdisk_output.txt` – `fdisk -l`.
- `disk_evidence/filesystem_usage.txt` – `df -hT`.

You can run these commands individually:

```bash
lsblk -o NAME,SIZE,TYPE,FSTYPE,MOUNTPOINTS,ROTA,MODEL
sudo fdisk -l
swapon --show
cat /proc/swaps
df -hT
```

On WSL, the visible block devices and swap layout may be virtualised and will not necessarily match a physical laptop or hospital server. Report exactly what your Linux environment shows.

## 7. Optional SSD and HDD Read-Speed Test

First identify the mount points for each physical device using `lsblk`. Choose a writable directory on the SSD and a writable directory on the HDD. Do not use a raw device path such as `/dev/sda`; the script writes a temporary test file inside the directories you provide.

Example (replace paths with mount points on your machine):

```bash
SSD_DIR="/path/on/ssd" HDD_DIR="/path/on/hdd" TEST_SIZE_MB=128 ./run_task6.sh
```

For an SSD-only system:

```bash
SSD_DIR="/path/on/ssd" ./run_task6.sh
```

For an HDD-only system:

```bash
HDD_DIR="/path/on/hdd" ./run_task6.sh
```

The script creates a temporary 128 MiB test file, flushes it, attempts a direct read using `dd iflag=direct`, records the throughput and removes the test file. If direct I/O is not supported by the filesystem, it falls back to a buffered read and labels that result.

For a fair comparison:
- Use the same test-file size and block size on both devices.
- Use local mounted filesystems, not network shares.
- Record whether direct or buffered I/O was used.
- Repeat the measurement if the result varies significantly.
- Do not compare a cached buffered read on one device with a direct read on the other.

If both device types are not available, state that the comparison was not possible on the current machine rather than inventing a result.

## 8. Swap-Space Decision

The report proposes an initial 8 GB swap area as a planning example for the hospital server, not as a universal sizing rule. Final sizing must be confirmed with peak-load measurements.

A practical sizing method is:

```text
Required swap >= peak committed memory
                 - RAM available for applications
                 + safety margin
```

Swap should be placed on storage protected against a single-disk failure, preferably the redundant SSD-backed RAID volume. A swap file is easier to resize; a dedicated swap partition is fixed and can offer predictable placement. Neither should be treated as a substitute for adequate RAM or as a place to store critical data.

## 9. RAID 6 Alternative

For six 4-TB drives:

```text
RAID 6 usable capacity = (6 - 2) × 4 TB = 16 TB
```

RAID 6 stores two independent parity values and can survive any two simultaneous drive failures. It has more write work than RAID 5 because it must calculate and update a second parity value. Small writes commonly involve additional read/modify/write operations and more parity computation.

## 10. Generated Evidence Files

After running the script, inspect:

| File | Purpose |
|---|---|
| `disk_evidence/disk_layout.txt` | Partitions, device information and swap status |
| `disk_evidence/fdisk_output.txt` | Partition-table details |
| `disk_evidence/filesystem_usage.txt` | Mounted filesystems and capacity |
| `disk_evidence/raid_calculations.txt` | RAID capacity and probability calculations |
| `disk_evidence/read_benchmark_status.txt` | Indicates which SSD/HDD tests were run or skipped |
| `disk_evidence/SSD_read_result.txt` | SSD read throughput if tested |
| `disk_evidence/HDD_read_result.txt` | HDD read throughput if tested |

Capture screenshots of `lsblk`, `fdisk -l`, `swapon --show`, RAID calculations and each available disk benchmark.

## 11. GitHub Submission Reminder

The repository name specified in the assignment is:

`CSA04-192525148-Assignment3`

Keep Task 6 in a separate `Task06` folder. Include the report, README, calculator, script, generated disk evidence and measurement worksheet. Commit Task 6 separately and share the working repository link with the course faculty.
