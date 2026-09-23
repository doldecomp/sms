#!/usr/bin/env python3
"""
Sweep float-zero test spellings in functions below 100%.

For every function under 100% in build/GMSE01/report.json, find float-zero tests
(`x == 0.0f`, `x != 0.0f`, `0.0f == x`, `0.0f != x`, and `!x` / truthiness on
names declared f32) and try the alternative spellings.  Each variant is compiled
in a lever-search shadow root and the whole unit is scored; a variant is kept
only if its function gains and no other symbol or section drops.  Kept variants
are then combined greedily per unit and re-verified.

Nothing is written to the worktree unless --apply is given.

Usage:
  tools/float-zero-sweep.py [--units Enemy/foo,...] [--skip Enemy/bar,...] [-j 3] [--apply] [--list]
"""

import argparse
import concurrent.futures as cf
import importlib.util
import json
import os
import re
import sys

ROOT = os.path.abspath(os.path.join(os.path.dirname(os.path.realpath(__file__)), ".."))
spec = importlib.util.spec_from_file_location("lever_search", os.path.join(ROOT, "tools", "lever-search.py"))
ls = importlib.util.module_from_spec(spec)
sys.modules["lever_search"] = ls
spec.loader.exec_module(ls)

ZERO = r"0(?:\.0*)?f"
OP = r"(?:this\s*->\s*)?[A-Za-z_]\w*(?:\s*(?:->|\.)\s*[A-Za-z_]\w*|\[[^\[\]]*\])*"
PREV_OK = ("(", "&&", "||", ",", "return", "?", ":", "=")
NEXT_OK = (")", "&&", "||", ";", "?", ",", ":")


def f32_names():
    names = set()
    for dp, _, fs in os.walk(os.path.join(ROOT, "include")):
        for f in fs:
            if f.endswith((".h", ".hpp")):
                t = open(os.path.join(dp, f), encoding="utf-8", errors="replace").read()
                for m in re.finditer(r"\b(?:f32|float)\s+([^;(){}]*);", t):
                    for part in m.group(1).split(","):
                        mm = re.match(r"\s*\**\s*([A-Za-z_]\w*)\s*(?:\[.*\])?\s*(?:=.*)?$", part)
                        if mm:
                            names.add(mm.group(1))
    return names


def prev_tok(text, i):
    j = i - 1
    while j >= 0 and text[j].isspace():
        j -= 1
    if j < 0:
        return ""
    if text[j] in "&|" and j > 0 and text[j - 1] == text[j]:
        return text[j] * 2
    if text[j].isalnum() or text[j] == "_":
        k = j
        while k >= 0 and (text[k].isalnum() or text[k] == "_"):
            k -= 1
        return text[k + 1:j + 1]
    if text[j] == "=" and j > 0 and text[j - 1] in "=!<>":
        return text[j - 1:j + 1]
    return text[j]


def prev_tok_pos(text, i):
    j = i - 1
    while j >= 0 and text[j].isspace():
        j -= 1
    return j


def next_tok(text, i):
    j = i
    while j < len(text) and text[j].isspace():
        j += 1
    if j >= len(text):
        return ""
    if text[j] in "&|" and j + 1 < len(text) and text[j + 1] == text[j]:
        return text[j] * 2
    if text[j] == "=" and j + 1 < len(text) and text[j + 1] == "=":
        return "=="
    return text[j]


