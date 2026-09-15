#!/usr/bin/env python3

"""CI driver: run validate-symbol-order.py for each changed .cpp file.

Given a list of changed paths (as arguments), map each .cpp to its decomp unit
via objdiff.json and run the symbol-order check on it. Files that aren't tracked
units -- new files, headers, SDK/JSystem sources without a built object, units
whose map TU can't be resolved -- are skipped, not failed.

Exit status:
  0  every checked unit passed (or nothing to check)
  1  at least one checked unit failed its validation

Usage:
  python tools/check-changed-symbol-order.py src/Foo/Bar.cpp src/Baz/Qux.cpp
  git diff --name-only BASE HEAD -- '*.cpp' | xargs python tools/check-changed-symbol-order.py
"""

import json
import os
import subprocess
import sys

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.abspath(os.path.join(script_dir, ".."))
VALIDATOR = os.path.join(script_dir, "validate-symbol-order.py")
OBJDIFF_JSON = os.path.join(root_dir, "objdiff.json")


def norm(p: str) -> str:
    return p.replace("\\", "/").lstrip("./")


def source_to_unit() -> dict:
    with open(OBJDIFF_JSON, encoding="utf-8") as f:
        units = json.load(f).get("units", [])
    out = {}
    for u in units:
        sp = u.get("metadata", {}).get("source_path")
        if sp:
            out[norm(sp)] = (u["name"], u.get("base_path", ""))
    return out


def main(argv) -> int:
    files = [norm(a) for a in argv if a.strip()]
    cpp = [f for f in files if f.endswith(".cpp")]
    if not cpp:
        print("No changed .cpp files to check.")
        return 0

    mapping = source_to_unit()
    passed, failed, skipped = [], [], []

    for f in cpp:
        entry = mapping.get(f)
        if entry is None:
            print(f"skip  {f}  (not a tracked decomp unit)")
            skipped.append(f)
            continue
        unit, base = entry
        if base and not os.path.exists(os.path.join(root_dir, base)):
            print(f"skip  {f}  ({unit}: object not built)")
            skipped.append(f)
            continue

        print(f"\n{'=' * 72}\n{f}  ->  {unit}\n{'=' * 72}")
        rc = subprocess.run([sys.executable, VALIDATOR, "-u", unit],
                            cwd=root_dir).returncode
        if rc == 0:
            passed.append(f)
        elif rc == 2:
            print(f"skip  {f}  ({unit}: check could not run -- see message above)")
            skipped.append(f)
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
