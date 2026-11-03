
# Task 9 – Evaluate the Memory Hierarchy and I/O Systems

## Student Details

| Field | Details |
|:---|:---|
| Name | Juhi Surin |
| Register Number | 192525148 |
| Course | CSA0403 – Operating Systems |
| Assignment | Assignment 1 – CO1 |
| Task | Task 9 – Evaluate the Memory Hierarchy and I/O Systems |
| Under the Supervision of | Dr. Velmurugan J. |

## Objective

To evaluate memory hierarchy, storage placement and I/O techniques for efficient data handling in a smart-hospital environment.

## 1. Introduction

Memory hierarchy and I/O systems are important for managing data efficiently in a smart hospital.

The hospital generates different types of data, including MRI images, patient records and continuous bedside-monitor readings.

The operating system must select suitable storage devices and I/O methods to provide fast access, reliability and efficient data transfer.

## 2. Levels of Memory Hierarchy

Memory hierarchy organises storage devices based on speed, capacity and cost.

| Memory Level | Typical Access Time | Typical Capacity | Purpose |
|:---|:---|:---|:---|
| Registers | Less than 1 ns | Bytes to KB | Store immediate CPU data and instructions. |
| Cache Memory | Approximately 1–20 ns | KB to tens of MB | Store frequently accessed data. |
| Main Memory (RAM) | Approximately 50–100 ns | GBs | Store currently running programs and data. |
| SSD | Approximately 50–200 µs | Hundreds of GB to several TB | Store frequently accessed files and databases. |
| Hard Disk (HDD) | Approximately 5–15 ms | Several TB | Store large amounts of data and backups. |
| Network Storage | Approximately 1–100 ms or more | TBs to PBs | Store and share data across multiple systems. |

**Note:** These are approximate illustrative ranges. Actual access times and capacities depend on hardware, workload and network conditions.

### Evaluation

Registers and cache provide the fastest access but have very limited capacity.

RAM provides fast temporary storage for active applications. SSDs provide faster permanent storage than HDDs. HDDs are suitable for large-capacity storage at lower cost. Network storage allows multiple hospital systems to access shared records.

Therefore, a combination of these storage levels is more suitable than depending on a single storage device.

## 3. Storage Placement for Hospital Data

Different hospital data require different storage methods.

| Data Type | Recommended Storage | Justification |
|:---|:---|:---|
| MRI Images | SSD for active images and HDD/network storage for archives | MRI images require fast processing and large storage capacity. |
| Patient Records | SSD-based database storage with regular backups | Patient records require fast access, reliability and protection. |
| Monitor Readings | RAM buffers for immediate processing and SSD/database storage for permanent records | Monitor readings require quick processing and reliable long-term storage. |

### Explanation

**MRI Images:** Active images should be stored on SSDs for faster processing. Older images can be transferred to HDD or network storage for long-term archival.

**Patient Records:** Patient records should be stored in a reliable central database using SSD storage. Regular backups and replication should be maintained to prevent data loss.

**Monitor Readings:** Recent readings can be temporarily stored in RAM for immediate processing. Important readings and historical records should be saved to permanent storage.

This arrangement balances speed, capacity and reliability.

## 4. I/O Methods

The operating system uses different I/O methods to transfer data between devices and memory.

### 4.1 Programmed I/O

- The CPU continuously checks the device status.
- The CPU controls the data transfer.
- CPU time is wasted while waiting for the device.
- Suitable for simple devices and small data transfers.

### 4.2 Interrupt-Driven I/O

- The CPU starts an I/O operation and continues other work.
- The device generates an interrupt when it needs attention or completes an operation.
- The CPU executes an interrupt handler.
- More efficient than continuously checking the device.

### 4.3 Direct Memory Access (DMA)

- DMA allows a device to transfer data directly between the device and main memory.
- The CPU only initializes the transfer and handles completion notification.
- Reduces continuous CPU involvement.
- Suitable for transferring large amounts of data.

### Comparison of I/O Methods

| Feature | Programmed I/O | Interrupt-Driven I/O | DMA |
|:---|:---|:---|:---|
| CPU Involvement | High | Moderate | Low during data transfer |
| Data Transfer | CPU controlled | CPU handles data during interrupts | Direct device-memory transfer |
| Efficiency | Low for large transfers | Better for moderate transfers | High for large transfers |
| CPU Usage | High | Lower than programmed I/O | Generally lowest |
| Suitable Application | Simple devices | Keyboard and sensor events | MRI image transfer |

