#!/usr/bin/env bash
set -euo pipefail
root=$(cd "$(dirname "$0")/../.." && pwd)
out=${C2_VITA_TEST_DIR:-/tmp/c2-vita-control-test}
mkdir -p "$out"
gcc -std=c11 -I"$root/src/carma2/platform/sdl3" \
 -I"$root/src/carma2/s3/include" -I"$root/src/carma2/include" -I"$root/src/carma2/hooks" \
 -I"$root/src/brender" -I"$root/src/brender/include" \
 "$root/scripts/packaging/tests/vita_controls_main.c" "$root/src/carma2/platform/sdl3/vita_controls.c" \
 -o "$out/vita-controls"
"$out/vita-controls"
