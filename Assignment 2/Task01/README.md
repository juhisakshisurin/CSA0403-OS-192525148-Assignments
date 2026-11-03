# Assignment 2 – Task 1: Process Creation and Process Management

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 – Operating Systems |
| **Assignment** | Assignment 2 – Task 1 |
| **Date** | 03-11-2026 |
| **Under the Supervision of** | Dr. Velmurugan J |

---

## 1. Aim

To understand process creation, parent-child relationships, process execution and process termination in a Linux operating system using the `fork()` and `waitpid()` system calls in C.

## 2. Description

This program demonstrates the creation and management of child processes using the `fork()` system call.

The parent process creates three child processes. Each child process displays its Process ID (PID) and Parent Process ID (PPID), remains active for 8 seconds for process observation, and then terminates.

The parent process uses `waitpid()` to wait for each child process and collect its termination status. Finally, the parent process displays a completion message.

## 3. Files Included

| File Name | Description |
|---|---|
| `process_states.c` | C source code implementing process creation and management. |
| `run_task1.sh` | Shell script for compiling, executing and collecting process information. |
| `Task1_Report.pdf` | Detailed assignment report. |
| `program_output.txt` | Captured output of the C program, if generated. |
| `process_observation.txt` | Process observation details using Linux commands, if generated. |
| `environment.txt` | Compiler and operating system information, if generated. |

## 4. Technologies and System Calls Used

- **Programming Language:** C
- **Operating System:** Ubuntu Linux (WSL 2)
- **Compiler:** GCC

### System Calls and Functions

| Function | Purpose |
|---|---|
| `fork()` | Creates a new child process. |
| `getpid()` | Retrieves the current process ID. |
| `getppid()` | Retrieves the parent process ID. |
| `waitpid()` | Waits for a specific child process to terminate. |
| `sleep()` | Keeps child processes active for a specified duration. |
| `_exit()` | Terminates a child process. |

## 5. Compilation and Execution

### Prerequisites

- Ubuntu Linux or another compatible Linux environment.
- GCC compiler installed.

Install GCC if required:

```bash
sudo apt update
sudo apt install build-essential -y
```

### Step 1: Navigate to the project directory

```bash
cd /mnt/c/Users/ashis/Downloads
```

### Step 2: Compile the C program

```bash
gcc -Wall -Wextra -std=c11 process_states.c -o process_states
```

### Step 3: Execute the program

```bash
./process_states
```

### Alternative: Execute using the shell script

If `run_task1.sh` is included in the project folder, execute:

```bash
chmod +x run_task1.sh
./run_task1.sh
```

## 6. Sample Execution Output

The following output was obtained during successful execution:

```text
Parent process PID: 926
Parent created child 1 with PID=927
Child 1 started: PID=927, PPID=926
Parent created child 2 with PID=928
Child 2 started: PID=928, PPID=926
Parent created child 3 with PID=929
Child 3 started: PID=929, PPID=926
Child 1 (PID=927) terminating.
Child 2 (PID=928) terminating.
Child 3 (PID=929) terminating.
Parent reaped child PID=927, exit status=0
Parent reaped child PID=928, exit status=0
Parent reaped child PID=929, exit status=0
Parent process completed.
```

**Note:** Process IDs may differ in subsequent executions. The output above represents the actual successful execution captured during testing.

## 7. Execution Verification

| Parameter | Observed Result |
|---|---|
| Parent Process ID | 926 |
| Number of Child Processes | 3 |
| Child Process 1 PID | 927 |
| Child Process 2 PID | 928 |
| Child Process 3 PID | 929 |
| Parent-child relationship | Verified |
| Child termination | Successful |
| Exit status of all children | 0 |
| Parent process completion | Successful |

## 8. Process Observation

The program can be observed using Linux process-monitoring commands.

### Display process information

```bash
ps -o pid,ppid,state,comm -C process_states
```

### View detailed process information

```bash
cat /proc/<PID>/status
```

Replace `<PID>` with the actual process ID obtained during execution.

These commands can be used while the child processes are active to inspect their process IDs, parent process IDs and execution states.

## 9. Expected Outcome

The program successfully demonstrates the creation of three child processes using `fork()`, establishes parent-child process relationships, and terminates the child processes safely using `waitpid()`.

## 10. Result

The C program was successfully compiled and executed in Ubuntu Linux using GCC. Three child processes were created, executed and terminated successfully. The parent process collected their exit statuses and completed execution without errors.
