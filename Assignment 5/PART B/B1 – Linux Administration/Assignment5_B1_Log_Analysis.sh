#!/usr/bin/env bash
# Analyse a standard Nginx combined access log (status field 9, bytes field 10).
set -euo pipefail
if [[ $# -ne 1 || ! -r "$1" ]]; then
  echo "Usage: $0 /path/to/readable/nginx-access.log" >&2; exit 2
fi
log=$1
echo "Requests per status code:"
awk 'NF >= 9 {print $9}' "$log" | sort | uniq -c | awk '{print $2, $1}' | sort -n -k1,1
echo "Busiest client:"
awk 'NF >= 9 {print $1}' "$log" | sort | uniq -c | sort -nr | head -n 1 | awk '{print $2, $1}'
echo "Total bytes sent: $(awk 'NF >= 10 {s += $10} END {print s+0}' "$log")"
