# Assignment 5 – Part B – B2: Containerise the Portal

| **Student Detail** | **Information** |
| ------------------ | --------------- |
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

## 1. Task requirements

B2 is worth 5 marks (K5 – Evaluate). The assignment requires:
1. Write a Dockerfile for the portal, build the image and run it with `docker run`, mapping the port.
2. Run the portal and its database using Docker Compose, with a volume for database data.
3. Limit CPU and memory using `--cpus` and `--memory`, then show the effect with `docker stats`.
4. Inspect image layers using `docker history` and reduce image size using a slim base or multi-stage build.
5. Explain what each step demonstrates about containers.

The Complete the Code question specifies `python:3.12-slim`, working directory `/app`, copy the app and run as non-root user `portal`, expose port 8000 and launch Gunicorn on `0.0.0.0:8000` using the `app` object in `app.py`.

## 2. Included files

| File | Purpose |
|---|---|
| `Assignment5_PartB_B2_Report.pdf` | Full report without student identity details. |
| `README.md` | Student details and procedure. |
| `Dockerfile` | Completed three TODOs using `python:3.12-slim`. |
| `app.py` | Sample Flask portal with `/` and `/healthz`; checks PostgreSQL when configured. |
| `requirements.txt` | Flask, Gunicorn and PostgreSQL driver dependencies. |
| `compose.yaml` | Portal + PostgreSQL Compose setup with named persistent volume, health check and resource limits. |
| `.env.example` | Local development environment template. Copy to `.env` and change the sample password. |
| `.dockerignore` | Excludes local secrets, caches and evidence files from the build context. |
| `Dockerfile.multistage` | Alternative multi-stage slim image for comparison. |
| `Dockerfile.ubuntu` | Optional full Ubuntu base image for the alternative evaluation. |
| `run_docker_demo.sh` | Optional helper for build, run, health check, image history and `whoami`. |
| `docker_commands.txt` | Command sequence for the required experiments. |
| `evidence/B2_measurement_template.txt` | Worksheet for actual image sizes, build/pull times, resource usage and scan findings. |

## 3. Completed Dockerfile – TODO 1, TODO 2 and TODO 3

```dockerfile
FROM python:3.12-slim

# TODO 1
WORKDIR /app

COPY requirements.txt .
RUN pip install --no-cache-dir -r requirements.txt

# Create a dedicated non-root account.
RUN groupadd --system portal \\
    && useradd --system --gid portal --create-home \\
       --home-dir /home/portal --shell /usr/sbin/nologin portal

# TODO 2
COPY --chown=portal:portal app.py /app/app.py
USER portal

# TODO 3
EXPOSE 8000
CMD ["gunicorn", "--workers", "2", "--bind", "0.0.0.0:8000", "app:app"]
```

Explanation:
- `WORKDIR /app` makes `/app` the working directory for subsequent build and runtime commands.
- Requirements are copied and installed before application source, so changes to `app.py` do not invalidate the dependency-install layer.
- A system group and user named `portal` are created; application code is owned by that user.
- `USER portal` ensures Gunicorn does not run as root.
- `EXPOSE 8000` documents the container port; it does not publish it on the host.
- Gunicorn binds to `0.0.0.0:8000` inside the container. Docker's `-p 8000:8000` publishes the host port.

## 4. Prerequisites

Install Docker Engine and the Docker Compose plugin for your Linux distribution, or use Docker Desktop with WSL integration enabled. Check:

```bash
docker --version
docker compose version
docker info
```

Docker commands must be run from a terminal connected to a working Docker daemon. If using Ubuntu under WSL, enable Docker Desktop WSL integration or install/configure a Docker Engine service in the Linux environment.

## 5. Build the image and run with docker run

From the folder containing `Dockerfile`, `app.py` and `requirements.txt`:

```bash
docker build -t result-portal .
docker image ls result-portal

docker run -d --name result-portal-standalone \\
  -p 8000:8000 --cpus="0.50" --memory="512m" result-portal
```

Test the application:

```bash
curl -i http://localhost:8000/
curl -i http://localhost:8000/healthz
docker ps
docker exec result-portal-standalone whoami
```

The expected `whoami` output is:

```text
portal
```

Record the actual image size from `docker image ls` and the running state from `docker ps`. Do not insert a guessed size.

Observe resource usage:

```bash
docker stats --no-stream result-portal-standalone
```

