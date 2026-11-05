#!/bin/sh
set -eu

echo "Recording compiler and Linux environment..."
{
    echo "=== GCC ==="
    gcc --version
    echo
    echo "=== Linux distribution ==="
    if command -v lsb_release >/dev/null 2>&1; then
        lsb_release -a
    else
        cat /etc/os-release
    fi
    echo
    echo "=== Kernel ==="
    uname -a
    echo
    echo "=== CPU count ==="
    nproc
} > environment.txt

echo "Compiling the supplied Banker's Algorithm program..."
gcc -std=c11 -Wall -Wextra bankers_algorithm.c -o bankers_algorithm

echo "Compiling the scaling benchmark..."
gcc -std=c11 -O2 -Wall -Wextra bankers_benchmark.c -o bankers_benchmark

echo
echo "=== Banker's safety and resource-request algorithm ==="
./bankers_algorithm | tee bankers_output.txt

echo
echo "=== Safety algorithm scaling: n = 10, 100, 1,000 ==="
./bankers_benchmark | tee bankers_benchmark_results.txt

echo
echo "Generated files:"
echo "  environment.txt"
echo "  bankers_output.txt"
echo "  bankers_benchmark_results.txt"
