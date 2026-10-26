#!/usr/bin/env python3
"""Build a private, local VPK containing the owner's DATA and MUSIC.
No downloads/uploads. The Vita bootstrap installs missing files on first boot.
"""
import argparse
import hashlib
import json
import os
from pathlib import Path
import tempfile
import zipfile

parser = argparse.ArgumentParser(description=__doc__)
parser.add_argument("base_vpk", type=Path)
parser.add_argument("game_directory", type=Path)
parser.add_argument("output_vpk", type=Path)
args = parser.parse_args()
source = args.game_directory.resolve()
output = args.output_vpk.resolve()
if output == args.base_vpk.resolve():
    parser.error("Output must differ from the small base VPK")
files = []
for directory in (source / "DATA", source / "MUSIC"):
    if not directory.is_dir():
        parser.error(f"Missing {directory}")
    for path in directory.rglob("*"):
        if path.is_symlink():
            parser.error(f"Symbolic links are not supported: {path}")
        if path.is_file():
            files.append(path)
files.sort(key=lambda p: p.relative_to(source).as_posix())
if not (source / "DATA/DATA.TWT").is_file():
    parser.error("Missing DATA/DATA.TWT")
if not any(p.is_relative_to(source / "MUSIC") for p in files):
    parser.error("MUSIC is empty")
output.parent.mkdir(parents=True, exist_ok=True)
fd, name = tempfile.mkstemp(prefix=output.name + ".", suffix=".building", dir=output.parent)
os.close(fd)
temporary = Path(name)
try:
    manifest = []
    total = 0
    with zipfile.ZipFile(args.base_vpk) as base, zipfile.ZipFile(temporary, "w", compression=zipfile.ZIP_DEFLATED, compresslevel=6) as target:
        assert set(base.namelist()) == {"eboot.bin", "sce_sys/param.sfo"}, "Use the small base VPK"
        assert base.testzip() is None
        for info in base.infolist():
            target.writestr(info, base.read(info.filename))
        for i, path in enumerate(files, 1):
            before = path.stat()
            relative = path.relative_to(source).as_posix()
            digest = hashlib.sha256()
            size = 0
            with path.open("rb") as data, target.open("bundled/" + relative, "w") as packed:
                while chunk := data.read(262144):
                    packed.write(chunk)
                    digest.update(chunk)
                    size += len(chunk)
            after = path.stat()
            if (before.st_size, before.st_mtime_ns) != (after.st_size, after.st_mtime_ns) or size != before.st_size:
                raise RuntimeError(f"Source changed while packaging: {path}")
            manifest.append({"path": relative, "size": size, "sha256": digest.hexdigest()})
            total += size
            if i % 200 == 0 or i == len(files):
                print(f"Packed {i}/{len(files)} files, {total / 1048576:.1f} MiB", flush=True)
        encoded = json.dumps(manifest, sort_keys=True, separators=(",", ":")).encode()
        identity = hashlib.sha256(encoded).hexdigest()
        target.writestr("bundled/manifest.json", encoded)
        target.writestr("bundled/bundle.id", identity)
    with zipfile.ZipFile(temporary) as check:
        assert check.testzip() is None, "VPK CRC check failed"
        assert len(check.namelist()) == len(files) + 4
        assert check.read("bundled/bundle.id").decode() == identity
    temporary.replace(output)
    with output.open("rb") as packaged:
        package_hash = hashlib.file_digest(packaged, "sha256").hexdigest()
    summary = {"output": str(output), "sha256": package_hash, "vpk_bytes": output.stat().st_size, "asset_files": len(files), "asset_bytes": total, "bundle_id": identity, "zip_crc": "PASS", "hardware_test": "PENDING"}
    output.with_suffix(".json").write_text(json.dumps(summary, indent=2) + "\n")
    print(json.dumps(summary, indent=2), flush=True)
finally:
    temporary.unlink(missing_ok=True)
