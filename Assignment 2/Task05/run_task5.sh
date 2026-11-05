#!/bin/sh
set -eu

echo "Capturing environment..."
{
  echo "=== GCC ==="; gcc --version
  echo; echo "=== Linux distribution ==="
  if command -v lsb_release >/dev/null 2>&1; then lsb_release -a; else cat /etc/os-release; fi
  echo; echo "=== Kernel ==="; uname -a
  echo; echo "=== CPU count ==="; nproc
} > environment.txt

echo "Compiling Task 5 programs..."
gcc -std=c11 -Wall -Wextra -pthread bounded_buffer.c -o bounded_buffer
gcc -std=c11 -Wall -Wextra -pthread peterson_demo.c -o peterson_demo
gcc -std=c11 -O2 -Wall -Wextra -pthread booking_locks.c -o booking_locks
gcc -std=c11 -O2 -Wall -Wextra -pthread atomic_spin_benchmark.c -o atomic_spin_benchmark

echo
echo "=== Bounded buffer: 1 producer / 1 consumer ==="
./bounded_buffer 1 1 | tee bounded_buffer_1p1c.txt

echo
echo "=== Bounded buffer: 4 producers / 4 consumers ==="
./bounded_buffer 4 4 | tee bounded_buffer_4p4c.txt

echo
echo "=== Repeat original 1P/1C bounded-buffer test 100 times ==="
: > bounded_buffer_100_runs.txt
i=1
while [ "$i" -le 100 ]; do
  output=$(./bounded_buffer 1 1)
  printf 'Run %03d: %s\n' "$i" "$output" >> bounded_buffer_100_runs.txt
  echo "$output" | grep -q 'sum=210 (expected 210)' || {
    echo "FAIL: incorrect sum at run $i"; exit 1;
  }
  echo "$output" | grep -q 'each item exactly once: PASS' || {
    echo "FAIL: item uniqueness check at run $i"; exit 1;
  }
  i=$((i + 1))
done
echo "100/100 runs passed; sum was 210 every time." | tee -a bounded_buffer_100_runs.txt

echo
echo "=== Peterson two-thread demonstration ==="
./peterson_demo | tee peterson_output.txt

echo
echo "=== Booking lock-granularity throughput ==="
./booking_locks | tee booking_lock_results.txt

echo
echo "=== Atomic / spin lock / mutex benchmark ==="
./atomic_spin_benchmark | tee atomic_spin_results.txt

echo
echo "Completed. See the generated .txt files and environment.txt."
echo "Optional unsafe experiment:"
echo "  gcc -DUNSAFE_NO_MUTEX -std=c11 -Wall -Wextra -pthread bounded_buffer.c -o bounded_buffer_unsafe"
echo "  ./bounded_buffer_unsafe 4 4"
