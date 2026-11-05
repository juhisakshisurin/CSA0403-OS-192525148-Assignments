# Assignment 4 – Task 6: Storage Design for a Video-Streaming Start-up

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 4 – CO4 |
| **Task** | Task 6 – Design Challenge |
| **Marks / Bloom's Level** | 13 Marks / K5 – Evaluate |

---

## 1. Task Overview

The question describes a start-up that streams lecture videos to **200,000 students**. Each video is between **300 MB and 2 GB**, is uploaded once, and is read many thousands of times, mostly during the first week. Students can jump to any minute of a video. The company has a budget for a few servers with hard disks and SSDs, and may also use cloud storage.

Task 6 requires five stages:

1. Interpret the workload, access patterns and measurable performance targets.
2. Propose the storage architecture, including SSD/HDD/cloud placement, filesystem and allocation method.
3. Select block size, disk scheduler, caching policy and replication factor.
4. Calculate capacity, throughput and disk reads required to start playback at any minute.
5. Simulate two alternative designs and report measured results.

The additional concept evaluates full cloud object storage plus a CDN, including playback start time for students in different cities, monthly storage/transfer cost and operational effort.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment4_Task06_Report.pdf` | Full report without student identity details. Covers all five stages, calculations, two-design simulation, cloud/CDN alternative and recommendation. |
| `README.md` | Student details, environment, exact GCC command, execution procedure and simulation interpretation. |
| `video_storage_sim.c` | C11 simulator comparing HDD-only origin against SSD hot-cache plus HDD origin. |
| `run_task6.sh` | Compiles the C program, executes it and saves the output and environment. |
| `simulation_results.txt` | Output from the completed two-design simulation. |
| `design_calculations.txt` | Capacity, replica, network-throughput, read-count and cloud-traffic calculations. |
| `measurement_assumptions.txt` | Inputs used by the simulation and design. |
| `measurement_worksheet.txt` | Worksheet for replacing planning assumptions with actual deployment measurements. |
| `environment.txt` | GCC, Linux distribution and kernel information from the preparation environment. |

## 3. Design Assumptions

The assignment does not provide the number of videos in the catalogue, average bitrate, concurrency, average view count, target latency, monthly data transfer or a particular cloud provider region. The report therefore states a planning scenario so its calculations can be repeated.

| Parameter | Planning assumption |
|---|---:|
| Students | 200,000 (given) |
| Catalogue size | 1,000 videos (assumed) |
| Average video size | 1 GiB (assumed, within the stated range) |
| Concurrent viewers at peak | 4,000 (2% of enrolled students; assumed) |
| Average streaming bitrate | 8 Mbit/s per viewer (assumed) |
| Headroom | 20% (assumed) |
| Hot-video request hit ratio | 85% (assumed for simulation) |
| Segment duration | 4 seconds (design choice) |
| Filesystem block size | 4 KiB (design choice) |
| Initial read-ahead | 1 MiB (design choice) |
| Replication factor | 2 copies across separate storage nodes |

These values are not facts supplied by the question. Replace them when real catalogue, analytics and traffic measurements become available.

## 4. Proposed Architecture

### Origin and cache layout

Use three Linux video-storage nodes:

- Each node: eight 8 TB HDDs in RAID 10, giving about 32 TB usable per node after RAID 10.
- Each node: two 1.92 TB NVMe SSDs in RAID 1, giving about 1.92 TB usable cache per node.
- Replication factor 2 across different nodes/failure domains.
- Three 25 GbE server links give 75 Gb/s aggregate link capacity; after one node/link fails, the remaining two links provide 50 Gb/s aggregate.
- XFS or ext4 with extent-based allocation for large, read-mostly video data. XFS is selected in the report for the Linux origin nodes.
- 4 KiB filesystem blocks; 1 MiB read-ahead requests for streaming.
- `mq-deadline` for HDDs when available to control read latency; `none` for NVMe where supported and validated. Check the actual kernel/device before changing the scheduler.

### Video layout and random seeks

Store videos as HTTP streaming segments (HLS/DASH style) with a manifest/time index. A 4-second segment at 8 Mbit/s is approximately 4 MB. A minute maps to segment number `minute × 60 / 4`, or 15 segments per minute.

The manifest and time index are cached in memory. Once the target segment is known, playback starts with a 1 MiB read-ahead from the target segment. With the index cached, that is one media read request; if metadata is cold, allow one extra index/metadata read. A 1 MiB prefetch contains 256 × 4 KiB filesystem blocks.

## 5. Capacity and Throughput Calculations

### Catalogue capacity

For 1,000 videos averaging 1 GiB:

```text
One catalogue copy = 1,000 GiB = 0.977 TiB
Two copies          = 2,000 GiB = 1.953 TiB
```

With three nodes, each having 32 TB usable HDD RAID 10, the cluster has 96 TB of post-RAID usable HDD capacity. At two copies, the theoretical logical capacity is about 48 TB before metadata, reserve and rebuild headroom.

### Peak throughput

```text
Concurrent viewers = 200,000 × 2% = 4,000
Streaming throughput = 4,000 × 8 Mbit/s = 32,000 Mbit/s = 32 Gbit/s
With 20% headroom = 32 × 1.20 = 38.4 Gbit/s
```

Three 25 GbE links provide 75 Gbit/s aggregate. After one node/link failure, two links provide 50 Gbit/s, which is still above the 38.4 Gbit/s planning target. This is a capacity check; the server CPUs, network switches, storage and application path must also be benchmarked.

At an 85% cache-hit ratio:
- SSD tier carries about 27.2 Gbit/s.
- HDD origin carries about 4.8 Gbit/s.

## 6. C Program: Two-Design Simulation

`video_storage_sim.c` compares:

- Design A: 24 HDD random-read service lanes.
- Design B: four SSD hot-cache service lanes plus the same 24 HDD lanes for cache misses.

The deterministic workload has 10,000 playback-start/segment requests at approximately 1,000 requests per second. The simulator measures mean, p50, p95, p99 and maximum response time.

### Exact compilation command

```bash
gcc -std=c11 -O2 -Wall -Wextra video_storage_sim.c -o video_storage_sim
```

### Run

```bash
./video_storage_sim
```

Or run both build and execution from the shell script:

```bash
chmod +x run_task6.sh
./run_task6.sh
```

### Reference execution result

| Metric | HDD-only | SSD cache + HDD origin |
|---|---:|---:|
| Requests | 10,000 | 10,000 |
| Mean response | 9.007 ms | 1.631 ms |
| p50 response | 9.011 ms | 0.339 ms |
| p95 response | 9.904 ms | 9.360 ms |
| p99 response | 9.979 ms | 9.886 ms |
| Maximum response | 10.000 ms | 10.000 ms |

The hybrid design reduced average simulated response by 81.9% (5.52× improvement). The p95 improves much less because 15% of requests remain cache misses and still use HDD. This is a useful design result: to improve tail latency, the start-up must increase the hot-cache hit ratio (or put more of the active catalogue on SSD), not just improve average latency.

These are program simulation outputs under the stated service-time assumptions. They are not physical measurements of the user's HDD or SSD.

## 7. Cloud Object Storage + CDN Alternative

Store the catalogue in cloud object storage and deliver segments through a CDN. The CDN serves cached segments from edge locations nearer to viewers and fetches a segment from the origin when it is not cached.

Illustrative monthly traffic assumption: each of 200,000 students watches one 1 GiB video in the month.

```text
Monthly transfer = 200,000 × 1 GiB
                 = 200,000 GiB
                 ≈ 195.3 TiB (about 214.7 TB decimal)

