#!/usr/bin/env bash
set -euo pipefail
: "${VITASDK:?Set VITASDK}"
root=$(cd "$(dirname "$0")/../.." && pwd)
out=${C2_VITA_TEST_DIR:-/tmp/c2-vita-control-test}
mkdir -p "$out"
for mode in short wide; do
    flag=-fshort-enums
    if [ "$mode" = wide ]; then flag=-fno-short-enums; fi
    "$VITASDK/bin/arm-vita-eabi-gcc" -c "$flag" -I"$VITASDK/arm-vita-eabi/include/SDL3" "$root/scripts/packaging/tests/vita_sdl_layout.c" -o "$out/sdl-layout-$mode.o"
    "$VITASDK/bin/arm-vita-eabi-objcopy" -O binary -j .rodata "$out/sdl-layout-$mode.o" "$out/sdl-layout-$mode.bin"
done
cmp "$out/sdl-layout-short.bin" "$out/sdl-layout-wide.bin"
echo "PASS SDK/game SDL struct sizes and field offsets agree (audio, surface, display, events, colour, format, filesystem); compile-only ARM check"
