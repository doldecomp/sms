#!/usr/bin/env python3

"""
Relocation-target bug census (batch 166's bug class).

A relocation that points at the *wrong symbol* -- a wrong callee, a
devirtualised call through an explicitly qualified `Base::method()`, a
swapped enum/constant symbol, a wrong table -- renders with an identical
opcode and identical operands.  objdiff therefore scores it 100% and
`ninja changes_all` never flags it.  These three detectors find it
anyway, by comparing what the *relocations* point at rather than what the
instructions look like.

Modes:
  virt   `blrl` count asymmetry per function (target dispatches through
         the vtable where we emit a direct `bl`): a devirtualisation.
  calls  per-function call-set differences (`bl` target multisets), so a
         wrong callee or a missing/extra call shows up even at 100%.
  reloc  relocation-target *name* mismatches per paired instruction
         index, skipping literal-pool ids and section-relative names
         which differ legitimately.
  all    run all three (the default).

Usage:
  python3 tools/check-relocs.py                       # every unlinked unit
  python3 tools/check-relocs.py --mode reloc
  python3 tools/check-relocs.py -u mario/Enemy/fireWanwan
  python3 tools/check-relocs.py --category game --tsv out/
"""

import argparse
import json
import os
import re
import subprocess
import sys
from collections import Counter
from concurrent.futures import ThreadPoolExecutor

script_dir = os.path.dirname(os.path.realpath(__file__))
root_dir = os.path.abspath(os.path.join(script_dir, ".."))

OBJDIFF_CLI = os.environ.get(
    "OBJDIFF_CLI", os.path.join(root_dir, "build", "tools", "objdiff-cli"))
REPORT = os.path.join(root_dir, "build", "GMSE01", "report.json")
OBJECTS = os.path.join(root_dir, "config", "GMSE01", "objects.json")

# Relocation target names that legitimately differ between the two sides:
# literal-pool ids are numbered by the compiler, dtk names gap fillers and
# string bases by address, and a vtable reference is checked structurally.
SKIP_PREFIXES = ("@", ".", "gap_", "__vt__", "...")


# `instance$3584` vs `instance$1852`: local-static and literal ids are
# numbered per TU by the compiler and legitimately renumber, so two names
# that differ only in their `$<digits>` suffixes are not a bug.
ID_SUFFIX = re.compile(r"\$\d+")


def skip_name(n):
    return n.startswith(SKIP_PREFIXES) or n == "?"


def same_modulo_ids(a, b):
    return ID_SUFFIX.sub("$", a) == ID_SUFFIX.sub("$", b)


