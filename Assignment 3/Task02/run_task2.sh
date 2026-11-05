#!/bin/sh
set -eu

echo "=== Capturing Linux environment ==="
{
    echo "GCC:"
    gcc --version
    echo
    echo "Linux:"
    if command -v lsb_release >/dev/null 2>&1; then
        lsb_release -a
    else
        cat /etc/os-release
    fi
    echo
    echo "Kernel:"
    uname -a
    echo
    echo "Native page size:"
    getconf PAGESIZE
} > environment.txt

echo
echo "=== Compiling programs ==="
gcc -std=c11 -Wall -Wextra paging_translation.c -o paging_translation
gcc -std=c11 -Wall -Wextra paging_experiments.c -o paging_experiments
gcc -std=c11 -Wall -Wextra inverted_page_table_demo.c -o inverted_page_table_demo
gcc -std=c11 -O2 -Wall -Wextra paging_memory_probe.c -o paging_memory_probe

echo
echo "=== Address translation with 1 KB pages ==="
./paging_translation | tee translation_1kb.txt

echo
echo "=== Recompile and run with 2 KB pages ==="
gcc -std=c11 -Wall -Wextra -DPAGE_SIZE=2048 \
    paging_translation.c -o paging_translation_2kb
./paging_translation_2kb | tee translation_2kb.txt

echo
echo "=== Internal fragmentation and TLB EAT experiments ==="
./paging_experiments | tee paging_experiment_results.txt

echo
echo "=== Inverted page-table alternative demonstration ==="
./inverted_page_table_demo | tee inverted_page_table_output.txt

echo
echo "=== Linux native page size and page-fault information ==="
getconf PAGESIZE | tee linux_page_size.txt
/usr/bin/time -v ./paging_memory_probe \
    > paging_memory_probe_output.txt 2> page_faults_time.txt
grep -E 'page faults|Maximum resident set size|Elapsed' page_faults_time.txt | tee page_fault_summary.txt

echo
echo "All Task 2 experiments completed."
