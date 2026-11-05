# ASSIGNMENT 5 – PART A: DESIGN
## OPERATING SYSTEMS (CO5)
### Linux, Containers and Virtualisation for Efficient Operations

## Student Details

| **Field** | **Details** |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 5 – CO5 |
| **Part** | Part A – Design |
| **Total Marks** | 25 |
| **Bloom's Level** | K5 – Evaluate |

---

## 1. Overview

Part A focuses on designing and evaluating Linux system components, system administration, virtualisation, containerisation and cloud deployment.

The case study involves a university result portal implemented as a Python web application connected to a database.

The portal serves a few hundred users on ordinary days, but 150,000 students log in within a two-hour period on result day. The existing deployment uses one physical Linux server and crashes during peak demand.

The university has a small private server room and access to a public cloud provider.

The objective is to evaluate suitable technologies and develop an efficient design for handling varying workloads, resource utilisation, service availability and operational costs.

## 2. Marks Distribution

| **Task** | **Title** | **Marks** |
|---|---|---:|
| A1 | Evaluate the Linux System Components | 7 |
| A2 | Design the System Administration | 5 |
| A3 | Evaluate Virtualisation | 5 |
| A4 | Evaluate Containerisation | 4 |
| A5 | Design the Cloud Deployment and Orchestration | 4 |
| | **TOTAL** | **25** |

---

# 3. A1 – Evaluate the Linux System Components (7 Marks)

## A1.1 – Linux Architecture (2 Marks)

Explain:
- Linux kernel.
- System libraries.
- Shell and utilities.
- How the kernel manages processes, memory, files, devices and networks.

## A1.2 – Process and Memory Management in Linux (3 Marks)

Evaluate:
- Completely Fair Scheduler (CFS).
- Page cache and swapping.
- `/proc` and `/sys` filesystems as tools for system observation.

## A1.3 – Linux File System and Permissions (2 Marks)

Explain:
- Linux directory hierarchy.
- Virtual File System (VFS).
- Owner, group and others permission model.
- Setuid and `sudo`.

### Alternative Concept

Evaluate Windows Server instead of Linux.

Compare:
- Base-system memory usage.
- Disk usage.
- Time required to automate administration tasks.
- Licence cost per server.

### Expected Deliverables

- Linux architecture explanation.
- Process and memory management evaluation.
- Linux filesystem and permissions explanation.
- Windows Server comparison.
- Reasoned conclusion.

---

# 4. A2 – Design the System Administration (5 Marks)

## A2.1 – Users, Services and Logs (2 Marks)

Design:
- User accounts and groups.
- `systemd` services.
- Log monitoring for the portal server.

## A2.2 – Networking Services (2 Marks)

Design:
- IP addressing.
- DNS configuration.
- Nginx reverse proxy.
- Firewall rules using `ufw` or `iptables`.
- SSH access using keys.

## A2.3 – Automation (1 Mark)

Identify routine administration tasks that can be automated using shell scripts and cron.

Examples:
- Backups.
- Log rotation.
- Disk-space alerts.
- Routine system monitoring.

### Alternative Concept

Evaluate Ansible/configuration management instead of hand-written shell scripts.

Compare:
- Time to configure one server and twenty servers.
- Risk of configuration differences between servers.
- Learning effort.
- Maintenance effort.

### Expected Deliverables

- User and group management design.
- Service and log management design.
- Network and firewall design.
- SSH security design.
- Automation plan.
- Ansible comparison.

---

# 5. A3 – Evaluate Virtualisation (5 Marks)

## A3.1 – Hypervisors (3 Marks)

Explain:

**Type-1 Hypervisors**
- KVM.
- VMware ESXi.
- Xen.
- Hyper-V.

**Type-2 Hypervisors**
- VirtualBox.
- VMware Workstation.

Also explain:
- Full virtualisation.
- Para-virtualisation.
- Intel VT-x.
- AMD-V.

## A3.2 – Benefits and Costs (2 Marks)