def run_objdiff(unit):
    p = subprocess.run(
        [OBJDIFF_CLI, "diff", "-c", "functionRelocDiffs=data_value",
         "-u", unit, "-o", "-", "--format", "json"],
        cwd=root_dir, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    if p.returncode != 0 or not p.stdout:
        return None
    try:
        return json.loads(p.stdout)
    except ValueError:
        return None


def insns(sym):
    return [i.get("instruction", {}).get("formatted", "")
            for i in (sym.get("instructions") or [])]


def call_stats(sym):
    """(number of `blrl`, list of direct `bl` targets)."""
    ins = insns(sym)
    blrl = sum(1 for i in ins if i.startswith("blrl"))
    bls = [i.split(None, 1)[1] for i in ins if i.startswith("bl ")]
    return blrl, bls


def reloc_names(sym, syms):
    """Per instruction index, the name of its relocation target (or None)."""
    out = []
    for it in (sym.get("instructions") or []):
        r = it.get("instruction", {}).get("relocation")
        if not r:
            out.append(None)
            continue
        t = r.get("target_symbol")
        out.append(syms[t].get("name", "?")
                   if t is not None and t < len(syms) else "?")
    return out


def paired_symbols(d):
    """Yield (target_sym, our_sym, match_percent) for non-100% functions."""
    lsyms = d.get("left", {}).get("symbols", [])
    rsyms = d.get("right", {}).get("symbols", [])
    for sym in lsyms:
        if sym.get("kind") != "SYMBOL_FUNCTION":
            continue
        mp = sym.get("match_percent")
        if mp is None or mp >= 100.0:
            continue
        ts = sym.get("target_symbol")
        if ts is None or ts >= len(rsyms):
            continue
        yield sym, rsyms[ts], mp, lsyms, rsyms


def scan(entry):
    """Run every requested detector over one unit."""
    unit, bytes_left, modes = entry
    d = run_objdiff(unit)
    if d is None:
        return []
    rows = []
    for sym, ours, mp, lsyms, rsyms in paired_symbols(d):
        fn = sym.get("demangled_name") or sym.get("name", "?")
        base = {"unit": unit, "bytes_left": bytes_left, "fn": fn, "pct": mp}
        tb, tbl = call_stats(sym)
        ob, obl = call_stats(ours)

        if "virt" in modes and tb > ob:
            ct, co = Counter(tbl), Counter(obl)
            extra = sorted((co - ct).elements())
            r = dict(base)
            r["mode"] = "virt"
            r["detail"] = "blrl %d/%d" % (tb, ob)
            r["note"] = "; ".join(extra)
            rows.append(r)

        if "calls" in modes:
            ct, co = Counter(tbl), Counter(obl)
            only_t = sorted((ct - co).elements())
            only_o = sorted((co - ct).elements())
            if only_t or only_o:
                r = dict(base)
                r["mode"] = "calls"
                r["detail"] = "blrl %d/%d" % (tb, ob)
                r["note"] = "; ".join(["T:" + c for c in only_t]
                                      + ["O:" + c for c in only_o])
                rows.append(r)

        if "reloc" in modes:
            ln = reloc_names(sym, lsyms)
            rn = reloc_names(ours, rsyms)
            seen = Counter()
            for a, b in zip(ln, rn):
                if a is None or b is None or a == b:
                    continue
                if skip_name(a) or skip_name(b):
                    continue
                if same_modulo_ids(a, b):
                    continue
                seen[(a, b)] += 1
            if seen:
                r = dict(base)
                r["mode"] = "reloc"
                r["detail"] = "%d site%s" % (sum(seen.values()),
                                             "" if sum(seen.values()) == 1
                                             else "s")
                r["note"] = " | ".join(
                    "%dx %s -> %s" % (v, k[0], k[1])
                    for k, v in sorted(seen.items(), key=lambda x: -x[1]))
                rows.append(r)
    return rows


def unlinked_units(unit_filter, category):
    """Units with code that are not source-linked yet, plus bytes left."""
    if not os.path.exists(REPORT):
        sys.exit("no %s -- run `build/venv/bin/ninja` first" % REPORT)
    linked = set(json.load(open(OBJECTS))) if os.path.exists(OBJECTS) else set()
    units = []
    for u in json.load(open(REPORT))["units"]:
        name = u["name"]
        if unit_filter and unit_filter not in name:
            continue
        md = u.get("metadata", {})
        cats = md.get("progress_categories") or []
        if category and category not in cats:
            continue
        src = md.get("source_path", "")
        if src.startswith("src/"):
            src = src[4:]
        if src in linked:
            continue
        m = u.get("measures", {})
        total = int(m.get("total_code", 0) or 0)
        if total == 0:
            continue
        units.append((name, total - int(m.get("matched_code", 0) or 0)))
    return units


def main():
    ap = argparse.ArgumentParser(description=__doc__,
                                 formatter_class=argparse.RawDescriptionHelpFormatter)
    ap.add_argument("--mode", default="all",
                    choices=["virt", "calls", "reloc", "all"],
                    help="which detector to run (default: all)")
    ap.add_argument("-u", "--unit", default=None,
                    help="only units whose name contains this substring")
    ap.add_argument("--category", default=None,
                    choices=["game", "sdk", "jsystem"],
                    help="only units in this progress category")
    ap.add_argument("-j", "--jobs", type=int, default=8)
    ap.add_argument("-n", "--limit", type=int, default=0,
                    help="print at most N rows (0 = all)")
    ap.add_argument("--tsv", default=None,
                    help="also write the full table to this file")
    args = ap.parse_args()

    modes = ["virt", "calls", "reloc"] if args.mode == "all" else [args.mode]
    units = unlinked_units(args.unit, args.category)
    if not units:
        sys.exit("no unlinked units matched")

    rows = []
    with ThreadPoolExecutor(max_workers=args.jobs) as ex:
        for r in ex.map(scan, [(u, b, modes) for u, b in units]):
            rows.extend(r)
    # Rank: smallest units first (cheapest to close), then worst match.
    order = {"virt": 0, "calls": 1, "reloc": 2}
    rows.sort(key=lambda r: (r["bytes_left"], order[r["mode"]], -r["pct"]))

    hdr = "%-7s %-6s %-30s %7s %-12s %s" % (
        "LEFT", "MODE", "UNIT", "MATCH%", "DETAIL", "FUNCTION || TARGET -> OURS")
    lines = [hdr, "-" * len(hdr)]
    for r in rows:
        lines.append("%-7d %-6s %-30s %7.2f %-12s %s || %s" % (
            r["bytes_left"], r["mode"], r["unit"], r["pct"], r["detail"],
            r["fn"], r["note"]))
    shown = lines[:2 + args.limit] if args.limit else lines
    print("\n".join(shown))
    if args.limit and len(rows) > args.limit:
        print("... %d more rows" % (len(rows) - args.limit))
    per_mode = Counter(r["mode"] for r in rows)
    print("\n%d unlinked units scanned, %d flagged sites (%s)" % (
        len(units), len(rows),
        ", ".join("%s %d" % (m, per_mode[m]) for m in modes)))
    if args.tsv:
        with open(args.tsv, "w") as f:
            f.write("bytes_left\tmode\tunit\tmatch%\tdetail\tfunction\tnote\n")
            for r in rows:
                f.write("%d\t%s\t%s\t%.2f\t%s\t%s\t%s\n" % (
                    r["bytes_left"], r["mode"], r["unit"], r["pct"],
                    r["detail"], r["fn"], r["note"]))
        print("full table -> %s" % args.tsv)


if __name__ == "__main__":
    main()
