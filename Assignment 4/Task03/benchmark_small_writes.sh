#!/usr/bin/env bash
set -euo pipefail

OUT_DIR="${OUT_DIR:-task3_benchmark_evidence}"
RUNTIME="${RUNTIME:-10}"
FILE_SIZE="${FILE_SIZE:-128m}"
BLOCK_SIZE="${BLOCK_SIZE:-4k}"
mkdir -p "$OUT_DIR"

if ! command -v fio >/dev/null 2>&1; then
    echo "fio is not installed. Install it with: sudo apt update && sudo apt install fio"
    exit 2
fi

if [[ -z "${EXT4_DIR:-}" || -z "${COW_DIR:-}" ]]; then
    cat <<'HELP'
Set both paths to EXISTING writable directories on the two filesystems being compared.
Do not pass raw device paths. Example:
  EXT4_DIR=/mnt/ext4 COW_DIR=/mnt/btrfs ./benchmark_small_writes.sh

The script creates a temporary test file under each directory and removes it afterward.
It does not format disks, create snapshots, or change mount options.
HELP
    exit 2
fi

run_one() {
    local label="$1"
    local base="$2"
    if [[ ! -d "$base" || ! -w "$base" ]]; then
        echo "Skipping $label: directory is missing or not writable: $base" | tee -a "$OUT_DIR/status.txt"
        return
    fi

    local tmp
    tmp="$(mktemp -d "${base%/}/.task3-fio-${label}.XXXXXX")"
    echo "Running $label test in $tmp" | tee -a "$OUT_DIR/status.txt"

    # 4 KiB random writes, synchronous engine, fdatasync after each write.
    # Direct I/O reduces page-cache influence; filesystem/device settings still matter.
    if fio \
        --name="${label}_4k_random_write" \
        --filename="$tmp/benchfile.bin" \
        --size="$FILE_SIZE" \
        --rw=randwrite \
        --bs="$BLOCK_SIZE" \
        --ioengine=psync \
        --iodepth=1 \
        --direct=1 \
        --fsync=1 \
        --runtime="$RUNTIME" \
        --time_based=1 \
        --group_reporting=1 \
        --output-format=json \
        --output="$OUT_DIR/${label}_fio.json"; then
        echo "Completed direct-I/O test for $label." | tee -a "$OUT_DIR/status.txt"
    else
        echo "Direct-I/O test failed for $label. No buffered fallback was used so results remain comparable." \
            | tee -a "$OUT_DIR/status.txt"
    fi

    rm -rf -- "$tmp"
}

: > "$OUT_DIR/status.txt"
{
    echo "Test time: $(date -Is)"
    echo "fio version: $(fio --version)"
    echo "Runtime: ${RUNTIME}s; file size: $FILE_SIZE; block size: $BLOCK_SIZE"
    echo "EXT4_DIR=$EXT4_DIR"
    echo "COW_DIR=$COW_DIR"
    echo
    echo "=== Filesystem identities ==="
    findmnt -T "$EXT4_DIR" -o TARGET,SOURCE,FSTYPE,OPTIONS
    findmnt -T "$COW_DIR" -o TARGET,SOURCE,FSTYPE,OPTIONS
} | tee "$OUT_DIR/benchmark_environment.txt"

run_one "ext4" "$EXT4_DIR"
run_one "cowfs" "$COW_DIR"

echo "JSON results, if successful, are in $OUT_DIR/."
echo "Compare write IOPS, bandwidth and clat percentiles; repeat each test several times."
