#!/bin/sh
set -eu

echo "=== Capture system environment ==="
{
    echo "GCC:"
    gcc --version
    echo
    echo "Kernel:"
    uname -a
    echo
    echo "Native page size:"
    getconf PAGESIZE
    echo
    echo "Memory:"
    grep -E 'MemTotal|MemAvailable|SwapTotal|SwapFree' /proc/meminfo
} > environment.txt

echo
echo "=== Compile experiments ==="
gcc -std=c11 -O2 -Wall -Wextra memory_access_bench.c -o memory_access_bench
gcc -std=c11 -O2 -Wall -Wextra cow_fork_demo.c -o cow_fork_demo
gcc -std=c11 -O2 -Wall -Wextra frame_faults.c -o frame_faults

echo
echo "=== Sequential/random page-touch benchmark ==="
: > memory_access_results.txt
SIZES="${MEM_SIZES_MB:-32 64 128}"
AVAILABLE_MB=$(awk '/MemAvailable:/ {print int($2/1024)}' /proc/meminfo)
SAFE_LIMIT_MB=$((AVAILABLE_MB / 2))
echo "MemAvailable=${AVAILABLE_MB} MiB; conservative automatic test limit=${SAFE_LIMIT_MB} MiB" | tee -a memory_access_results.txt

for size in $SIZES; do
    if [ "${ALLOW_MEMORY_PRESSURE:-0}" != "1" ] && [ "$size" -gt "$SAFE_LIMIT_MB" ]; then
        echo "Skipping ${size} MiB: exceeds conservative half-MemAvailable limit." | tee -a memory_access_results.txt
        continue
    fi
    echo "--- ${size} MiB ---" | tee -a memory_access_results.txt
    ./memory_access_bench "$size" | tee -a memory_access_results.txt
done

echo
echo "=== Copy-on-write fork() demonstration ==="
./cow_fork_demo "${COW_SIZE_MB:-64}" | tee cow_fork_results.txt

echo
echo "=== Page faults against number of frames ==="
./frame_faults | tee frame_faults_output.txt

echo
echo "=== Plot page faults ==="
if command -v python3 >/dev/null 2>&1 && python3 -c 'import matplotlib' >/dev/null 2>&1; then
    python3 plot_faults.py
else
    echo "matplotlib not found. Install with: sudo apt install python3-matplotlib"
    echo "Then run: python3 plot_faults.py"
fi

echo
echo "Completed Task 4 experiments."
echo "For a beyond-available-RAM test, use a dedicated memory-limited VM and see README.md."
