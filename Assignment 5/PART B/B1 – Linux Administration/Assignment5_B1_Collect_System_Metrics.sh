#!/usr/bin/env bash
set -u
OUT="${1:-system_observation_$(date +%Y%m%d_%H%M%S).txt}"
{
 echo "LINUX SERVER OBSERVATION"; echo "Date: $(date -Is)"; echo "Host: $(hostname)"; echo "Kernel: $(uname -a)"; echo
 for cmd in \
 "ps -eo pid,ppid,user,stat,%cpu,%mem,comm --sort=-%cpu | head -n 16" \
 "free -h" "vmstat 1 5" "iostat -xz 1 2" "df -hT" "ss -tulpn" "top -b -n 1 | head -n 20"; do
   echo "===== $cmd ====="; bash -c "$cmd" 2>&1 || echo "Command unavailable or requires installation/permissions."; echo
 done
} | tee "$OUT"
echo "Saved observation to: $OUT"
