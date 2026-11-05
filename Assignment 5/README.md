# ASSIGNMENT 5 – OPERATING SYSTEMS (CO5)
## Linux, Containers and Virtualisation for Efficient Operations

## Student Details

| **Field** | **Details** |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 5 – CO5 |
| **Nature** | Individual |
| **Duration** | One Week |
| **Total Marks** | 50 |
| **Bloom's Level** | K5 – Evaluate |

---

## 1. Assignment Overview

This assignment evaluates how Linux operating systems, virtual machines, containers and cloud orchestration can be used to deploy services efficiently, support varying workloads, recover from failures and manage operational costs.

The case study is the university result portal. The portal is a Python web application connected to a database. It serves a few hundred users on ordinary days, but 150,000 students log in within a two-hour period on result day. The existing deployment uses one physical Linux server and fails during peak demand. The university has a small private server room and access to a public cloud provider.

The expected engineering workflow is:

**UNDERSTAND → DESIGN → DEPLOY → LOAD → MEASURE → INTERPRET → DECIDE**

Simply making a service run is not sufficient. Design reasoning, implementation evidence, measurements, comparisons and interpretation are also assessed.

## 2. Marks Distribution

| **Component** | **Task** | **Marks** | **Bloom's Level** |
|---|---|---:|---|
| Part A – Design | A1 – Evaluate the Linux System Components | 7 | K5 – Evaluate |
| Part A – Design | A2 – Design the System Administration | 5 | K5 – Evaluate |
| Part A – Design | A3 – Evaluate Virtualisation | 5 | K5 – Evaluate |
| Part A – Design | A4 – Evaluate Containerisation | 4 | K5 – Evaluate |
| Part A – Design | A5 – Design the Cloud Deployment and Orchestration | 4 | K5 – Evaluate |
|  | **Part A Total** | **25** | |
| Part B – Implementation & Experimental Investigation | B1 – Administer the Linux Server | 7 | K5 – Evaluate |
| Part B – Implementation & Experimental Investigation | B2 – Containerise the Portal | 5 | K5 – Evaluate |
| Part B – Implementation & Experimental Investigation | B3 – Compare Bare Metal, Virtual Machine and Container | 8 | K5 – Evaluate |
| Part B – Implementation & Experimental Investigation | B4 – Final Engineering Decision | 5 | K5 – Evaluate |
|  | **Part B Total** | **25** | |
|  | **OVERALL TOTAL** | **50** | |

---

# PART A – DESIGN (25 MARKS)

## 3. A1 – Evaluate the Linux System Components (7 Marks)

### A1.1 – Linux Architecture (2 Marks)

Explain the Linux kernel, system libraries, shell and utilities, and how the kernel manages processes, memory, files, devices and the network.

### A1.2 – Process and Memory Management in Linux (3 Marks)

Evaluate:
- Completely Fair Scheduler (CFS).
- Page cache and swapping.
- `/proc` and `/sys` filesystems as tools for observing the system.

### A1.3 – Linux File System and Permissions (2 Marks)

Explain:
- Linux directory hierarchy and Virtual File System (VFS).
- Owner, group and others permission model.
- Setuid and `sudo`.

**Alternative concept:** Evaluate Windows Server instead of Linux, comparing base-system memory and disk use, administration automation time, and licence cost per server.

**Expected evidence:** Correct description of Linux components and reasoned evaluation of scheduling, memory and file management.

---

## 4. A2 – Design the System Administration (5 Marks)

### A2.1 – Users, Services and Logs (2 Marks)

Design user accounts and groups, `systemd` services and log monitoring for the portal server.

### A2.2 – Networking Services (2 Marks)

Design IP addressing, DNS, Nginx reverse proxy, firewall rules using `ufw` or `iptables`, and SSH access using keys.

### A2.3 – Automation (1 Mark)

Identify routine tasks to automate with shell scripts and cron, including backups, log rotation and disk-space alerts.

**Alternative concept:** Compare hand-written shell scripts with Ansible/configuration management. Evaluate setup time for one server and twenty servers, risk of configuration differences, and learning/maintenance effort.

