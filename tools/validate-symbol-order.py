#!/usr/bin/env python3

"""
Validate that a translation unit's function symbol order in our compiled object
file matches the order recorded in the original linker map (mario.MAP).

The linker map's ".text section layout" is the ground truth: it lists every
function a TU contributed, in emission order, *including* deadstripped ones
(marked UNUSED). A correct decompilation must reproduce that exact order, must
not be missing any of those symbols, and must emit the UNUSED ones at the size
the map records for them (UNUSED symbols have no counterpart in the target
object, so objdiff can't check them -- the map size is the only signal we have).

What is checked:
  * MISSING (ERROR)   -- a map symbol (used or UNUSED) absent from our object.
                         Nothing stops you from just defining the function.
  * ORDER   (ERROR)   -- non-weak map symbols present in our object but in the
                         wrong relative order (extra object-only symbols
                         ignored). Reordering the definitions to match is
                         mechanical. A disorder involving ONLY weak symbols is a
                         WARNING instead (compiler-controlled, won't block a PR).
  * BINDING (ERROR)   -- a *placed* (linked) symbol whose weak/local/global
                         linkage disagrees with the map's symbol-closure section
                         (e.g. we defined an out-of-line global that should be a
                         weak header inline, or dropped a `static`). UNUSED
                         symbols are skipped: dead-stripping removes weak, local
                         and global symbols alike and records no binding, so the
                         linkage of an UNUSED symbol is unknowable from the map.
  * SIZE    (WARNING) -- an UNUSED map symbol whose size in our object differs
                         from the size the map records. Reproducing an inlined
                         body's exact size is genuinely hard, so this only warns.

Exit status is non-zero only for ERRORs (MISSING/ORDER/BINDING). SIZE warnings
on their own still exit 0.

Note on "-inline deferred" / reverse_fn_order: empirically the map and our
object are ALWAYS in the same forward order (verified across every unit), so
comparison is forward by default. `--reverse` is provided as a manual escape
hatch should a genuinely reversed unit ever turn up.

Usage:
  python tools/validate-symbol-order.py -u mario/MarioUtil/MathUtil
  python tools/validate-symbol-order.py -u mario/Enemy/areacylinder --map orig/GMSE01/files/marioUS.MAP
"""

import argparse
import json
import os
import re
import subprocess
import sys
from typing import Dict, List, Optional, Tuple

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.abspath(os.path.join(script_dir, ".."))

DEFAULT_MAP = os.environ.get(
    "MARIO_MAP", os.path.join(root_dir, "orig", "GMSJ01", "files", "mario.MAP")
)
NM = os.environ.get("NM", os.path.join(root_dir, "build", "binutils", "powerpc-eabi-nm.exe"))
OBJDIFF_JSON = os.path.join(root_dir, "objdiff.json")


def die(msg: str) -> None:
    """Exit code 2 == cannot run the check (unresolved unit, missing object or
    map, absent nm). Distinct from a genuine validation failure (exit 1), so a
    CI driver can tell 'skip this file' apart from 'this file is wrong'."""
    print(msg, file=sys.stderr)
    sys.exit(2)


class MapSym:
    __slots__ = ("name", "size", "unused")

    def __init__(self, name: str, size: int, unused: bool):
        self.name = name
        self.size = size
        self.unused = unused


def load_units() -> List[Dict]:
    with open(OBJDIFF_JSON, encoding="utf-8") as f:
        return json.load(f).get("units", [])


def resolve_unit(units: List[Dict], arg: str) -> Dict:
    """Resolve a unit by exact name, then by suffix, matching decomp-diff -u."""
    for u in units:
        if u.get("name") == arg:
            return u
    matches = [u for u in units if u.get("name", "").endswith(arg)]
    if len(matches) == 1:
        return matches[0]
    if len(matches) > 1:
        names = "\n  ".join(m["name"] for m in matches)
        die(f"Ambiguous unit '{arg}', candidates:\n  {names}")
    die(f"Unit not found in objdiff.json: {arg}")


# ---------------------------------------------------------------------------
# Map parsing
# ---------------------------------------------------------------------------

# UNUSED line:  "  UNUSED   00005c ........ GetAtanTable__Fff MarioUtil.a MathUtil.cpp"
_RE_UNUSED = re.compile(r"^\s*UNUSED\s+([0-9a-fA-F]+)\s+\.+\s+(\S+)\s+(.+?)\s*$")
# Placed line:  "  000c01d0 000028 800c57d0  4 MsVECMag2__FP3Vec \t MarioUtil.a MathUtil.cpp"
_RE_PLACED = re.compile(
    r"^\s*([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s+(\d+)\s+(\S+)\s+(.+?)\s*$"
)


