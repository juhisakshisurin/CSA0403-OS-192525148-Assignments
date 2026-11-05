# Assignment 4 – Task 3: Modern File Systems

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 4 – CO4 |
| **Task** | Task 3 – Modern File Systems |
| **Marks / Bloom's Level** | 9 Marks / K5 – Evaluate |

---

## 1. Task Overview

Task 3 requires:

- Compare FAT32, NTFS, ext4, XFS, Btrfs, ZFS and APFS for maximum file/volume size, journaling, snapshots, checksums and copy-on-write.
- Explain the consistency problem after power failure and how journaling and COW address it.
- Compare ext4 `data=journal`, `data=ordered` and `data=writeback`.
- Choose a file system for a Windows/Linux pen drive, Linux database server, laptop and snapshot-based backup server.
- Inspect an actual Linux filesystem using `stat`, `df -i`, `ls -i` and `tune2fs -l` or `dumpe2fs -h`.
- Evaluate Btrfs/ZFS COW as an alternative to ext4 journaling, including small-random-write latency, snapshot space, checksums and recovery.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment4_Task03_Report.pdf` | Complete report without personal details. Contains all four subtasks, COW alternative analysis, complexity discussion and source references. |
| `README.md` | This file, with student details and exact execution steps. |
| `inspect_filesystem.sh` | Read-only Linux inspection script. Captures `findmnt`, `stat`, `stat -f`, `df -hT`, `df -i`, `ls -lai` and filesystem-specific details. |
| `benchmark_small_writes.sh` | Optional `fio` test comparing 4 KiB random synchronous writes on existing ext4 and Btrfs/ZFS mount directories. It creates and removes temporary test files only. |
| `measurement_worksheet.txt` | Worksheet for local filesystem, benchmark, snapshot and recovery observations. |
| `preparation_environment.txt` | Environment in which the supplied scripts/report were prepared. This is not the student's personal Linux environment. |

## 3. Environment Information

The files were prepared and syntax-checked in this reference environment:

- GCC: 14.2.0
- Distribution: Debian GNU/Linux 13 (trixie)
- Kernel: Linux x86_64 6.18.57

The filesystem inspection container used an `overlay` filesystem, not a native ext4 block device. Therefore, its output is not a substitute for inspecting the student's own Linux partition.

Record your actual environment before submission:

```bash
gcc --version
cat /etc/os-release
uname -a
findmnt -T .
```

**C compilation:** No C program or “Complete the Code” block is requested in Task 3. GCC compilation is therefore not applicable. The supplied deliverables are Bash inspection/benchmark scripts.

## 4. Run the Required Linux Filesystem Inspection

Place `inspect_filesystem.sh` in a folder on the Linux filesystem you want to inspect.

```bash
chmod +x inspect_filesystem.sh
./inspect_filesystem.sh .
```

To inspect a particular mounted Linux filesystem:

```bash
./inspect_filesystem.sh /path/to/mounted/filesystem
```

The script creates:

```text
task3_fs_evidence/
├── filesystem_inspection.txt
└── filesystem_specific_details.txt
```

The first file records:
- `findmnt -T` – mount point, source device, filesystem type and options.
- `stat` – directory inode, permissions, size and timestamps.
- `stat -f` – filesystem type, block size, total/free blocks and inode counts.
- `df -hT` – total, used and available space.
- `df -i` – total, used and free inodes.
- `ls -lai` – directory entries and inode numbers.

The second file attempts the appropriate filesystem-specific inspection:
- ext2/ext3/ext4: `sudo tune2fs -l` or `sudo dumpe2fs -h`.
- Btrfs: `btrfs filesystem usage` and `btrfs filesystem df`.
- XFS: `xfs_info`.
- ZFS: `zfs list` and `zpool list`.

The script is read-only. It does not format, repair, resize or change mount options.

### Important WSL note

A path under `/mnt/c` in WSL is often backed by a Windows/DrvFS or virtual filesystem, not a native ext4 block device. `stat`, `df` and `ls -i` still report the mounted view, but `tune2fs` cannot inspect an NTFS volume. For ext4 superblock/inode evidence, inspect a real ext4 partition or a disposable ext4 virtual disk inside Linux. Never guess a device name; use `findmnt` and verify the source.

## 5. Required Commands – Manual Execution

The question explicitly lists these commands. You can run them individually:

```bash
findmnt -T .
stat .
stat -f .
df -hT .
df -i .
ls -lai .
sudo tune2fs -l /dev/<ext-filesystem-device>
```

Alternatively:

```bash
sudo dumpe2fs -h /dev/<ext-filesystem-device>
```

`tune2fs -l` and `dumpe2fs -h` display ext filesystem details; they are not formatting commands. Substitute the actual ext block-device path shown by `findmnt`.

## 6. Optional ext4 vs Btrfs/ZFS Small-Random-Write Benchmark

The assignment's alternative asks you to evaluate the effect of COW on small random writes. The optional script uses `fio`.

Install it on Ubuntu:

```bash
sudo apt update
sudo apt install fio
```

Identify existing writable mount directories using `findmnt`. Then run:

```bash
EXT4_DIR=/mnt/ext4 COW_DIR=/mnt/btrfs ./benchmark_small_writes.sh
```

Replace those example paths with actual existing mount directories. For ZFS, set `COW_DIR` to a writable directory in a ZFS dataset.

The test uses:
- 4 KiB random writes.
- Direct I/O.
- A synchronous I/O engine.
- `fdatasync` after each write.
- A 128 MiB temporary test file.
- A 10-second run per filesystem.

The temporary file is deleted after each test. JSON output is stored in `task3_benchmark_evidence/`. Repeat at least three times and compare IOPS, bandwidth and p50/p95/p99 completion latency. Use identical settings and hardware conditions. The script does not silently fall back to buffered I/O.

Do not benchmark a production database mount using this test. Run it on a dedicated test directory or disposable volume.

## 7. Snapshot-Space and Recovery Evaluation

Snapshots initially share existing blocks. Additional space is consumed as live data changes and the snapshot must retain old blocks. Use the worksheet to record the dataset size, snapshot-held space and changed-block amount.

For Btrfs, `btrfs filesystem du -s <subvolume>` can help show shared and exclusive usage. For ZFS, use:

```bash
zfs list -o name,used,avail,refer,usedbysnapshots
```

Do not create snapshots on an important dataset just for this assignment without a backup and administrator approval.

Power-failure recovery should only be tested in a disposable virtual machine with a throwaway filesystem. Do not pull power or force-reset a production laptop/server. Record mount/recovery time, filesystem messages and file-hash checks after recovery.

## 8. Key Recommendations in the Report

| Device | Recommended file system |
|---|---|
| Pen drive used on Windows and Linux | FAT32 for broad compatibility, provided no single file exceeds 4 GiB - 1 byte. NTFS is an alternative when large files are required and Linux NTFS support is available. |
| Linux database server | ext4 with `data=ordered`, correct database WAL and fsync discipline. |
| Laptop | APFS for macOS; ext4 for Linux; NTFS for Windows. The question does not specify the laptop OS. |
| Backup server needing snapshots | ZFS with redundancy, snapshots and scheduled scrubs; Btrfs is a Linux-native alternative. |

These are workload-based recommendations, not a claim that one filesystem is best on every operating system.

## 9. Evidence to Preserve

After inspection/benchmark execution, keep:
- `task3_fs_evidence/filesystem_inspection.txt`
- `task3_fs_evidence/filesystem_specific_details.txt`
- `task3_benchmark_evidence/benchmark_environment.txt`
- `task3_benchmark_evidence/ext4_fio.json` and `cowfs_fio.json` (if the tests complete)
- Terminal screenshots showing the required commands and results.
- The completed `measurement_worksheet.txt`.

Do not copy the preparation container's `overlay` measurements and present them as your laptop's filesystem data.