Evaluate:
- Isolation.
- Consolidation.
- Snapshots.
- Live migration.
- CPU overhead.
- Memory overhead.
- Memory requirements per VM.
- Licence costs.

### Alternative Concept

Evaluate Type-2 VirtualBox instead of Type-1 KVM.

Compare:
- CPU-bound workload overhead.
- I/O-bound workload overhead.
- Host operating-system memory usage.
- Production suitability.

### Expected Deliverables

- Hypervisor classification.
- Virtualisation concepts.
- Hardware virtualisation support.
- Benefits and cost evaluation.
- VirtualBox versus KVM comparison.
- Final recommendation.

---

# 6. A4 – Evaluate Containerisation (4 Marks)

## A4.1 – How Containers Work (2 Marks)

Explain:
- Linux namespaces.
- Control groups (cgroups).
- Container image layers.

## A4.2 – Containers versus Virtual Machines (2 Marks)

Compare:
- Start-up time.
- Memory usage.
- Isolation.
- Security.

Decide where containers and virtual machines should be used for the university result portal.

### Alternative Concept

Evaluate running the portal and database in separate VMs instead of containers.

Compare:
- Start-up time (minutes versus seconds).
- Memory per instance (full guest OS versus shared kernel).
- Isolation strength.

### Expected Deliverables

- Namespace explanation.
- Cgroup explanation.
- Container image-layer explanation.
- Containers versus VM comparison.
- Separate-VM alternative analysis.
- Final recommendation.

---

# 7. A5 – Design the Cloud Deployment and Orchestration (4 Marks)

## A5.1 – Cloud Set-up (2 Marks)

Compare:
- IaaS.
- PaaS.
- SaaS.

Design a result-day cloud setup using:
- Virtual machines or managed Kubernetes.
- Load balancer.
- Managed database.

## A5.2 – Kubernetes (2 Marks)

Explain:
- Pods.
- Deployments.
- Services.
- Replicas.
- Resource requests and limits.
- Horizontal Pod Autoscaler (HPA).
- Scaling and failure recovery.

### Alternative Concept

Evaluate fixed virtual machines sized for result day instead of Kubernetes autoscaling.

Compare:
- Result-day response time.
- Resources idle on the other 364 days.
- Cost.

### Expected Deliverables

- Cloud service-model comparison.
- Proposed cloud architecture.
- Load balancer and managed database design.
- Kubernetes object explanation.
- Autoscaling and failure-recovery analysis.
- Fixed VM versus Kubernetes comparison.
- Final cloud deployment recommendation.

---

# 8. Suggested Part A Folder Structure

```text
Part_A/
│
├── README.md
│
├── A1_Linux_System_Components/
│   ├── Report.pdf
│   ├── diagrams/
│   └── supporting_files/
│
├── A2_System_Administration/
│   ├── Report.pdf
│   ├── network_design/
│   └── automation_plan/
│
├── A3_Virtualisation/
│   ├── Report.pdf
│   ├── architecture_diagrams/
│   └── comparison_tables/
│
├── A4_Containerisation/
│   ├── Report.pdf
│   ├── architecture_diagrams/
│   └── comparison_tables/
│
└── A5_Cloud_Orchestration/
    ├── Report.pdf
    ├── architecture_diagrams/
    └── comparison_tables/
```

---

# 9. General Submission Guidelines

1. Answer every question and its subdivisions.
2. Follow the marks distribution and evaluation requirements.
3. Include technical explanations, comparisons and justified decisions.
4. Include diagrams and tables wherever they improve clarity.
5. Address the alternative concept specified in each task.
6. Use appropriate technical terminology.
7. Do not provide unsupported benchmark measurements or cost figures.
8. Clearly identify assumptions wherever exact input data is unavailable.
9. Ensure each report addresses all the requirements of its respective task.
10. Maintain consistency with the university result-portal case study.

---

# 10. Final Outcome

Part A establishes the theoretical and architectural foundation for Assignment 5.

It evaluates Linux internals, system administration, virtualisation, containerisation and cloud orchestration to develop a suitable infrastructure design for the university result portal.

**END OF PART A**
