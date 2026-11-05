#!/usr/bin/env bash
set -euo pipefail

OUT_DIR="${OUT_DIR:-disk_evidence}"
mkdir -p "$OUT_DIR"

echo "Collecting Linux storage layout..."
{
    echo "=== Date / Host ==="
    date
    hostname
    echo
    echo "=== lsblk ==="
    lsblk -o NAME,SIZE,TYPE,FSTYPE,MOUNTPOINTS,ROTA,MODEL
    echo
    echo "=== Swap ==="
    swapon --show --bytes || true
    echo
    cat /proc/swaps
} | tee "$OUT_DIR/disk_layout.txt"

echo
echo "Collecting fdisk -l output (may request sudo password)..."
if command -v sudo >/dev/null 2>&1; then
    sudo fdisk -l 2>&1 | tee "$OUT_DIR/fdisk_output.txt" || true
else
    fdisk -l 2>&1 | tee "$OUT_DIR/fdisk_output.txt" || true
fi

echo
echo "Collecting filesystem usage..."
df -hT | tee "$OUT_DIR/filesystem_usage.txt"

echo
echo "Calculating 6 × 4 TB RAID capacities and illustrative failure probabilities..."
python3 raid_calculator.py \
    --disks "${DISKS:-6}" \
    --size-tb "${DISK_SIZE_TB:-4}" \
    --failure-probability "${DISK_FAILURE_PROBABILITY:-0.02}" \
    | tee "$OUT_DIR/raid_calculations.txt"

benchmark_read() {
    local label="$1"
    local dir="$2"
    local size_mb="${TEST_SIZE_MB:-128}"

    if [[ ! -d "$dir" || ! -w "$dir" ]]; then
        echo "Skipping ${label}: directory not found or not writable: ${dir}" \
            | tee -a "$OUT_DIR/read_benchmark_status.txt"
        return 0
    fi

    local tmp
    tmp=$(mktemp "${dir%/}/.task6_read_test.XXXXXX")
    echo "${label}: creating ${size_mb} MiB test file at ${dir}"
    if ! dd if=/dev/zero of="$tmp" bs=1M count="$size_mb" conv=fdatasync status=progress \
            2> "$OUT_DIR/${label}_write.txt"; then
        echo "${label}: test-file creation failed" | tee -a "$OUT_DIR/read_benchmark_status.txt"
        rm -f "$tmp"
        return 0
    fi

    echo "${label}: direct read (bypasses page cache where supported)"
    {
        echo "Device label: ${label}"
        echo "Test file size: ${size_mb} MiB"
        if dd if="$tmp" of=/dev/null bs=4M iflag=direct status=progress \
                2> "$OUT_DIR/${label}_read_direct.txt"; then
            cat "$OUT_DIR/${label}_read_direct.txt"
        else
            echo "Direct I/O not supported by this filesystem; trying buffered read."
            dd if="$tmp" of=/dev/null bs=4M status=progress \
                2> "$OUT_DIR/${label}_read_buffered.txt" || true
            cat "$OUT_DIR/${label}_read_buffered.txt" 2>/dev/null || true
        fi
    } | tee "$OUT_DIR/${label}_read_result.txt"

    rm -f "$tmp"
}

: > "$OUT_DIR/read_benchmark_status.txt"
if [[ -n "${SSD_DIR:-}" ]]; then
    benchmark_read "SSD" "$SSD_DIR"
else
    echo "SSD benchmark skipped. Set SSD_DIR to a writable directory on the SSD." \
        | tee -a "$OUT_DIR/read_benchmark_status.txt"
fi

if [[ -n "${HDD_DIR:-}" ]]; then
    benchmark_read "HDD" "$HDD_DIR"
else
    echo "HDD benchmark skipped. Set HDD_DIR to a writable directory on the hard disk." \
        | tee -a "$OUT_DIR/read_benchmark_status.txt"
fi

echo
echo "Evidence saved in: $OUT_DIR/"
echo "No partitioning, formatting, RAID creation, or swap changes were performed."
