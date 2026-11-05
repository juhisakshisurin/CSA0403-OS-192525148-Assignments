#!/usr/bin/env bash
set -euo pipefail

echo "1) Build the slim image"
docker build -t result-portal:latest .

echo
echo "2) Image size"
docker image ls result-portal:latest

echo
echo "3) Run the standalone portal with CPU and memory limits"
docker rm -f result-portal-standalone >/dev/null 2>&1 || true
docker run -d --name result-portal-standalone \
  -p 8000:8000 --cpus="0.50" --memory="512m" \
  result-portal:latest

echo
echo "4) Test the portal"
curl -i http://localhost:8000/
curl -i http://localhost:8000/healthz

echo
echo "5) Confirm non-root runtime identity"
docker exec result-portal-standalone whoami

echo
echo "6) Review image history"
docker history result-portal:latest

echo
echo "7) Live resource use (leave this command running to observe)"
docker stats result-portal-standalone

# To remove the standalone test container afterwards:
# docker rm -f result-portal-standalone
