#!/usr/bin/env bash
# Collect baseline process, memory, disk and network observations.
set -u
OUT="${1:-evidence/linux_metrics_$(date +%Y%m%d_%H%M%S).txt}"
{
  echo "Timestamp: $(date -Is)"
  echo "Host: $(hostname)"
  uname -a
  echo
  echo "===== memory ====="
  free -h
  echo
  echo "===== top processes ====="
  ps -eo pid,ppid,user,rss,%cpu,%mem,comm --sort=-%cpu | head -n 20
  echo
  echo "===== filesystems ====="
  df -hT
  echo
  echo "===== listeners ====="
  ss -tulpn
} | tee "$OUT"
