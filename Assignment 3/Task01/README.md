# Assignment 3 – Task 1: Linking, Loading and Address Binding

## Student Details

| Field | Details |
|---|---|
| **Name** | Juhi Surin |
| **Register Number** | 192525148 |
| **Course** | CSA0403 Operating Systems |

---

## 1. Task Overview

**Q1 – Linking, Loading and Address Binding (6 Marks, K5 – Evaluate)** uses a banking application scenario. The bank currently distributes a 200 MB executable to every branch whenever its encryption library receives a security update.

The task requires:

- Building the same application with static and dynamic linking.
- Comparing executable file sizes and using `ldd` to list shared libraries.
- Loading a library at run time with `dlopen()` and `dlsym()`.
- Displaying application and library load addresses using `/proc/self/maps`.
- Measuring startup time and memory use for static and dynamic versions.
- Replacing the shared library without recompiling the dynamic application.
- Running two copies and identifying shared library pages.
- Testing the behaviour when the shared library is missing.
- Evaluating an on-demand `dlopen()` design.

## 2. Files Included

| File | Description |
|---|---|
| `Assignment3_Task01_Report.pdf` | Detailed report; intentionally excludes student identity details. |
| `bank_app.c` | Same application source used for static and dynamic linking; prints the loaded library version and relevant memory mappings. |
| `bankcrypto_v1.c` | First demonstration shared/static library implementation. |
| `bankcrypto_v2.c` | Replacement library implementation with the same exported interface. |
| `bank_app_dlopen.c` | Alternative application that loads the library only when the feature is requested, using `dlopen()` and `dlsym()`. |
| `run_task1.sh` | Builds and runs all experiments and captures output files. |
| `lib/` | Generated static and shared libraries after running the script. |

**Important:** The library routine is a deterministic demonstration transform, not real encryption. It is used only to demonstrate linking, replacement and symbol loading; it must not be used to protect banking data.

## 3. Requirements

| Component | Requirement |
|---|---|
| Language | C |
| Compiler | GCC |
| Operating System | Linux / Ubuntu / WSL 2 |
| Static library tools | `ar`, `ranlib` (provided by binutils) |
| Dynamic loading | `dlopen()`, `dlsym()`, `dlclose()` |
| Inspection | `ldd`, `/proc/self/maps`, `/proc/<pid>/smaps`, `/usr/bin/time` |

Record the environment used for your own execution:

```bash
gcc --version
lsb_release -a
uname -a
```

The script writes the compiler, Linux distribution and kernel information to `environment.txt`.

## 4. Build and Execute All Experiments

Keep these files in the same `Task01` folder:

```text
Task01/
├── Assignment3_Task01_Report.pdf
├── README.md
├── bank_app.c
├── bankcrypto_v1.c
├── bankcrypto_v2.c
├── bank_app_dlopen.c
└── run_task1.sh
```

Open Ubuntu/WSL terminal in this folder and run:

```bash
chmod +x run_task1.sh
./run_task1.sh
```

The script creates:

- `bank_app_static` – fully statically linked application.
- `bank_app_dynamic` – dynamically linked application.
- `bank_app_dlopen` – application with on-demand loading.
- `lib/libbankcrypto.a` – static library.
- `lib/libbankcrypto.so.1` – shared library.

## 5. Main Compilation Commands

### Create the static library

```bash
gcc -std=c11 -Wall -Wextra -fPIC -c bankcrypto_v1.c -o bankcrypto_v1.o
ar rcs lib/libbankcrypto.a bankcrypto_v1.o
```

### Create the shared library

```bash
gcc -std=c11 -Wall -Wextra -fPIC -shared \
  -Wl,-soname,libbankcrypto.so.1 \
  -o lib/libbankcrypto.so.1 bankcrypto_v1.c
```

### Build the statically linked executable

```bash
gcc -std=c11 -Wall -Wextra -static \
  bank_app.c lib/libbankcrypto.a -o bank_app_static
```

### Build the dynamically linked executable

```bash
gcc -std=c11 -Wall -Wextra bank_app.c \
  -L./lib -lbankcrypto \
  -Wl,-rpath,'$ORIGIN/lib' \
  -o bank_app_dynamic
```

### Build the run-time loading version

```bash
gcc -std=c11 -Wall -Wextra bank_app_dlopen.c -ldl \
  -o bank_app_dlopen
```

If the static build reports that a static C library cannot be found, install the Ubuntu development package and retry:

```bash
sudo apt update
sudo apt install build-essential libc6-dev
```

## 6. Executable Size and Shared-Library Inspection

The script uses:

```bash
stat -c '%n %s bytes' bank_app_static bank_app_dynamic bank_app_dlopen
ldd ./bank_app_dynamic
ldd ./bank_app_static
```

A fully static executable should normally be reported by `ldd` as “not a dynamic executable”. The dynamic executable should list `libbankcrypto.so.1` and its runtime dependencies.

The static executable is expected to be larger on disk because the required library code is incorporated into the executable. The dynamic executable is smaller but requires the compatible shared library to be available at load time.

## 7. Address Binding and `/proc/self/maps`

`bank_app.c` reads `/proc/self/maps` and prints mappings for:

