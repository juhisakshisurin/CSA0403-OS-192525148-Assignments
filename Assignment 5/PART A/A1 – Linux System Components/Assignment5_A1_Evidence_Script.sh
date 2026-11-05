#!/usr/bin/env bash
# Assignment 5 Part A - A1. Read-only Linux evidence collector.
set -u
OUT="${1:-a1_linux_evidence.txt}"

run_cmd() {
    echo
    echo "===== $* ====="
    if command -v "$1" >/dev/null 2>&1; then
        "$@" 2>&1 || echo "[Command returned a non-zero status]"
    else
        echo "[Command not installed in this Linux environment]"
    fi
}

{
    echo "ASSIGNMENT 5 - PART A - A1: LINUX SYSTEM OBSERVATION"
    echo "Captured: $(date -Is 2>/dev/null || date)"
    echo
    echo "===== Operating system ====="
    [[ -r /etc/os-release ]] && cat /etc/os-release
    uname -a

    run_cmd lscpu
    run_cmd uptime
    run_cmd free -h
    run_cmd swapon --show
    run_cmd vmstat 1 5

    echo
    echo "===== CPU and memory pressure ====="
    for f in /proc/pressure/cpu /proc/pressure/memory /proc/pressure/io; do
        if [[ -r "$f" ]]; then
            echo "--- $f ---"
            cat "$f"
        fi
    done

    echo
    echo "===== Selected /proc observations ====="
    for f in /proc/loadavg /proc/stat /proc/meminfo /proc/swaps /proc/filesystems; do
        if [[ -r "$f" ]]; then
            echo "--- $f ---"
            case "$f" in
                /proc/stat) head -n 12 "$f" ;;
                /proc/meminfo) head -n 25 "$f" ;;
                *) cat "$f" ;;
            esac
        fi
    done

    echo
    echo "===== Process snapshot ====="
    if command -v ps >/dev/null 2>&1; then
        ps -eo pid,ppid,ni,pri,stat,pcpu,pmem,comm --sort=-pcpu | head -n 25
    else
        echo "ps not installed"
    fi

    echo
    echo "===== Selected /sys observations ====="
    if [[ -d /sys/class/net ]]; then
        echo "Network interfaces:"
        for d in /sys/class/net/*; do
            [[ -e "$d" ]] || continue
            printf "%s " "$(basename "$d")"
            [[ -r "$d/operstate" ]] && cat "$d/operstate" || echo "state unavailable"
        done
    fi
    if [[ -d /sys/block ]]; then
        echo
        echo "Block devices and queue schedulers:"
        for d in /sys/block/*; do
            [[ -e "$d" ]] || continue
            dev="$(basename "$d")"
            printf "%s: " "$dev"
            [[ -r "$d/queue/scheduler" ]] && cat "$d/queue/scheduler" || echo "scheduler attribute unavailable"
        done
    fi

    echo
    echo "===== Filesystem and storage ====="
    run_cmd findmnt -T /
    run_cmd df -hT
    run_cmd lsblk -o NAME,TYPE,SIZE,FSTYPE,MOUNTPOINTS

    echo
    echo "===== Linux directory hierarchy ====="
    ls -ld / /bin /boot /dev /etc /home /lib /media /mnt /opt /proc /root /run /sbin /srv /sys /tmp /usr /var 2>&1

    echo
    echo "===== Current identity and default permission mask ====="
    id
    umask
    echo
    echo "===== Example permission metadata ====="
    [[ -e /etc/passwd ]] && stat -c '%A %a %U:%G %n' /etc/passwd
    [[ -d /tmp ]] && stat -c '%A %a %U:%G %n' /tmp
    echo
    echo "END OF READ-ONLY OBSERVATION"
    echo "Values describe this machine/container at capture time."
} > "$OUT"

echo "Linux evidence saved to: $OUT"
echo "Read-only collection complete; no settings or permissions were changed."
