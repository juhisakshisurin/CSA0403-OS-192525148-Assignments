# Assignment 5 – Part B – B3: Compare Bare Metal, Virtual Machine and Container

| **Student Detail** | **Information** |
| ------------------ | --------------- |
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

## 1. Task requirements

B3 is worth 8 marks (K5 – Evaluate).

| Section | Marks | Required work |
|---|---:|---|
| B3.1 – Deploy Three Ways | 2 | Run the same web application directly on Linux, inside a VM (VirtualBox or KVM), and inside a Docker container. |
| B3.2 – Measure | 2 | Measure startup time, memory, image/disk size and requests per second using `ab`, `wrk` or `hey`. |
| B3.3 – Scale with Kubernetes | 2 | Deploy the container to minikube or kind with 3 replicas, delete one Pod and show it is recreated, then scale to 6 replicas under load. |
| B3.4 – Engineering Trade-Off | 2 | Explain the measurements and state when each deployment approach should be used. |

The supplied Kubernetes TODO task is completed in `kubernetes/deployment.yaml`. TODO 1 sets 3 replicas and the label selector. TODO 2 sets requests to 250m CPU and 256Mi RAM, and limits to 500m CPU and 512Mi RAM. The file also includes the Service shown in the question.

## 2. Files included

| File | Purpose |
|---|---|
| `Assignment5_PartB_B3_Report.pdf` | Report without student identity details. |
| `README.md` | Student details and full execution procedure. |
| `app.py` | Same Flask application for all deployment types; includes `/`, `/health` and a CPU-work endpoint `/work?n=10000`. |
| `requirements.txt` | Flask and Gunicorn dependencies. |
| `Dockerfile` | Builds the common app image as `simats/result-portal:1.0`. |
| `kubernetes/deployment.yaml` | Completed Deployment TODOs and LoadBalancer Service. |
| `systemd/result-portal.service` | Starts the same app automatically in the VM using systemd. |
| `scripts/measure_startup.py` | Measures command start to HTTP health readiness. |
| `scripts/benchmark_requests.sh` | Uses ApacheBench to measure requests/second and latency for a URL. |
| `scripts/capture_linux_metrics.sh` | Captures memory, process, disk and listener evidence. |
| `scripts/capture_docker_metrics.sh` | Captures container memory, image size and layer history. |
| `scripts/k8s_scale_test.sh` | Applies Kubernetes manifest, waits for Pods and demonstrates deletion/recreation. |
| `evidence/B3_measurement_template.csv` | Comparison table for actual measured values. |
| `evidence/B3_measurement_notes.txt` | Experimental controls, commands and limitations. |

## 3. Use the same application for all three deployments

The common application is `app.py`. It exposes:
- `/` – portal home page.
- `/health` – readiness/health response.
- `/work?n=10000` – deterministic CPU work for comparing request throughput.

Keep the same application code, Gunicorn worker count, request path, request count and concurrency in all tests.

## 4. B3.1 – Deploy directly on Linux (bare-metal method)

On native Linux (not inside a VM or WSL2), install dependencies and start the app:

```bash
sudo apt update
sudo apt install -y python3 python3-venv apache2-utils
python3 -m venv .venv
. .venv/bin/activate
pip install -r requirements.txt
gunicorn --workers 2 --bind 0.0.0.0:8000 app:app
```

Test from another terminal:

```bash
curl http://127.0.0.1:8000/
curl http://127.0.0.1:8000/health
```

Measure process memory:

```bash
pgrep -af gunicorn
ps -o pid,rss,%cpu,%mem,comm -p GUNICORN_PID
du -sh .venv app.py
free -h
```

Replace `GUNICORN_PID` with the actual master/worker PID.

### Startup time

Stop the previous app instance, then run:

```bash
python3 scripts/measure_startup.py \
  --url http://127.0.0.1:8000/health -- \
  .venv/bin/gunicorn --workers 2 --bind 0.0.0.0:8000 app:app
```

The helper starts the command, polls `/health`, prints elapsed seconds and stops the launched process after it becomes ready.

## 5. B3.1 – Deploy inside a VirtualBox/KVM VM

1. Create an Ubuntu VM in VirtualBox or KVM. Record guest vCPU, RAM and virtual disk size.
2. Install Python, create a virtual environment and copy the same `app.py` and `requirements.txt` into the guest.
3. Start the same Gunicorn command on guest port 8000.
4. Configure VirtualBox NAT port forwarding from host port 8001 to guest port 8000 (or use a bridged address). For VirtualBox, the NAT rule can be set in Network > Adapter 1 > Advanced > Port Forwarding.
5. Test from the host:

