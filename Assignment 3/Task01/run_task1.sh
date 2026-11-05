#!/bin/bash
set -euo pipefail

ROOT="$(cd "$(dirname "$0")" && pwd)"
cd "$ROOT"

echo "=== Build the v1 static and shared libraries ==="
mkdir -p lib
gcc -std=c11 -Wall -Wextra -fPIC -c bankcrypto_v1.c -o bankcrypto_v1.o
ar rcs lib/libbankcrypto.a bankcrypto_v1.o

gcc -std=c11 -Wall -Wextra -fPIC -shared \
    -Wl,-soname,libbankcrypto.so.1 \
    -o lib/libbankcrypto.so.1 bankcrypto_v1.c
ln -sf libbankcrypto.so.1 lib/libbankcrypto.so

echo
echo "=== Build the same application statically and dynamically ==="
gcc -std=c11 -Wall -Wextra -static bank_app.c \
    lib/libbankcrypto.a -o bank_app_static

gcc -std=c11 -Wall -Wextra bank_app.c \
    -L"$ROOT/lib" -lbankcrypto \
    -Wl,-rpath,'$ORIGIN/lib' \
    -o bank_app_dynamic

echo
echo "=== Build the on-demand dlopen application ==="
gcc -std=c11 -Wall -Wextra bank_app_dlopen.c -ldl \
    -o bank_app_dlopen

echo
echo "=== Executable sizes ==="
stat -c '%n %s bytes' bank_app_static bank_app_dynamic bank_app_dlopen \
    | tee executable_sizes.txt

echo
echo "=== ldd for dynamically linked application ==="
ldd ./bank_app_dynamic | tee ldd_dynamic.txt

echo
echo "=== Static executable check ==="
if ldd ./bank_app_static > static_ldd_check.txt 2>&1; then
    cat static_ldd_check.txt
else
    cat static_ldd_check.txt
fi

echo
echo "=== Output and /proc/self/maps: static build ==="
./bank_app_static | tee static_run.txt

echo
echo "=== Output and /proc/self/maps: dynamic build ==="
./bank_app_dynamic | tee dynamic_run_v1.txt

echo
echo "=== On-demand dlopen: launch without loading the library ==="
./bank_app_dlopen | tee dlopen_not_loaded.txt

echo
echo "=== On-demand dlopen: load only when requested ==="
./bank_app_dlopen --encrypt ./lib/libbankcrypto.so.1 | tee dlopen_loaded.txt

echo
echo "=== Startup-time measurement (30 launches, average milliseconds) ==="
measure_startup() {
    local program="$1"
    local count=30
    local start end total
    start=$(date +%s%N)
    for ((i=0; i<count; i++)); do
        "$program" >/dev/null
    done
    end=$(date +%s%N)
    total=$((end-start))
    awk -v ns="$total" -v n="$count" \
        'BEGIN { printf "%s: %.3f ms/launch (average over %d launches)\n", ARGV[1], ns/1000000/n, n }' \
        "$program"
}
{
    measure_startup ./bank_app_static
    measure_startup ./bank_app_dynamic
} | tee startup_time.txt

echo
echo "=== Peak RSS measurement with /usr/bin/time ==="
{
    echo "STATIC executable:"
    /usr/bin/time -v -o memory_static.txt ./bank_app_static >/dev/null
    echo "DYNAMIC executable:"
    /usr/bin/time -v -o memory_dynamic.txt ./bank_app_dynamic >/dev/null
} | tee memory_run_labels.txt
{
    echo "STATIC:"
    grep -E 'Maximum resident set size|Elapsed \\(wall clock\\) time' memory_static.txt || true
    echo "DYNAMIC:"
    grep -E 'Maximum resident set size|Elapsed \\(wall clock\\) time' memory_dynamic.txt || true
} | tee memory_summary.txt

