#!/bin/sh
set -eu

echo "=== Compiler and environment ==="
{
    echo "GCC:"
    gcc --version
    echo
    echo "Linux distribution:"
    if command -v lsb_release >/dev/null 2>&1; then
        lsb_release -a
    else
        cat /etc/os-release
    fi
    echo
    echo "Kernel:"
    uname -a
} > environment.txt

echo
echo "=== Compile file allocation program ==="
gcc -std=c11 -Wall -Wextra file_allocation.c -o file_allocation

echo
echo "=== Execute program ==="
./file_allocation | tee file_allocation_output.txt

echo
echo "=== Expected output comparison ==="
cat > expected_output.txt <<'EOF'
AAAAAA........CCCCC.............
E (10 blocks) contiguous: start 19
E (10 blocks) indexed: index block 6, data blocks 7 8 9 10 11 12 13 29 30 31
AAAAAAeEEEEEEECCCCCEEEEEEEEEEEEE
EOF

if diff -u expected_output.txt file_allocation_output.txt; then
    echo "PASS: output exactly matches the expected output."
else
    echo "FAIL: output differs from the expected output."
    exit 1
fi

echo
echo "Evidence saved: file_allocation_output.txt, expected_output.txt, environment.txt"