```bash
curl http://127.0.0.1:8001/health
```

Install the provided `systemd/result-portal.service` inside the guest so the app starts automatically at VM boot. Create the service account and directories, install the app and venv under `/opt/b3-result-portal`, adjust file ownership, then copy the service file to `/etc/systemd/system/`.

```bash
sudo useradd --system --no-create-home --shell /usr/sbin/nologin b3portal
sudo install -d -o root -g b3portal -m 0750 /opt/b3-result-portal
# Copy app.py and requirements.txt to /opt/b3-result-portal first.
sudo python3 -m venv /opt/b3-result-portal/.venv
sudo /opt/b3-result-portal/.venv/bin/pip install -r /opt/b3-result-portal/requirements.txt
sudo chown -R root:b3portal /opt/b3-result-portal
sudo chmod 0640 /opt/b3-result-portal/app.py /opt/b3-result-portal/requirements.txt
sudo install -m 0644 systemd/result-portal.service /etc/systemd/system/result-portal.service
sudo systemctl daemon-reload
sudo systemctl enable --now result-portal
sudo systemctl status result-portal
```

Use the correct VM IP/forwarded port. For VM startup measurement, shut down the guest first and use:

```bash
python3 scripts/measure_startup.py \
  --url http://127.0.0.1:8001/health --leave-running -- \
  VBoxManage startvm "B3-Ubuntu" --type headless
```

Replace `B3-Ubuntu` with your VM name. This measures from the VM start command until the app health endpoint responds. The guest must already have the systemd service enabled.

Record guest `free -h`, host VirtualBox/QEMU process RSS, guest virtual disk configured capacity and actual VDI/QCOW2 file size. Do not compare VM configured RAM with process RSS as if they were the same metric.

## 6. B3.1 – Deploy inside Docker

Build the common app image:

```bash
docker build -t simats/result-portal:1.0 .
docker images simats/result-portal
```

Run it with port mapping and resource limits:

```bash
docker run -d --name result-portal-container \
  -p 8002:8000 --cpus="0.50" --memory="512m" \
  simats/result-portal:1.0
```

Test and record:

```bash
curl http://127.0.0.1:8002/health
docker ps
docker stats --no-stream result-portal-container
docker exec result-portal-container whoami
docker image ls simats/result-portal:1.0
```

Expected user: `portal`.

To measure startup from a stopped state, remove the prior test container first and run:

```bash
docker rm -f result-portal-container 2>/dev/null || true
python3 scripts/measure_startup.py \
  --url http://127.0.0.1:8002/health -- \
  docker run --rm --name result-portal-container \
  -p 8002:8000 --cpus=0.50 --memory=512m simats/result-portal:1.0
```

## 7. B3.2 – Measure all three methods

Install ApacheBench (`ab`) using `sudo apt install apache2-utils`. Keep the benchmarking client outside the deployment under test.

Run the same workload for each deployment:

```bash
chmod +x scripts/benchmark_requests.sh
scripts/benchmark_requests.sh baremetal http://127.0.0.1:8000/work?n=10000 2000 20
scripts/benchmark_requests.sh vm http://127.0.0.1:8001/work?n=10000 2000 20
scripts/benchmark_requests.sh docker http://127.0.0.1:8002/work?n=10000 2000 20
```

Run each test at least three times. Use the median Requests/second and 95th percentile latency. Ensure the app is ready and warmed up before each run. Do not run the three tests simultaneously on the same laptop.

Memory and size evidence:
- Bare metal: Gunicorn process RSS (`ps`), virtual environment/app disk footprint (`du -sh`) and host `free -h`.
- VM: guest RAM use (`free -h`), configured guest RAM, host VirtualBox/QEMU process RSS, virtual disk provisioned size and actual VDI/QCOW2 file size.
- Docker: `docker stats`, `docker image ls`, and optionally `docker inspect --size`.

Startup: measure from a stopped state to the first successful HTTP 200 response on `/health`. Repeat three times and report the median. For a VM, include guest OS boot and systemd app readiness; for a container, include container start and application readiness.

## 8. B3.3 – Kubernetes with 3 replicas, recovery and scale to 6

Build the image using the exact tag expected in the YAML:

```bash
docker build -t simats/result-portal:1.0 .
```

Create a local cluster. Choose **one**:

### Option A – kind

