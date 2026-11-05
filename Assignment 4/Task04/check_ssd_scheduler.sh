#!/usr/bin/env bash
set -euo pipefail

echo "=== Block devices ==="
lsblk -d -o NAME,TYPE,ROTA,MODEL,SIZE

echo
if [[ $# -gt 0 ]]; then
    DEV="$1"
    DEV="${DEV##*/}"
    if [[ ! -e "/sys/block/${DEV}/queue/scheduler" ]]; then
        echo "No scheduler file found for /sys/block/${DEV}."
        echo "Check the device name with lsblk -d."
        exit 2
    fi

    echo "=== Scheduler for ${DEV} ==="
    cat "/sys/block/${DEV}/queue/scheduler"
    if [[ -r "/sys/block/${DEV}/queue/rotational" ]]; then
        printf "rotational="
        cat "/sys/block/${DEV}/queue/rotational"
    fi
else
    found=0
    for path in /sys/block/*/queue/scheduler; do
        [[ -e "$path" ]] || continue
        found=1
        dev="$(basename "$(dirname "$(dirname "$path")")")"
        echo
        echo "=== Scheduler for ${dev} ==="
        cat "$path"
        if [[ -r "/sys/block/${dev}/queue/rotational" ]]; then
            printf "rotational="
            cat "/sys/block/${dev}/queue/rotational"
        fi
    done

    if [[ "$found" -eq 0 ]]; then
        echo "No block-device scheduler files are visible in this environment."
        echo "This can happen in restricted containers or some virtualised environments."
    fi
fi

echo
echo "The scheduler in square brackets is the active scheduler."
echo "This script is read-only; it does not change the scheduler."
