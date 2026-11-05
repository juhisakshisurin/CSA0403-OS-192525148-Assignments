# Assignment 5 – Part B – B4: Final Engineering Decision

| **Student Detail** | **Information** |
| ------------------ | --------------- |
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

## 1. Task Requirements

B4 is worth 5 marks (K5 – Evaluate) and has three parts:

| Section | Marks | Requirement |
|---|---:|---|
| B4.1 – Implement a Practical Extension | 2 | Add a second service to the Kubernetes or Docker Compose setup and show both services running with their own resource limits. |
| B4.2 – Estimate the Capacity | 1 | Use the measured requests/second from B3 to estimate containers or VMs required for 150,000 students in two hours. |
| B4.3 – Production Decision | 2 | Choose own servers with VMs, own servers with Kubernetes containers, public cloud or hybrid; justify with measurements and cost estimates. |

The alternative analysis also asks for public-cloud-only annual cost with one result day per semester, capacity-addition time and control of student data.

## 2. Files Included

| File | Purpose |
|---|---|
| `Assignment5_PartB_B4_Report.pdf` | Full report without student identity details. |
| `README.md` | Student details and detailed execution procedure. |
| `kubernetes/b4-services.yaml` | Two Deployments and two Services, each with separate CPU/memory requests and limits; includes namespace quota. |
| `portal/app.py`, `portal/Dockerfile`, `portal/requirements.txt` | Result portal image used in the B4 extension. |
| `lms_exam/app.py`, `lms_exam/Dockerfile`, `lms_exam/requirements.txt` | Second LMS/online-exam sample service. |
| `compose.yaml` | Optional Docker Compose version of both services on one host. |
| `scripts/b4_capacity_estimator.py` | Calculates required portal replicas from measured B3 per-container RPS and explicit traffic assumptions. |
| `evidence/B4_capacity_cost_worksheet.txt` | Capacity, cluster resource and public-cloud annual cost worksheet. |
| `B4_execution_commands.txt` | Build, deploy, test, observe and scaling commands. |

## 3. B4.1 – Add the Second Service

The practical extension uses Kubernetes, continuing the B3 approach:
- Result Portal: 3 replicas, CPU request 250m, memory request 256Mi, CPU limit 500m, memory limit 512Mi.
- LMS / Online Examination: 2 replicas, CPU request 200m, memory request 256Mi, CPU limit 400m, memory limit 512Mi.

The services use separate Deployments, labels, Services, images and resource settings. They run in the `b4-university` namespace. The portal application retains the B3 `/work?n=10000` endpoint so its measured B3 throughput can be reused in B4.2. A namespace ResourceQuota caps aggregate requests and limits so the two workloads cannot consume unlimited cluster resources.

Kubernetes schedules Pods using resource requests and enforces configured limits at runtime. The supplied values are lab starting settings and should be validated with measured load; they are not claimed as final production sizing.

## 4. Build and Run the Services

Build the two images:

```bash
docker build -t university/result-portal:b4 ./portal
docker build -t university/lms-exam:b4 ./lms_exam
```

For a local kind cluster:

```bash
kind create cluster --name b4
kind load docker-image university/result-portal:b4 --name b4
kind load docker-image university/lms-exam:b4 --name b4
kubectl apply -f kubernetes/b4-services.yaml
kubectl get deployments,pods,services -n b4-university
```

For minikube, use `minikube start`, then run `minikube image load` for both images, and apply the same YAML.

## 5. Test Both Services

Portal service:

```bash
kubectl port-forward -n b4-university service/result-portal 8000:80
curl http://127.0.0.1:8000/health
```

LMS/exam service in another terminal:

```bash
kubectl port-forward -n b4-university service/lms-exam 8001:80
curl http://127.0.0.1:8001/health
curl http://127.0.0.1:8001/exam/status
```

The LMS/exam program is a demonstration service, not a complete LMS or live examination platform. It proves a second independently deployed workload with its own resource limits.

Collect evidence:

```bash
kubectl get pods -n b4-university -o wide
kubectl describe deployment result-portal -n b4-university
kubectl describe deployment lms-exam -n b4-university
kubectl get resourcequota -n b4-university
kubectl top pods -n b4-university
```

`kubectl top` requires a working Metrics API such as metrics-server. If unavailable, the Deployment/Pod descriptions still show configured requests and limits; record the metrics limitation honestly.

## 6. B4.2 – Capacity Estimation from B3 Measurements

The question gives 150,000 students over two hours:

`Average login arrival = 150,000 / 7,200 = 20.83 logins/second`

A login may produce more than one backend request. Therefore estimate:

