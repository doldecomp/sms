# Codex sub-agents under a Claude orchestrator

Plan written 2026-09-23 (session 5eae6404); not yet built or trialled.
Goal: run Codex CLI agents (`codex exec`, model from `~/.codex/config.toml`, currently `gpt-6-sol`) as extra unit workers beside Claude sub-agents, with one orchestrator owning assignment, landing and bookkeeping.

## Why

- Adds agent capacity from a second budget, so more units are in flight at once.
- A Codex orchestrator already worked this tree on 2026-09-22 (HANDOFF "Session codex-orchestrator"): medium-effort whole-file passes landed several improvements and gave clean blocker audits, so Codex handles the brief.
- Different models find different spellings; a unit one model gave up on is a fresh attempt for the other.

## Shape

1. **One orchestrator, one lander.**
   The Claude session assigns every unit, lands every branch with `tools/worktree.sh land`, refreshes `PROGRESS.md`/`TIMELINE.md` and writes `HANDOFF.md`.
   Codex agents never merge, never touch `local/decomp-progress`, never edit docs outside their unit's TODO comments.
2. **One worktree per agent, as now.**
   `tools/worktree.sh add <name>` builds `../sms-wt/<name>` on `wt/<name>`; the Codex agent runs with `-C ../sms-wt/<name> -s workspace-write`, so its sandbox can only write inside its own worktree (its `build/` is inside it; the shared toolchain symlinks are read-only).
3. **A claim ledger so the two agent pools never collide.**
   `tools/claim.py` (to build): a `flock`-guarded JSON file under `.git/` (shared by all worktrees, untracked) mapping unit -> {agent, model, worktree, started}.
   `claim <unit> <agent>` fails if the unit is taken; `release <unit>`; `list`.
   The orchestrator claims before spawning either kind of agent and releases after landing or discarding, so a unit is never worked by Claude and Codex at once.
   Headers stay orchestrator-only (the existing rule: worktree agents park shared-header needs as TU-local `static inline` and report them).
4. **Launch.** From the orchestrator, in the background:
   `codex exec -C ../sms-wt/<name> -s workspace-write --json -o ../sms-wt/<name>-scratch/report.md "<spawn message>" > ../sms-wt/<name>-scratch/codex.jsonl`
   The spawn message is the same short one Claude agents get (worktree, unit, scratch dir, methods, refusals, commit trailer, "final report short per brief").
   `AGENTS.md` in the repo root points Codex at `docs/agent-brief.md`, `docs/catalog/LEVERS.md` and the non-negotiable rules in `CLAUDE.md` (no fakematches, no leaked sources, verification before commit, never push).
5. **Completion.** The background command's exit re-invokes the orchestrator; it reads `report.md` (never the JSONL), lands with the usual checks (`changes_all` zero regressions, DOL SHA-1, symbol order), releases the claim and refills the slot.
6. **Review.** Codex commits get the same review as Claude ones before landing: the orchestrator reads the diff for refused levers (identity binders, dead padding, pragmas, invented helpers) and strips them, as it did for the Application binder on 2026-09-23.

## Trial before scaling

- Phase 1: two Codex agents beside the four Claude agents for ~3 hours, on units neither pool has touched today, each with the same method brief.
- Measure per agent: exact functions closed, fuzzy bytes gained, refused-lever rate, wall time, and cost (`tools/agent-timing.py` for Claude; token counts from the Codex JSONL).
- Scale Codex up only if its exact-functions-per-hour is at least half of Claude's with a refused-lever rate no worse; otherwise keep it for audits and bug hunts (the 85-98% band, where it found real bugs on 2026-09-22).

## Risks

- Commit trailer: Codex commits need the orchestrator session's `Claude-Session:` trailer (or an agreed Codex trailer); put it in every spawn message.
- `git stash` is shared across worktrees; `AGENTS.md` must repeat the never-stash rule.
- Two orchestrators at once (a Codex orchestrator session and this one) would double-land; only one orchestrator may run at a time, and the claim ledger records which.
- Machine load: each agent builds in parallel; keep the total at six or fewer agents until CPU load is measured.

## To build

1. `tools/claim.py` (~60 lines) and a `tools/worktree.sh` hook that refuses `add` for a claimed unit's name.
2. `AGENTS.md` at the repo root (short, points at the briefs).
3. A `tools/codex-agent.sh <name> <unit> <prompt-file>` wrapper: claims, adds the worktree, launches `codex exec` in the background with the flags above.