echo
echo "=== Live-process RSS/VM measurement ==="
capture_status() {
    local label="$1" program="$2"
    "$program" --hold > "${label}_hold_output.txt" &
    local pid=$!
    sleep 1
    {
        echo "$label PID=$pid"
        grep -E '^(VmSize|VmRSS|RssAnon|RssFile|VmData):' "/proc/$pid/status" || true
    } | tee -a live_memory.txt
    kill "$pid" 2>/dev/null || true
    wait "$pid" 2>/dev/null || true
}
: > live_memory.txt
capture_status static ./bank_app_static
capture_status dynamic ./bank_app_dynamic

echo
echo "=== Replace shared library without rebuilding the dynamic executable ==="
./bank_app_dynamic | tee dynamic_before_replacement.txt

gcc -std=c11 -Wall -Wextra -fPIC -shared \
    -Wl,-soname,libbankcrypto.so.1 \
    -o lib/libbankcrypto.so.1.new bankcrypto_v2.c
mv lib/libbankcrypto.so.1.new lib/libbankcrypto.so.1

echo "Running the SAME dynamic executable after replacing only libbankcrypto.so.1:"
./bank_app_dynamic | tee dynamic_after_replacement.txt

echo "Static executable is unchanged and still contains the original v1 code:"
./bank_app_static | tee static_after_replacement.txt

# Restore v1 shared object for the remaining library-mapping demonstrations.
gcc -std=c11 -Wall -Wextra -fPIC -shared \
    -Wl,-soname,libbankcrypto.so.1 \
    -o lib/libbankcrypto.so.1.restore bankcrypto_v1.c
mv lib/libbankcrypto.so.1.restore lib/libbankcrypto.so.1

echo
echo "=== Run two dynamic copies and inspect their shared library pages ==="
./bank_app_dynamic --hold > dynamic_copy_A.txt &
pid_a=$!
./bank_app_dynamic --hold > dynamic_copy_B.txt &
pid_b=$!
sleep 1

inspect_shared_pages() {
    local pid="$1"
    echo "PID=$pid"
    awk '
      /^[0-9a-f]+-/ {
          inlib = (index($0, "libbankcrypto.so.1") > 0)
      }
      inlib && /^Size:/ { size += $2 }
      inlib && /^Rss:/ { rss += $2 }
      inlib && /^Pss:/ { pss += $2 }
      inlib && /^Shared_Clean:/ { shared_clean += $2 }
      inlib && /^Private_Clean:/ { private_clean += $2 }
      END {
          printf "libbankcrypto mapping: Size=%d kB Rss=%d kB Pss=%d kB Shared_Clean=%d kB Private_Clean=%d kB\n",
                 size, rss, pss, shared_clean, private_clean
      }' "/proc/$pid/smaps"
}
{
    inspect_shared_pages "$pid_a"
    inspect_shared_pages "$pid_b"
} | tee shared_pages.txt

kill "$pid_a" "$pid_b" 2>/dev/null || true
wait "$pid_a" 2>/dev/null || true
wait "$pid_b" 2>/dev/null || true

echo
echo "=== Missing shared library test ==="
mkdir -p missing_test/empty
cp bank_app_dynamic missing_test/
if (
    cd missing_test
    LD_LIBRARY_PATH="$ROOT/missing_test/empty" ./bank_app_dynamic
) > missing_library_output.txt 2>&1; then
    echo "Unexpected: dynamic executable ran without its dependency." \
        | tee -a missing_library_output.txt
else
    echo "Expected failure: the dynamic loader could not find libbankcrypto.so.1." \
        | tee -a missing_library_output.txt
fi
cat missing_library_output.txt

echo
echo "=== Compile and run the dlopen missing-library fallback test ==="
./bank_app_dlopen --encrypt ./missing_test/empty/libbankcrypto.so.1 \
    2>&1 | tee dlopen_missing_fallback.txt

echo
echo "Completed. Review the generated .txt files and capture screenshots."
