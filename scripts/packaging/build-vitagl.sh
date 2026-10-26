#!/usr/bin/env bash
set -euo pipefail
: "${VITASDK:?Set VITASDK}"
: "${C2_VITAGL_SOURCE:?Set C2_VITAGL_SOURCE to a local vitaGL checkout}"
export PATH="$VITASDK/bin:$PATH"
python3 "$(dirname "$0")/patch-vitagl.py"
# Depth must survive the separate world, overlay and shadow passes.
# Rebuild when changing defines; make does not include flag values in timestamps.
make -C "$C2_VITAGL_SOURCE" clean
# Use the safe existing three-frame circular pool for transient buffers.
# No drawing/memory speedhacks. Compiled shaders are cached in ux0:data/shader_cache (patch 0004
# keeps their uniform blocks), so only the first launch pays the ~8 s of runtime compilation.
make -C "$C2_VITAGL_SOURCE" -j4 install NO_SPLASHSCREEN=1 LOG_ERRORS=2 \
    HAVE_CUSTOM_HEAP=1 STORE_DEPTH_STENCIL=1 SAFE_UNIFORMS=1 USE_SCRATCH_MEMORY=1 \
    HAVE_SHADER_CACHE=1
