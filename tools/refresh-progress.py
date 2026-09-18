#!/usr/bin/env python3
"""Refresh PROGRESS.md's summary rows from build/GMSE01/report.json and objects.json, then print them."""
import json, os, re
r = json.load(open("build/GMSE01/report.json"))
linked = json.load(open("config/GMSE01/objects.json"))
lstems = {os.path.splitext(x)[0] for x in linked}
cats = {c["id"]: c["measures"] for c in r["categories"]}
def row(label, cat_id):
    m = cats[cat_id]
    units = [u for u in r["units"] if cat_id in u.get("metadata", {}).get("progress_categories", [])]
    n = sum(1 for u in units if u["name"].split("mario/", 1)[-1] in lstems)
    return f"| {label} | {m['matched_code_percent']:.2f}% | {m['complete_code_percent']:.2f}% | {n} / {len(units)} |"
a = r["measures"]
s = open("PROGRESS.md").read().split("\n")
for i, l in enumerate(s):
    if l.startswith("| Game |"): s[i] = row("Game", "game")
    elif l.startswith("| JSystem |"): s[i] = row("JSystem", "jsystem")
    elif l.startswith("| SDK |"): s[i] = row("SDK", "sdk")
    elif l.startswith("| All |"):
        s[i] = f"| All | {a['matched_code_percent']:.2f}% | {a['complete_code_percent']:.2f}% | {len(linked)} / {len(r['units'])} |"
    elif re.match(r"^[\d,]+ of [\d,]+ functions match", l):
        s[i] = re.sub(r"^[\d,]+ of [\d,]+ functions match", f"{int(a['matched_functions']):,} of {int(a['total_functions']):,} functions match", l)
open("PROGRESS.md", "w").write("\n".join(s))
for l in s:
    if l.startswith(("| Game |", "| JSystem |", "| SDK |", "| All |")): print(l)