**Expected evidence:** Complete administration and networking design with an automation plan.

---

## 5. A3 – Evaluate Virtualisation (5 Marks)

### A3.1 – Hypervisors (3 Marks)

Explain:
- Type-1 hypervisors: KVM, VMware ESXi, Xen and Hyper-V.
- Type-2 hypervisors: VirtualBox and VMware Workstation.
- Full virtualisation and para-virtualisation.
- Hardware support: Intel VT-x and AMD-V.

### A3.2 – Benefits and Costs (2 Marks)

Evaluate virtual-machine isolation, consolidation, snapshots, live migration, overhead, memory per VM and licence costs.

**Alternative concept:** Evaluate Type-2 VirtualBox instead of Type-1 KVM. Compare CPU-bound and I/O-bound overhead, host operating-system memory usage and production suitability.

**Expected evidence:** Correct explanation of virtualisation and reasoned evaluation of its benefits and costs for the portal.

---

## 6. A4 – Evaluate Containerisation (4 Marks)

### A4.1 – How Containers Work (2 Marks)

Explain Linux namespaces, control groups (cgroups) and how a container image is built in layers.

### A4.2 – Containers versus Virtual Machines (2 Marks)

Compare start-up time, memory use, isolation and security; decide where each should be used for the portal.

**Alternative concept:** Evaluate running the portal and database in separate VMs instead of containers. Compare start-up time (minutes versus seconds), memory per instance (full guest OS versus shared kernel) and isolation strength.

**Expected evidence:** Correct explanation of namespaces, cgroups and image layers, with a justified comparison.

---

## 7. A5 – Design the Cloud Deployment and Orchestration (4 Marks)

### A5.1 – Cloud Set-up (2 Marks)

Compare IaaS, PaaS and SaaS. Design a result-day cloud setup using virtual machines or managed Kubernetes, a load balancer and a managed database.

### A5.2 – Kubernetes (2 Marks)

Explain Pods, Deployments, Services, replicas, resource requests and limits, Horizontal Pod Autoscaler (HPA), and how Kubernetes supports scaling and recovery.

**Alternative concept:** Compare fixed VMs sized for result day with Kubernetes autoscaling. Evaluate result-day response time, resources idle on the other 364 days and cost.

**Expected evidence:** Justified cloud design and correct explanation of Kubernetes scaling and recovery.

---

# PART B – IMPLEMENTATION & EXPERIMENTAL INVESTIGATION (25 MARKS)

## 8. B1 – Administer the Linux Server (7 Marks)

### B1.1 – Observe the System (2 Marks)

Use `ps`, `top` or `htop`, `free`, `vmstat`, `iostat`, `df` and `ss` to record process, memory, disk and network use, and explain the values.

### B1.2 – Automate (2 Marks)

Write shell scripts for log analysis and disk-space alerts, schedule them with cron and show their output. Complete the supplied access-log script's two TODOs using `awk`, `sort` and `uniq`.

### B1.3 – Services and Network (2 Marks)

Run the portal or a sample web application as a `systemd` service behind Nginx. Configure firewall rules to allow HTTP, HTTPS and SSH.

### B1.4 – Compare with Your Design (1 Mark)

Compare the implementation with the Part A design and explain any changes.

**Alternative concept:** Compare local shell-script log analysis with central logging such as journald plus a log server or ELK. Evaluate time to find an error across ten servers, extra storage and memory, and setup effort.

**Expected evidence:** Command outputs, working scripts, running service and comparison with the design.

---

## 9. B2 – Containerise the Portal (5 Marks)

Required work:

1. Write a Dockerfile, build the image and run it with `docker run`, mapping the port.
2. Run the portal and database with Docker Compose, using a persistent database volume.
3. Limit CPU and memory with `--cpus` and `--memory`, and show the effect using `docker stats`.
4. Inspect image layers using `docker history`.
5. Reduce image size using a slim base image or multi-stage build.
6. Explain what each step demonstrates about containers.

### Complete the Code

Complete the three Dockerfile TODOs:

