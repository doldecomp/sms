#!/usr/bin/env python3
"""Check changed tracked sources with one clang-format process; never edit files."""

import argparse
import os
from pathlib import Path
import subprocess


EXTENSIONS = {
    ".h", ".H", ".hpp", ".hh", ".h++", ".hxx",
    ".c", ".C", ".cpp", ".cc", ".c++", ".cxx",
    ".ino", ".pde", ".proto", ".cu",
}


def git_paths(*args):
    output = subprocess.check_output(["git", *args, "-z"])
    return [os.fsdecode(path) for path in output.split(b"\0") if path]


def select_files(base=None):
    if base:
        # Fail on an invalid base instead of silently skipping the check.
        subprocess.run(["git", "rev-parse", "--verify", base + "^{commit}"],
                       check=True, stdout=subprocess.DEVNULL)
        changed = git_paths("diff", "--name-only", base, "HEAD")
        config_changed = any(Path(p).name in {
            ".clang-format", "_clang-format", ".clang-format-ignore",
        } for p in changed)
        paths = git_paths("ls-files") if config_changed else changed
    else:
        paths = git_paths("ls-files")
    return [path for path in paths
            if Path(path).suffix in EXTENSIONS and Path(path).is_file()]


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--base", help="compare HEAD to this commit; default: all")
    parser.add_argument("--clang-format", default="clang-format")
    args = parser.parse_args()
    files = select_files(args.base)
    print(f"Checking formatting in {len(files)} source file(s).", flush=True)
    if not files:
        return 0
    return subprocess.run([
        args.clang_format, "--dry-run", "--Werror", "--style=file",
        "--fallback-style=none", "--", *files,
    ]).returncode


if __name__ == "__main__":
    raise SystemExit(main())
