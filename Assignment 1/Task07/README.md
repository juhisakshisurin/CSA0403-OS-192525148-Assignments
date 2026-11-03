
# Task 7 – Evaluate Process Control System Calls

## Student Details

| Field | Details |
|:---|:---|
| Name | Juhi Surin |
| Register Number | 192525148 |
| Course | CSA0403 – Operating Systems |
| Assignment | Assignment 1 – CO1 |
| Task | Task 7 – Evaluate Process Control System Calls |
| Under the Supervision of | Dr. Velmurugan J. |

## Objective

To evaluate process creation, execution, management and termination using process-control system calls such as `fork()`, `exec()`, `wait()` and `waitpid()`.

## 1. Introduction

Process control system calls allow an operating system to create, execute, manage and terminate processes.

In a hospital database server, the command shell uses these calls to start programs, monitor their execution and collect their exit status.

## 2. Functions of Process Control System Calls

| System Call | Purpose |
|:---|:---|
| `fork()` | Creates a new child process. |
| `exec()` | Replaces the current process image with a new program. |
| `wait()` | Waits for a child process to terminate. |
| `waitpid()` | Waits for a specific child process. |
| `exit()` | Terminates a process and performs standard library cleanup. |
| `_exit()` | Terminates a process without flushing standard I/O buffers. |
| `getpid()` | Returns the process ID of the calling process. |
| `getppid()` | Returns the parent process ID. |

## 3. How a Shell Runs a Command

A command shell generally follows these steps:

1. The shell receives a command from the user.
2. It calls `fork()` to create a child process.
3. The child process calls `exec()` to execute the requested command.
4. The parent shell calls `wait()` or `waitpid()` to wait for the child.
5. After execution, the child returns an exit status.
6. The parent collects the exit status and continues accepting commands.

The shell uses `WEXITSTATUS(status)` to obtain the exit code when the child terminates normally.

## 4. Zombie and Orphan Processes

### Zombie Process

A zombie process is a process that has completed execution, but its parent has not yet collected its exit status using `wait()` or `waitpid()`.

The operating system keeps a small process-table entry until the parent collects the status.

### Orphan Process

An orphan process is a child process whose parent terminates before it.

The operating system reassigns the orphan process to an appropriate system process, which can collect its exit status after it terminates.

## 5. Why `fork()` Followed by `exec()` Is Used

Using `fork()` followed by `exec()` provides flexibility.

- `fork()` creates a separate child process.
- The child can prepare its execution environment.
- `exec()` replaces the child with the required program.
- The parent process remains available to manage other commands.

A single process-creation operation would provide less flexibility for operations such as input/output redirection and setting up pipes before executing a command.

## 6. C Program Using `fork()`, `execlp()` and `waitpid()`

The C program demonstrates process creation, program execution and exit-status collection using `fork()`, `execlp()` and `waitpid()`.

**Compilation:**

```bash
gcc task7.c -o task7
```

**Execution:**

```bash
./task7
```

## 7. Observed Output

```text
shell: running job 1
42
job 1 exit status 0
shell: running job 2
job 2 exit status 3
```

### Output Explanation

- Job 1 executes the `expr` command to calculate `6 × 7`.
- The result is `42`.
- Exit status `0` indicates successful execution.
- Job 2 terminates with exit code `3`.
- The parent process collects both exit statuses using `waitpid()`.

## 8. Why `stdout` Is Flushed Before `fork()`

The `fflush(stdout)` function clears the standard output buffer before creating a child process.

When `fork()` is called, the child receives a copy of the parent's memory, including any buffered output.

If the buffer is not flushed, both the parent and child may flush the same buffered text, resulting in duplicate output when standard output is redirected to a file.

Therefore, flushing `stdout` before `fork()` prevents duplicate output.

### What Happens If `fflush()` Is Removed and `exit(3)` Is Used?

If the second `fflush(stdout)` is removed and `_exit(3)` is changed to `exit(3)`, the following can happen:

1. The message `shell: running job 2` remains in the output buffer.
2. `fork()` creates a child with a copy of that buffer.
3. The child calls `exit(3)`, which flushes its copied buffer.
4. The parent later terminates and flushes its own buffer.
5. The same message appears twice in the output file.

The `_exit()` function terminates the process without flushing standard I/O buffers, unlike `exit()`.

## 9. Alternative Concept and Complexity Analysis

**Alternative:** `posix_spawn()`

Instead of using `fork()` followed by `exec()`, the program can use `posix_spawn()` to create a new process and execute a program through a single interface.

### Advantages

- Can reduce process-startup overhead on some systems.
- Avoids requiring a separate application-level `fork()` and `exec()` sequence.
- Supports file actions and process attributes.

### Time Complexity

For launching `n` independent commands:

- `fork()` + `exec()`: `O(n)`
- `posix_spawn()`: `O(n)`
- Threads: `O(n)`

Here, `n` represents the number of commands or tasks. Actual execution time depends on implementation and system resources.

### Space Complexity

- Each process requires its own virtual address space and process-management information.
- Threads share code, data and heap memory, but each thread requires its own stack and thread-management information.

### Justification

`fork()` followed by `exec()` is suitable when a shell needs flexibility to configure child processes before execution. `posix_spawn()` is an alternative for directly launching programs.

## 10. Screenshot Evidence

The supplied terminal screenshots show:

- Compilation of `task7.c`.
- Execution of the first child process.
- The result `42`.
- Exit status `0` for Job 1.
- Exit status `3` for Job 2.
- Output redirection using `output.txt`.

![Task 7 – Process Execution](Task07_Process_Execution.png)

![Task 7 – Redirected Output](Task07_Redirected_Output.png)

## 11. Conclusion

Process-control system calls are essential for creating, executing and managing processes.

The experiment demonstrates how `fork()` creates child processes, `execlp()` executes a command and `waitpid()` collects the exit status.

The output confirms that the first child calculates `6 × 7 = 42` and exits successfully with status `0`, while the second child exits with status `3`.

Flushing standard output before `fork()` prevents duplicated buffered output when output is redirected to a file.