def sites_in(text, lo, hi, fnames, local_f):
    """Yield (start, end, [replacements], desc) within text[lo:hi]."""
    body = text[lo:hi]
    out = []
    seen = set()
    # OP ==/!= 0.0f
    for m in re.finditer(r"(?<![\w.>\]])(" + OP + r")\s*(==|!=)\s*(" + ZERO + r")(?![\w.])", body):
        s, e = lo + m.start(), lo + m.end()
        if prev_tok(text, s) not in PREV_OK or next_tok(text, e) not in NEXT_OK:
            continue
        op, cmp, z = m.group(1), m.group(2), m.group(3)
        if cmp == "==":
            reps = ["!" + op, z + " == " + op]
        else:
            reps = [op, z + " != " + op]
        out.append((s, e, reps, m.group(0)))
        seen.add(s)
    # 0.0f ==/!= OP
    for m in re.finditer(r"(?<![\w.])(" + ZERO + r")\s*(==|!=)\s*(" + OP + r")(?![\w(])", body):
        s, e = lo + m.start(), lo + m.end()
        if prev_tok(text, s) not in PREV_OK or next_tok(text, e) not in NEXT_OK:
            continue
        z, cmp, op = m.group(1), m.group(2), m.group(3)
        if cmp == "==":
            reps = ["!" + op, op + " == " + z]
        else:
            reps = [op, op + " != " + z]
        out.append((s, e, reps, m.group(0)))
    # !OP / truthy OP on f32 names
    for m in re.finditer(r"(?<![\w.>\]])(" + OP + r")(?![\w(\[])", body):
        s, e = lo + m.start(), lo + m.end()
        last = re.findall(r"[A-Za-z_]\w*", m.group(1))[-1]
        if last in ("f32", "f64", "float", "double") or (last not in fnames and last not in local_f):
            continue
        nt = next_tok(text, e)
        if nt not in NEXT_OK:
            continue
        pt = prev_tok(text, s)
        if pt == "!":
            bang = prev_tok_pos(text, s)
            pp = prev_tok(text, bang)
            if pp not in PREV_OK + ("!",):
                continue
            op = m.group(1)
            out.append((bang, e, [op + " == 0.0f", "0.0f == " + op], text[bang:e]))
        elif pt in ("(", "&&", "||", "?") and nt in (")", "&&", "||", "?"):
            if pt == "(":
                pp = prev_tok(text, prev_tok_pos(text, s))
                if pp not in ("if", "while", "(", "&&", "||", "!", "return", "="):
                    continue
            if pt == "?":
                continue
            op = m.group(1)
            out.append((s, e, [op + " != 0.0f", "0.0f != " + op], m.group(0)))
    return out


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--units")
    ap.add_argument("--skip", default="")
    ap.add_argument("-j", "--jobs", type=int, default=3)
    ap.add_argument("--apply", action="store_true")
    ap.add_argument("--list", action="store_true", help="only list sites (uses the built object)")
    ap.add_argument("--log", default=None)
    args = ap.parse_args()
    skip = set(x for x in args.skip.split(",") if x)
    rep = json.load(open(os.path.join(ROOT, "build/GMSE01/report.json")))
    want = {}
    for u in rep["units"]:
        src = u.get("metadata", {}).get("source_path")
        if not src or u.get("metadata", {}).get("auto_generated"):
            continue
        bad = [f["name"] for f in u.get("functions", []) if float(f.get("fuzzy_match_percent", 100)) < 100.0]
        if bad:
            want[u["name"]] = bad
    if args.units:
        sel = set("mario/" + x if not x.startswith("mario/") else x for x in args.units.split(","))
        want = {k: v for k, v in want.items() if k in sel}
    fnames = f32_names()
    logf = open(args.log, "a") if args.log else None

    def log(*a):
        s = " ".join(str(x) for x in a)
        print(s, flush=True)
        if logf:
            logf.write(s + "\n")
            logf.flush()

    total_sites = 0
    for uname, bad in sorted(want.items()):
        short = uname.split("/", 1)[1]
        if short in skip:
            continue
        s = ls.Searcher(uname, "__none__", None, args.jobs, "/tmp", verbose=False)
        try:
            text = s.base_text
            base_obj = os.path.join(ROOT, s.obj)
            if args.list:
                data = ls.objdiff_json(s.target, base_obj)
            else:
                _, data = s.compile(text, keep_json=True)
                if data is None:
                    log("SKIP", uname, "base compile failed")
                    continue
            base_prof = ls.unit_profile(data)
            dem = {x["name"]: x.get("demangled_name", x["name"]) for x in data["left"]["symbols"]}
            toks = ls.lex(text)
            bm = ls.bracket_map(toks)
            cands = []  # (fn, start, end, rep, desc)
            for fn in bad:
                if fn not in dem:
                    continue
                try:
                    loc = ls.locate_function(toks, bm, ls.parse_demangled(dem[fn]))
                except Exception:
                    loc = None
                if loc is None:
                    continue
                lo, hi = toks[loc.bo].s, toks[loc.bc].s
                body = text[lo:hi]
                local_f = set(re.findall(r"\b(?:f32|float)\s+([A-Za-z_]\w*)", text[toks[loc.lp].s:hi]))
                for (st, en, reps, desc) in sites_in(text, lo, hi, fnames, local_f):
                    for r in reps:
                        cands.append((fn, st, en, r, desc))
            nsites = len(set((c[1]) for c in cands))
            total_sites += nsites
            if not cands:
                continue
            log("UNIT", uname, "sites", nsites, "variants", len(cands))
            if args.list:
                for c in cands:
                    ln = text.count("\n", 0, c[1]) + 1
                    log("   ", c[0], "L%d" % ln, repr(c[4]), "->", repr(c[3]))
                continue

            def trial(c):
                fn, st, en, r, desc = c
                t = text[:st] + r + text[en:]
                sc, d = s.compile(t, keep_json=True)
                if d is None:
                    return c, None, ["compile fail"]
                prof = ls.unit_profile(d)
                return c, prof, ls.regressions(base_prof, prof, fn)

            winners = []
            with cf.ThreadPoolExecutor(args.jobs) as ex:
                for c, prof, bad_r in ex.map(trial, cands):
                    fn = c[0]
                    ln = text.count("\n", 0, c[1]) + 1
                    if prof is None:
                        continue
                    b, v = base_prof.get(fn, 0), prof.get(fn, 0)
                    tag = "GAIN" if v > b + 1e-6 and not bad_r else ("drop" if bad_r else "same" if abs(v - b) < 1e-6 else "loss")
                    log("  %s %s L%d %r -> %r  %.2f->%.2f %s" % (tag, fn, ln, c[4], c[3], b, v,
                                                               "; ".join(bad_r[:3])))
                    if tag == "GAIN":
                        winners.append((v - b, c))
            if not winners:
                continue
            # greedy combine
            winners.sort(key=lambda x: -x[0])
            chosen, cur_prof, cur_text = [], base_prof, text
            used = set()
            for gain, c in winners:
                if c[1] in used:
                    continue
                edits = sorted(chosen + [c], key=lambda x: -x[1])
                t = text
                for (_, st, en, r, _) in edits:
                    t = t[:st] + r + t[en:]
                sc, d = s.compile(t, keep_json=True)
                if d is None:
                    continue
                prof = ls.unit_profile(d)
                if ls.regressions(cur_prof, prof, c[0]) or prof.get(c[0], 0) <= cur_prof.get(c[0], 0) + 1e-6:
                    log("  combine-reject", c[0], repr(c[4]))
                    continue
                chosen.append(c)
                used.add(c[1])
                cur_prof, cur_text = prof, t
            log("RESULT", uname, "kept", len(chosen), " ".join("%s:%.2f->%.2f" % (fn, base_prof.get(fn, 0), cur_prof.get(fn, 0))
                                                         for fn in sorted(set(c[0] for c in chosen))))
            if args.apply and chosen:
                with open(s.orig_path, "w", encoding="utf-8", newline="") as f:
                    f.write(cur_text)
        finally:
            s.close()
    log("TOTAL sites", total_sites)


if __name__ == "__main__":
    main()