### Recommended Method for MRI Scanner

DMA is recommended for MRI image transfer because MRI scanners generate large amounts of image data.

DMA transfers data directly into main memory without requiring the CPU to handle every individual data unit.

This improves CPU availability and allows the processor to perform image processing and other hospital operations simultaneously.

## 5. Effect of Caching on Patient Record Lookup

Caching improves the speed of patient record lookup by storing frequently accessed data in faster memory.

**Example:**

1. A doctor requests a patient's medical record.
2. The database checks whether the required data is available in the cache.
3. If the data is available, it is returned directly from memory.
4. If the data is unavailable, the database retrieves it from SSD storage and may place it in the cache for future requests.

### Cache Hit

A cache hit occurs when the requested data is already available in the cache.

- Faster response time.
- Fewer storage accesses.
- Reduced database workload.

### Cache Miss

A cache miss occurs when the requested data is not available in the cache.

- The database must retrieve the data from storage.
- Response time increases.
- The retrieved data may be stored in the cache.

**Evaluation:** Caching is useful in the hospital database because doctors frequently access patient records. Frequently accessed records can be retrieved faster from memory rather than repeatedly accessing SSD storage.

However, sensitive patient information must be protected through access control, and cached records must be updated or invalidated when the original data changes.

## 6. Alternative Concept and Complexity Analysis

**Alternative:** Interrupt-Driven I/O Instead of DMA.

If the MRI scanner transfers a 500 MB image using interrupt-driven I/O with one interrupt per word instead of DMA, the CPU must handle a very large number of interrupts.

### Given

- Image size = 500 MB
- Word size = 4 bytes
- One interrupt per word

### Number of Words

`500 MB = 500 × 1024 × 1024 bytes`

`Number of words = (500 × 1024 × 1024) / 4`

`= 131,072,000 words`

Therefore:

`Number of interrupts = 131,072,000`

This means the CPU must handle approximately 131 million interrupts to transfer the image.

### CPU Time Calculation

Assume each interrupt requires 1 microsecond of CPU handling time.

`Total CPU time = Number of interrupts × Time per interrupt`

`= 131,072,000 × 1 microsecond`

`= 131.072 seconds`

Therefore, approximately 131 seconds of CPU time would be spent handling interrupts under this assumption.

**Note:** This is an illustrative calculation. Actual interrupt-handling time depends on the processor, operating system and device driver.

### DMA vs Interrupt-Driven I/O

| Feature | Interrupt-Driven I/O (One Interrupt per Word) | DMA |
|:---|:---|:---|
| Interrupt Count | Approximately 131 million | Usually a small number per transfer or block |
| CPU Involvement | Very high | Low during data transfer |
| Transfer Method | CPU handles each word | Device transfers data directly to RAM |
| CPU Time | High | Much lower |
| Suitable for MRI | Not recommended | Recommended |

### Memory Buffer Requirements

**Interrupt-Driven I/O:**

- Requires a data buffer to receive incoming image data.
- The CPU processes incoming words through interrupt handling.
- Buffer management is required while the image is being transferred.

**DMA:**

- Requires a memory buffer or DMA buffer in RAM.
- The DMA controller transfers data directly into the allocated memory region.
- The CPU is notified after the transfer completes.

### Time Complexity

For `n` bytes of data:

- Interrupt-driven I/O with one interrupt per word: `O(n)`, because each word requires interrupt handling.
- DMA: `O(n)` for transferring `n` bytes, but the CPU performs much less work because data movement is handled by DMA hardware.

### Space Complexity

Both methods require `O(n)` buffer space if the entire image is held in memory.

However, DMA can also use smaller block or streaming buffers instead of allocating memory for the entire image at once.

### Recommendation

DMA is the better choice for MRI image transfer because it reduces CPU involvement and avoids millions of individual interrupt operations. It allows the CPU to focus on image processing and other hospital activities.

## 7. Conclusion

Memory hierarchy and I/O systems play an important role in the performance of a smart hospital.

SSDs are recommended for active MRI images and patient records, while HDD and network storage are suitable for long-term storage and backups. RAM supports immediate processing of monitor readings.

DMA is recommended for MRI image transfers because it reduces CPU involvement, and caching improves patient-record lookup speed.

Therefore, combining suitable storage levels, DMA and caching provides an efficient and reliable hospital computing environment.

