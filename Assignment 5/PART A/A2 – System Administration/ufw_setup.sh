#!/usr/bin/env bash
# Safe-by-default UFW plan.
# Review ADMIN_CIDR and run with APPLY=1 only from a trusted console or
# after confirming that the current SSH source is included in ADMIN_CIDR.
set -euo pipefail

ADMIN_CIDR="${ADMIN_CIDR:-10.20.10.0/24}"
APPLY="${APPLY:-0}"

if ! command -v ufw >/dev/null 2>&1; then
    echo "UFW is not installed. Install with: sudo apt install ufw"
    exit 1
fi

echo "Planned policy:"
echo "  Default deny incoming"
echo "  Default allow outgoing"
echo "  Allow SSH (TCP/22) only from ${ADMIN_CIDR}"
echo "  Allow HTTP (TCP/80) and HTTPS (TCP/443) from clients"
echo "  Do not expose application TCP/8000 or database TCP/5432 publicly"

if [[ "$APPLY" != "1" ]]; then
    echo
    echo "Dry run only. Review the plan, then use:"
    echo "  sudo ADMIN_CIDR='YOUR_ADMIN_VPN_CIDR' APPLY=1 ./ufw_setup.sh"
    exit 0
fi

# Add the management SSH rule before enabling the firewall.
sudo ufw allow from "$ADMIN_CIDR" to any port 22 proto tcp
sudo ufw allow 80/tcp
sudo ufw allow 443/tcp
sudo ufw default deny incoming
sudo ufw default allow outgoing
sudo ufw logging medium

echo
echo "Rules added. Enabling UFW now. Ensure this terminal is not your only"
echo "recovery path and that the current administrator address is in ADMIN_CIDR."
sudo ufw --force enable
sudo ufw status verbose