- Set the working directory to `/app`.
- Copy the application source and run it as non-root user `portal`.
- Expose port 8000 and start Gunicorn on `0.0.0.0:8000` using the `app` object in `app.py`.

Build and run:

```bash
docker build -t result-portal .
docker run -p 8000:8000 result-portal
```

Verify that:

- `docker ps` shows the container running.
- `http://localhost:8000` displays the portal.
- `docker images` shows the image size.
- `docker exec <container> whoami` returns `portal`.

**Alternative concept:** Compare a full Ubuntu base image with `python:3.12-slim` or a multi-stage build. Evaluate image size, build time, pull time to a new server and packages that may contain security problems.

**Expected evidence:** Working container setup, resource-limit evidence, optimised image and reasoned explanation.

---

## 10. B3 – Compare Bare Metal, Virtual Machine and Container (8 Marks)

### B3.1 – Deploy Three Ways (2 Marks)

Run the same web application directly on Linux, inside a VirtualBox/KVM virtual machine and inside a Docker container.

### B3.2 – Measure (2 Marks)

Measure start-up time, memory use, image or disk size and requests per second using `ab`, `wrk` or `hey`.

### B3.3 – Scale with Kubernetes (2 Marks)

- Deploy the container to minikube or kind with three replicas.
- Delete one Pod and show that it is recreated.
- Scale to six replicas under load.
- Show the resource requests and limits.

Complete the two TODOs in `deployment.yaml`:

- Set three replicas and a selector for Pods labelled `app: result-portal`.
- Set CPU request to 250m and memory request to 256Mi.
- Set CPU limit to 500m and memory limit to 512Mi.

Apply using:

```bash
kubectl apply -f deployment.yaml
```

### B3.4 – Engineering Trade-Off (2 Marks)

Explain the results and state when each deployment method should be used.

**Alternative concept:** Compare Docker Compose scaling to six containers on one server with Kubernetes on several nodes. Evaluate recovery time after container/server failure, resource requirements and maximum load.

**Expected evidence:** Three working deployments, measured comparison table, Kubernetes scaling/recovery evidence and reasoned trade-off.

---

## 11. B4 – Final Engineering Decision (5 Marks)

The university will also host its learning-management system and online examinations on the same infrastructure and wants result day to be handled without buying new servers.

### B4.1 – Implement a Practical Extension (2 Marks)

Add a second service to the Kubernetes or Docker Compose setup and show that both services run with their own resource limits.

### B4.2 – Estimate the Capacity (1 Mark)

Use measured requests per second from B3 to estimate the number of containers or VMs needed for 150,000 students in two hours.

The average login arrival rate from the case study is:

```text
150,000 / 7,200 seconds = 20.83 logins per second
```

For a defensible estimate, state and justify requests per login, burst factor, capacity headroom and measured sustainable requests per second per container or VM.

### B4.3 – Production Decision (2 Marks)

Choose and justify one of the following:

- Own servers with virtual machines.
- Own servers with containers and Kubernetes.
- Public cloud.
- Hybrid deployment.

Justify the decision using measurements and cost estimates.

**Alternative concept:** Evaluate a public-cloud-only solution with hourly billing. Estimate annual cost with one result day per semester, time to add capacity and who controls student data.

**Expected evidence:** Working second service, capacity estimate and evidence-based production decision.

---

## 12. Suggested Folder Organisation

Keep each task's report, source code, scripts, configuration files, inputs, outputs and evidence together.

```text
Assignment5/
│
├── README.md
│
├── Part_A/
│   ├── A1_Linux_System_Components/
│   ├── A2_System_Administration/
│   ├── A3_Virtualisation/
│   ├── A4_Containerisation/
│   └── A5_Cloud_Orchestration/
│
└── Part_B/
    ├── B1_Linux_Administration/
    │   ├── scripts/
    │   ├── systemd/
    │   ├── nginx/
    │   └── evidence/
    ├── B2_Containerisation/
    │   ├── Dockerfile
    │   ├── app.py
    │   ├── requirements.txt
    │   ├── compose.yaml
    │   └── evidence/
    ├── B3_Deployment_Comparison/
    │   ├── app.py
    │   ├── Dockerfile
    │   ├── kubernetes/
    │   ├── scripts/
    │   └── evidence/
    └── B4_Final_Engineering_Decision/
        ├── portal/
        ├── lms_exam/
        ├── kubernetes/
        ├── scripts/
        └── evidence/
```

