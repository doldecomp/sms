#!/usr/bin/env python3

"""CI driver: run validate-symbol-order.py for each changed .cpp file.

Given a list of changed paths (as arguments), map each .cpp to its decomp unit
via objdiff.json and run the symbol-order check on it. Files that aren't tracked
units are skipped. A tracked unit whose object, map, or validator is unavailable
fails the check. With --baseline-dir, inherited errors remain visible and only
new errors fail; without it, validation is strict.

Exit status:
  0  every checked unit passed (or nothing to check)
  1  at least one checked unit failed its validation

Usage:
  python tools/check-changed-symbol-order.py src/Foo/Bar.cpp src/Baz/Qux.cpp
  git diff --name-only BASE HEAD -- '*.cpp' | xargs python tools/check-changed-symbol-order.py
"""

import json
import argparse
import os
import subprocess
import sys

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.abspath(os.path.join(script_dir, ".."))
VALIDATOR = os.path.join(script_dir, "validate-symbol-order.py")
OBJDIFF_JSON = os.path.join(root_dir, "objdiff.json")


def norm(p: str) -> str:
    return p.replace("\\", "/").lstrip("./")


def source_to_unit(path=OBJDIFF_JSON) -> dict:
    with open(path, encoding="utf-8") as f:
        units = json.load(f).get("units", [])
    out = {}
    for u in units:
        sp = u.get("metadata", {}).get("source_path")
        if sp:
            out[norm(sp)] = (u["name"], u.get("base_path", ""))
    return out


def main(argv) -> int:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--baseline-dir",
                        help="checkout of the base revision with built objects")
    parser.add_argument("files", nargs="*")
    args = parser.parse_args(argv)
    files = [norm(a) for a in args.files if a.strip()]
    cpp = [f for f in files if f.endswith(".cpp")]
    if not cpp:
        print("No changed .cpp files to check.")
        return 0

    mapping = source_to_unit()
    baseline = (source_to_unit(os.path.join(args.baseline_dir, "objdiff.json"))
                if args.baseline_dir else {})
    passed, failed, skipped = [], [], []

    for f in cpp:
        entry = mapping.get(f)
        if entry is None:
            print(f"skip  {f}  (not a tracked decomp unit)")
            skipped.append(f)
            continue
        unit, base = entry
        if base and not os.path.exists(os.path.join(root_dir, base)):
            print(f"FAIL  {f}  ({unit}: object not built)")
            failed.append((f, unit))
            continue

        print(f"\n{'=' * 72}\n{f}  ->  {unit}\n{'=' * 72}")
        command = [sys.executable, VALIDATOR, "-u", unit]
        if args.baseline_dir and os.path.exists(os.path.join(args.baseline_dir, f)):
            entry = baseline.get(f)
            if entry is None or not entry[1]:
                print(f"FAIL  {f}: existing source has no baseline object mapping")
                failed.append((f, unit))
                continue
            command.extend(["--baseline-object",
                            os.path.join(args.baseline_dir, entry[1])])
        rc = subprocess.run(command,
                            cwd=root_dir).returncode
        if rc == 0:
            passed.append(f)
        elif rc == 2:
            print(f"FAIL  {f}  ({unit}: check could not run -- see message above)")
            failed.append((f, unit))
        else:
            failed.append((f, unit))

    print(f"\n{'=' * 72}")
    print(f"Symbol-order check: {len(passed)} passed, {len(failed)} failed, "
          f"{len(skipped)} skipped.")
    if failed:
        print("\nFailed:")
        for f, unit in failed:
            print(f"  - {f}  ({unit})")
        return 1
    return 0


if __name__ == "__main__":
    sys.exit(main(sys.argv[1:]))
