#!/bin/sh
set -eu

echo "=== Capture environment ==="
{
    echo "GCC:"
    gcc --version
    echo
    echo "Linux distribution:"
    if command -v lsb_release >/dev/null 2>&1; then lsb_release -a; else cat /etc/os-release; fi
    echo
    echo "Kernel:"
    uname -a
    echo
    echo "Native page size:"
    getconf PAGESIZE
    echo
    echo "RAM and swap:"
    grep -E 'MemTotal|MemAvailable|SwapTotal|SwapFree' /proc/meminfo
    swapon --show
    echo
    echo "Memlock limit:"
    ulimit -l
} > environment.txt

echo
echo "=== Compile programs ==="
gcc -std=c11 -O2 -Wall -Wextra realtime_lock_bench.c -o realtime_lock_bench
gcc -std=c11 -O2 -Wall -Wextra task_memory_sim.c -o task_memory_sim

echo
echo "=== Locked / unlocked real-time loop measurement ==="
./realtime_lock_bench "${RT_TEST_MIB:-4}" | tee realtime_measurements.txt

echo
echo "=== Three-task page-replacement and memory-budget simulation ==="
./task_memory_sim | tee task_memory_simulation.txt

echo
echo "All Task 7 experiments completed."
echo "Review realtime_measurements.txt, task_memory_simulation.txt and environment.txt."