---

## 13. Software and Environment Details

Record the actual environment used. Do not enter version numbers without checking them.

| **Environment Item** | **Version / Details Used** |
|---|---|
| Linux distribution | ______________________________ |
| Linux distribution version | ______________________________ |
| Linux kernel | ______________________________ |
| Python | ______________________________ |
| Docker Engine | ______________________________ |
| Docker Compose | ______________________________ |
| Kubernetes | ______________________________ |
| Kubernetes platform (kind/minikube) | ______________________________ |
| Virtualisation platform (VirtualBox/KVM) | ______________________________ |
| ApacheBench / wrk / hey | ______________________________ |
| GCC, if installed | ______________________________ |

Useful commands:

```bash
cat /etc/os-release
uname -a
python3 --version
docker --version
docker compose version
kubectl version
kind version
minikube version
gcc --version
ab -V
```

**Compiler note:** The task-specific questions in Assignment 5 ask for Bash scripts, Dockerfiles, Python application code and Kubernetes YAML. They do not contain a C “Complete the Code” program. A GCC compilation command is therefore not applicable to these implementations. Record the installed GCC version if required by the general environment details.

---

## 14. Evidence and Execution Checklist

### Part A – Design

- [ ] A1: Linux architecture, process/memory management, file system and Windows Server alternative.
- [ ] A2: User/service/log design, network design, automation and Ansible alternative.
- [ ] A3: Type-1/Type-2 hypervisors, hardware support, VM benefits/costs and VirtualBox alternative.
- [ ] A4: Namespaces, cgroups, image layers, container/VM comparison and separate-VM alternative.
- [ ] A5: Cloud service models, cloud architecture, Kubernetes objects, scaling and fixed-VM alternative.

### Part B – Implementation

- [ ] B1: Actual Linux command outputs and explanations.
- [ ] B1: Completed Bash TODOs, automation scripts and cron output.
- [ ] B1: systemd, Nginx and firewall evidence.
- [ ] B2: Docker image build and running-container evidence.
- [ ] B2: Docker Compose database-volume and persistence evidence.
- [ ] B2: CPU/memory limits, `docker stats`, image history and size comparison.
- [ ] B3: Direct Linux, VM and Docker deployments.
- [ ] B3: Start-up time, memory, disk/image size and requests-per-second measurements.
- [ ] B3: Three Kubernetes replicas, Pod deletion/recreation and six-replica load test.
- [ ] B4: Second service and separate resource limits.
- [ ] B4: Capacity estimate based on measured B3 throughput.
- [ ] B4: Annual cloud-cost calculation, capacity-addition time and student-data control decision.

---

## 15. Important Measurement and Reporting Guidelines

1. Record commands and their actual outputs. Screenshots alone are insufficient.
2. Do not fabricate CPU, memory, disk, image-size, start-up-time, throughput, latency, cloud-cost or vulnerability-scan results.
3. Keep the application and benchmark conditions consistent when comparing Linux, VM and Docker.
4. Repeat performance measurements and state the statistic used, such as the median of three runs.
5. Distinguish configured resource limits from observed resource consumption.
6. Identify assumptions such as requests per login, burst factor, headroom and peak duration.
7. State environment limitations. For example, a process running inside WSL2 is not a native bare-metal Linux deployment.
8. Explain differences between the proposed design and the implementation.
9. Keep the result portal and LMS/exam service separately identified and resource-limited in B4.
10. Ensure that each alternative-concept discussion contains a reasoned comparison, not only definitions.

---

## 16. Final Outcome

The assignment should demonstrate a complete engineering progression: understanding Linux internals, designing system administration, evaluating virtualisation and containerisation, planning cloud orchestration, implementing and measuring deployments, testing failure recovery, and making a justified production decision.

The final engineering decision must be supported by actual measurements and cost estimates collected during implementation.
