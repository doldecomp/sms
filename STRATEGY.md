# GMSE01 work order

Rewritten 2026-09-16. The earlier near-match Top 10 (batches 38-51) is finished and archived in `docs/progress/strategy-batches-38-51.md`.
Project rules in `CLAUDE.md` still apply in full.

## Goal and measures

The end goal is a game built entirely from source into a byte-identical DOL.
**Source-linked code** is the measure that tracks that goal; matched bytes guide the intermediate work.
A 99.9% function, or an exact function in an object with other differences, adds nothing to source-linked code.

## Order

This reflects what recent batches have actually done (see `git log`); the user has not revised it since 2026-09-15.

1. **Cheap links first.** A unit one function or one frame gap from linking is worth more than the same effort elsewhere. Check `docs/catalog/frame-gaps.md` for the list and what has already been tried.
2. **Whole-TU reconstruction from the map.** The current mainline. Pick an unstarted or empty unit (enemies so far: `tobiPuku`, `seal`, `chuuhana`, `igaiga`, `cannon`, `popo`, `koopajr`, `hanasambo`), then:
   - recover the full symbol inventory from `orig/GMSE01/files/marioUS.MAP` and scaffold per `docs/PROGRAM_STRUCTURE_REVVING.md`;
   - declare virtuals in vtable order and params from `.rodata` first (`docs/catalog/tu-reconstruction.md`);
   - draft bodies with m2c, write every function including nerves and UNUSED helpers, and put the file in map order.
3. **Near-match queue.** Nonmatching game functions at 98%+ similarity, largest first, grouped by shared cause. Regenerate with the script below.

## Rules of thumb

- **Time-box a function to about 20-30 minutes.** If it is still stuck, leave a `// TODO:` naming the remaining difference and move on.
- **Group by shared cause.** When a fix works, check the same pattern in neighbouring functions and headers (batch 18's sound-layout fix made 15 functions exact at once).
- **Verify cheaply but always.** A batch touching one `.cpp` can reuse the last baseline; take a fresh `ninja baseline` before any header change. Run `ninja changes_all`, `validate-symbol-order.py` on changed units, and the DOL SHA-1 check every batch.
- **Parallel agents work in worktrees.** `tools/worktree.sh add <name>` gives each agent a buildable checkout; agents own disjoint units and never edit shared headers. See "Parallel agents and worktrees" in `CLAUDE.md`.
- **Keep docs small.** Commit messages carry the batch detail. Update `PROGRESS.md` numbers in place. Edit the matching `docs/catalog/` entry only when a finding is reusable; do not write per-batch audit files.

## Refreshing the near-match list

```
python3 - <<'EOF'
import json
r = json.load(open("build/GMSE01/report.json"))
fns = []
for u in r["units"]:
    # Drop this filter to include JSystem and SDK units.
    if "game" not in u.get("metadata", {}).get("progress_categories", []):
        continue
    for f in u.get("functions", []) or []:
        fz = f.get("fuzzy_match_percent", 0) or 0
        if 98 <= fz < 100:
            name = f.get("metadata", {}).get("demangled_name", f["name"])
            fns.append((int(f["size"]), fz, u["name"], name))
for size, fz, unit, name in sorted(fns, key=lambda x: -x[0])[:40]:
    print(f"{size:6d} {fz:7.3f} {unit} :: {name}")
EOF
```

## Human decisions outstanding

- **Back up the branch.** All commits are local only; push to a fork.
- **Review shared-code judgment calls** before more work builds on them: the water-hit field union, archive link order for duplicate globals, US-only paths in shared code, `TFootHitActor` in the shared boss header. Spot-check for fakematches.
- **Upstream intent.** Upstream `doldecomp/sms` builds only `GMSJ01`/`GMSP01`. Upstreaming needs a Japanese image so header changes can be checked against it.
