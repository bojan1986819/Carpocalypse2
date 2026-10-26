# Bounded TWT streaming (port builds)

Non-matching builds mount only the 8-byte top header and the 56-byte file
records. The original aligned payload offsets are retained in those records;
`gTwatVfsMountPoints[].data` is NULL. A shared unbuffered source stays open while the mount or any of its entries owns a reference.
Enumeration continues to use the records without loading payloads.

Each packed `PFfopen` references the mounted source and allocates one
65,536-byte `kMem_packed_file` window. It makes **no fopen call**. Offset, size,
position and window contents remain independent per entry in private port
state. Physical reads seek the shared source to that entry's absolute offset.
No payload is read at open. Unmount releases the mount reference; retained
entries keep the old source alive even when that mount slot is reused.
`PFfclose` frees its window and releases its source reference. The final owner
closes the FILE. Fixed metadata holds up to five mounts plus fifty orphan
source generations; no payload cache or whole-entry fallback is added.
CRT buffering is disabled before the first source read. PF operations retain
the existing main-thread usage model.

Reads of at least 64 KB go straight into caller storage; smaller reads, fgets
and getc use the window. Seek/tell/rewind use integer offsets instead of
pointers into entry memory. DRfgetc2 retains signed-byte results, while
DRfgetpos/DRfsetpos round-trip opaque offsets. Short physical I/O reports an
error.

Retail ungetc mutates the entry, even across rewinds. Same-byte pushback needs
no extra storage. Changed bytes use a private unbuffered temporary journal,
replayed on subsequent reads, so repeated changes survive window eviction
without growing RAM or writing the source pack. Closing the entry also closes
and removes its journal. Temporary-file failure reports an ungetc error. The
game's pushback callers return the byte just read and do not create journals.

The port readers retain packed-file position/error conventions, including
position-based `PFfeof`, unsigned `PFfgetc`, unchanged CR/LF bytes in `PFfgets`,
and clearing the error on successful reads, seeks, rewind and tell. At EOF,
`PFfgets` now returns NULL without reading outside the entry. Short multi-byte
`PFfread` returns the complete-element count and consumes the available bytes;
the old arithmetic could copy beyond the entry. Invalid seeks fail without
constructing out-of-range pointers. Retail matching implementations are intact.

## Consumer audit and memory limits

- TIMB.TWT (39,257,128 bytes) mounts with a 120-byte allocation. Its P16 entry
  (37,909,496 bytes) and P08 entry (1,347,512 bytes) each open with a 65,536-byte
  window. Two simultaneous entries plus the table peak at 131,192 bytes.
- BrPixelmapLoadMany -> DfOpen -> BrFileOpenRead -> DRStdioOpenRead/DRfopen;
  block, character, line, advance and close callbacks use PF operations.
  Datafile block_read allocates the final object, not an entry copy. TIMB's
  largest serialized pixel block is 131,072 bytes.
- world3 LoadAllImagesInDirectory uses PF enumeration and AddPixelmaps, which
  reaches that BRender reader. No direct mount payload pointers remain.
- PolyPixGet now streams PIXIES.P16, retaining individual decoded glyphs. It
  follows ASCIZ header fields like BRender: retail header lengths undercount
  names. The former whole-font-TWT allocation could be 1,238,888 bytes; the
  largest installed font P16 is 824,054 bytes. Largest individual retained
  glyph: 8,192 bytes. PolyPixGetGlyphMap copies that glyph into a BRender map.
- loading.c's direct packed memory readers now use PF bytes/offsets; its error
  helpers still use the public error field. Port `start` is only a slot marker
  and window pointer; `pos/end` are unused. Palette/link readers using raw
  stdio on PF handles also use PF calls in ports.
- LoadFlicData's memory decoder retains pointers into whole caller-owned data.
  Packed assets are allowed only below 4 MiB; larger ones fail that fallback.
  Its disk mode already streams 75,000 bytes; the initial read is now PF-aware.
  Installed packs contain no FLI/FLC/ANIM entries: maximum packed whole-FLIC
  fallback size in this dataset is zero.
- LoadBRFont retains serialized glyph data; installed FNT files are loose,
  largest 1,967 bytes, with no packed FNT entries. Saved-game loading retains
  application-root SAVEDGAMES.ARS, outside mounted DATA paths; no packed entry
  exists. Sound/vorbis and joystick/config whole-file readers use real stdio
  files. No get-whole-packed-file helper or other direct start/data payload
  consumer was found.

## Host verification

After configuring the desktop SDL3 build:

```text
cmake --build build-sdl3 --target carpocalypse2 test_lazy_twt
python scripts/test_lazy_twt.py build-sdl3/test_lazy_twt.exe <test dir>/game/DATA
```

Set `C2_PACK_TRACE=1` before launch to opt into per-entry allocation logs;
normal load runs use aggregate phase counters to avoid thousands of diagnostic
writes. See [loadtime.md](loadtime.md) for timings and counter definitions.

The verifier compares every entry against a direct whole-pack reference using
whole reads, 4,093-byte chunks, and mixed getc/fgets/fread/seeks at every 64 KB
boundary. It checks independent handles, persistent changed-byte pushback,
unmount/remount, empty/binary data, partial elements, errors, slot exhaustion,
invalid packs and allocation bounds. It also asserts no disk fopen at packed
entry open and stresses fifty unmounted retained source generations alongside
five new mounts. Reference/caller buffers are deliberately
outside the tracked PF allocator. All 381 packs, 4,132 entries and 608,734,543
payload bytes pass, with no tracked leaks.

For desktop smoke tests, C2_TEST_RACE=1 skips menus. Optional port-only
C2_TEST_RACE_INDEX selects a validated zero-based race index and its group;
index 4 is TWIN SHRIEKS / TIMBER1.TXT. Use CARPOCALYPSE2_NO_DIALOG=1,
C2_RENDERER=gl and C2_TEST_KEYS="3:Keypad 8:hold". Default and TIMBER races
both passed 60-second desktop tests, with maximum entry allocation 64 KB and
no pack allocation >=1 MiB. Matching .text/.data are byte-identical to baseline.

Detailed results are appended to
<local notes>/codex_lazy_twt/PROGRESS.md. No Vita run is implied.
