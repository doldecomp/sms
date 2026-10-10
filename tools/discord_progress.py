#!/usr/bin/env python3
"""Build a report for each main commit in the last N hours and post a progress graph to Discord."""
import argparse
import json
import os
import subprocess
import sys
from datetime import datetime, timedelta, timezone
from pathlib import Path

import matplotlib

matplotlib.use("Agg")
import matplotlib.dates as mdates
import matplotlib.pyplot as plt
import requests
from matplotlib.ticker import MaxNLocator


def git(*args):
    return subprocess.check_output(["git", *args], text=True).strip()


def commits_in_window(since):
    iso = since.isoformat()
    baseline = git("rev-list", "-1", "--first-parent", f"--before={iso}", "HEAD")
    log = git("log", "--first-parent", "--reverse", f"--since={iso}",
              "--format=%H %cI", "HEAD")
    commits = [line.split() for line in log.splitlines()]
    return baseline, commits


def build_measures(sha, args):
    subprocess.run(["git", "checkout", "-q", "-f", sha], check=True)
    configure = [sys.executable, "configure.py", "--version", args.version]
    if args.binutils:
        configure += ["--binutils", args.binutils]
    if args.compilers:
        configure += ["--compilers", args.compilers]
    report = Path("build") / args.version / "report.json"
    report.unlink(missing_ok=True)
    for cmd in (configure, ["ninja", str(report)]):
        r = subprocess.run(cmd, capture_output=True, text=True)
        if r.returncode:
            print(f"::warning::{sha[:10]} failed: {' '.join(cmd[:2])}\n"
                  f"{(r.stdout + r.stderr)[-1500:]}", file=sys.stderr)
            return None
    m = json.loads(report.read_text()).get("measures") or {}
    return (
        float(m.get("fuzzy_match_percent", 0)),
        (int(m["total_code"]) - int(m.get("matched_code", 0))) / 1000,
        int(m["total_functions"]) - int(m.get("matched_functions", 0)),
    )


def collect(since, args):
    head = git("rev-parse", "HEAD")
    baseline, commits = commits_in_window(since)
    pts = []
    try:
        if baseline:
            m = build_measures(baseline, args)
            if m:
                pts.append((since, *m))
        for sha, ts in commits:
            m = build_measures(sha, args)
            print(f"{sha[:10]} {ts} {m}", file=sys.stderr)
            if m:
                pts.append((datetime.fromisoformat(ts), *m))
    finally:
        subprocess.run(["git", "checkout", "-q", "-f", head], check=True)
    return pts, len(commits)


def window_label(hours):
    return f"{hours // 24}d" if hours >= 48 and hours % 24 == 0 else f"{hours}h"


def plot(pts, since, now, args):
    label = window_label(args.hours)
    times = [p[0] for p in pts] + [now]
    panels = [
        ([p[1] for p in pts], "Fuzzy match %", "#4cc9f0", "{:.2f}%"),
        ([p[2] for p in pts], "Code remaining (KB)", "#f72585", "{:,.1f} KB"),
        ([p[3] for p in pts], "Functions remaining", "#ffd166", "{:,.0f}"),
    ]
    plt.style.use("dark_background")
    fig, axes = plt.subplots(3, 1, figsize=(12, 9.6), sharex=True)
    for ax, (values, title, color, fmt) in zip(axes, panels):
        values = values + [values[-1]]
        ax.step(times, values, where="post", color=color, linewidth=1.5)
        lo, hi = min(values), max(values)
        pad = max((hi - lo) * 0.12, abs(hi) * 0.001, 1e-3)
        if all(float(v).is_integer() for v in values):
            pad = max(pad, 1)
            ax.yaxis.set_major_locator(MaxNLocator(integer=True))
        ax.set_ylim(lo - pad, hi + pad)
        ax.set_title(title, color=color, fontsize=10, loc="left")
        ax.grid(True, alpha=0.2)
        ax.annotate(fmt.format(values[-1]), xy=(times[-1], values[-1]),
                    xytext=(-8, 5), textcoords="offset points", ha="right",
                    color=color, fontsize=10, fontweight="bold")
        ax.set_xlim(since, now)
        if args.hours <= 48:
            ax.xaxis.set_major_locator(mdates.HourLocator(interval=2))
            ax.xaxis.set_major_formatter(mdates.DateFormatter("%H:%M"))
        else:
            ax.xaxis.set_major_locator(mdates.DayLocator(interval=1))
            ax.xaxis.set_major_formatter(mdates.DateFormatter("%a %d %b"))
    fig.suptitle(f"SMS decomp progress, last {label} ({args.version}) — "
                 f"{now:%Y-%m-%d %H:%M} UTC")
    fig.autofmt_xdate()
    fig.tight_layout()
    out = Path(args.output)
    out.parent.mkdir(parents=True, exist_ok=True)
    fig.savefig(out, dpi=150)
    plt.close(fig)
    return out


def summary(pts, n_commits, hours):
    f0, f1 = pts[0], pts[-1]
    return "\n".join([
        f"**Super Mario Sunshine decomp — last {window_label(hours)}** "
        f"({n_commits} commit{'s' if n_commits != 1 else ''})",
        f"Fuzzy: {f1[1]:.2f}% ({f1[1] - f0[1]:+.2f}%)  •  "
        f"Code remaining: {f1[2]:,.1f} KB ({f1[2] - f0[2]:+.1f} KB)  •  "
        f"Fns remaining: {f1[3]:,} ({f1[3] - f0[3]:+d})",
    ])


def post(webhook, content, image):
    r = requests.post(
        webhook,
        data={"payload_json": json.dumps({"content": content})},
        files={"files[0]": (image.name, image.read_bytes(), "image/png")},
        timeout=60,
    )
    r.raise_for_status()
    print(f"posted to Discord ({r.status_code})")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--version", default="GMSJ01")
    parser.add_argument("--hours", type=int, default=24)
    parser.add_argument("--binutils")
    parser.add_argument("--compilers")
    parser.add_argument("--output", default="build/discord_progress.png")
    parser.add_argument("--dry-run", action="store_true")
    args = parser.parse_args()

    now = datetime.now(timezone.utc)
    since = now - timedelta(hours=args.hours)
    pts, n_commits = collect(since, args)
    if not pts:
        sys.exit("no commits could be measured")

    image = plot(pts, since, now, args)
    content = summary(pts, n_commits, args.hours)
    print(content)

    webhook = os.environ.get("DISCORD_WEBHOOK_URL")
    if args.dry_run or not webhook:
        print("not posting (dry run or DISCORD_WEBHOOK_URL unset)")
        return
    post(webhook, content, image)


if __name__ == "__main__":
    main()
