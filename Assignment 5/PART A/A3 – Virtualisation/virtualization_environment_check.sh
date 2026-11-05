#!/usr/bin/env bash
# Assignment 5 - Part A - A3
# Read-only check of Linux host virtualization capability and installed tools.
# It does not install packages, load modules or modify firmware/kernel settings.
set -u

OUT="${1:-a3_virtualization_host_check.txt}"

section() {
    printf '\n===== %s =====\n' "$1"
}

run_if_available() {
    local cmd="$1"
    shift
    if command -v "$cmd" >/dev/null 2>&1; then
        "$cmd" "$@" 2>&1 || echo "[Command returned non-zero status]"
    else
        echo "[$cmd is not installed]"
    fi
}

{
    echo "ASSIGNMENT 5 - PART A - A3: VIRTUALIZATION HOST CHECK"
    echo "Captured: $(date -Is 2>/dev/null || date)"
    section "Operating system and kernel"
    cat /etc/os-release 2>/dev/null || true
    uname -a

    section "CPU and virtualization flags"
    run_if_available lscpu
    if [[ -r /proc/cpuinfo ]]; then
        echo
        echo "Virtualization flag counts (vmx = Intel VT-x, svm = AMD-V):"
        awk '
            /flags[[:space:]]*:/ {
                for (i=1; i<=NF; i++) {
                    if ($i=="vmx") vmx++
                    if ($i=="svm") svm++
                }
            }
            END {
                printf "CPU records with vmx: %d\n", vmx
                printf "CPU records with svm: %d\n", svm
                if (vmx==0 && svm==0)
                    print "No vmx/svm flag visible in /proc/cpuinfo."
            }
        ' /proc/cpuinfo
    fi

    section "Firmware / KVM visibility"
    if [[ -e /dev/kvm ]]; then
        ls -l /dev/kvm
        [[ -r /dev/kvm && -w /dev/kvm ]] && echo "Current user can read/write /dev/kvm." \
            || echo "Current user cannot read/write /dev/kvm; group/permission may be required."
    else
        echo "/dev/kvm is not visible in this environment."
    fi

    if command -v lsmod >/dev/null 2>&1; then
        lsmod | grep -E '^(kvm|kvm_intel|kvm_amd)[[:space:]]' || echo "No KVM modules shown by lsmod."
    else
        echo "lsmod not installed."
    fi

    section "Virtualization detection"
    run_if_available systemd-detect-virt
    run_if_available virt-host-validate

    section "Installed virtualization tools"
    if command -v qemu-system-x86_64 >/dev/null 2>&1; then
        qemu-system-x86_64 --version
    else
        echo "qemu-system-x86_64 not installed."
    fi

    if command -v virsh >/dev/null 2>&1; then
        virsh version
    else
        echo "virsh/libvirt client not installed."
    fi

    if command -v VBoxManage >/dev/null 2>&1; then
        VBoxManage --version
    else
        echo "VirtualBox CLI not installed."
    fi

    section "Memory and current load"
    run_if_available free -h
    run_if_available uptime

    section "Block storage and filesystems"
    run_if_available lsblk -o NAME,TYPE,SIZE,FSTYPE,MOUNTPOINTS
    run_if_available df -hT

    section "Environment limitation notice"
    if grep -qi microsoft /proc/version 2>/dev/null; then
        echo "WSL detected. Nested virtualization and /dev/kvm visibility depend on WSL, Windows and host configuration."
    fi
    if [[ -e /.dockerenv ]] || grep -qa 'container' /proc/1/cgroup 2>/dev/null; then
        echo "Container environment detected. CPU virtualization flags or /dev/kvm may be hidden by the outer host."
    fi

    echo
    echo "END OF READ-ONLY CHECK"
    echo "A visible CPU flag does not by itself prove that virtualization is enabled and usable."
    echo "Firmware settings, hypervisor modules, device permissions and outer virtualization policy also matter."
} > "$OUT"

echo "Virtualization host check saved to: $OUT"
echo "No packages, services, modules, firmware or system settings were changed."
