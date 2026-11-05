# ASSIGNMENT 5 – PART B
## IMPLEMENTATION AND EXPERIMENTAL INVESTIGATION
### OPERATING SYSTEMS (CO5)

## Student Details

| **Field** | **Details** |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 5 – CO5 |
| **Part** | Part B – Implementation and Experimental Investigation |
| **Total Marks** | 25 |
| **Bloom's Level** | K5 – Evaluate |

---

## 1. Overview

Part B focuses on implementing and experimentally investigating the technologies and designs developed in Part A.

The implementation uses Linux administration tools, shell scripts, systemd, Nginx, Docker, Docker Compose, virtual machines and Kubernetes.

The practical work evaluates:
- Linux system performance.
- Automated system administration.
- Container deployment.
- Resource usage.
- Start-up time.
- Memory and storage utilisation.
- Requests per second.
- Kubernetes scaling and recovery.
- Production deployment decisions.

All measurements must be collected from the actual execution environment.

## 2. Marks Distribution

| **Task** | **Title** | **Marks** |
|---|---|---:|
| B1 | Administer the Linux Server | 7 |
| B2 | Containerise the Portal | 5 |
| B3 | Compare Bare Metal, Virtual Machine and Container | 8 |
| B4 | Final Engineering Decision | 5 |
| | **TOTAL** | **25** |

---

# 3. B1 – Administer the Linux Server (7 Marks)

## B1.1 – Observe the System (2 Marks)

Use the following Linux commands:

```bash
ps
top
htop
free
vmstat
iostat
df
ss
```

Record and explain:
- Process information.
- CPU utilisation.
- Memory utilisation.
- Swap activity.
- Disk utilisation.
- Network sockets.

## B1.2 – Automate (2 Marks)

Implement:
- Web-log analysis shell script.
- Disk-space alert shell script.
- Cron scheduling.
- Script output verification.

Complete the supplied access-log script's two TODOs using:
- `awk`
- `sort`
- `uniq`

## B1.3 – Services and Network (2 Marks)

Implement:
- A sample portal application as a `systemd` service.
- Nginx reverse proxy.
- Firewall rules allowing HTTP, HTTPS and SSH.
- SSH access using keys.

## B1.4 – Compare with Your Design (1 Mark)

Compare the implementation with the Part A administration design and explain any changes.

### Alternative Concept

Compare local shell-script log analysis with central logging such as journald plus a log server or ELK stack.

Evaluate:
- Time to locate an error across ten servers.
- Additional storage and memory requirements.
- Setup effort.

### Expected Deliverables

- System observation output.
- Completed Bash TODO script.
- Log-analysis script.
- Disk-space alert script.
- Cron configuration.
- systemd service file.
- Nginx configuration.
- Firewall configuration evidence.
- Part A design comparison.

---

# 4. B2 – Containerise the Portal (5 Marks)

## Required Implementation

1. Write a Dockerfile for the portal.
2. Build the Docker image.
3. Run the container using `docker run` with port mapping.
4. Run the portal and database using Docker Compose.
5. Use a persistent volume for database data.
6. Apply CPU and memory limits using `--cpus` and `--memory`.
7. Demonstrate resource usage using `docker stats`.
8. Inspect image layers using `docker history`.
9. Optimise image size using a slim base image or multi-stage build.
10. Explain what each step demonstrates about containers.

## Complete the Code – Dockerfile

Complete the three supplied Dockerfile TODOs:

- TODO 1: Set the working directory to `/app`.
- TODO 2: Copy the application source and run it as a non-root user named `portal`.
- TODO 3: Expose port 8000 and start Gunicorn on `0.0.0.0:8000` using the `app` object in `app.py`.

Build and run:

```bash
docker build -t result-portal .
docker run -p 8000:8000 result-portal
```

Verify:

```bash
docker ps
docker images
docker exec <container> whoami
```

The application should be accessible at:

```text
http://localhost:8000
```

The expected `whoami` output is:

```text
portal
```

### Alternative Concept

Compare a full Ubuntu base image with `python:3.12-slim` or a multi-stage build.

Evaluate:
- Image size.
- Build time.
- Pull time on a new server.
- Installed package count.
- Potential security exposure.