def parse_text_layout(map_path: str) -> Dict[str, List[MapSym]]:
    """Return {tu_id: [MapSym,...]} for the .text section layout (in map order)."""
    if not os.path.exists(map_path):
        die(f"Map file not found: {map_path}")
    with open(map_path, encoding="utf-8", errors="replace") as f:
        lines = f.read().splitlines()

    start = None
    for i, l in enumerate(lines):
        if l.strip() == ".text section layout":
            start = i + 1
            break
    if start is None:
        die(f"No '.text section layout' section in {map_path}")
    end = len(lines)
    for i in range(start, len(lines)):
        if lines[i].strip().endswith("section layout"):
            end = i
            break

    tus: Dict[str, List[MapSym]] = {}
    for l in lines[start:end]:
        if not l.strip():
            continue
        m = _RE_UNUSED.match(l)
        if m:
            tus.setdefault(m.group(3).strip(), []).append(
                MapSym(m.group(2), int(m.group(1), 16), True)
            )
            continue
        m = _RE_PLACED.match(l)
        if m:
            name = m.group(5)
            if name == ".text":  # per-TU section header, not a symbol
                continue
            tus.setdefault(m.group(6).strip(), []).append(
                MapSym(name, int(m.group(2), 16), False)
            )
    return tus


def find_map_tu(tus: Dict[str, List[MapSym]], source_path: str,
                override: Optional[str]) -> str:
    if override:
        if override not in tus:
            die(f"--map-tu '{override}' not found in .text layout")
        return override
    base = os.path.basename(source_path)             # e.g. MathUtil.cpp
    stem = os.path.splitext(base)[0]                  # e.g. MathUtil
    cands = [
        t for t in tus
        if t.endswith(" " + base) or t == base or t == stem + ".o"
    ]
    if len(cands) == 1:
        return cands[0]
    if not cands:
        die(
            f"Could not find a .text-layout TU for source '{source_path}'.\n"
            f"Pass --map-tu with the exact map identifier (e.g. 'MarioUtil.a {base}')."
        )
    listed = "\n  ".join(cands)
    die(
        f"Multiple map TUs match '{base}'. Disambiguate with --map-tu:\n  {listed}"
    )


# ---------------------------------------------------------------------------
# Object file parsing (our compiled code) -- true emission order via nm
# ---------------------------------------------------------------------------

# nm -S --numeric-sort:  "00000000 00000028 T MsVECMag2__FP3Vec"
_RE_NM = re.compile(r"^([0-9a-fA-F]+)\s+([0-9a-fA-F]+)\s+([a-zA-Z])\s+(\S.*)$")


# nm type letter -> linkage/binding, matching the map's (func,<binding>) tags.
_NM_BIND = {"T": "global", "t": "local", "W": "weak", "w": "weak"}


def obj_functions(obj_path: str) -> List[Tuple[str, int, str]]:
    """[(name, size, binding), ...] for .text symbols in emission order.

    binding is 'global' | 'local' | 'weak', derived from the nm type letter.
    """
    if not os.path.exists(obj_path):
        die(
            f"Object not found: {obj_path}\n"
            f"Build it first (e.g. `ninja {obj_path}`)."
        )
    if not os.path.exists(NM):
        die(f"nm not found: {NM} (set $NM to override)")
    out = subprocess.run(
        [NM, "-S", "--numeric-sort", "--defined-only", obj_path],
        capture_output=True, text=True,
    )
    if out.returncode != 0:
        die(f"nm failed: {out.stderr.strip()}")
    syms: List[Tuple[str, int, str]] = []
    for line in out.stdout.splitlines():
        m = _RE_NM.match(line)
        if not m:
            continue
        size, typ, name = int(m.group(2), 16), m.group(3), m.group(4)
        # t/T = text (local/global); w/W = weak (functions & thunks live here).
        # Weak *objects* are v/V, data is d/D/b/B/r/R -> excluded.
        if typ in _NM_BIND:
            syms.append((name, size, _NM_BIND[typ]))
    return syms


# ---------------------------------------------------------------------------
# Linker-map symbol closure -- the authority on weak/global/local binding.
# Only *linked* symbols appear here; deadstripped (UNUSED) ones never do, which
# is exactly why UNUSED symbols are non-weak by construction.
# ---------------------------------------------------------------------------