`Design RPS = (150,000 / 7,200) × requests_per_login × burst_factor × headroom`

Then:

`Required portal replicas = CEILING(Design RPS / measured sustainable RPS per one container)`

Use the actual per-container sustainable requests/second from B3, measured while keeping the response-time and error-rate target acceptable. The provided `scripts/b4_capacity_estimator.py` accepts this measured rate.

Example only: with 5 backend requests per login, burst factor 2, headroom 1.25 and a measured 40 requests/second per container, the design target is about 260.42 requests/second and the estimate is 7 portal replicas. These traffic factors and 40 RPS are illustrative assumptions, not your actual measurement. Replace them with evidence from your B3 benchmark and, ideally, a portal access-log/request trace.

Run:

```bash
python3 scripts/b4_capacity_estimator.py --measured-rps YOUR_B3_MEASURED_RPS
```

Optional explicit assumptions:

```bash
python3 scripts/b4_capacity_estimator.py --measured-rps 40 \\
  --requests-per-login 5 --burst-factor 2 --headroom 1.25
```

## 7. B4.3 – Production Decision

The report recommends a hybrid design:
- Retain baseline workloads and authoritative data services on the university's existing infrastructure, provided measured capacity and failure reserve are sufficient.
- Burst stateless portal replicas to public cloud ahead of result day through a separately managed cloud deployment/cluster and traffic routing.
- Use encrypted private connectivity, restricted service identities, monitoring and approved data-transfer policies.
- University retains data ownership and decides access, retention, region and encryption-key policy. The cloud provider hosts/operates the contracted infrastructure; the university must verify contractual, regulatory and residency requirements.

Do not assume a stretched Kubernetes cluster across an unreliable WAN. A practical hybrid can use separate on-premises and cloud environments connected by secure networking and an application-level/global traffic-routing design.

## 8. Public-Cloud-Only Annual Cost Model

The assignment says one result day per semester, so the model uses two peak days per year. The stated login burst is two hours per result day: four peak hours/year, excluding any pre-warming period.

For a managed Kubernetes public-cloud design:

`Annual cloud cost = baseline 8,760-hour worker nodes + control plane + managed DB + load balancer + 4-hour extra peak nodes + storage + backups + egress + monitoring + support + tax`

More precisely:

`C = 8,760 × (N_base × P_node + P_control + P_db + P_lb)`
`  + 4 × (N_peak - N_base) × P_node`
`  + annual storage + backup + egress + monitoring + support + taxes`

Enter the chosen provider, region, currency and current unit rates in `evidence/B4_capacity_cost_worksheet.txt`. The assignment does not name a provider or region and supplies no price list; therefore no exact provider cost is invented. Include the continuously provisioned database, load balancer and control plane in the annual estimate.

Capacity addition time should be measured from the scale request until new Pods/nodes are Ready. New cloud nodes may take longer than scheduling Pods on already-free nodes; pre-warm capacity before the known event.

## 9. Alternative and Engineering Trade-off

- Own servers with VMs: clear guest-OS separation, but higher per-workload RAM/disk and boot overhead; existing server capacity may cap peak load.
- Own servers with Kubernetes: efficient container density and Pod recovery; still bounded by on-prem node capacity and failure reserve.
- Public cloud only: elastic capacity and provider-managed services, but baseline database/control-plane/network charges continue through the year and student records are hosted by a provider under contract.
- Hybrid: balances use of existing equipment with event-driven cloud burst; requires secure connectivity, observability, a tested routing/data strategy and careful control of data replication.

## 10. Compiler and Environment Information

- **C compiler / version:** Not applicable; B4 does not request a C source program.
- **GCC compile command:** Not applicable; the implementation uses Python application files and Kubernetes YAML.
- **Python version:** Record with `python3 --version`.
- **Docker version:** Record with `docker --version`.
- **Kubernetes client / cluster:** Record with `kubectl version` and `kubectl get nodes -o wide`.
- **Linux distribution / kernel:** Record with `cat /etc/os-release` and `uname -a`.

Do not write version values that you have not checked on the machine used for the practical.

## 11. Result and Evidence

Before final submission, retain:
1. Both images build successfully.
2. `kubectl get deployments,pods,services -n b4-university` showing both services running.
3. `kubectl describe` showing each service's independent CPU/memory requests and limits.
4. `kubectl top pods` output if Metrics API is available.
5. B3 measured RPS used by the capacity estimator and its output.
6. Completed cloud pricing inputs and annual total from a provider calculator.
7. Measured time to add cloud capacity and a short data-control explanation.

No live cluster deployment, B3 benchmark result or provider quote is included as if it had been executed; those values must come from your environment/account.
