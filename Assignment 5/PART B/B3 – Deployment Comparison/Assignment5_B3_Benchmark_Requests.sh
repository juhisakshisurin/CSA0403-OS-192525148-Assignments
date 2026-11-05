#!/usr/bin/env bash
# B3.2 - Same ApacheBench workload for each deployment.
set -euo pipefail

if [[ $# -lt 2 ]]; then
  echo "Usage: $0 LABEL URL [REQUESTS=2000] [CONCURRENCY=20]" >&2
  exit 2
fi

LABEL="$1"
URL="$2"
REQUESTS="${3:-2000}"
CONCURRENCY="${4:-20}"
OUT="evidence/${LABEL}_ab_$(date +%Y%m%d_%H%M%S).txt"

if ! command -v ab >/dev/null 2>&1; then
  echo "ApacheBench not installed. Install with: sudo apt install apache2-utils" >&2
  exit 3
fi

echo "Label: $LABEL"
echo "URL: $URL"
echo "Requests: $REQUESTS"
echo "Concurrency: $CONCURRENCY"
echo "Output: $OUT"
ab -n "$REQUESTS" -c "$CONCURRENCY" -k "$URL" | tee "$OUT"
echo
echo "Extracted Requests per second:"
grep -E "Requests per second" "$OUT" || true
echo "Extracted 50/66/75/80/90/95/98/99/100% latencies:"
grep -E "^[[:space:]]*(50|66|75|80|90|95|98|99|100)%[[:space:]]" "$OUT" || true
