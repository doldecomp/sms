#!/usr/bin/env python3
"""Per-hour progress and agent token spend.

usage: hourly-report.py [--since 'YYYY-MM-DD HH:MM'] [--projects DIR]

Progress comes from the PROGRESS.md history (last All/Game matched and linked
values committed in each hour). Tokens come from every subagent transcript
under the Claude project directory, bucketed by message hour (local time):
output tokens, fresh input (input + cache writes) and cache reads.
"""
import argparse, glob, json, os, re, subprocess
from collections import defaultdict
from datetime import datetime

ap = argparse.ArgumentParser()
ap.add_argument("--since", default=None)
# Claude Code names a project's directory after its path, with every other
# character replaced by "-".
here = subprocess.run(["git", "rev-parse", "--show-toplevel"], capture_output=True, text=True).stdout.strip()
ap.add_argument("--projects", default=os.path.expanduser(
    "~/.claude/projects/" + re.sub(r"[^A-Za-z0-9]", "-", here or os.getcwd())))
a = ap.parse_args()
since = datetime.strptime(a.since, "%Y-%m-%d %H:%M") if a.since else None

def hour(dt): return dt.strftime("%Y-%m-%d %H:00")

# progress: last PROGRESS.md snapshot per hour
prog = {}
log = subprocess.run(["git", "log", "--reverse", "--format=%H %ci", "--", "PROGRESS.md"],
                     capture_output=True, text=True).stdout.split("\n")
for line in log:
    if not line: continue
    sha, ts = line.split(" ", 1)
    dt = datetime.strptime(ts[:19], "%Y-%m-%d %H:%M:%S")
    if since and dt < since: continue
    txt = subprocess.run(["git", "show", f"{sha}:PROGRESS.md"], capture_output=True, text=True).stdout
    g = re.search(r"^\| Game \| ([\d.]+)% \| [\d.]+% \| (\d+) /", txt, re.M)
    al = re.search(r"^\| All \| ([\d.]+)% \| [\d.]+% \| (\d+) /", txt, re.M)
    if g and al:
        prog[hour(dt)] = (float(g.group(1)), float(al.group(1)), int(g.group(2)), int(al.group(2)))

# tokens and agent counts from subagent transcripts
tok = defaultdict(lambda: [0, 0, 0])
agents = defaultdict(set)
for p in glob.glob(os.path.join(a.projects, "*", "subagents", "*.jsonl")):
    for l in open(p, errors="ignore"):
        try: e = json.loads(l)
        except Exception: continue
        if e.get("type") != "assistant" or not e.get("timestamp"): continue
        dt = datetime.fromisoformat(e["timestamp"].replace("Z", "+00:00")).astimezone().replace(tzinfo=None)
        if since and dt < since: continue
        u = (e.get("message") or {}).get("usage") or {}
        h = hour(dt)
        tok[h][0] += u.get("output_tokens", 0)
        tok[h][1] += u.get("input_tokens", 0) + u.get("cache_creation_input_tokens", 0)
        tok[h][2] += u.get("cache_read_input_tokens", 0)
        agents[h].add(os.path.basename(p))

hours = sorted(set(prog) | set(tok))
print("| Hour | All matched | Game matched | Linked (all) | Agents | Output tok | Fresh input tok | Cache-read tok |")
print("|---|---:|---:|---:|---:|---:|---:|---:|")
last = None
for h in hours:
    if h in prog: last = prog[h]
    g, al, gl, ll = last if last else (0, 0, 0, 0)
    o, i, c = tok.get(h, [0, 0, 0])
    print(f"| {h} | {al:.2f}% | {g:.2f}% | {ll} | {len(agents.get(h, ()))} | {o/1e3:.0f}k | {i/1e3:.0f}k | {c/1e6:.1f}M |")
