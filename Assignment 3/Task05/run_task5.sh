#!/bin/sh
set -eu

echo "=== Capture environment ==="
{
    echo "GCC:"
    gcc --version
    echo
    echo "Kernel:"
    uname -a
    echo
    echo "CPU count:"
    nproc
} > environment.txt

echo
echo "=== Compile page-replacement program ==="
gcc -std=c11 -O2 -Wall -Wextra page_replacement.c -o page_replacement

echo
echo "=== Run fixed and experimental reference strings ==="
./page_replacement | tee page_replacement_output.txt

echo
echo "=== Generate page-fault comparison plot ==="
if command -v python3 >/dev/null 2>&1 && python3 -c 'import matplotlib' >/dev/null 2>&1; then
    python3 plot_page_faults.py
else
    echo "matplotlib is not installed."
    echo "Install it with: sudo apt update && sudo apt install python3-matplotlib"
    echo "Then run: python3 plot_page_faults.py"
fi

echo
echo "Generated:"
echo "  page_replacement_output.txt"
echo "  faults_locality.csv"
echo "  faults_random.csv"
echo "  runtime_100k.csv"
echo "  page_fault_comparison.png (if matplotlib is installed)"
