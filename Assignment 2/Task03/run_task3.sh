#!/bin/sh
set -eu

echo "Compiling POSIX threads camera workload..."
gcc -std=c11 -O2 -Wall -Wextra -pthread camera_threads.c -o camera_threads

echo
echo "Running benchmark. This may take a little while..."
./camera_threads | tee benchmark_results.txt

echo
echo "Saved output to benchmark_results.txt"