### Expected Deliverables

- Dockerfile.
- Flask application.
- Requirements file.
- Docker Compose configuration.
- Database volume configuration.
- Docker build and run evidence.
- CPU and memory limit evidence.
- `docker stats` output.
- `docker history` output.
- Image optimisation comparison.

---

# 5. B3 – Compare Bare Metal, Virtual Machine and Container (8 Marks)

## B3.1 – Deploy Three Ways (2 Marks)

Run the same web application:

1. Directly on Linux.
2. Inside a VirtualBox or KVM virtual machine.
3. Inside a Docker container.

## B3.2 – Measure (2 Marks)

Measure all three deployment methods:

- Start-up time.
- Memory used.
- Image or disk size.
- Requests per second using `ab`, `wrk` or `hey`.

Keep the application and benchmark conditions consistent.

## B3.3 – Scale with Kubernetes (2 Marks)

- Deploy the container to minikube or kind with three replicas.
- Delete one Pod and demonstrate automatic recreation.
- Scale to six replicas under load.
- Show the resource requests and limits.

### Complete the Code – deployment.yaml

**TODO 1**
- Configure three replicas.
- Configure matching selectors for Pods labelled `app: result-portal`.

**TODO 2**
- CPU request: 250m.
- Memory request: 256Mi.
- CPU limit: 500m.
- Memory limit: 512Mi.

Apply:

```bash
kubectl apply -f deployment.yaml
```

Check the Pods:

```bash
kubectl get pods
```

Delete one Pod to demonstrate recovery:

```bash
kubectl delete pod POD_NAME
kubectl get pods -w
```

Scale to six replicas:

```bash
kubectl scale deployment/result-portal --replicas=6
kubectl get pods -w
```

## B3.4 – Engineering Trade-Off (2 Marks)

Explain the measurements and state when each deployment approach should be used.

### Alternative Concept

Compare Kubernetes on several nodes with Docker Compose scaling to six containers on one server.

Evaluate:
- Recovery time after a container or server failure.
- Resources required.
- Maximum load each approach can handle.

### Expected Deliverables

- Direct Linux deployment.
- Virtual machine deployment.
- Docker deployment.
- Start-up time measurements.
- Memory measurements.
- Disk/image-size measurements.
- Requests-per-second results.
- Kubernetes three-replica deployment.
- Pod deletion and recreation evidence.
- Six-replica scaling evidence.
- Engineering trade-off analysis.

---

# 6. B4 – Final Engineering Decision (5 Marks)

The university will also host its learning-management system and online examinations on the same infrastructure and wants result day to be handled without purchasing new servers.

## B4.1 – Implement a Practical Extension (2 Marks)

- Add a second service to the Kubernetes or Docker Compose setup.
- Demonstrate both services running.
- Configure separate resource limits for both services.

## B4.2 – Estimate the Capacity (1 Mark)

Use measured requests per second from B3 to estimate the number of containers or virtual machines required for 150,000 students in two hours.

Average login arrival rate:

```text
150,000 / 7,200 seconds = 20.83 logins per second
```

For the capacity calculation, identify:
- Backend requests per login.
- Peak/burst factor.
- Capacity headroom.
- Measured sustainable requests per second per container or VM.

Calculate:

```text
Design RPS =
Average logins per second
× Requests per login
× Burst factor
× Headroom factor
```

```text
Required replicas =
CEILING(Design RPS / Measured RPS per container)
```

## B4.3 – Production Decision (2 Marks)

Choose and justify one of the following:

- Own servers with virtual machines.
- Own servers with containers and Kubernetes.
- Public cloud.
- Hybrid deployment.

Support the decision using actual measurements and cost estimates.

### Alternative Concept

Evaluate a public-cloud-only deployment with hourly billing.

Consider:
- Annual cost with one result day per semester.
- Time required to add capacity.
- Who controls the student data.

### Expected Deliverables

- Second application service.
- Separate resource limits.
- Working Kubernetes or Docker Compose deployment.
- Capacity-estimation calculations.
- Annual public-cloud cost model.
- Production recommendation.
- Student-data ownership and control explanation.

---

# 7. Suggested Part B Folder Structure

