# Load-time changes (port builds)

Work is local to `local/loadtime`, based on `local/glrend` at `002b889c`.

## Changes

- One reference-counted unbuffered FILE per mounted TWT generation replaces
  reopening the archive for every packed PFfopen. Each entry retains its own
  position, 64 KB window and optional changed-byte journal. Unmounted entries
  retain their old source independently of mount-slot reuse. No payload cache
  or larger allocation was introduced.
- Once a font's PIXIES.P16 has been decoded, subsequent glyph lookups use that
  known set instead of reopening/probing the same entry for every character.
  The cached source path must match, and KillThePixies invalidates it. The
  decoder, glyph pixels, tinting and loose-file fallback are unchanged.
- Per-entry PACK ENTRY logging is opt-in via `C2_PACK_TRACE=1`. PACK OPEN and
  aggregate load summaries remain enabled.
- The existing Vita branch already caches case-fix directory lookups. This
  change does not replace or duplicate that cache.

## Reading the next Vita log

`LOAD phase=... depth=... end_ms=... ms=...` is written to dr_dprintf and, under
`__vita__`, C2VitaLog. The clock works before PDInitTimer. Scopes cover startup,
InitPolyFonts and individual font names, frontend setup/images/models, race
initialization through mainloop entry, track images/materials/models, cars,
drones and pedestrian initialization/specs/finalization.

Nested scopes are **inclusive**: do not sum parents with their children.
`race-to-mainloop` includes the loading-screen hold and later race setup;
`race-init` and `track-load` isolate the resource work inside it. `end_ms` is a
platform monotonic timestamp; use `ms` for comparisons across platforms.

Counters cover the main-thread PF path, not all process I/O:

| Field | Meaning |
| --- | --- |
| PFfopen | Packed and loose PFfopen attempts, including failed probes |
| packed | Successful packed entry opens |
| fopen | Calls from PF/mount code to stdio fopen, including failed attempts |
| reads | PF-path disk stdio read calls (fread, fgetc, fgets), including journal reads |
| bytes | Returned stdio bytes: complete fread elements, successful fgetc bytes, strlen of fgets output |
| total_* | Cumulative corresponding counters since process start |

These are stdio request/return counts, not kernel syscall counts. Bytes include
pack header and bounded entry read-ahead, so they differ from the caller's
logical bytes. Text fgets counts exclude embedded NUL tails; game text files
have none. Diagnostics, audio, raw platform I/O and extra retries inside the
case-fix wrapper are excluded. No per-read clock or logging call is added.

Profiler headers/calls are excluded by preprocessor guards from matching
builds. Even no-op macros can change VC5 float scheduling; keep these guards.

## Desktop evidence (2026-10-22)

Windows x86 SDL3 RelWithDebInfo, OpenGL, locally installed game data. Single-run
samples, warm filesystem cache; these are not Vita time predictions.

| Phase/metric | Baseline | Optimized |
| --- | ---: | ---: |
| Each InitPolyFonts: PF-path disk fopen | 1,684 | 15 |
| Second InitPolyFonts: PFfopen attempts | 1,670 | 43 |
| Second InitPolyFonts time | 377 ms | 234 ms |
| Frontend setup disk fopen | 3,567 | 112 |
| Frontend setup time | 945 ms | 641 ms |
| Frontend bytes returned | 5,357,034 | 5,357,034 |
| Cumulative disk fopen through menu setup | 5,729 | 540 |
| Startup plus default race, cumulative disk fopen | 2,804 | 767 |

The menu cumulative disk fopen reduction is 90.6%. InitPolyFonts still runs
at startup and frontend entry as before. Race participants and asset counts
vary with the random seed; total desktop race timings were noisy (about
2.6-2.8 s including the loading-screen hold), so no race latency gain is claimed
from these samples. Default track bytes matched exactly at 38,603,434.

Menu, default race and TIMBER race each survived 22/30/30-second harness runs.
Only C2_TEST_RACE/C2_TEST_RACE_INDEX/C2_TEST_KEYS were used for game input;
PrintWindow captures used the supplied winshot.py. Title, car details and
race-description text masks in the menu have zero differing red/green mask
pixels. Animated backgrounds and simulation HUD values naturally differ.
The original test executable was backed up/restored in finally and its SHA256
verified after every series:
`276c90dff9a623d324f05b7d21bcf656cec84a32860aa35a32431d0b47f56be3`.

## Verification and Vita integration

- Production all-packs verifier passes: 381 packs, 4,132 entries,
  608,734,543 payload bytes, zero tracked leaks. Existing independent-position,
  seek/read/window/error/journal tests remain intact. Added no-fopen-at-entry-open
  assertions and fifty retained orphan generations plus five current mounts.
  TIMB's 37,909,496-byte P16 still uses a 65,536-byte window; the two-entry plus
  mount-table peak remains 131,192 bytes.
- `python scripts/test_font_probe.py` in the MSVC environment passes. The same
  production-body fixture passes WSL ASan/UBSan: legacy/cached paths return the
  same maps and keep decoder/free counts identical; probes reduce 10 to 6.
- Fresh matching baseline at the identical source path is byte-identical to
  final: .text 1,554,432 bytes, SHA256
  `2c0bd6abe5639bbfa1c11a8078546c650a1e06dd08c6cead06e4fa6a023db695`;
  .data 687,104 bytes, SHA256
  `dae7f0b683a2f52f761586f513edde6e4a833103c33c360c925ba582684c25d0`.
- Vita compile/link passes against a detached local/vita snapshot `2bc9175f`
  in `wt-loadtime-vita-check`, separate WSL build `~/vita-tools/build-loadtime`.
  Built only the `carpocalypse2` target; ARM ELF contains C2LoadBegin/C2LoadEnd
  and gC2LoadCounts. Existing SDK enum-size linker warnings remain. No Vita
  branch edits, package install, upload or hardware launch occurred.

Integration into the Vita snapshot had one three-way conflict at the end of
InitPolyPedSystem: retain both its existing
`VITA_PED_CHECKPOINT("InitPolyPedSystem: complete")` and the guarded C2LoadEnd.
All other changes applied cleanly. Claude can merge the local/loadtime commits
into local/vita; hardware startup/race measurements remain for the next run.

Detailed logs, backups and PrintWindow captures are local under
`<local notes>/codex_loadtime/`; see PROGRESS.md there. No private
project data was pushed or uploaded.
