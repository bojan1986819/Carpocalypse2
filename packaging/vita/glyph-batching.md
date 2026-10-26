# Polyfont glyph batching verification

Verified locally on 2026-10-21, based on `a6797d3c`, on branch
`local/vita-glyph`. No hardware run was performed. Nothing was pushed or
uploaded, and no other worktree was modified.

## Implementation

`__vita__` or `C2_GLYPH_BATCH` enables consecutive texture-page runs in
`PolyFontText`. Each run has an identity-transform actor, a material snapshot
with identity UV transform, and prepared vertices containing the original UV
transform and glyph translation. Vertex normals, vertex colours and triangle
winding come from the original prepared glyph. Materials retain rendering
fields, with separate copies of any extra surface/primitive token lists.
Changes to material state or token values also split a run.

The original actor list traverses glyphs in reverse insertion order. Runs
retain that order both between pages and within a page, including overlapping
glyphs and strings. No page sorting is performed.

Each pooled slot contains a stable, contiguous prepared block compatible with
the existing exact-byte Vita geometry cache. A run can hold 512 vertices and
256 faces (128 ordinary quad glyphs); longer runs continue in another slot.
The pool grows to its high-water mark, with no recurring actor/model/material
allocation for a repeated workload. Detached slots remain held through scene
end. The first HUD pass and final text pass use the same pool and both existing
cleanup entry points release their actors.

`VITA TEXT` reports `glyphs` and `glyph_batches` accumulated since the previous
outer text-batch end, including the first HUD pass. Batches are submitted
models, before any renderer visibility rejection. Scene and CPU-write barriers
remain in place. Matching and ordinary desktop builds retain their legacy
font path.

## Automated geometry fixture

Run from this worktree under WSL Ubuntu:

```sh
python3 scripts/packaging/test-vita-glyph.py
```

Both 64-bit and 32-bit GCC runs passed with AddressSanitizer and
UndefinedBehaviorSanitizer. The fixture extracts the current production
`PolyFontText` and `GetPolyFontMaterial`, compiles both guarded and legacy
paths, and compares flattened rendered triangles, UVs, normals, colours and
material fields. It uses the real production batching and scene code with
mocked BRender submission. Coordinates allow a 1e-6 tolerance for x87
intermediate rounding; material and colour fields compare exactly.

Coverage includes deferred first-pass assembly, immediate final-pass scenes,
three font colours and justifications, lowercase conversion, blank/unused
glyphs, carriage returns, consecutive and alternating pages, 700 glyphs,
font availability, eviction of the real 80-entry material cache, mutable token
snapshots, scene-end lifetime, contiguous cache eligibility and pool reuse.
The legacy actor array is enlarged only in the fixture to provide an oracle
for long strings. Eviction cases use individual legacy draws as the oracle so
legacy cache aliasing cannot corrupt the expected material state.

## Builds

All completed successfully:

- Windows x86 `build-baseline`: SDL3, vendored libraries, RelWithDebInfo,
  `CARPOCALYPSE2_FIX_BUGS=ON`, `C2_FAST_OVERLAY`, no glyph-batch define.
- Windows x86 `build-desk`: identical settings plus `C2_GLYPH_BATCH`.
- WSL `~/vita-tools/build-glyph`: Release, Ninja, SDL3, vendored libraries,
  SDK `~/vita-tools/vitasdk` and its Vita CMake toolchain. The complete build
  compiled, linked, and also completed the default SELF/VPK generation.

Desktop builds used `<test dir>\env.bat` and
`SDL3_DIR=<test dir>/sdl3-x86/cmake`. Existing compiler/linker warnings
remain, including the Vita SDK enum-size linker warning.

## Desktop screenshots

Both builds ran with `C2_RENDERER=gl` through the existing `glshot.py` helper:

```text
shots: 9,14,24,32
keys:  7:enter;10:enter;13:enter;16:enter;20:104
```

Private local captures are in `<test dir>\screens` as
`glyph_old_009/014/024/032.png` and `glyph_new_009/014/024/032.png`.
The 9-second captures show the new-game skill menu; later captures show the
race HUD, including timer, credits, lap/kills, speed and opponent text.
Visual inspection found matching layout, glyph placement, colour and layering.

Saturated-colour text masks matched exactly for the menu title (5,419 pixels),
skill label (1,487), menu choices (9,194), and the stable race lap (816), wasted
(853), and kills (350) labels. These comparisons had zero differing mask
pixels. The moving scene, changing numeric/status values, pulsing menu
brightness and translucent HUD backgrounds prevent whole-frame pixel identity
between timed runs; they are not treated as deterministic pixel fixtures.
The automated geometry comparison supplies the deterministic equivalence check.

The helper kills its game process without waiting for its executable to
unlock. The capture wrapper therefore retries copy/restore until shutdown
completes. The original test executable was restored after both capture runs,
and its SHA-256 was verified:

```text
276c90dff9a623d324f05b7d21bcf656cec84a32860aa35a32431d0b47f56be3
```

Build logs, fixture output, comparison metrics, original-exe backup, and the
capture/restore wrapper remain locally in the ignored `build-verification`
directory. Vita speed and appearance on hardware remain for the user's later
test; desktop/build results do not establish a hardware frame-rate improvement.
