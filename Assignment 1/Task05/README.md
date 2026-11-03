
# Task 5 – Examine System Calls

## Student Details

| Field | Details |
|:---|:---|
| Name | Juhi Surin |
| Register Number | 192525148 |
| Course | CSA0403 – Operating Systems |
| Assignment | Assignment 1 – CO1 |
| Task | Task 5 – Examine System Calls |
| Under the Supervision of | Dr. Velmurugan J. |

## Objective

To evaluate how system calls provide an interface between user applications and the operating-system kernel, and to analyse their categories, working mechanism and performance.

## 1. Introduction

A system call is a mechanism through which a user program requests services from the operating-system kernel. Programs cannot directly access protected hardware or kernel resources. Therefore, they use system calls to perform operations such as reading files, creating processes, accessing devices and communicating with other systems.

In a smart hospital, system calls are required for reading patient-monitor data, accessing patient records, communicating between hospital servers and controlling medical devices.

## 2. Categories of System Calls

System calls are classified into six major categories.

| Category | Purpose | Examples | Hospital Application |
|:---|:---|:---|:---|
| Process Control | Creates, executes and terminates processes. | `fork()`, `exec()`, `exit()`, `wait()` | Running patient-monitoring and database applications. |
| File Management | Creates, opens, reads, writes and closes files. | `open()`, `read()`, `write()`, `close()` | Reading and storing patient records and medical reports. |
| Device Management | Requests and controls device operations. | `ioctl()`, `read()`, `write()` | Communicating with bedside monitors and imaging devices. |
| Information Maintenance | Obtains or modifies system information. | `getpid()`, `time()`, `uname()` | Checking process IDs, system time and OS information. |
| Communication | Exchanges information between processes or systems. | `pipe()`, `socket()`, `send()`, `recv()` | Sharing patient records between partner hospitals. |
| Protection | Controls access to system resources. | `chmod()`, `chown()`, `access()` | Restricting unauthorised access to patient records. |

**Evaluation:** All six categories are important for the hospital. Process control and device management support medical applications, file management handles patient records, communication enables information sharing, and protection maintains patient-data security.

## 3. Working of the `read()` System Call

The `read()` system call is used to read data from a file or device.

**Syntax:**

```c
read(file_descriptor, buffer, size);
```

The working process is explained below:

1. **Library Call:** The application calls the `read()` function to request data from a file or device.
2. **Trap:** The system-call interface generates a trap or controlled transition to the kernel.
3. **Mode Switch:** The CPU changes from user mode to kernel mode.
4. **Kernel Work:** The kernel checks the file descriptor, validates the requested operation and reads the required data.
5. **Return:** The kernel returns the number of bytes read or an error indication. The CPU switches back to user mode.

**Example:** A bedside monitoring application may request sensor data using a read operation. The operating system processes the request and returns the data to the application.

**System-call flow:**

```text
Application Program
        |
        v
read() Library Wrapper
        |
        v
      Trap
        |
        v
   Kernel Mode
        |
        v
  Kernel Reads Data
        |
        v
Return to User Mode
        |
        v
Application Continues
```

## 4. How Parameters Are Passed to the Kernel

When a system call is executed, the required parameters must be passed from the application to the operating-system kernel.

| Method | Explanation |
|:---|:---|
| Registers | Parameters are stored directly in CPU registers. Suitable for a small number of parameters. |
| Block in Memory | Parameters are stored in a memory block, and its address is passed to the kernel. Useful for multiple parameters. |
| Stack | Parameters are placed on the program stack and accessed during the system-call process. |

**Evaluation:** Registers are efficient for passing a small number of parameters. Memory blocks are useful when several parameters must be transferred. The stack provides another method depending on the system-call interface and architecture.

## 5. How `strace` Shows System Calls

`strace` is a Linux command-line utility used to trace the system calls made by a program.

**Example command:**

```bash
strace ./readfile
```

To display only file-related system calls:

```bash
strace -e trace=openat,read,write,close ./readfile
```

**Sample output:**

```text
openat(AT_FDCWD, "patient.txt", O_RDONLY) = 3
read(3, "Patient record data", 100) = 19
close(3) = 0
```

**Explanation:**

- `openat()` – Opens the requested file.
- `read()` – Reads data from the file.
- `close()` – Closes the file descriptor.
- `= 3` – Indicates the returned file descriptor.
- `= 19` – Indicates that 19 bytes were read.
- `= 0` – Indicates successful completion of the close operation.

**Evaluation:** `strace` helps identify the number and sequence of system calls. It is useful for analysing file access, detecting errors and understanding application performance.

## 6. Alternative Concept and Complexity Analysis

**Alternative:** Using C library functions `fopen()`, `fread()` and `fwrite()` instead of directly using `open()`, `read()` and `write()`.

| Direct System Calls | C Library Functions |
|:---|:---|
| `open()` opens the file. | `fopen()` opens a file and creates a file stream. |
| `read()` reads data. | `fread()` reads data using library buffering. |
| `write()` writes data. | `fwrite()` writes data using library buffering. |
| `close()` closes the file. | `fclose()` closes the stream and flushes pending buffered data. |

Let:

- `n` = Total file size in bytes.
- `b` = Buffer size used by the direct system-call program.
- `4096` = Example library buffer size in bytes.

### Time and Space Complexity

**Direct system calls:**

- Approximate number of read operations: `ceil(n / b)`
- Extra buffer space: `O(b)`

**C library buffering:**

- Approximate number of underlying read operations: `ceil(n / 4096)`, assuming a 4096-byte buffer.
- Extra buffer space: `O(4096)` per buffered stream.

Both methods must process all `n` bytes, so overall data-processing time is `O(n)`. Library buffering can reduce transitions between user mode and kernel mode.

**Note:** The actual number of system calls depends on the library implementation, buffer configuration, file size and operating system. The 4096-byte value is an example, not a guaranteed buffer size.

### Justification

For the hospital's patient-record backup program, buffered file operations using `fread()` and `fwrite()` are suitable because they reduce repeated system calls and simplify file handling. Direct system calls are useful when precise control over file operations is required.

## 7. Conclusion

System calls provide the interface between application programs and the operating-system kernel. The six categories support process management, file handling, device access, information maintenance, communication and protection.

The `read()` system call demonstrates how a program requests kernel services through a controlled mode switch. The `strace` utility helps observe these operations. For large hospital backup files, library buffering can improve efficiency by reducing the number of system calls.
