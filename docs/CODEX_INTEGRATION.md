# Parallel orchestrators: Claude and Codex

Concept written 2026-09-23 (session 5eae6404); nothing built yet.
Goal: a Codex orchestrator runs beside the Claude orchestrator on this tree, each with its own sub-agents, landing into the same branch, with no conflicts.

## What actually conflicts today

Everything below is shared by any two orchestrators on this machine:

| Shared thing | What goes wrong | Fix |
| --- | --- | --- |
| Units | Both pools push the same unit; the second land conflicts or undoes the first | Claim ledger |
| `local/decomp-progress` and the main checkout | Two `worktree.sh land` runs interleave merges, `ninja`, `changes_all` and `ninja baseline` in one `build/` | Land lock |
| Shared headers (`include/`) | A header change lands while the other side has worktrees built on the old header; their `changes_all` baseline is stale and "regressions" appear that are not theirs | Header claims plus rebase-before-land (already in `land`) |
| `PROGRESS.md`, `docs/TIMELINE.md` | Both regenerate and commit, then rebase conflicts | Regenerate only under the land lock |
| `docs/HANDOFF.md`, `docs/catalog/*.md` | Both append at the file end, then textual conflicts (happened with fd1 on 2026-09-23) | Separate files per orchestrator |
| `config/GMSE01/objects.json` | Two link tests edit neighbouring lines | Only under the land lock; `tools/resolve-objects-conflict.py` exists |
| Worktree names under `../sms-wt/` | Name collisions | Name prefixes |
| `git stash` | Shared by all worktrees | Already forbidden |
| CPU | Two pools of builds | Agent cap across both |

## The design

1. **Claim ledger (units and headers).**
   A small JSON file in the git common dir (`$(git rev-parse --git-common-dir)/sms-claims.json`, untracked, visible from every worktree), guarded by `flock`.
   Each entry: key, owner orchestrator, agent name, worktree, start time.
   Keys: `unit:Enemy/Koopa` for unit work, `header:include/JSystem/JGeometry/JGVec3.hpp` for any shared-header edit, `research:<topic>` for research batches so both sides don't run the same study.
   Rule: claim before spawning an agent or starting a header round; release after landing or discarding. A claim held by the other orchestrator is a hard no.
   Stale claims (owner session gone, over 12 hours old) are released only by a human or by their owner.
2. **One land lock.**
   A single `flock` file (`<git common dir>/sms-land.lock`) held for the whole landing sequence: rebase the worktree branch, merge, `ninja`, `changes_all`, DOL SHA-1, symbol order, link test, `ninja baseline`, `PROGRESS.md`/`TIMELINE.md` refresh, commit.
   `tools/worktree.sh land` takes it itself, so neither orchestrator can bypass it.
   Any other main-checkout build or commit (doc commits, header rounds) also goes through it.
   Landing stays serial and short, a few minutes, so contention is small.
3. **Rebase-before-verify is the conflict detector.**
   `land` already rebases the worktree branch onto the current tip, so a unit landed by the other side is picked up automatically.
   A rebase conflict means a claim was broken; the lander stops and reports rather than resolving it.
4. **Separate paper trails.**
   Each orchestrator writes its own handoff (`docs/HANDOFF.md` for Claude, `docs/HANDOFF-codex.md` for Codex) and its own findings files (`docs/catalog/findings-claude.md`, `docs/catalog/findings-codex.md`), merged into the topic files occasionally by whichever side holds the land lock.
   `PROGRESS.md` and `TIMELINE.md` are generated, so whoever lands last regenerates them.
5. **Namespaced worktrees and branches.**
   Claude uses `wt/c-<name>`, Codex `wt/x-<name>`.
   `worktree.sh remove` already refuses unlanded branches.
6. **Split the work so claims rarely collide.**
   A standing split, recorded in `docs/ORCHESTRATION.md`, avoids contention in the first place.
   Example: Codex takes the bug-hunting and semantic band (functions under 95%, real-instruction diffs, the ~110 logically wrong functions), where it found real bugs on 2026-09-22.
   Claude takes the near-exact band (stack, register, pair sweeps, links).
   Either can take the other's units only through a claim.
7. **Agent cap across both.**
   Each orchestrator gets a fixed number of slots (for example 4 Claude and 3 Codex) so the machine does not thrash; both read the total from the ledger before spawning.
8. **Shared rules, one source.**
   Both orchestrators read the same `CLAUDE.md`/`AGENTS.md` rules (no fakematches, no leaked sources, verification before commit, never push) and the same `docs/agent-brief.md`.
   Each lands the other's style of work under the same checks, so neither pool can lower the bar.

## Do we need two orchestrators, or Codex sub-agents under one?

Two orchestrators, as asked, is fine: the ledger and the land lock are what make it safe, and they would be needed anyway.
The one thing a single orchestrator gives for free is judgement across both pools (who takes which unit, spotting refused levers in the other pool's diffs).
The standing split in point 6 recovers most of that.

## Build list (when approved)

1. `tools/claim.py`: `claim`, `release`, `list`, `who`; flock-guarded; about 60 lines.
2. `tools/worktree.sh`: take the land lock in `land` and in a new `locked <cmd>` wrapper; refuse `add` without a matching claim.
3. `docs/HANDOFF-codex.md` and the two findings files; a short "Parallel orchestrators" section in `docs/ORCHESTRATION.md` and `AGENTS.md` pointing here.
4. Trial: Codex orchestrator with 2-3 agents for a few hours on its band, then compare per-hour exact functions and refused-lever rates.
