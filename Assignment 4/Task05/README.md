# Assignment 4 – Task 5: Distributed and Cloud-Based Storage

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |
| **Assignment** | Assignment 4 – CO4 |
| **Task** | Task 5 – Distributed and Cloud-Based Storage |
| **Marks / Bloom's Level** | 7 Marks / K5 – Evaluate |

---

## 1. Task Overview

The question describes a university that needs student and staff files to be available from any campus and from home.

Task 5 requires:

- Compare NFS, a distributed file system such as HDFS or Ceph, and cloud object storage such as Amazon S3 or Google Cloud Storage for consistency, availability, performance and cost.
- Explain how replication (for example, three copies in HDFS) protects data.
- Explain the trade-off between strong and eventual consistency.
- Explain the operating system's storage-device management: device drivers, buffering, caching, spooling and the block-device interface.
- Evaluate the alternative of using one large NFS server instead of a replicated distributed file system, including read/write response as users grow, storage for one copy versus three replicas, and availability if one server fails.
- Include pseudocode and Big-O comparison for the NFS alternative, following the assignment's general instruction for alternative-concept analysis.

**This task does not contain a C "Complete the Code" block and does not require a C program.** Pseudocode is supplied for the single-NFS-server alternative. There is therefore no GCC compilation command or program execution procedure for Task 5.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment4_Task05_Report.pdf` | Complete report without student identity details. Covers Tasks 5.1–5.3, the alternative concept, equations, pseudocode, complexity and recommendation. |
| `README.md` | This file, with student details in a table and a summary of the solution. |
| `single_nfs_alternative_pseudocode.txt` | Copyable pseudocode for a single-server NFS design and a three-replica distributed design. |
| `comparison_calculation_worksheet.txt` | Worksheet for applying the capacity, queueing, cost and availability equations to actual university workload data. |

## 3. Comparison Summary

| Option | Best fit | Main limitation |
|---|---|---|
| NFS | Familiar shared-file access with POSIX-style file operations and central administration. | A single NFS server can become a throughput bottleneck and a single point of failure unless a high-availability design is added. |
| Distributed file system | Scale-out shared storage across multiple nodes. CephFS is suitable for general POSIX-style university file sharing; HDFS is more suitable for large-file, high-throughput analytics workloads. | More infrastructure, network, metadata and cluster-management complexity. |
| Cloud object storage | Large-scale, API-based storage available across locations without operating the storage hardware. | Object APIs are not identical to a normal POSIX shared file system; network latency, request charges and data-transfer charges must be considered. |

The report distinguishes the storage interfaces. CephFS is a POSIX-compliant distributed file system. HDFS is designed for large files and high-throughput access patterns. S3 and Google Cloud Storage are object stores accessed through service APIs.

## 4. Replication and Capacity Calculation

For logical data volume `D`:

```text
One stored copy:       raw capacity = D
Three full replicas:   raw capacity = 3D
Extra replication:     raw overhead = 2D
```

Example using an illustrative logical data set of 10 TB:

```text
Single copy:       10 TB raw
Three replicas:    30 TB raw
Extra capacity:    20 TB raw
```

The example is only a capacity illustration, not a university storage requirement. Real deployments also need room for metadata, temporary writes, snapshots, growth and failure-recovery operations.

With three copies on separate nodes, one node failure leaves two good copies. The system can continue serving data from the remaining copies and then create a replacement replica. Actual availability also depends on replica placement, metadata-service health, quorum and network availability.

## 5. Strong Versus Eventual Consistency

- **Strong consistency:** after a successful write, a subsequent read returns the latest committed value. This simplifies collaboration but may require coordination among replicas.
- **Eventual consistency:** a read may temporarily return an older replica value while updates propagate. Replicas converge if updates stop and communication resumes. This can improve availability/latency in some architectures, but applications must tolerate stale reads or resolve conflicts.

Do not assume that every modern cloud object service is eventually consistent. Current Amazon S3 and Google Cloud Storage documentation describes strong read-after-write and listing consistency for object operations. Client-side caching, public caches, permissions propagation or application-level multi-object workflows can still introduce separate delay or coordination issues.

## 6. Single NFS Server – Scaling Model

Let:

- `U` = number of active users.
- `λ0` = average I/O requests generated per user per second.
- `λ = U × λ0` = aggregate request arrival rate.
- `μ` = one NFS server's sustainable I/O service rate.
- `ρ = λ / μ` = server utilisation, where `ρ < 1` is required for a stable simple queue.

A simplified single-server queue gives:

```text
Mean response time W = 1 / (μ - λ)
                     = S / (1 - ρ)
