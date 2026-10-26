#!/usr/bin/env bash
set -euo pipefail
: "${VITASDK:?Set VITASDK to your VitaSDK installation}"
export PATH="$VITASDK/bin:$PATH"
root=$(cd "$(dirname "$0")/../.." && pwd)
build=${C2_VITA_BUILD_DIR:-"$root/build-vita"}
cmake -S "$root" -B "$build" -G Ninja \
    -DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake" \
    -DCMAKE_BUILD_TYPE=Release -DCARPOCALYPSE2_PLATFORM=SDL3 \
    -DCARPOCALYPSE2_VENDORED=ON "$@"
cmake --build "$build" --parallel 4 -- -k 0
