#!/usr/bin/env python3
"""Host filesystem tests of the production Vita copy implementation, no game."""
import ctypes
from pathlib import Path
import subprocess
import tempfile

root = Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory(prefix="c2-vita-data-") as tmp:
    tmp = Path(tmp)
    library = tmp / "copy.so"
    subprocess.run(["gcc", "-std=c11", "-O2", "-Wall", "-Wextra", "-Werror", "-shared", "-fPIC", str(root / "src/carma2/platform/sdl3/vita_data.c"), "-o", str(library)], check=True)
    dll = ctypes.CDLL(str(library))
    callback = ctypes.CFUNCTYPE(ctypes.c_int, ctypes.c_void_p, ctypes.c_char_p, ctypes.c_uint)
    copy = dll.C2VitaCopyMissingTree
    copy.argtypes = [ctypes.c_char_p, ctypes.c_char_p, callback, ctypes.c_void_p]
    copy.restype = ctypes.c_int
    source = tmp / "source"
    (source / "nested").mkdir(parents=True)
    binary = bytes(range(256)) * 1200
    (source / "nested/large.bin").write_bytes(binary)
    (source / "OPTIONS.TXT").write_text("default")
    (source / "empty").touch()
    destination = tmp / "data"
    destination.mkdir()
    (destination / "OPTIONS.TXT").write_text("my settings and saves")
    copied = []
    @callback
    def progress(context, path, size):
        copied.append(size)
        return 1
    def run(src, dst, cb=progress):
        return copy(str(src).encode(), str(dst).encode(), cb, None)
    assert run(source, destination)
    assert (destination / "nested/large.bin").read_bytes() == binary
    assert (destination / "empty").read_bytes() == b""
    assert (destination / "OPTIONS.TXT").read_text() == "my settings and saves"
    assert sum(copied) == len(binary)
    copied.clear()
    assert run(source, destination) and sum(copied) == 0
    # A stopped copy never publishes a truncated final file; retry succeeds.
    cancel_source = tmp / "cancel-source"
    cancel_source.mkdir()
    (cancel_source / "large.bin").write_bytes(binary)
    cancelled = tmp / "cancelled"
    @callback
    def cancel(context, path, size):
        return 0
    assert not run(cancel_source, cancelled, cancel)
    assert not (cancelled / "large.bin").exists()
    assert not (cancelled / "large.bin.c2-part").exists()
    (cancelled / "large.bin.c2-part").write_bytes(b"old interrupted attempt")
    assert run(cancel_source, cancelled)
    assert (cancelled / "large.bin").read_bytes() == binary
    assert not (cancelled / "large.bin.c2-part").exists()
    # Missing source and file/directory collisions must fail without clobbering.
    assert not run(tmp / "absent", destination)
    collision = tmp / "collision"
    collision.write_text("keep")
    assert not run(source, collision) and collision.read_text() == "keep"
    occupied = tmp / "occupied"
    (occupied / "large.bin").mkdir(parents=True)
    assert not run(cancel_source, occupied) and (occupied / "large.bin").is_dir()
    print("PASS production bundle copy: nested/binary/empty data, preserve existing, repeat, interrupted cleanup/resume/stale temporary, missing source and collisions; host files only")
