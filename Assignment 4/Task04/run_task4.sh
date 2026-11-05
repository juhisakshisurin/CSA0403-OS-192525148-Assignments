#!/usr/bin/env bash
set -euo pipefail

echo "=== Capture execution environment ==="
{
    echo "GCC:"
    gcc --version
    echo
    echo "Linux distribution:"
    cat /etc/os-release
    echo
    echo "Kernel:"
    uname -a
    echo
    echo "Available block devices:"
    lsblk -d -o NAME,TYPE,ROTA,MODEL,SIZE
} > environment.txt

echo
echo "=== Compile supplied SSTF program with completed TODO 1 ==="
gcc -std=c11 -Wall -Wextra sstf_complete.c -o sstf_complete

echo
echo "=== Execute required sample calculation ==="
./sstf_complete | tee sample_output.txt

echo
echo "=== Compile 10,000-request simulator ==="
gcc -std=c11 -O2 -Wall -Wextra disk_scheduler_sim.c -o disk_scheduler_sim

echo
echo "=== Simulate all six algorithms at three load levels ==="
./disk_scheduler_sim | tee simulation_output.txt

echo
echo "=== Inspect active Linux disk scheduler ==="
if [[ -n "${DISK_DEVICE:-}" ]]; then
    ./check_ssd_scheduler.sh "$DISK_DEVICE" | tee ssd_scheduler_output.txt
else
    ./check_ssd_scheduler.sh | tee ssd_scheduler_output.txt
fi

echo
echo "Evidence saved:"
echo "  environment.txt"
echo "  sample_output.txt"
echo "  simulation_output.txt"
echo "  scheduler_simulation.csv"
echo "  ssd_scheduler_output.txt"
