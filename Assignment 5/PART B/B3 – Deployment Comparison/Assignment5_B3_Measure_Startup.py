#!/usr/bin/env python3
"""Measure command-start to HTTP health readiness for a local process/container/VM command."""
import argparse
import os
import signal
import subprocess
import sys
import time
import urllib.error
import urllib.request

parser = argparse.ArgumentParser()
parser.add_argument("--url", required=True, help="Health URL to poll, e.g. http://127.0.0.1:8000/health")
parser.add_argument("--timeout", type=float, default=180.0)
parser.add_argument("--leave-running", action="store_true",
                    help="Do not terminate the command after readiness (use for VBoxManage startvm)")
parser.add_argument("command", nargs=argparse.REMAINDER)
args = parser.parse_args()

cmd = args.command[1:] if args.command and args.command[0] == "--" else args.command
if not cmd:
    parser.error("Provide a command after --")

start = time.perf_counter()
proc = subprocess.Popen(cmd, stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL,
                        start_new_session=True)
ready = False
last_error = "not yet available"
try:
    while time.perf_counter() - start <= args.timeout:
        try:
            req = urllib.request.Request(args.url, headers={"User-Agent": "B3-startup-test"})
            with urllib.request.urlopen(req, timeout=1.5) as response:
                if response.status == 200:
                    ready = True
                    break
                last_error = f"HTTP {response.status}"
        except (urllib.error.URLError, TimeoutError, OSError) as exc:
            last_error = str(exc)
        if proc.poll() not in (None, 0) and not args.leave_running:
            raise RuntimeError(f"Start command exited with status {proc.returncode}")
        time.sleep(0.1)

    elapsed = time.perf_counter() - start
    if not ready:
        raise TimeoutError(f"Service did not become ready within {args.timeout}s; last result: {last_error}")

    print(f"READY_URL={args.url}")
    print(f"STARTUP_SECONDS={elapsed:.3f}")
    print(f"START_COMMAND={' '.join(cmd)}")
finally:
    if not args.leave_running and proc.poll() is None:
        try:
            os.killpg(proc.pid, signal.SIGTERM)
            proc.wait(timeout=8)
        except Exception:
            try:
                os.killpg(proc.pid, signal.SIGKILL)
            except Exception:
                pass
