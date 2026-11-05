#!/usr/bin/env python3
import csv
import matplotlib.pyplot as plt

def read_csv(path):
    rows = []
    with open(path, newline="", encoding="utf-8") as f:
        rows.extend(csv.DictReader(f))
    return rows

locality = read_csv("faults_locality.csv")
random = read_csv("faults_random.csv")

fig, axes = plt.subplots(1, 2, figsize=(12, 4.8), sharey=False)

for ax, rows, title in [
    (axes[0], locality, "Strong-locality reference string"),
    (axes[1], random, "Uniform-random reference string"),
]:
    frames = [int(r["Frames"]) for r in rows]
    for key, label in [
        ("FIFO", "FIFO"),
        ("LRU", "LRU"),
        ("Optimal", "Optimal"),
        ("Clock", "Clock / second chance"),
    ]:
        faults = [int(r[key]) for r in rows]
        ax.plot(frames, faults, marker="o", label=label)

    ax.set_title(title)
    ax.set_xlabel("Number of frames")
    ax.set_xticks(frames)
    ax.set_ylabel("Page faults")
    ax.grid(True, alpha=0.3)
    ax.legend(fontsize=8)

fig.suptitle("Page faults vs number of frames (3–10)")
fig.tight_layout()
fig.savefig("page_fault_comparison.png", dpi=180)
print("Saved page_fault_comparison.png")
