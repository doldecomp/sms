#!/usr/bin/env python3

###
# Downloads various tools from GitHub releases.
#
# Usage:
#   python3 tools/download_tool.py wibo build/tools/wibo --tag 1.0.0
#
# If changes are made, please submit a PR to
# https://github.com/encounter/dtk-template
###

import argparse
import hashlib
import io
import os
import platform
import shutil
import stat
import tarfile
import tempfile
import urllib.request
import zipfile
from typing import Callable, Dict
from pathlib import Path


def binutils_url(tag):
    uname = platform.uname()
    system = uname.system.lower()
    arch = uname.machine.lower()
    if system == "darwin":
        system = "macos"
        arch = "universal"
    elif arch == "amd64":
        arch = "x86_64"

    repo = "https://github.com/encounter/gc-wii-binutils"
    return f"{repo}/releases/download/{tag}/{system}-{arch}.zip"


def compilers_url(tag: str) -> str:
    return f"https://files.decomp.dev/compilers_{tag}.zip"


def dtk_url(tag: str) -> str:
    uname = platform.uname()
    suffix = ""
    system = uname.system.lower()
    if system == "darwin":
        system = "macos"
    elif system == "windows":
        suffix = ".exe"
    arch = uname.machine.lower()
    if arch == "amd64":
        arch = "x86_64"

    repo = "https://github.com/encounter/decomp-toolkit"
    return f"{repo}/releases/download/{tag}/dtk-{system}-{arch}{suffix}"


def objdiff_cli_url(tag: str) -> str:
    uname = platform.uname()
    suffix = ""
    system = uname.system.lower()
    if system == "darwin":
        system = "macos"
    elif system == "windows":
        suffix = ".exe"
    arch = uname.machine.lower()
    if arch == "amd64":
        arch = "x86_64"

    repo = "https://github.com/encounter/objdiff"
    return f"{repo}/releases/download/{tag}/objdiff-cli-{system}-{arch}{suffix}"


def dsptool_url(tag: str) -> str:
    uname = platform.uname()
    system = uname.system.lower()
    arch = uname.machine.lower()
    if arch in ("amd64", "x86_64"):
        arch = "x64"
    elif arch == "aarch64":
        arch = "arm64"
    if system == "darwin":
        system = "macos"
    if (system, arch) not in (
        ("windows", "x64"), ("linux", "x64"),
        ("macos", "x64"), ("macos", "arm64"),
    ):
        raise RuntimeError(
            f"No DSPTool release for {uname.system}/{uname.machine}; "
            "use configure.py --dsptool PATH with a local binary"
        )
    extension = "zip" if system == "windows" else "tar.gz"
    repo = "https://github.com/doldecomp/dolphin-dsptool"
    return f"{repo}/releases/download/{tag}/dsptool-{tag}-{system}-{arch}.{extension}"


def sjiswrap_url(tag: str) -> str:
    repo = "https://github.com/encounter/sjiswrap"
    return f"{repo}/releases/download/{tag}/sjiswrap-windows-x86.exe"


def wibo_url(tag: str) -> str:
    uname = platform.uname()
    arch = uname.machine.lower()
    system = uname.system.lower()
    if system == "darwin":
        arch = "macos"

    repo = "https://github.com/decompals/wibo"
    return f"{repo}/releases/download/{tag}/wibo-{arch}"


TOOLS: Dict[str, Callable[[str], str]] = {
    "binutils": binutils_url,
    "compilers": compilers_url,
    "dtk": dtk_url,
    "dsptool": dsptool_url,
    "objdiff-cli": objdiff_cli_url,
    "sjiswrap": sjiswrap_url,
    "wibo": wibo_url,
}


def open_url(url):
    req = urllib.request.Request(url, headers={"User-Agent": "Mozilla/5.0"})
    try:
        return urllib.request.urlopen(req)
    except urllib.error.URLError as e:
        if "CERTIFICATE_VERIFY_FAILED" not in str(e):
            raise
        try:
            import certifi
            import ssl
        except ImportError as error:
            raise RuntimeError(
                'Install certifi using "python -m pip install certifi".'
            ) from error
        return urllib.request.urlopen(
            req, context=ssl.create_default_context(cafile=certifi.where())
        )


def ensure_dsptool(tag: str, output: Path) -> None:
    url = dsptool_url(tag)
    stamp = output.with_name(output.name + ".version")
    if output.is_file() and stamp.is_file() and stamp.read_text().strip() == url:
        return

    print(f"Downloading {url} to {output}")
    filename = url.rsplit("/", 1)[1]
    with open_url(url.rsplit("/", 1)[0] + "/SHA256SUMS") as response:
        checksums = {
            parts[1].lstrip("*"): parts[0]
            for parts in (line.split() for line in response.read().decode("utf-8").splitlines())
            if len(parts) == 2
        }
    with open_url(url) as response:
        data = response.read()
    if hashlib.sha256(data).hexdigest() != checksums.get(filename):
        raise ValueError(f"DSPTool checksum mismatch for {filename}")

    # Copy only the executable; never extract paths supplied by the archive.
    if filename.endswith(".zip"):
        member = filename[:-4] + "/bin/dsptool.exe"
        with zipfile.ZipFile(io.BytesIO(data)) as archive:
            binary = archive.read(member)
    else:
        member = filename[:-7] + "/bin/dsptool"
        with tarfile.open(fileobj=io.BytesIO(data), mode="r:gz") as archive:
            info = archive.getmember(member)
            if not info.isfile():
                raise ValueError(f"DSPTool archive member is not a file: {member}")
            with archive.extractfile(info) as executable:
                binary = executable.read()

    output.parent.mkdir(parents=True, exist_ok=True)
    temporary = None
    try:
        with tempfile.NamedTemporaryFile(dir=output.parent, delete=False) as file:
            temporary = Path(file.name)
            file.write(binary)
        temporary.chmod(0o755)
        temporary.replace(output)
        stamp.write_text(url + "\n", encoding="utf-8")
    finally:
        if temporary is not None and temporary.exists():
            temporary.unlink()


def download(url, response, output) -> None:
    if url.endswith(".zip"):
        data = io.BytesIO(response.read())
        with zipfile.ZipFile(data) as f:
            f.extractall(output)
        # Make all files executable
        for root, _, files in os.walk(output):
            for name in files:
                os.chmod(os.path.join(root, name), 0o755)
        output.touch(mode=0o755)  # Update dir modtime
    else:
        with open(output, "wb") as f:
            shutil.copyfileobj(response, f)
        st = os.stat(output)
        os.chmod(output, st.st_mode | stat.S_IEXEC)


def main() -> None:
    parser = argparse.ArgumentParser()
    parser.add_argument("tool", help="Tool name")
    parser.add_argument("output", type=Path, help="output file path")
    parser.add_argument("--tag", help="GitHub tag", required=True)
    args = parser.parse_args()

    if args.tool == "dsptool":
        ensure_dsptool(args.tag, args.output)
        args.output.touch()
        return

    url = TOOLS[args.tool](args.tag)
    output = Path(args.output)

    print(f"Downloading {url} to {output}")
    with open_url(url) as response:
        download(url, response, output)


if __name__ == "__main__":
    main()
