#!/usr/bin/env bash
# Disk-space alert. Logs warnings via syslog and optionally sends email.
set -euo pipefail

THRESHOLD="${THRESHOLD:-80}"
ALERT_EMAIL="${ALERT_EMAIL:-}"
HOST="$(hostname -f 2>/dev/null || hostname)"

if ! [[ "$THRESHOLD" =~ ^[0-9]+$ ]] || (( THRESHOLD < 1 || THRESHOLD > 100 )); then
    echo "THRESHOLD must be an integer from 1 to 100." >&2
    exit 2
fi

df -P -x tmpfs -x devtmpfs -x squashfs | awk -v limit="$THRESHOLD" '
    NR > 1 {
        used=$5
        gsub(/%/, "", used)
        if (used ~ /^[0-9]+$/ && used + 0 >= limit) {
            print $6, used
        }
    }
' | while read -r mountpoint percent; do
    message="Disk usage alert on ${HOST}: ${mountpoint} is ${percent}% full (threshold ${THRESHOLD}%)."
    logger -p user.warning -t result-portal-disk-alert -- "$message"
    echo "$message"

    if [[ -n "$ALERT_EMAIL" ]] && command -v mail >/dev/null 2>&1; then
        printf '%s\n' "$message" | mail -s "Result portal disk alert" "$ALERT_EMAIL"
    fi
done
