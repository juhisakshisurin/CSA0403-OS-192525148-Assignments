#!/usr/bin/env bash
# Create the portal service account, operational groups and required directories.
# Run as root after reviewing the paths and existing account policy.
set -euo pipefail

if [[ "${EUID}" -ne 0 ]]; then
    echo "Run with sudo: sudo ./create_portal_accounts.sh" >&2
    exit 1
fi

for group in portal portal-admins portal-deploy portal-logreaders; do
    getent group "$group" >/dev/null || groupadd --system "$group"
done

if ! id portal >/dev/null 2>&1; then
    useradd --system --gid portal --home-dir /nonexistent \
        --shell /usr/sbin/nologin --no-create-home portal
else
    usermod --gid portal --shell /usr/sbin/nologin portal
fi

install -d -o root -g portal -m 0750 /srv/result-portal
install -d -o root -g portal -m 0750 /etc/result-portal
install -d -o portal -g portal -m 0750 /var/lib/result-portal
install -d -o portal -g portal -m 0750 /var/log/result-portal
if id postgres >/dev/null 2>&1; then
    install -d -o postgres -g postgres -m 0750 /var/backups/result-portal
else
    echo "PostgreSQL OS account not found; install PostgreSQL or provision its backup directory separately."
fi

echo "Service identity, groups and directories are ready."
echo "Add named human accounts to portal-admins, portal-deploy or portal-logreaders"
echo "according to university policy; this script does not create human accounts."
