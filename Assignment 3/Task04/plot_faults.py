#!/usr/bin/env python3
import csv
from pathlib import Path
import matplotlib.pyplot as plt

csv_path = Path("frame_faults.csv")
rows = []
with csv_path.open(newline="", encoding="utf-8") as f:
    for row in csv.DictReader(f):
        rows.append((int(row["Frames"]), int(row["PageFaults"])))

if not rows:
    raise SystemExit("frame_faults.csv has no data.")

frames, faults = zip(*rows)
plt.figure(figsize=(7.5, 4.8))
plt.plot(frames, faults, marker="o")
plt.xlabel("Number of frames")
plt.ylabel("Page faults")
plt.title("FIFO Page Faults vs Number of Frames")
plt.xticks(frames)
plt.grid(True, alpha=0.3)
plt.tight_layout()
plt.savefig("frame_faults.png", dpi=180)
print("Saved frame_faults.png")
