#!/usr/bin/env bash
set -euo pipefail
THRESHOLD="${THRESHOLD:-80}"
LOG_FILE="${DISK_ALERT_LOG:-$HOME/disk_space_alert.log}"
[[ "$THRESHOLD" =~ ^[0-9]+$ ]] && (( THRESHOLD >= 1 && THRESHOLD <= 100 )) || { echo "THRESHOLD must be 1-100" >&2; exit 2; }
alerts="$(df -P -x tmpfs -x devtmpfs | awk -v limit="$THRESHOLD" 'NR>1 {gsub(/%/,"",$5); if ($5+0>=limit) printf "WARNING: %s is %s%% full (mounted on %s)\\n",$1,$5,$6}')"
if [[ -n "$alerts" ]]; then
  { echo "[$(date '+%F %T')] Disk-space alert on $(hostname)"; printf '%s\\n' "$alerts"; } | tee -a "$LOG_FILE"
else
  echo "[$(date '+%F %T')] OK: all monitored filesystems are below ${THRESHOLD}% on $(hostname)" | tee -a "$LOG_FILE"
fi