#   "  3] __dt__26__partial_array_destructorFv (func,weak) found in Runtime..."
_RE_CLOSURE = re.compile(
    r"^\s*(?:\d+\]\s*)?(\S+)\s+\((\w+),(\w+)\)\s+found in\s+(.+?)\s*$"
)


def parse_closure(map_path: str) -> Tuple[Dict[Tuple[str, str], str], Dict[str, str]]:
    """Return (binding_by_tu_name, binding_by_name) for func symbols.

    binding_by_name maps a name to its binding only when unambiguous across
    every TU that defines it (so weak/global resolve cleanly; a name that is
    local in several TUs still collapses to 'local').
    """
    with open(map_path, encoding="utf-8", errors="replace") as f:
        lines = f.read().splitlines()

    by_tu: Dict[Tuple[str, str], str] = {}
    seen: Dict[str, set] = {}
    for l in lines:
        if l.strip().endswith("section layout"):
            break  # closure precedes all the section layouts
        m = _RE_CLOSURE.match(l)
        if not m or m.group(2) != "func":
            continue
        name, binding, tu = m.group(1), m.group(3), m.group(4).strip()
        by_tu[(tu, name)] = binding
        seen.setdefault(name, set()).add(binding)

    by_name = {n: next(iter(b)) for n, b in seen.items() if len(b) == 1}
    return by_tu, by_name


# ---------------------------------------------------------------------------
# Comparison + reporting
# ---------------------------------------------------------------------------

def order_diff(expected: List[str], actual: List[str]) -> List[str]:
    """Readable unified-style diff of two orderings of the same symbol set."""
    import difflib

    sm = difflib.SequenceMatcher(a=expected, b=actual, autojunk=False)
    out: List[str] = []
    for tag, i1, i2, j1, j2 in sm.get_opcodes():
        if tag == "equal":
            for n in expected[i1:i2]:
                out.append(f"    {n}")
        else:
            for n in expected[i1:i2]:
                out.append(f"  - {n}   (map position)")
            for n in actual[j1:j2]:
                out.append(f"  + {n}   (object position)")
    return out


