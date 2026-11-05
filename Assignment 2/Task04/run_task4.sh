#!/bin/sh
set -eu

echo "Capturing compiler and Linux environment..."
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
} > environment.txt

echo "Compiling the supplied pipe example..."
gcc -std=c11 -Wall -Wextra pipe_readings.c -o pipe_readings

echo "Compiling the IPC transfer benchmark..."
gcc -std=c11 -Wall -Wextra ipc_benchmark.c -o ipc_benchmark -pthread -lrt

echo
echo "=== Supplied pipe example ==="
./pipe_readings | tee pipe_readings_output.txt

echo
echo "=== Pipe vs shared-memory transfer benchmark ==="
./ipc_benchmark | tee ipc_benchmark_results.txt

echo
echo "Done. Generated:"
echo "  environment.txt"
echo "  pipe_readings_output.txt"
echo "  ipc_benchmark_results.txt"
