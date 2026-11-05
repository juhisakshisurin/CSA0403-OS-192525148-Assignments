#!/usr/bin/env python3
"""Capacity and simplified independent-disk failure probability calculator."""
import argparse

def main():
    parser = argparse.ArgumentParser(
        description="RAID capacity and simplified data-loss probability calculator"
    )
    parser.add_argument("--disks", type=int, default=6)
    parser.add_argument("--size-tb", type=float, default=4.0)
    parser.add_argument(
        "--failure-probability", type=float, default=0.02,
        help="Per-disk probability of failure over the selected assessment interval (default 0.02)"
    )
    args = parser.parse_args()

    n, size, p = args.disks, args.size_tb, args.failure_probability

    if n < 2 or size <= 0 or not 0 <= p <= 1:
        parser.error("disks must be >= 2, size-tb > 0, and failure probability in [0,1]")

    capacities = {
        "RAID 0": n * size,
        "RAID 1 (one n-way mirror)": size,
        "RAID 5": (n - 1) * size,
        "RAID 10 (n even)": (n // 2) * size if n % 2 == 0 else None,
        "RAID 6": (n - 2) * size if n >= 4 else None,
    }

    print(f"RAID capacity for {n} disks × {size:g} TB (decimal drive units)")
    print("RAID 1 assumes all disks form one n-way mirror; RAID 10 assumes paired mirrors.")
    for name, cap in capacities.items():
        if cap is not None:
            print(f"{name:28s}: {cap:g} TB usable (before filesystem/metadata overhead)")

    p0 = (1 - p) ** n
    p1 = n * p * (1 - p) ** (n - 1)
    raid0_loss = 1 - p0
    raid1_loss = p ** n
    raid5_loss = 1 - p0 - p1

    # For n=6, RAID10 made of 3 independent mirror pairs loses data if any pair loses both disks.
    if n % 2 == 0:
        pair_count = n // 2
        raid10_loss = 1 - (1 - p**2) ** pair_count
    else:
        raid10_loss = None

    print()
    print(f"Illustrative independent-drive failure model: p={p:.6g} per disk per chosen interval")
    print("Assumes failures occur within the same interval, no rebuild/replacement, and independence.")
    print(f"RAID 0 loss probability (any disk fails): {raid0_loss:.10g} ({raid0_loss*100:.6g}%)")
    print(f"RAID 1 loss probability (all {n} mirror copies fail): {raid1_loss:.10g} ({raid1_loss*100:.6g}%)")
    print(f"RAID 5 loss probability (2 or more disks fail): {raid5_loss:.10g} ({raid5_loss*100:.6g}%)")
    if raid10_loss is not None:
        print(f"RAID 10 loss probability (any mirror pair loses both): {raid10_loss:.10g} ({raid10_loss*100:.6g}%)")

    if n == 6 and abs(size - 4.0) < 1e-9:
        print()
        print("For the assignment's six 4-TB disks, RAID 6 capacity is 16 TB and it tolerates any two disk failures.")
        print("RAID 6 write work is higher than RAID 5 because two parity values must be calculated and written.")

if __name__ == "__main__":
    main()