def main() -> None:
    ap = argparse.ArgumentParser(
        description="Validate TU function symbol order against the linker map."
    )
    ap.add_argument("-u", "--unit", required=True,
                    help="objdiff unit name, e.g. mario/MarioUtil/MathUtil")
    ap.add_argument("--map", default=DEFAULT_MAP,
                    help=f"path to the linker map (default: {DEFAULT_MAP})")
    ap.add_argument("--map-tu",
                    help="override the map .text-layout TU identifier")
    ap.add_argument("--reverse", action="store_true",
                    help="compare against the reversed object order (escape hatch; "
                         "no known unit needs this)")
    ap.add_argument("-v", "--verbose", action="store_true",
                    help="list the object-only symbols not in the map "
                         "(by default only their count is shown)")
    args = ap.parse_args()

    unit = resolve_unit(load_units(), args.unit)
    meta = unit.get("metadata", {})
    source_path = meta.get("source_path", "")
    base_path = os.path.join(root_dir, unit["base_path"])
    reverse_fn = bool(meta.get("reverse_fn_order"))

    tus = parse_text_layout(args.map)
    tu_id = find_map_tu(tus, source_path, args.map_tu)
    map_syms = tus[tu_id]
    bind_by_tu, bind_by_name = parse_closure(args.map)

    def map_binding(name: str) -> Optional[str]:
        return bind_by_tu.get((tu_id, name)) or bind_by_name.get(name)

    obj_syms = obj_functions(base_path)
    if args.reverse:
        obj_syms = list(reversed(obj_syms))

    map_names = [s.name for s in map_syms]
    map_set = set(map_names)
    map_size = {s.name: s.size for s in map_syms}
    unused_names = {s.name for s in map_syms if s.unused}

    obj_names = [n for n, _, _ in obj_syms]
    obj_set = set(obj_names)
    obj_size = {n: sz for n, sz, _ in obj_syms}
    obj_bind = {n: b for n, _, b in obj_syms}

    print(f"Unit        : {unit['name']}")
    print(f"Source      : {source_path}")
    print(f"Object      : {os.path.relpath(base_path, root_dir)}")
    print(f"Map TU      : {tu_id}")
    print(f"Map symbols : {len(map_names)} ({len(unused_names)} UNUSED)   "
          f"Object .text symbols: {len(obj_names)}")
    print(f"reverse_fn_order: {reverse_fn}"
          + ("   [comparing reversed]" if args.reverse else ""))
    print("-" * 78)

    failures = 0

    # --- MISSING ----------------------------------------------------------
    missing = [s for s in map_syms if s.name not in obj_set]
    if missing:
        failures += 1
        print(f"\n[FAIL] {len(missing)} map symbol(s) MISSING from the object:")
        for s in missing:
            tag = " (UNUSED)" if s.unused else ""
            print(f"  - {s.name}{tag}")
    else:
        print("\n[ OK ] All map symbols are present in the object.")

    # --- ORDER (over the intersection) ------------------------------------
    # Non-weak ordering is an error. Weak-symbol ordering is compiler-controlled
    # and painful to steer, so a disorder that only involves weak symbols is a
    # warning that does not fail the check (and won't block a PR).
    present_map = [n for n in map_names if n in obj_set]
    obj_in_map = [n for n in obj_names if n in map_set]
    nonweak = lambda n: obj_bind.get(n) != "weak"
    present_map_nw = [n for n in present_map if nonweak(n)]
    obj_in_map_nw = [n for n in obj_in_map if nonweak(n)]
    order_weak_warn = False
    if present_map_nw != obj_in_map_nw:
        failures += 1
        print("\n[FAIL] Non-weak symbol ORDER differs from the map "
              "(weak & object-only symbols ignored):")
        for line in order_diff(present_map_nw, obj_in_map_nw):
            print(line)
    elif present_map != obj_in_map:
        order_weak_warn = True
        print("\n[WARN] Only weak symbols are out of order "
              "(compiler-controlled -- does not fail the check):")
        for line in order_diff(present_map, obj_in_map):
            print(line)
    else:
        print("[ OK ] Symbol order matches the map.")

    # --- SIZE of UNUSED symbols ------------------------------------------
    size_bad = [
        s for s in map_syms
        if s.unused and s.name in obj_size and obj_size[s.name] != s.size
    ]
    if size_bad:
        # Warning only: matching an inlined body's exact size is hard, and it
        # doesn't affect correctness of what does get emitted. Does not fail.
        print(f"\n[WARN] {len(size_bad)} UNUSED symbol(s) with wrong size "
              f"(best-effort -- does not fail the check):")
        for s in size_bad:
            print(f"  - {s.name}")
            print(f"      map:    0x{s.size:x} ({s.size})")
            print(f"      object: 0x{obj_size[s.name]:x} ({obj_size[s.name]})")
    elif unused_names:
        checked = sum(1 for n in unused_names if n in obj_size)
        print(f"[ OK ] UNUSED symbol sizes match ({checked}/{len(unused_names)} checked).")

    # --- BINDING (weak / local / global) ---------------------------------
    # Only *placed* (linked) symbols can be checked: the closure records their
    # exact binding. UNUSED symbols are deadstripped, so they are absent from
    # the closure and the .text layout records no binding for them -- their
    # weak/local/global linkage is simply unknowable from the map (a weak symbol
    # referenced only by dead code is dead-stripped and listed UNUSED just like
    # a local or global one), so we cannot and do not check them.
    bind_bad = []  # (name, expected, actual)
    for s in map_syms:
        if s.unused:
            continue
        actual = obj_bind.get(s.name)
        if actual is None:
            continue  # missing -> already reported
        expected = map_binding(s.name)
        if expected is not None and expected != actual:
            bind_bad.append((s.name, expected, actual))
    if bind_bad:
        failures += 1
        print(f"\n[FAIL] {len(bind_bad)} symbol(s) with wrong linkage "
              "(weak/local/global):")
        for name, exp, act in bind_bad:
            print(f"  - {name}")
            print(f"      map:    {exp}")
            print(f"      object: {act}")
    else:
        print("[ OK ] Symbol linkage (weak/local/global) matches the map.")

    # --- extras (informational) ------------------------------------------
    extras = [n for n in obj_names if n not in map_set]
    if extras:
        print(f"\n[INFO] {len(extras)} object-only symbol(s) not in the map "
              "(usually deduplicated weak/inline helpers; not an error"
              + ("; -v to list)." if not args.verbose else "):"))
        if args.verbose:
            for n in extras:
                print(f"    {n}")

    warns = []
    if order_weak_warn:
        warns.append("weak-symbol order")
    if size_bad:
        warns.append(f"{len(size_bad)} UNUSED size mismatch(es)")

    print("-" * 78)
    if failures:
        tail = f", {'; '.join(warns)} warning(s)" if warns else ""
        print(f"RESULT: FAIL ({failures} error category/categories{tail})")
        sys.exit(1)
    if warns:
        print(f"RESULT: PASS with warnings ({'; '.join(warns)} -- see above)")
    else:
        print("RESULT: PASS")


if __name__ == "__main__":
    main()
