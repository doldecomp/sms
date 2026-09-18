# Orchestration guide

Read by the orchestrating session only; agents do not need it.
`STRATEGY.md` is the work order, `PROGRESS.md` the numbers, `docs/TIMELINE.md` the history.

### Parallel agents and worktrees

Several agents can work at once, each in its own git worktree, as long as they edit disjoint units.
`build/` and `orig/` are untracked, so a bare `git worktree add` cannot build; use the helper instead:

```bash
tools/worktree.sh add <name>              # ../sms-wt/<name> on branch wt/<name>, built, with a baseline (~1 min)
tools/worktree.sh land <name> <unit>...   # merge, then changes_all, DOL SHA-1 and symbol order in one report
tools/worktree.sh remove <name>
```

Unit agents are briefed by `docs/BATCH_BRIEF.md`; a spawn message only names the worktree, the units and the scratch directory.

Inside a worktree every command in this guide works unchanged: `build/venv/bin/ninja`, `decomp-diff.py`, `validate-symbol-order.py` and `ninja baseline`/`changes_all` all run against that worktree's own `build/GMSE01`.
The toolchain and disc files are shared read-only symlinks.

Rules for a worktree agent:
- Edit only the unit you were given (its `.cpp` and header). A change that needs a shared header is reported back, not made, because it perturbs every other agent's units.
- Verify inside the worktree (`changes_all`, symbol order), commit on `wt/<name>`, and do not touch the main checkout.
- The orchestrator merges with `tools/worktree.sh merge`, then reruns `ninja changes_all` and the DOL SHA-1 check in the main checkout before anything counts as committed. A merge conflict or a red check stops the batch.


## Landing routine

1. `tools/worktree.sh land <wt> <unit>...` — merge (rebase + ff), build, `changes_all`, DOL SHA-1, symbol order. A red build or DOL mismatch reverts the merge automatically; a rebase conflict stops before the merge: resolve it in the worktree (keep the already-landed spelling of any linked unit), commit, and land again.
2. Link test every unit that reports 100% code and 100% data: add its **`.cpp` spelling** to `config/GMSE01/objects.json`, run a full `build/venv/bin/ninja` and **check the exit code**, then `sha1sum build/GMSE01/mario.dol` must start `a6782903ef79`. A misspelled entry makes configure fail and leaves the old DOL in place, which reads as a false pass. A 100/100 unit that changes the DOL is a link blocker: record it in the queue for a structural batch, never leave it linked.
3. Record reusable rules in the matching `docs/catalog/` topic file and one line each in `docs/catalog/RULES.md`; update the scratchpad `queue.md`.
4. `build/venv/bin/ninja baseline`, refresh `PROGRESS.md` (rows 14 and 17 from `build/GMSE01/report.json`) and `python3 tools/progress-timeline.py`, commit with the session trailer, `tools/worktree.sh remove <wt>`.
5. Refill the slot from a pre-built spare worktree (`tools/worktree.sh add <name>` in the background while a batch runs), fast-forwarded to HEAD with a fresh baseline.

## Batch types and model policy

- Closure, research and header batches run on Opus; the Sonnet calibration (batch 123) closed nothing on units Opus later closed.
- Script-driven sweeps, data sweeps, link tests and bookkeeping run on Sonnet: the trial loop is in bash and the model only writes the driver and reads the tallies.
- Concurrency: two agents by default (usage budget), never more than four.
- Do not send a unit to a third closure pass once its residue is recorded as a known-open class (by-value return geometry, JGadget pool word, FPR permutation); those wait for a research result.

## Token discipline

- Agents read `docs/AGENT_GUIDE.md`, the brief and `docs/catalog/RULES.md`; never the whole catalog.
- Diffs: `tools/decomp-diff.py --clusters` first; a full `--no-collapse` read of a function over 2 KB at most once per function, never after every trial.
- Spawn messages name the worktree, units, scratch directory and the one or two catalog sections that apply; the brief carries the rest.
