# Assignment 5 – Part A – A1: Evaluate the Linux System Components

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 5 – CO5 |
| **Part** | Part A – Design |
| **Task** | A1 – Evaluate the Linux System Components |
| **Marks / Bloom's Level** | 7 Marks / K5 – Evaluate |

## 1. Task Requirements

| Section | Marks | Requirement |
|---|---:|---|
| A1.1 – Linux Architecture | 2 | Explain the Linux kernel, system libraries, shell and utilities, and how the kernel manages processes, memory, files, devices and network. |
| A1.2 – Process and Memory Management | 3 | Evaluate CFS, page cache and swapping, and `/proc` and `/sys` as observation tools. |
| A1.3 – Linux File System and Permissions | 2 | Explain directory hierarchy, VFS, owner/group/others permissions, setuid and sudo. |
| Alternative Concept | Included | Evaluate Windows Server base memory/disk requirements, administration automation time and licence cost per server. |

The case study gives 150,000 student logins within two hours on result day and states that the current single physical Linux server crashes at peak. The report contextualises Linux scheduling, memory, file management and observation tools using this scenario.

## 2. Files Included

| File | Purpose |
|---|---|
| `Assignment5_PartA_A1_Report.pdf` | Complete report without your name or register number. |
| `README.md` | Student details, task summary and evidence procedure. |
| `a1_linux_evidence.sh` | Optional read-only Bash script to collect Linux process, memory, `/proc`, `/sys`, filesystem and permission observations. |
| `a1_evidence_commands.txt` | Manual commands corresponding to the evidence topics. |

## 3. Compiler and Environment

A1 is a design/evaluation task. It does **not** ask for a C program or a “Complete the Code” implementation. Therefore:
- C compiler: not required for A1.
- GCC compilation command: not applicable.
- The included evidence collector is a Bash script and does not require GCC.

Record your actual environment in Ubuntu/WSL:

```bash
gcc --version
cat /etc/os-release
uname -a
```

## 4. Run the Optional Linux Evidence Collector

Open Ubuntu/WSL in the folder containing the script:

```bash
chmod +x a1_linux_evidence.sh
./a1_linux_evidence.sh
```

The script creates `a1_linux_evidence.txt`. To choose another output name:

```bash
./a1_linux_evidence.sh A1_my_system_evidence.txt
```

The output records the Linux distribution/kernel, CPU and process observations, RAM/swap/pressure data, selected `/proc` entries, `/sys` network and block-device information, mounted filesystems and permission examples.

The script is read-only. It does not change `/proc`, `/sys`, kernel parameters, permissions or services.

### WSL note

A WSL environment exposes the Linux kernel interfaces of the WSL virtual machine. Its block devices and scheduler attributes may differ from the Windows host's physical disk. A missing `/sys/block/<device>/queue/scheduler` attribute can occur with virtualised storage and is not a script failure.

## 5. Main Evaluation Points

- Linux user space contains applications, libraries, shells and utilities; the privileged kernel provides process, memory, VFS, device and network services.
- Classic CFS shares CPU time using virtual runtime. Current kernel documentation notes a transition toward EEVDF beginning in Linux 6.6; identify the kernel version actually observed.
- Page cache speeds file access and buffers writes. Swap can back selected anonymous pages, but excessive swapping causes latency and thrashing.
- `/proc` exposes process/kernel information; `/sys` exposes kernel objects, devices, drivers and selected attributes.
- VFS provides a common file-operation interface across filesystem implementations.
- Owner/group/others permissions enforce access. setuid changes effective identity under applicable conditions; sudo grants only commands permitted by its policy.

## 6. Windows Server Alternative – Qualification

The report compares published minimum installation requirements, not measured idle RAM or disk footprint. It uses Windows Server 2025 and Ubuntu Server 24.04 LTS references:

- Ubuntu Server 24.04 LTS: 1.5 GB RAM and 5 GB disk minimum for ISO installation; Canonical suggests 3 GB RAM and 25 GB or more storage.
- Windows Server 2025: 2 GB minimum RAM for Server Core; 2 GB minimum for Desktop Experience with 4 GB recommended; 32 GB is an absolute minimum system partition.

The report evaluates PowerShell/DSC automation and Microsoft's published US suggested 16-core MSRP for Windows Server 2025. Microsoft states prices vary by country; the India pricing page currently displays variable pricing. CAL and core licensing requirements must also be confirmed.

The automation-time example is an illustrative planning calculation, not a measured benchmark.

## 7. Evidence to Preserve

After running the script, retain:
- `a1_linux_evidence.txt`
- A terminal screenshot showing the command and completion message.
- Outputs/screenshots for `free -h`, `vmstat 1 5`, `/proc/meminfo`, `/proc/self/status`, `findmnt -T /`, `df -hT` and `ls -ld`, as needed.
- Actual observations from your Ubuntu/WSL environment, not sample values from another machine.

## 8. Result

The report evaluates Linux architecture and kernel functions; CFS, page cache, swap, `/proc` and `/sys`; the directory hierarchy, VFS and permissions; and the memory, disk, automation and licence implications of the Windows Server alternative. No C program is required for A1.
