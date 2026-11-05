# Assignment 5 – Part A – A5: Design the Cloud Deployment and Orchestration

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 5 – CO5 |
| **Part** | Part A – Design |
| **Task** | A5 – Design the Cloud Deployment and Orchestration |
| **Marks / Bloom's Level** | 4 Marks / K5 – Evaluate |

---

## 1. Task Requirements

| Section | Marks | Requirement |
|---|---:|---|
| A5.1 – Cloud Set-up | 2 | Compare IaaS, PaaS and SaaS, and design a result-day cloud setup using VMs or managed Kubernetes, a load balancer and a managed database. |
| A5.2 – Kubernetes | 2 | Explain Pods, Deployments, Services, replicas, resource requests/limits and Horizontal Pod Autoscaler (HPA), and how they scale and recover from failures. |
| Alternative Concept | Included | Compare fixed VMs sized for result day against Kubernetes with autoscaling, including result-day response, idle resources during the other 364 days and cost. |

The case states that 150,000 students log in within two hours on result day and the existing single physical Linux server crashes during the peak. The available public-cloud account is part of the design context.

## 2. Files Included

| File | Purpose |
|---|---|
| `Assignment5_PartA_A5_Report.pdf` | Complete report without your name or register number. |
| `README.md` | Student details in the requested table and instructions. |
| `A5_cloud_sizing_worksheet.txt` | Clearly marked planning assumptions, calculations, provider cost fields and measurement fields. |
| `k8s/portal-deployment.yaml` | Example Kubernetes Deployment for the stateless Python portal. |
| `k8s/portal-service.yaml` | Cloud LoadBalancer Service routing to ready portal Pods. |
| `k8s/portal-hpa.yaml` | HPA example: 3–8 replicas, CPU target 65%, scale-down stabilisation. |
| `k8s/portal-pdb.yaml` | PodDisruptionBudget example to retain at least two available Pods during voluntary disruption. |

## 3. Main Design

The report recommends:
- IaaS or a managed Kubernetes platform for the portal application tier.
- A managed external load balancer for user traffic.
- A managed relational database for persistent student-result data.
- A Kubernetes Deployment with multiple stateless application replicas.
- A stable Service to route traffic to ready Pods.
- Resource requests and limits for predictable scheduling and resource protection.
- HPA for Pod-level horizontal scaling.
- Cluster/node autoscaling in addition to HPA, so worker nodes can scale when the Pod count grows or falls.

## 4. Important Assumptions

The assignment provides the 150,000-student / two-hour login window but no cloud provider, region, peak concurrency, request rate, database workload, VM price or benchmark output.

Therefore, the worked cost/resource example is illustrative and is not a measured deployment:
- 3 application instances for ordinary days.
- 8 application instances for the result-day peak.
- Each equivalent instance requests 1 vCPU and 2 GiB memory.
- 4-hour elevated window on the result day (including pre-scaling and cool-down).
- 365-day year with 8,760 hours.

The example estimates 43,680 extra instance-hours of fixed capacity across the other 364 days and a 62.47% reduction in annual application replica-hours in the HPA model. These are workload-model calculations only, not total cloud-bill savings.

The cloud cost comparison must include worker nodes, Kubernetes control plane, load balancer, managed database, storage, backup, network egress, monitoring, support and taxes. No provider price has been invented.

## 5. Kubernetes Manifest Notes

### Deployment

- Uses an illustrative image name: `registry.example.edu/university/result-portal:1.0.0`. Replace it with the actual image registry and immutable tag.
- Reads database settings from the pre-created Secret `result-portal-db-credentials`.
- Sets CPU/memory requests and limits.
- Includes readiness and liveness probes; implement `/readyz` and `/healthz` in the actual application.
- Uses a non-root security context and a read-only root filesystem.
- Omits `spec.replicas` because the HPA manages replica count.

### Service

`type: LoadBalancer` asks a supported cloud provider integration to provision an external load balancer and direct traffic to matching ready Pods.

### HPA

- Minimum replicas: 3.
- Maximum replicas: 8.
- CPU target: 65% of the requested CPU.
- Scale-down stabilisation: 300 seconds.
- Requires the Kubernetes resource metrics API (commonly metrics-server for CPU/memory resource metrics).
- HPA scales Pods, not cloud worker nodes. Configure the provider's cluster autoscaler or managed node auto-provisioning as well.

### PodDisruptionBudget

Keeps at least two matching Pods available during voluntary disruptions. It does not replace multi-zone node placement, readiness probes or application/database high availability.

## 6. Optional Validation / Deployment Procedure

These manifests are design examples. Do not apply them to a production cluster without replacing the sample image, creating the database Secret, implementing health endpoints, setting namespaces/storage/network policies and confirming actual capacity.

Check client and cluster versions:

```bash
kubectl version
kubectl get nodes -o wide
kubectl get pods -A
kubectl get apiservices
```

Create the required database secret from values supplied securely by the cloud provider or administrator. Do not place a real password in a YAML file committed as plaintext.

Apply the example resources:

```bash
kubectl apply -f k8s/portal-deployment.yaml
kubectl apply -f k8s/portal-service.yaml
kubectl apply -f k8s/portal-hpa.yaml
kubectl apply -f k8s/portal-pdb.yaml
```

Check status and autoscaling:

```bash
kubectl get deployments,pods,services,hpa,pdb
kubectl describe hpa result-portal
kubectl top pods
kubectl top nodes
kubectl get events --sort-by=.lastTimestamp
```

A configured metrics API is required for CPU-based HPA. If `kubectl top` reports that metrics are unavailable, resolve metrics-server/provider metrics before evaluating HPA behaviour.

For a known result-release time, increase the minimum replicas or pre-scale the Deployment before traffic arrives. Do not wait for CPU to rise before starting all peak capacity; HPA is reactive.

## 7. C Compiler / C Program

A5 Part A is a cloud-design and Kubernetes evaluation task. It does not include a C “Complete the Code” program. GCC compilation is not applicable. The YAML files are orchestration configuration examples, not C source code.

## 8. Compiler and Environment Information

- **Compiler / version:** Not applicable for Part A A5; no C source program is requested.
- **GCC compilation command:** Not applicable; no C program is compiled for this task.
- **Linux distribution / version:** Not supplied because the A5 Part A deliverable is a cloud design. If you validate the manifests from a Linux workstation, record the actual environment using `cat /etc/os-release` and `uname -r`.
- **Kubernetes client / cluster version:** Record the actual values using `kubectl version` before deployment.

## 9. Result

The report compares IaaS, PaaS and SaaS, proposes a cloud load-balancer / Kubernetes / managed-database arrangement, explains Kubernetes scaling and recovery objects, and evaluates fixed result-day VMs against HPA-based scaling with an explicit 364-day idle-capacity model.
