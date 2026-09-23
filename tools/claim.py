#!/usr/bin/env python3
"""Coordinate unit, header, and research work across worktrees."""

import argparse
import fcntl
import json
import os
import subprocess
import sys
import tempfile
from datetime import datetime, timezone
from pathlib import Path


def common_dir():
    raw = subprocess.check_output(["git", "rev-parse", "--git-common-dir"], text=True).strip()
    return Path(raw).resolve()


def valid_key(key):
    return any(key.startswith(prefix) and len(key) > len(prefix)
               for prefix in ("unit:", "header:", "research:"))


def save(path, entries):
    fd, name = tempfile.mkstemp(prefix="sms-claims-", dir=path.parent)
    try:
        with os.fdopen(fd, "w") as output:
            json.dump(entries, output, indent=2, sort_keys=True)
            output.write("\n")
        os.replace(name, path)
    finally:
        if os.path.exists(name):
            os.unlink(name)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    sub = parser.add_subparsers(dest="command", required=True)
    claim = sub.add_parser("claim")
    claim.add_argument("key")
    claim.add_argument("--owner", required=True, choices=("codex", "claude"))
    claim.add_argument("--agent", required=True)
    claim.add_argument("--worktree", required=True)
    release = sub.add_parser("release")
    release.add_argument("key")
    release.add_argument("--owner", required=True, choices=("codex", "claude"))
    release.add_argument("--agent")
    sub.add_parser("list")
    who = sub.add_parser("who")
    who.add_argument("key")
    who.add_argument("--worktree")
    who.add_argument("--owner", choices=("codex", "claude"))
    args = parser.parse_args()
    if getattr(args, "key", None) and not valid_key(args.key):
        parser.error("key must begin unit:, header:, or research:")

    root = common_dir()
    path = root / "sms-claims.json"
    with open(root / "sms-claims.lock", "a+") as lock:
        fcntl.flock(lock, fcntl.LOCK_EX)
        entries = json.loads(path.read_text()) if path.exists() else {}
        if args.command == "claim":
            existing = entries.get(args.key)
            if existing and (existing["owner"], existing["agent"], existing["worktree"]) != (args.owner, args.agent, args.worktree):
                print(f"refused: {args.key} held by {existing['owner']}/{existing['agent']} in {existing['worktree']}", file=sys.stderr)
                return 1
            if not existing:
                entries[args.key] = {"owner": args.owner, "agent": args.agent,
                                     "worktree": args.worktree,
                                     "start_time": datetime.now(timezone.utc).isoformat()}
                save(path, entries)
            print(f"claimed {args.key} by {args.owner}/{args.agent} in {args.worktree}")
        elif args.command == "release":
            existing = entries.get(args.key)
            if not existing or existing["owner"] != args.owner or (args.agent and existing["agent"] != args.agent):
                print(f"refused: no matching claim for {args.key}", file=sys.stderr)
                return 1
            del entries[args.key]
            save(path, entries)
            print(f"released {args.key}")
        elif args.command == "who":
            existing = entries.get(args.key)
            if not existing or (args.worktree and existing["worktree"] != args.worktree) or (args.owner and existing["owner"] != args.owner):
                return 1
            print(json.dumps(existing, sort_keys=True))
        else:
            for key, value in sorted(entries.items()):
                print(f"{key}\t{value['owner']}\t{value['agent']}\t{value['worktree']}\t{value['start_time']}")
    return 0


if __name__ == "__main__":
    sys.exit(main())
