#!/usr/bin/env bash
set -euo pipefail

echo "=== Build environment ==="
{
    echo "Compiler:"
    gcc --version
    echo
    echo "Distribution:"
    cat /etc/os-release
    echo
    echo "Kernel:"
    uname -a
} > environment.txt

echo
echo "=== Compile video workload simulator ==="
gcc -std=c11 -O2 -Wall -Wextra video_storage_sim.c -o video_storage_sim

echo
echo "=== Run two-design simulation ==="
./video_storage_sim | tee simulation_results.txt

echo
echo "Simulation completed. Results saved in simulation_results.txt."
