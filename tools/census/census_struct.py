#!/usr/bin/env python3
"""Structural-residue census.

For every *unlinked* GAME unit (progress category "game", source not in
config/GMSE01/objects.json), run objdiff-cli and count, per non-100%
function, the *opcode-level* diff kinds (DIFF_REPLACE / DIFF_INSERT /
DIFF_DELETE) versus the operand-only ones (DIFF_ARG_MISMATCH /
DIFF_OP_MISMATCH).

Functions whose replace+insert+delete count is in [1, 12] are small
structural residues: a wrong instruction, a missing one or an extra one,
which usually means a real reconstruction bug rather than a frame or
register permutation.

Ranked by the unit's remaining bytes ascending (cheapest unit to close).

Usage: python3 census_struct.py [out.tsv]
"""
import json
import os
import subprocess
import sys
from concurrent.futures import ThreadPoolExecutor

ROOT = os.environ.get("SMS_ROOT", os.getcwd())
CLI = os.path.join(ROOT, "build", "tools", "objdiff-cli")
REPORT = os.path.join(ROOT, "build", "GMSE01", "report.json")
OBJECTS = os.path.join(ROOT, "config", "GMSE01", "objects.json")

STRUCT = ("DIFF_REPLACE", "DIFF_INSERT", "DIFF_DELETE")
SOFT = ("DIFF_ARG_MISMATCH", "DIFF_OP_MISMATCH")

LO, HI = 1, 12


def counts(sym):
    c = {k: 0 for k in STRUCT + SOFT}
    for item in sym.get("instructions") or []:
        k = item.get("diff_kind", "")
        if k in c:
            c[k] += 1
    return c


def scan(entry):
    name, left_bytes = entry
    p = subprocess.run(
        [CLI, "diff", "-c", "functionRelocDiffs=data_value",
         "-u", name, "-o", "-", "--format", "json"],
        cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    if p.returncode != 0 or not p.stdout:
        return []
    try:
        d = json.loads(p.stdout)
    except Exception:
        return []
    right = d.get("right", {}).get("symbols", [])
    out = []
    for sym in d.get("left", {}).get("symbols", []):
        if sym.get("kind") != "SYMBOL_FUNCTION":
            continue
        mp = sym.get("match_percent")
        if mp is None or mp >= 100.0:
            continue
        ts = sym.get("target_symbol")
        # count on both sides: INSERT lives on our side, DELETE on target's
        cl = counts(sym)
        cr = counts(right[ts]) if ts is not None and ts < len(right) else {}
        c = {k: max(cl.get(k, 0), cr.get(k, 0)) for k in STRUCT + SOFT}
        s = sum(c[k] for k in STRUCT)
        if not (LO <= s <= HI):
            continue
        out.append({
            "unit": name,
            "bytes_left": left_bytes,
            "fn": sym.get("demangled_name") or sym["name"],
            "mangled": sym["name"],
            "pct": mp,
            "size": int(sym.get("size", "0")),
            "struct": s,
            "rep": c["DIFF_REPLACE"],
            "ins": c["DIFF_INSERT"],
            "dele": c["DIFF_DELETE"],
            "soft": c["DIFF_ARG_MISMATCH"] + c["DIFF_OP_MISMATCH"],
        })
    return out


def main():
    out_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "census_struct.tsv")
    linked = set(json.load(open(OBJECTS)))
    rep = json.load(open(REPORT))
    units = []
    for u in rep["units"]:
        md = u.get("metadata", {})
        if "game" not in (md.get("progress_categories") or []):
            continue
        src = md.get("source_path", "")
        if src.startswith("src/"):
            src = src[4:]
        if src in linked:
            continue
        m = u.get("measures", {})
        total = int(m.get("total_code", 0) or 0)
        matched = int(m.get("matched_code", 0) or 0)
        if total == 0:
            continue
        units.append((u["name"], total - matched))

    rows = []
    with ThreadPoolExecutor(max_workers=8) as ex:
        for r in ex.map(scan, units):
            rows.extend(r)

    rows.sort(key=lambda r: (r["bytes_left"], -r["pct"]))
    with open(out_path, "w") as f:
        f.write("bytes_left\tunit\tmatch%\tsize\tstruct\trep\tins\tdel\tsoft\tfunction\n")
        for r in rows:
            f.write("%d\t%s\t%.2f\t%d\t%d\t%d\t%d\t%d\t%d\t%s\n" % (
                r["bytes_left"], r["unit"], r["pct"], r["size"], r["struct"],
                r["rep"], r["ins"], r["dele"], r["soft"], r["fn"]))
    json.dump(rows, open(out_path.replace(".tsv", ".json"), "w"), indent=1)
    print("units scanned: %d, hits: %d -> %s" % (len(units), len(rows), out_path))
    print()
    print("%-9s %-34s %6s %5s %3s %2s %2s %2s %4s  %s" % (
        "bytesleft", "unit", "match%", "size", "st", "R", "I", "D", "soft", "function"))
    for r in rows[:40]:
        print("%-9d %-34s %6.2f %5d %3d %2d %2d %2d %4d  %s" % (
            r["bytes_left"], r["unit"], r["pct"], r["size"], r["struct"],
            r["rep"], r["ins"], r["dele"], r["soft"], r["fn"][:90]))


if __name__ == "__main__":
    main()