If 80% of viewing is in the first week:
First-week transfer ≈ 156.25 TiB
```

With 4 MB segments, one 1 GiB view produces roughly 269 segment requests; 200,000 views produce about 53.8 million viewer segment requests before CDN cache effects.

A CloudFront flat-rate pricing page currently lists a Premium base plan at USD 1,000/month with 50 TB of transfer and 5 TB of S3 storage included; the page says the transfer allowance can be configured up to 600 TB for a new flat-rate price. The illustrative 214.7 TB/month workload exceeds the standard 50 TB allowance, so the configured tier price must be confirmed with AWS. This is a reference plan example, not a final cloud quotation. Request charges, extra services, taxes and contract terms must be checked in the current calculator/quote.

Cloud playback start time depends on the student's RTT to the nearest edge, DNS/TLS setup, initial buffering, cache hit/miss and client bandwidth. The report shows a model with assumed RTTs of 20, 50 and 100 ms and a 2-second startup buffer; these are not measured city-network values.

## 8. Result and Recommendation

The recommended budget design is a three-node origin cluster with HDD RAID 10 for durable catalogue capacity, mirrored NVMe SSDs for hot video cache, two data replicas placed on separate nodes, extent-based files, 4 KiB filesystem blocks and 1 MiB read-ahead. Use segmented streaming with a cached manifest/time index to support minute-level seeking.

The simulation indicates that SSD hot caching sharply improves average startup response under the assumed workload. The p95 remains near the HDD path because cache misses remain; hot-set sizing and cache-hit monitoring are therefore critical.

Use cloud object storage plus CDN if the start-up prioritises rapid geographic reach and reduced hardware operations, but budget the large monthly data transfer carefully. A hybrid on-prem origin plus CDN can be a later scaling option.

## 9. Important Limitations

- The question does not specify catalogue size or viewing behaviour; values in the report are planning assumptions.
- Simulation response times come from the C model, not physical drives or a live network.
- Real costs depend on region, contract, storage tier, request count, cache-hit ratio and egress.
- City latency estimates are based on assumed RTTs and a simple startup-buffer model, not a measurement campaign.
- Production acceptance needs tests on the actual server, storage devices, network and representative client locations.
