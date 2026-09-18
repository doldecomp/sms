#!/usr/bin/env python3
"""Depth census: find functions where the TARGET has a `bl` to a
budget-measuring callee (std::sqrtf or a small JGeometry::TVec3<f> member)
that OUR build lacks (or has fewer of).

Each such missing `bl` means our call site sits one or more inline levels
too shallow (research batch 146 / 104).

Usage: python3 census_depth.py [out.tsv]
"""
import json
import os
import re
import subprocess
import sys
from collections import Counter

ROOT = os.environ.get("SMS_ROOT", os.getcwd())
CLI = os.path.join(ROOT, "build", "tools", "objdiff-cli")
REPORT = os.path.join(ROOT, "build", "GMSE01", "report.json")

VEC_MEMBERS = (
    "sub|add|scale|length|squared|normalize|set|setLength|cross|dot|mult|"
    "div|negate|setMin|setMax|setAll|__as|__apl|__ami|__amu"
)
# mangled callee patterns that are depth measurements
CALLEE_RE = re.compile(
    r"^(?:sqrtf__3stdFf"
    r"|(?:%s)__Q29JGeometry8TVec3<f>"
    r"|__ct__Q29JGeometry8TVec3<f>FRCQ29JGeometry8TVec3<f>"
    r"|(?:sqrt|inv_sqrt)__Q23JGeometry5TUtil<f>"
    r"|(?:sqrt|inv_sqrt)__5TUtil<f>"
    r")" % VEC_MEMBERS
)

BL_RE = re.compile(r"^bl\s+(\S+)")


def bl_counts(sym):
    c = Counter()
    for item in sym.get("instructions") or []:
        ins = item.get("instruction")
        if not ins:
            continue
        m = BL_RE.match(ins.get("formatted", ""))
        if not m:
            continue
        callee = m.group(1)
        if CALLEE_RE.match(callee):
            c[callee] += 1
    return c


def main():
    out_path = sys.argv[1] if len(sys.argv) > 1 else os.path.join(
        os.path.dirname(os.path.abspath(__file__)), "census.tsv")
    rep = json.load(open(REPORT))
    units = []
    for u in rep["units"]:
        m = u.get("measures", {})
        if u.get("metadata", {}).get("complete"):
            continue  # linked
        total = int(m.get("total_code", 0) or 0)
        matched = int(m.get("matched_code", 0) or 0)
        if total == 0:
            continue
        units.append((u["name"], total - matched, total,
                      m.get("matched_code_percent", 0.0)))

    rows = []
    for name, left_bytes, total, pct in units:
        p = subprocess.run(
            [CLI, "diff", "-c", "functionRelocDiffs=data_value",
             "-u", name, "-o", "-", "--format", "json"],
            cwd=ROOT, stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
        if p.returncode != 0 or not p.stdout:
            continue
        try:
            d = json.loads(p.stdout)
        except Exception:
            continue
        right = d.get("right", {}).get("symbols", [])
        for sym in d.get("left", {}).get("symbols", []):
            if sym.get("kind") != "SYMBOL_FUNCTION":
                continue
            mp = sym.get("match_percent")
            ts = sym.get("target_symbol")
            if mp is None or mp >= 100.0:
                continue
            tgt = bl_counts(sym)
            if not tgt:
                continue
            ours = bl_counts(right[ts]) if ts is not None and ts < len(right) else Counter()
            missing = {}
            for k, v in tgt.items():
                if ours.get(k, 0) < v:
                    missing[k] = (v, ours.get(k, 0))
            if not missing:
                continue
            rows.append({
                "unit": name,
                "bytes_left": left_bytes,
                "fn": sym.get("demangled_name") or sym["name"],
                "mangled": sym["name"],
                "pct": mp,
                "size": int(sym.get("size", "0")),
                "missing": missing,
            })

    rows.sort(key=lambda r: (r["bytes_left"], -r["pct"]))
    with open(out_path, "w") as f:
        f.write("bytes_left\tunit\tmatch%\tsize\tfunction\tmissing (target/ours)\n")
        for r in rows:
            miss = ", ".join("%s %d/%d" % (k, v[0], v[1])
                             for k, v in sorted(r["missing"].items()))
            f.write("%d\t%s\t%.2f\t%d\t%s\t%s\n" % (
                r["bytes_left"], r["unit"], r["pct"], r["size"], r["fn"], miss))
    json.dump(rows, open(out_path.replace(".tsv", ".json"), "w"), indent=1)
    print("units scanned: %d, hits: %d -> %s" % (len(units), len(rows), out_path))


if __name__ == "__main__":
    main()