```bash
kind create cluster --name b3
kind load docker-image simats/result-portal:1.0 --name b3
kubectl cluster-info --context kind-b3
```

### Option B – minikube

```bash
minikube start
minikube image load simats/result-portal:1.0
kubectl get nodes
```

Apply the completed manifest:

```bash
kubectl apply -f kubernetes/deployment.yaml
kubectl rollout status deployment/result-portal --timeout=180s
kubectl get pods -l app=result-portal -o wide
kubectl describe pod POD_NAME
```

Expected: three ready Pods, with requests of `250m` CPU / `256Mi` memory and limits of `500m` CPU / `512Mi` memory.

### Delete one Pod and show automatic recreation

```bash
POD=$(kubectl get pods -l app=result-portal -o jsonpath='{.items[0].metadata.name}')
kubectl delete pod "$POD"
kubectl get pods -l app=result-portal -w
```

The Deployment/ReplicaSet controller should create a replacement Pod to return the desired count to three. Record the deleted name and the replacement name.

### Expose the Service locally

The manifest keeps `type: LoadBalancer` as specified in the question. For local kind/minikube tests, use port-forwarding in a separate terminal:

```bash
kubectl port-forward service/result-portal 8003:80
```

### Scale to six while load is running

In a second terminal, start a longer benchmark:

```bash
ab -n 50000 -c 50 -k http://127.0.0.1:8003/work?n=10000
```

While it is running, scale the Deployment:

```bash
kubectl scale deployment/result-portal --replicas=6
kubectl get pods -l app=result-portal -w
```

After the test, collect:

```bash
kubectl get deployment result-portal
kubectl get pods -l app=result-portal -o wide
kubectl describe deployment result-portal
kubectl top pods
```

`kubectl top` requires the Metrics API (commonly metrics-server). If metrics are unavailable, report that fact; the replica-count and Pod-recreation evidence can still be collected.

A one-node local cluster demonstrates Kubernetes replica reconciliation and Pod replacement, but it does not prove multi-node high availability. To test node failure recovery, use a multi-node cluster.

## 9. B3.4 – Engineering trade-off

- **Bare metal:** Avoids a guest OS and container runtime layer and provides direct access to host resources. It can offer strong raw performance, but application dependencies and resource isolation must be managed on the shared host. Use where maximum predictable host-level performance and direct hardware access are important.
- **Virtual machine:** Adds guest OS boot time and memory/disk overhead, but provides a separate guest kernel and a clearer OS boundary. Use for stronger workload isolation, running different operating systems or consolidating services with independent guest administration.
- **Container:** Shares the host kernel, starts quickly and typically has less per-instance overhead than a full VM. Use for repeatable packaging, fast replica creation, CI/CD and orchestration, while hardening the shared-kernel boundary and limiting resources.
- **Kubernetes:** Maintains the desired Pod count and recreates failed Pods; it can scale replicas. Real recovery across hosts requires multiple healthy nodes, networking and storage design. A local single-node cluster only demonstrates controller behaviour.
- **Docker Compose alternative:** `docker compose up --scale portal=6` can run six replicas on one server, but has no built-in multi-node scheduling. If the single host fails, all six replicas are lost. Port publishing and load balancing must be designed so replicas do not compete for the same fixed host port.

Use the actual measurements to explain whether startup, memory, disk and throughput differ on your hardware. Do not assume bare metal is always fastest or that containers always have a fixed percentage overhead.

## 10. Evidence to retain

- Startup time from three runs for direct Linux, VM and Docker.
- Memory used and configured allocation for each method.
- App/environment, VM disk and container image sizes.
- Three `ab` outputs per deployment with request count and concurrency.
- Kubernetes output showing 3 Pods Ready.
- Deleted Pod name and replacement Pod evidence.
- Output showing 6 Pods Ready while the benchmark is active.
- `kubectl describe pod` showing requests and limits.
- Completed `evidence/B3_measurement_template.csv` and notes.

## 11. Compiler and environment

No C program is required by B3; the task uses Python, Docker and Kubernetes YAML. GCC version and compile command are not applicable. Record your actual Linux distribution, Docker, VM and Kubernetes versions from the machine used.

## 12. Result

B3 is complete when the same application has been run directly on Linux, in a VM and in Docker; startup, memory, disk/image size and requests-per-second have been measured consistently; Kubernetes has maintained three Pods after one deletion and has been scaled to six under load; and the measured results have been interpreted with deployment trade-offs.
