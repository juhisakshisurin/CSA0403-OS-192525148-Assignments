# Assignment 5 – Part A – A4: Evaluate Containerisation

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 5 – CO5 |
| **Part** | Part A – Design |
| **Task** | A4 – Evaluate Containerisation |
| **Marks / Bloom's Level** | 4 Marks / K5 – Evaluate |

---

## 1. Task Requirements

| Section | Marks | Requirement |
|---|---:|---|
| A4.1 – How Containers Work | 2 | Explain Linux namespaces, control groups (cgroups), and how a container image is built in layers. |
| A4.2 – Containers versus Virtual Machines | 2 | Compare startup time, memory use, isolation and security, and decide where each should be used for the result portal. |
| Alternative Concept | Included | Evaluate the result if the portal and database each run in their own VM rather than in containers, comparing start-up time, memory and isolation strength. |

The case study is the university result portal: a Python web application and database used by 150,000 students during the two-hour result-day peak. The existing deployment runs on one physical Linux server and crashes during the peak.

## 2. Files Included

| File | Purpose |
|---|---|
| `Assignment5_PartA_A4_Report.pdf` | Complete A4 report without student name or register number. |
| `README.md` | Student details in the requested table, task summary and observation instructions. |
| `container_observation_commands.txt` | Optional Docker/Linux commands to inspect namespaces, cgroups, image layers, running containers and resource use. |

## 3. Is C Code Required?

A4 is a design/evaluation task. The question does **not** contain a C “Complete the Code” program and does not require a C implementation. Therefore:
- C source code: not required for A4.
- GCC compilation command: not applicable.
- The optional evidence commands use Docker and Linux command-line tools; no C compiler is needed.

## 4. Main Concepts

- **Namespaces** provide process-view isolation. Relevant Linux namespaces include PID, network, mount, UTS, IPC, user and cgroup namespaces.
- **Cgroups** account for and control resource use, including CPU, memory, I/O and process count, subject to available controllers and kernel configuration.
- **Image layers** are immutable filesystem changes stacked to form a container image. Containers add a per-container writable layer. Use volumes for data that must survive container replacement/removal.
- **Containers** share the host kernel. They start quickly and use less per-instance memory, but share a kernel security boundary.
- **Virtual machines** run a complete guest OS with its own kernel. They generally start more slowly and consume more memory, but offer a stronger isolation boundary.

## 5. Recommended Result-Portal Placement

The report recommends a hybrid design:

1. Run the stateless Python portal/web-worker tier as multiple containers, with CPU and memory limits. Multiple replicas can be started quickly and distributed across hosts.
2. Run the database in a managed database service or a dedicated VM with persistent, backed-up storage and controlled access. If it is containerised, use persistent volumes, least privilege, replication and tested recovery.
3. Keep container images immutable and store database files, uploads and other persistent state outside the disposable container writable layer.
4. Do not treat namespaces and cgroups as a substitute for security hardening. Run non-root, drop unnecessary capabilities, use seccomp/AppArmor where available, avoid privileged mode and restrict daemon access.

## 6. Optional Evidence Commands

The commands in `container_observation_commands.txt` can be used on a Linux machine with Docker installed.

Check versions:

```bash
cat /etc/os-release
uname -r
docker --version
docker version
```

Inspect an existing container:

```bash
docker ps
docker inspect CONTAINER_NAME
docker stats --no-stream CONTAINER_NAME
docker exec CONTAINER_NAME sh -c 'readlink /proc/self/ns/pid; cat /proc/self/cgroup'
```

Inspect an image:

```bash
docker history IMAGE_NAME
docker image inspect IMAGE_NAME
```

Replace the placeholders with names shown by `docker ps` and `docker images`. The commands are observational; `docker inspect`, `docker stats` and `docker history` do not change the container or image.

### Environment information to record

| Item | Command |
|---|---|
| Linux distribution | `cat /etc/os-release` |
| Kernel version | `uname -r` |
| Docker CLI | `docker --version` |
| Docker Engine / Server | `docker version` |
| Running containers | `docker ps` |

If Docker is not installed or running in the WSL environment, record that as an environment limitation rather than fabricating results.

## 7. Result

The report explains namespaces, cgroups and image layers, compares containers and VMs on start-up, memory, isolation and security, and evaluates the alternative of running the portal and database in separate VMs. It recommends containers for stateless application replicas and a managed database or dedicated VM for the persistent database tier.