- The application executable.
- `libbankcrypto.so.1`.
- `libc.so`.
- The dynamic loader.

The address ranges are chosen at run time and can differ between executions because of address-space layout randomisation (ASLR).

## 8. Startup-Time and Memory Measurement

The script measures average startup time over 30 launches for both the static and dynamic executable. It also records peak resident set size using `/usr/bin/time -v` and live-process `VmSize`, `VmRSS`, `RssAnon`, `RssFile` and `VmData` values from `/proc/<pid>/status`.

Generated files:

| File | Measurement |
|---|---|
| `startup_time.txt` | Average milliseconds per launch over 30 executions. |
| `memory_static.txt` | Peak RSS and elapsed-time details for the static executable. |
| `memory_dynamic.txt` | Peak RSS and elapsed-time details for the dynamic executable. |
| `memory_summary.txt` | Side-by-side peak RSS and elapsed-time summary. |
| `live_memory.txt` | Live process memory fields read from `/proc/<pid>/status`. |

Timing and RSS are machine-specific. Use the output from your own Ubuntu/WSL computer in the final report.

## 9. Replace the Shared Library Without Recompiling

The script first runs `bank_app_dynamic` with `bankcrypto-library-v1`.

It then compiles `bankcrypto_v2.c` to the same shared-library SONAME (`libbankcrypto.so.1`) and replaces only the shared-object file. It runs the exact same `bank_app_dynamic` executable again.

Expected observation:

- Before replacement: `bankcrypto-library-v1`.
- After replacement: `bankcrypto-library-v2`.
- The statically linked executable remains at version 1 because its library code was copied into the executable at link time.

The script restores the v1 shared object after this experiment.

## 10. Two Copies and Shared Pages

The script starts two `bank_app_dynamic --hold` processes at the same time and reads each process's `/proc/<pid>/smaps` entries for `libbankcrypto.so.1`.

The important field is `Shared_Clean`. A non-zero value for file-backed read-only library pages is evidence that pages are mapped as clean shared pages in the two processes. The two processes may map the library at different virtual addresses; shared physical file-backed pages do not require identical virtual addresses.

The result is saved in `shared_pages.txt`.

## 11. Missing Shared Library Test

The script copies the dynamic executable into an isolated `missing_test` folder without copying the required shared library. It runs the executable with an empty `LD_LIBRARY_PATH`.

Expected behaviour: the dynamic loader prints an error that `libbankcrypto.so.1` cannot be opened, and the program does not reach `main()`.

The result is saved in `missing_library_output.txt`.

## 12. Alternative Concept – Load Only When Needed

Run:

```bash
./bank_app_dlopen
```

The program starts without loading `libbankcrypto.so.1`.

Load the optional feature only when it is needed:

```bash
./bank_app_dlopen --encrypt ./lib/libbankcrypto.so.1
```

This version uses `dlopen()` to load the library, `dlsym()` to obtain function addresses and `dlclose()` to release the handle. If the library is missing, it reports the error and continues without the optional feature. This can reduce startup work and memory use when the feature is not needed, but the first feature use incurs load/relocation/symbol-lookup latency and needs explicit error handling.

## 13. Generated Evidence Files

After running `./run_task1.sh`, the folder contains:

| Evidence file | Description |
|---|---|
| `executable_sizes.txt` | Static, dynamic and dlopen executable sizes. |
| `ldd_dynamic.txt` | Runtime shared-library dependencies. |
| `static_ldd_check.txt` | Static executable check. |
| `static_run.txt` | Static version output and `/proc/self/maps`. |
| `dynamic_run_v1.txt` | Dynamic version output and v1 mappings. |
| `startup_time.txt` | Average startup-time measurement. |
| `memory_static.txt`, `memory_dynamic.txt` | Peak RSS/time details. |
| `memory_summary.txt`, `live_memory.txt` | Memory comparisons. |
| `dynamic_before_replacement.txt` | Output before shared-library replacement. |
| `dynamic_after_replacement.txt` | Output after replacement, without rebuilding the executable. |
| `static_after_replacement.txt` | Confirms static binary still uses v1. |
| `shared_pages.txt` | Shared-clean library-page measurements for two copies. |
| `missing_library_output.txt` | Missing dependency error. |
| `dlopen_not_loaded.txt`, `dlopen_loaded.txt`, `dlopen_missing_fallback.txt` | On-demand loading tests. |
| `environment.txt` | GCC and Linux environment information. |

Capture terminal screenshots that show the size comparison, `ldd`, v1→v2 replacement, shared-page output and missing-library behaviour.

## 14. Result

The experiments distinguish static linking, load-time dynamic linking and on-demand run-time loading. They demonstrate how static and shared library code is incorporated or mapped, how load addresses can be inspected, how a shared object can be updated without rebuilding a dynamically linked executable, how file-backed pages may be shared across processes, and how the program should handle a missing optional library.

## 15. Repository Submission Reminder

The repository name specified in the assignment is:

`CSA04-192525148-Assignment3`

Keep Task 1 inside its own `Task01` folder. Include the report, all C source files, the execution script, generated outputs and screenshots. Commit Task 1 separately and add the actual repository URL to the report cover page and repository-level README before submitting.
