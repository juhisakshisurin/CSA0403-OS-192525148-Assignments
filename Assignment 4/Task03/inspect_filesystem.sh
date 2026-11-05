#!/usr/bin/env bash
set -euo pipefail

TARGET="${1:-.}"
if [[ ! -e "$TARGET" ]]; then
    echo "Path does not exist: $TARGET" >&2
    exit 2
fi
TARGET="$(cd "$TARGET" && pwd -P)"
OUT_DIR="${OUT_DIR:-task3_fs_evidence}"
mkdir -p "$OUT_DIR"

{
    echo "ASSIGNMENT 4 - TASK 3: REAL FILESYSTEM INSPECTION"
    echo "Timestamp: $(date -Is)"
    echo "Inspected path: $TARGET"
    echo
    echo "=== Operating system and kernel ==="
    cat /etc/os-release 2>/dev/null || true
    uname -a
    echo
    echo "=== Mount source and filesystem type ==="
    findmnt -T "$TARGET" -o TARGET,SOURCE,FSTYPE,OPTIONS
    echo
    echo "=== stat: target directory metadata ==="
    stat "$TARGET"
    echo
    echo "=== stat -f: filesystem statistics ==="
    stat -f "$TARGET"
    echo
    echo "=== df -hT: capacity and free space ==="
    df -hT "$TARGET"
    echo
    echo "=== df -i: inode totals and free inodes ==="
    df -i "$TARGET"
    echo
    echo "=== ls -i: inode numbers for directory entries ==="
    ls -lai "$TARGET"
} 2>&1 | tee "$OUT_DIR/filesystem_inspection.txt"

SOURCE="$(findmnt -n -o SOURCE -T "$TARGET" | head -n 1)"
FSTYPE="$(findmnt -n -o FSTYPE -T "$TARGET" | head -n 1)"

{
    echo "Target: $TARGET"
    echo "Filesystem type: $FSTYPE"
    echo "Mount source: $SOURCE"
    echo

    case "$FSTYPE" in
        ext2|ext3|ext4)
            echo "=== tune2fs -l (read-only filesystem details) ==="
            if command -v tune2fs >/dev/null 2>&1; then
                if [[ -b "$SOURCE" ]]; then
                    sudo tune2fs -l "$SOURCE"
                else
                    echo "Mount source '$SOURCE' is not a block device accessible to tune2fs."
                    echo "Run tune2fs -l against the actual ext filesystem block device."
                fi
            elif command -v dumpe2fs >/dev/null 2>&1; then
                if [[ -b "$SOURCE" ]]; then
                    sudo dumpe2fs -h "$SOURCE"
                else
                    echo "Mount source '$SOURCE' is not an accessible block device."
                fi
            else
                echo "Install e2fsprogs to use tune2fs or dumpe2fs."
            fi
            ;;
        btrfs)
            echo "=== Btrfs filesystem usage ==="
            if command -v btrfs >/dev/null 2>&1; then
                sudo btrfs filesystem usage "$TARGET"
                sudo btrfs filesystem df "$TARGET"
            else
                echo "Install btrfs-progs for Btrfs-specific inspection."
            fi
            ;;
        xfs)
            echo "=== XFS filesystem geometry ==="
            if command -v xfs_info >/dev/null 2>&1; then
                sudo xfs_info "$TARGET"
            else
                echo "Install xfsprogs for xfs_info."
            fi
            ;;
        zfs)
            echo "=== ZFS dataset and pool usage ==="
            if command -v zfs >/dev/null 2>&1; then
                zfs list -o name,used,avail,refer,mountpoint
                zpool list
            else
                echo "ZFS command-line utilities are unavailable."
            fi
            ;;
        *)
            echo "Filesystem type '$FSTYPE' is not ext2/ext3/ext4, Btrfs, XFS or ZFS."
            echo "The general stat, df, ls and findmnt evidence is still valid."
            echo "For a Windows/WSL virtual filesystem, inspect the underlying host filesystem separately."
            ;;
    esac
} 2>&1 | tee "$OUT_DIR/filesystem_specific_details.txt"

echo
echo "Saved inspection evidence to $OUT_DIR/"
echo "This script only reads filesystem information; it does not format, resize, repair or modify a filesystem."
