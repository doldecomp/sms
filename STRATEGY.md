# GMSE01 work order

Rewritten 2026-09-16. The earlier near-match Top 10 (batches 38-51) is finished and archived in `docs/progress/strategy-batches-38-51.md`.
Project rules in `CLAUDE.md` still apply in full.

## Goal and measures

The end goal is a game built entirely from source into a byte-identical DOL.
**Source-linked code** is the measure that tracks that goal; matched bytes guide the intermediate work.
A 99.9% function, or an exact function in an object with other differences, adds nothing to source-linked code.

## Order (revised 2026-09-17, after the whole-TU phase)

The from-scratch phase is over: every game unit has a full reconstruction, 259 of the 327 unlinked game units are at 95%+ and 134 are within three functions of matching. What moves the goal now is **closing** units, not lifting them. Batches are one of:

1. **Closure batches.** 3-5 units per agent, each with 1-3 non-exact functions (regenerate the list with the script below, sorted by non-exact count then bytes left). The whole toolkit goes on those last functions; a unit whose code *and* data reach 100% is source-linked at landing (`config/GMSE01/objects.json`, DOL SHA-1 check).
2. **Research batches.** One agent, a scratch TU compiled with the game flags, one residue class that blocks many closures (the "pushed inlined accessor" 4-8 byte family, the 16 bytes per extra `theNerve()`, float-register permutations inside `cross()`, the plain unexplained 8-byte gap). The statement-count table, the two-`return` refusal and the jump-table rule all came from such probes; each rule unlocks dozens of closures.
3. **Header batches.** One agent at a time owns shared headers; `changes_all` after every change; the queue is kept by the orchestrator.
4. **Data-only blockers.** Units with every function exact whose data or link still fails (`JUTDirectPrint`, `JASPlayer_impl`, `dsptask`, `CameraInbetween`): cheap links.

Not worth a batch any more: sweeping a 95%+ unit for its own sake. The last dozen such sweeps lifted bytes but rarely closed anything; the exact-function count barely moves on 2-5 KB functions and the token cost per byte is 2-3x that of the structural work. Report bytes left and links, not function counts.

## Rules of thumb

- **Time-box a function to about 20-30 minutes.** If it is still stuck, leave a `// TODO:` naming the remaining difference and move on.
- **Group by shared cause.** When a fix works, check the same pattern in neighbouring functions and headers (batch 18's sound-layout fix made 15 functions exact at once).
- **Verify cheaply but always.** A batch touching one `.cpp` can reuse the last baseline; take a fresh `ninja baseline` before any header change. Run `ninja changes_all`, `validate-symbol-order.py` on changed units, and the DOL SHA-1 check every batch.
- **Parallel agents work in worktrees.** `tools/worktree.sh add <name>` gives each agent a buildable checkout; agents own disjoint units and never edit shared headers. See `docs/ORCHESTRATION.md`.
- **Keep docs small.** Commit messages carry the batch detail. Update `PROGRESS.md` numbers in place. Edit the matching `docs/catalog/` entry only when a finding is reusable; do not write per-batch audit files.

## Refreshing the closure list

Run after each landing to rank unlinked game units by distance to linking:

```
python3 - <<'EOF'
import json
r = json.load(open("build/GMSE01/report.json"))
rows = []
for u in r["units"]:
    m = u["measures"]
    if "game" not in u.get("metadata", {}).get("progress_categories", []):
        continue
    if int(m.get("complete_code", 0) or 0) > 0:
        continue  # already source-linked
    fs = u.get("functions") or []
    if not fs:
        continue
    ne = [f for f in fs if (f.get("fuzzy_match_percent") or 0) < 100]
    left = int(m.get("total_code", 0)) - int(m.get("matched_code", 0) or 0)
    data = float(m.get("matched_data_percent", 0) or 0)
    names = [f.get("metadata", {}).get("demangled_name", f["name"]) for f in ne]
    rows.append((len(ne), left, u["name"], data, names))
for ne, left, name, data, names in sorted(rows)[:40]:
    print(f"{name:40} nonexact={ne} bytes_left={left:5d} data={data:5.1f}%  {', '.join(names)[:80]}")
EOF
```

## Human decisions outstanding

- **Back up the branch.** All commits are local only; push to a fork.
- **Review shared-code judgment calls** before more work builds on them: the water-hit field union, archive link order for duplicate globals, US-only paths in shared code, `TFootHitActor` in the shared boss header. Spot-check for fakematches.
- **Upstream intent.** Upstream `doldecomp/sms` builds only `GMSJ01`/`GMSP01`. Upstreaming needs a Japanese image so header changes can be checked against it.

## Timeline

`docs/TIMELINE.md` is the run's progress history (per-commit snapshots, per-day rates, linear projections to full linking), rebuilt from git by `python3 tools/progress-timeline.py`.
Refresh it together with `PROGRESS.md` after every landing.