```text
Part_B/
│
├── README.md
│
├── B1_Linux_Administration/
│   ├── Report.pdf
│   ├── scripts/
│   ├── systemd/
│   ├── nginx/
│   └── evidence/
│
├── B2_Containerisation/
│   ├── Report.pdf
│   ├── Dockerfile
│   ├── app.py
│   ├── requirements.txt
│   ├── compose.yaml
│   └── evidence/
│
├── B3_Deployment_Comparison/
│   ├── Report.pdf
│   ├── app.py
│   ├── Dockerfile
│   ├── kubernetes/
│   ├── scripts/
│   └── evidence/
│
└── B4_Final_Engineering_Decision/
    ├── Report.pdf
    ├── portal/
    ├── lms_exam/
    ├── kubernetes/
    ├── scripts/
    └── evidence/
```

---

# 8. Software and Environment Details

Record the actual versions used during practical implementation.

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

**Compiler note:** The task-specific questions in Assignment 5 require Bash scripts, Dockerfiles, Python application code and Kubernetes YAML. They do not contain a C "Complete the Code" program. Therefore, a GCC compilation command is not applicable to these implementations. Record the installed GCC version if required by the general environment details.

---

# 9. Evidence and Execution Checklist

## B1 – Linux Server Administration

- [ ] Linux command outputs.
- [ ] Process and memory observations.
- [ ] Disk and network observations.
- [ ] Completed access-log script.
- [ ] Log-analysis script output.
- [ ] Disk-space alert output.
- [ ] Cron configuration.
- [ ] systemd service status.
- [ ] Nginx configuration test.
- [ ] Firewall status.

## B2 – Containerisation

- [ ] Successful Docker image build.
- [ ] Running container evidence.
- [ ] Browser or curl access to the portal.
- [ ] Non-root user verification.
- [ ] Docker Compose deployment.
- [ ] Database volume persistence test.
- [ ] CPU and memory limits.
- [ ] `docker stats` output.
- [ ] `docker history` output.
- [ ] Image-size comparison.

## B3 – Deployment Comparison

- [ ] Direct Linux deployment.
- [ ] VM deployment.
- [ ] Docker deployment.
- [ ] Start-up time comparison.
- [ ] Memory comparison.
- [ ] Disk/image-size comparison.
- [ ] Requests-per-second results.
- [ ] Three Kubernetes replicas.
- [ ] Pod deletion and recreation.
- [ ] Six-replica scaling under load.
- [ ] Engineering trade-off analysis.

## B4 – Final Engineering Decision

- [ ] Second service implemented.
- [ ] Both services running.
- [ ] Separate resource requests and limits.
- [ ] B3 measured RPS used for capacity estimation.
- [ ] Required replica calculation.
- [ ] Annual public-cloud cost calculation.
- [ ] Capacity-addition time.
- [ ] Student-data ownership and protection discussion.
- [ ] Final deployment recommendation.

---

# 10. Important Measurement and Reporting Guidelines

1. Record the commands used and their actual outputs.
2. Do not fabricate CPU, memory, disk, image-size, start-up-time, throughput, latency, cloud-cost or vulnerability-scan results.
3. Keep application code and benchmark conditions consistent when comparing Linux, VM and Docker.
4. Repeat performance measurements and state the statistic used, such as the median of three runs.
5. Distinguish configured resource limits from observed resource consumption.
6. Identify assumptions such as requests per login, burst factor, headroom and peak duration.
7. State environment limitations. A process running inside WSL2 is not a native bare-metal Linux deployment.
8. Explain differences between the proposed design and the actual implementation.
9. Keep the result portal and LMS/exam service separately identified and resource-limited in B4.
10. Ensure each alternative-concept discussion includes a reasoned comparison, not only definitions.
11. Include actual execution outputs wherever practical implementation is required.
12. Ensure configuration files and source code correspond to the versions and settings described in the reports.

---

# 11. Final Outcome

Part B implements and investigates the technologies and designs developed in Part A.

It demonstrates Linux administration, automation, containerisation, virtualisation, performance measurement, Kubernetes orchestration, workload scaling, recovery and final engineering decision-making.

The final deployment recommendation must be supported by actual implementation evidence, measured performance and cost estimates.

**END OF PART B**
