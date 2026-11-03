# Task 5 – Examine System Calls

## Student Details

| Field | Details |
|:---|:---|
| Name | Juhi Surin |
| Register Number | 192525148 |
| Course | CSA0403 – Operating Systems |
| Assignment | Assignment 1 – CO1 |
| Task | Task 5 – System Calls |
| Under the Supervision of | Dr. Velmurugan J. |

## Objective

To examine how application programs request operating-system services through system calls and understand the system-call execution mechanism.

## Description

This task explains the categories of system calls and how a program interacts with the operating-system kernel.

The major system-call categories include:

- Process Control
- File Management
- Device Management
- Information Maintenance
- Communication
- Protection

The execution sequence of the `read()` system call is also analysed, including the library call, trap, mode switch, kernel operation and return to user mode.

## Key Concepts

- System-call interface
- User mode and kernel mode
- Trap mechanism
- Parameter passing
- Registers and memory blocks
- File operations
- System-call tracing

## Commands

Trace system calls:

```bash
strace ./program
