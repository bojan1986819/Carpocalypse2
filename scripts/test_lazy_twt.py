"""Run the production PF verifier against every TWT recursively under DATA.

Build: cmake --build build-sdl3 --target test_lazy_twt
Run: python scripts/test_lazy_twt.py build-sdl3/test_lazy_twt.exe <test dir>/game/DATA
"""
import pathlib
import subprocess
import sys
import tempfile

exe, data = (pathlib.Path(arg).resolve() for arg in sys.argv[1:])
packs = sorted(p for p in data.rglob("*") if p.is_file() and p.suffix.lower() == ".twt")
if not packs:
    raise SystemExit(f"No TWT packs found under {data}")
with tempfile.TemporaryDirectory(prefix="lazy-twt-", dir=exe.parent) as scratch:
    manifest = pathlib.Path(scratch) / "packs.txt"
    manifest.write_text("".join(f"{p.as_posix()}\n" for p in packs), encoding="ascii")
    subprocess.run([str(exe), str(manifest)], cwd=scratch, check=True)
