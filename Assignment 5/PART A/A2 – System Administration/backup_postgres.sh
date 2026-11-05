#!/usr/bin/env bash
# PostgreSQL backup example. Run from the postgres account (peer authentication).
# Configure an independent off-host copy in addition to this local backup.
set -euo pipefail
umask 077

DB_NAME="${DB_NAME:-result_portal}"
BACKUP_DIR="${BACKUP_DIR:-/var/backups/result-portal}"
RETENTION_DAYS="${RETENTION_DAYS:-14}"
STAMP="$(date -u +%Y%m%dT%H%M%SZ)"
TMP_FILE="${BACKUP_DIR}/.${DB_NAME}-${STAMP}.dump.tmp"
FINAL_FILE="${BACKUP_DIR}/${DB_NAME}-${STAMP}.dump"

if [[ "$(id -un)" != "postgres" ]]; then
    echo "Run this script as the postgres OS account." >&2
    exit 2
fi

mkdir -p "$BACKUP_DIR"
pg_dump --format=custom --no-owner --file="$TMP_FILE" "$DB_NAME"
mv "$TMP_FILE" "$FINAL_FILE"
find "$BACKUP_DIR" -type f -name "${DB_NAME}-*.dump" \
    -mtime +"$RETENTION_DAYS" -delete

echo "Backup completed: $FINAL_FILE"