For live observation, remove `--no-stream`. The CPU and memory caps are upper bounds; `docker stats` reports actual use at the time sampled. A light sample request may not push the container near its limits.

## 6. Docker Compose – portal and database with persistent storage

Copy the example environment file and change the local password:

```bash
cp .env.example .env
```

Edit `.env` and replace the sample `POSTGRES_PASSWORD`. Do not commit a real production secret.

Start the services:

```bash
docker compose up --build -d
docker compose ps
docker compose logs --tail=50
curl -i http://localhost:8000/
docker compose exec portal whoami
docker stats --no-stream
```

The Compose file creates a named volume called `postgres_data`, mounted at `/var/lib/postgresql/data` inside the PostgreSQL container. The database health check gates portal startup. The application connects to the service name `db` on the private Compose network.

To demonstrate persistence, stop and recreate the containers without deleting the volume:

```bash
docker compose down
docker compose up -d
docker compose ps
```

Do **not** use `docker compose down -v` for this persistence test; `-v` deletes the named database volume.

## 7. CPU and memory limits

The direct `docker run` example uses:

```bash
--cpus="0.50" --memory="512m"
```

The Compose file also applies `cpus` and `mem_limit` to the portal and database services. These settings constrain the container's CPU and memory consumption. Inspect the values and live usage with:

```bash
docker inspect result-portal-standalone
docker stats --no-stream
```

`docker stats` shows current usage, not a guarantee that the workload reached the limit. For meaningful evidence, record usage before and during a controlled request/load test. Avoid intentionally exhausting memory on a shared or production machine.

## 8. Image layers and size reduction

Inspect the built image:

```bash
docker history result-portal:latest
docker image inspect result-portal:latest
docker images result-portal
```

The Dockerfile copies `requirements.txt` and installs dependencies before copying `app.py`. This makes the dependency layer reusable when only application code changes. The slim Python base reduces the starting OS content compared with a full Ubuntu image. `.dockerignore` prevents local environments, secrets and caches from being sent in the build context.

For comparison, build optional variants:

```bash
docker build -f Dockerfile.multistage -t result-portal:multistage .
docker build -f Dockerfile.ubuntu -t result-portal:ubuntu .
docker images result-portal
```

The multi-stage file separates dependency installation from the final runtime stage. For this small app, the slim image may already be compact and the multi-stage build may offer only a small further reduction because psycopg's binary wheel avoids many compile-time packages. Actual sizes must be measured.

## 9. Alternative concept – full Ubuntu base or multi-stage build

A full Ubuntu base generally contains a broader set of operating-system utilities and packages than a purpose-built slim Python image. This tends to increase:
- Image size and transfer time.
- Number of installed packages and potentially vulnerable components.
- Update and patching work.

A smaller image usually reduces the number of components that must be maintained, but it does not automatically make an application secure. The Python packages and application code also require updates and scanning.

A multi-stage build uses a builder stage for build tools/dependencies and copies only the runtime artifacts into the final stage. It is most beneficial when build tools are otherwise needed but not required at runtime. This project includes an alternative multi-stage Dockerfile and a full-Ubuntu comparison Dockerfile.

Pull-time estimate:

`Transfer time in seconds ≈ image size in MB × 8 ÷ available throughput in Mbps`

This is a theoretical lower-bound approximation; registry latency, protocol overhead, decompression, layer caching and network contention affect real pull time.

Optional vulnerability scan if Trivy is installed:

```bash
trivy image --scanners vuln result-portal:latest
trivy image --scanners vuln result-portal:ubuntu
```

Record actual critical/high/medium findings and affected packages. Do not state a package-vulnerability count without running a scanner on the built image.

## 10. Evidence to retain

Fill `evidence/B2_measurement_template.txt` with:
1. Docker Engine and Compose versions.
2. Successful image build output.
3. `docker images` size.
4. `docker ps`, URL response and `docker exec ... whoami`.
5. `docker stats` usage with the resource limits.
6. `docker compose ps` and proof that the database volume persists after `docker compose down` (without `-v`).
7. `docker history` output.
8. Measured build/pull times and any scanner findings.

## 11. Result

B2 is complete when the Flask image builds and runs on port 8000, the portal runs with PostgreSQL through Compose using persistent volume storage, CPU/memory limits are visible in Docker statistics, image layers and size are inspected, and the slim/multi-stage alternative is evaluated using actual measurements.
