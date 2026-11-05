# Assignment 5 – Part A – A3: Evaluate Virtualisation

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 5 – CO5 |
| **Part** | Part A – Design |
| **Task** | A3 – Evaluate Virtualisation |
| **Marks / Bloom's Level** | 5 Marks / K5 – Evaluate |

---

## 1. Task Requirements

A3 has two assessed parts and an alternative-concept evaluation:

| Section | Marks | Requirement |
|---|---:|---|
| A3.1 – Hypervisors | 3 | Explain Type-1 and Type-2 hypervisors, full and para-virtualisation, and Intel VT-x / AMD-V hardware support. |
| A3.2 – Benefits and Costs | 2 | Evaluate VM isolation, consolidation, snapshots, live migration, overhead, per-VM memory and licences. |
| Alternative Concept | Included | Compare Type-2 VirtualBox with Type-1 KVM for CPU-bound work, I/O-bound work, host OS memory and production suitability. |

The report uses the university result-portal case study: 150,000 students log in within two hours on result day, while the current portal runs on one physical Linux server that crashes at peak.

## 2. Files Included

| File | Purpose |
|---|---|
| `Assignment5_PartA_A3_Report.pdf` | Full A3 report without student identity details. |
| `README.md` | Student details, task summary and host-check instructions. |
| `virtualization_environment_check.sh` | Optional read-only script to inspect CPU virtualization flags, KVM visibility, installed tools, memory and storage. |
| `virtualization_measurement_plan.txt` | Optional fair-test procedure and worksheet for comparing CPU, I/O, memory and startup time under KVM and VirtualBox. |

## 3. Is C Code Required?

A3 is a design/evaluation task. The question does **not** contain a C “Complete the Code” program and does not require a C implementation. Therefore, GCC compilation is not applicable.

The included Bash script is an optional environment-observation tool; it does not create virtual machines or change host settings.

## 4. Run the Optional Host Check

Run on a Linux host or Ubuntu VM (not necessarily inside the guest being evaluated):

```bash
chmod +x virtualization_environment_check.sh
./virtualization_environment_check.sh
```

The script creates:

```text
a3_virtualization_host_check.txt
```

To use a custom output name:

```bash
./virtualization_environment_check.sh my_a3_host_check.txt
```

It records:
- Linux distribution and kernel.
- CPU model, core/thread details and visible `vmx` or `svm` flags.
- `/dev/kvm` visibility and permissions.
- Loaded KVM modules where `lsmod` is available.
- Virtualisation detection and `virt-host-validate` output if installed.
- QEMU, libvirt and VirtualBox command-line versions if installed.
- Host memory, load, block storage and filesystem information.

The script is read-only. It does not install software, load kernel modules, enable firmware settings, create VMs or alter system configuration.

### WSL note

A WSL environment may not expose `/dev/kvm` or all host CPU virtualization features. The visible flags and permissions depend on Windows, WSL and the outer hypervisor. A missing `/dev/kvm` inside WSL does not prove that the physical CPU lacks VT-x/AMD-V.

## 5. Main A3 Evaluation Points

- Type-1 hypervisors run directly on hardware; the assignment lists KVM, VMware ESXi, Xen and Hyper-V.
- Type-2 hypervisors run as applications on a host operating system; the assignment lists VirtualBox and VMware Workstation.
- Full virtualisation presents virtual hardware so an unmodified guest OS can run. Para-virtualisation makes the guest aware of the hypervisor and uses hypercalls or optimised virtual I/O interfaces.
- Intel VT-x and AMD-V are processor hardware-virtualisation extensions. They must be supported by the CPU and enabled in firmware; a visible CPU flag alone does not prove that the current environment can use them.
- VMs provide isolation, consolidation, snapshots and live migration, but consume guest RAM, host RAM, storage, CPU and network capacity and may incur licensing and management costs.
- A snapshot is a point-in-time rollback mechanism, not a substitute for an independent backup.
- Live migration depends on compatible destination hosts, shared/accessible storage and suitable network/hypervisor configuration.
- Type-2 VirtualBox is useful for development, demonstrations and small labs. For a high-demand production portal, the report recommends a managed Type-1 KVM cluster because the host stack can be kept lean and operational controls can be centralised.

## 6. Measurement Guidance

The assignment does not provide host CPU, RAM, SSD/HDD, VM allocation or benchmark results. The report therefore makes a reasoned qualitative comparison and uses formulas for memory budgeting rather than claiming a benchmark that was not run.

To produce actual measurements, use `virtualization_measurement_plan.txt`. It specifies:
- The same guest image, vCPU count and memory allocation for both hypervisors.
- A CPU test using `sysbench`.
- A random-read I/O test using `fio` on a disposable virtual disk.
- Host and guest RAM observations.
- VM cold-start time.
- Repeated trials and recording of actual results.

Do not write illustrative memory/overhead values as measured results. Do not run the `fio` test against a raw disk or a volume containing important data.

## 7. Result

The report explains hypervisor types, full and para-virtualisation, hardware support, the benefits and costs of VMs, and the Type-2 VirtualBox alternative. It recommends Type-1 KVM for the production result-portal infrastructure, with Type-2 VirtualBox reserved for development and test environments.