```

where `S = 1/μ` is the mean service time when the server is idle.

Illustrative normalised response time:

| Server utilisation | Mean response time |
|---:|---:|
| 50% | 2 × idle service time |
| 80% | 5 × idle service time |
| 95% | 20 × idle service time |

This is a teaching queue model, not a benchmark of a real NFS appliance. It shows why latency rises sharply as a single server approaches saturation. A distributed cluster can raise aggregate capacity by spreading work across nodes, provided metadata, network and coordination layers do not become the bottleneck.

## 7. One Copy Versus Three Replicas

For a data set of size `D`:

| Design | Raw data capacity | Failure behaviour |
|---|---:|---|
| Single NFS server, one copy | `D` | If the server or its storage fails, the share is unavailable until recovery unless a separate HA/replication solution exists. |
| Distributed, three replicas | Approximately `3D` | A node failure can be served from remaining replicas; the system rebuilds the missing copy when safe capacity is available. |

A three-replica design spends about twice as much raw capacity as a single copy (before metadata, free-space reserve and backups). Replication improves availability but is not a backup against accidental deletion, malware or logically corrupted data that is replicated to all copies.

## 8. Alternative Concept – Single NFS Server Pseudocode

The separate text file contains pseudocode for:

1. One NFS server handling all client reads and writes through one service/queue.
2. A three-replica distributed storage system that selects healthy replicas for reads and pipelines writes to replicas.

The analysis compares:

- Single-server request selection: approximately O(1) per request once the path/metadata entry is located, but queue delay grows with aggregate load.
- Cluster request routing: metadata/placement lookup plus selecting a healthy replica; actual cost depends on the metadata index and cluster topology.
- Storage: `D` versus `3D`.
- Availability: one central server failure versus continued service from remaining replicas, subject to metadata/quorum health.

## 9. Device Management Concepts

| Component | Role in storage I/O |
|---|---|
| Device driver | Translates generic operating-system I/O requests into device-specific commands and handles completions/errors. |
| Buffering | Temporarily holds data to absorb speed differences, combine small writes or handle alignment/transfer constraints. |
| Caching | Keeps frequently used data/metadata in RAM to avoid repeated device or network reads; dirty write-back data must be flushed safely. |
| Spooling | Queues work for a device/service to process in order. In storage systems, asynchronous writeback and request queues are related queueing mechanisms; classic spooling is most visible for printers. |
| Block-device interface | Exposes storage as addressable fixed-size logical blocks. The file system maps file offsets to blocks and submits block I/O through the kernel block layer and device driver. |

## 10. Cost Model

The question provides no logical data volume, user count, request rate, cloud region, service tier, transfer volume or supplier quotation. Therefore, an exact monthly cost cannot be calculated from the assignment alone.

Use these cost structures:

```text
NFS monthly cost =
  storage(D) + NFS server(s) + network + backup + administration

Distributed three-replica cost =
  storage(3D) + storage nodes + network + metadata/monitoring
  + backup + administration

Cloud object monthly cost =
  stored GB-month + request charges + data retrieval charges (if any)
  + internet/region egress + API/management services
```

Use provider price calculators and the university's measured data to fill the worksheet; do not insert unsupported prices.

## 11. Result and Recommendation

For general university documents and shared folders, a distributed POSIX file system such as CephFS is a strong fit because users expect normal directory, file and permission semantics while the storage capacity and data service can scale across nodes. Three-way replication gives a clear node-failure tolerance at the cost of approximately three times the raw data capacity. A managed cloud object store is an attractive option for remote access and large unstructured data when the university can use an application, portal or sync layer instead of requiring a POSIX filesystem interface.

A single NFS server is simpler and uses one primary copy, but read/write latency increases as its request queue approaches capacity, and the server is a single point of failure without HA. If NFS is chosen for simplicity, use a redundant NFS service with redundant storage and tested failover rather than one unprotected machine.

## 12. Technical References

The PDF report lists current vendor and protocol documentation for NFSv4 caching/delegations, CephFS, HDFS replication/rack awareness, Amazon S3 and Google Cloud Storage consistency, and Linux block-layer caching and request queues.
