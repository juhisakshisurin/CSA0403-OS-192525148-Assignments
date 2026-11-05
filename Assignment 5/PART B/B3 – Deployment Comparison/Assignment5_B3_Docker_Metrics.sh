#!/usr/bin/env bash
set -euo pipefail
NAME="${1:-result-portal-container}"
OUT="evidence/docker_metrics_$(date +%Y%m%d_%H%M%S).txt"
{
  echo "Timestamp: $(date -Is)"
  docker ps --filter "name=$NAME"
  docker stats --no-stream "$NAME"
  docker inspect --size "$NAME" --format 'Image={{.Config.Image}} SizeRw={{.SizeRw}} SizeRootFs={{.SizeRootFs}}'
  docker image ls simats/result-portal
  docker history simats/result-portal:1.0
} | tee "$OUT"
