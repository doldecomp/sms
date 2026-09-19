# Orchestrator handoff

Written 2026-09-18 by the session that ran batches 92 to 181.
Read this before `docs/ORCHESTRATION.md`; it is the judgement that document does not carry.
Keep it current: every orchestrator appends a dated "State" entry below and rewrites the advice when it stops being true.

## Where things stand (2026-09-18, late night, session 110dc638)

| | Matched code | Linked units |
| --- | ---: | ---: |
| Game | 45.03% | 162 / 385 |
| JSystem | 89.93% | 179 / 198 |
| SDK | 95.71% | 146 / 149 |
| All | 54.86% | 489 / 732 |

Matched data is 95.2% (from 92.8%); eleven more units have 100% data than this morning.

`docs/TIMELINE.md` has the per-commit history and per-day rates; regenerate it with `python3 tools/progress-timeline.py`.
Per-batch token costs are in `docs/progress/agent-costs-2026-09-18.md`; the batch-by-batch outcome log is `docs/progress/queue-2026-09-18.md`; the census scripts are under `tools/census/`.

## What the numbers say about yield

Measured over 52 batches on 2026-09-18 (about 14.5M agent tokens, 57 links, roughly 75 functions to exact); the second session's 27 batches (5.6M tokens, 0 links, ~60 exact, eleven units to 100% data) confirmed the verdicts and added the data-pass row:

| Batch type | Typical cost | Typical result | Verdict |
| --- | ---: | --- | --- |
| Research (one residue class, scratch TU, driver script) | 190k-240k | one reusable rule, then 2-10 closures in the follow-up passes | **Best value.** Every rule found today unblocked units within hours. |
| Header round (one agent owns shared headers, `changes_all` per change) | 110k-260k | 1-7 exact, often a link | High value when items are verified and the lever-removal rule is applied (see below). |
| Relocation-target pass (`tools/check-relocs.py`) | 350k-390k | 8-11 exact, real gameplay bugs, data gains | High value; ~300 flagged sites remain, mostly in big units. |
| Structural-residue pass (opcode-marker census) | 270k-350k | 4 exact, a third of them real bugs | Good; census entries above 170 unworked. |
| Re-pass with new rules over one/two-function units | 220k-340k | 1-3 links | Good the first time after a new rule; zero the third time. |
| Library closure / re-pass | 220k-480k | 0-5 links | Good once (region constants, one-arg levers); the remaining 17 library units are research residues. |
| Closure batch on units worked twice before | 300k-570k | 0-2 links | **Stop.** Every remaining one/two-function game unit is diagnosed as a known-open class. |
| Sonnet on script-driven sweeps | 240k | 0 exact | Same tokens per call as Opus, less result. Opus only. |
| Data pass (sub-100 sections, `objdump -r`, `$NNNN` static sections, rogue-include position) | 170k-250k | 3-5 units to 100% data, 1-4 exact as a side effect | Best value per token while sub-100 units with non-jump-table defects remain (none left on 2026-09-18). |

Tokens per tool call are ~2,300 for every agent regardless of policy; savings come from agents reading and rereading less (rules card, `--clusters`), not from the model thinking less.

## How to run for the fastest results under a usage cap

1. **Spend on research first when a class blocks several units.** A research batch that finds a rule is the only thing that has ever moved the wall. Give it a scratch TU with the game flags, a driver script, and the exact list of blocked sites to test the rule on.
2. **Keep the detectors running.** `tools/check-relocs.py --mode all` and the opcode-marker census (`tools/census/census_struct.py`; `census_depth.py` beside it finds sqrtf/TVec3 depth sites) find bugs objdiff cannot score. Work them in slices ranked by unit bytes-left.
3. **Header rounds only with verified items and the lever-removal rule:** a function that drops from 100% after a header binding usually carries a removable TU-local lever of the same size; remove it in the same commit instead of rejecting. Receiver bindings are destructive; argument bindings pay.
4. **Verify a queue item against the tree before spawning it** (two stale items cost a batch on 2026-09-18). One `--clusters` call per item is enough.
5. **Never a third closure pass on a unit whose TODO names a known-open class.** The classes with no legitimate spelling: the `a = b - c` allocation order (retail's 4-below/12-above geometry needs two dead bindings), the JGadget iterator block stride (8 retail vs 12 ours), volatile-register block trades, dead low regions with no legal carrier (no UNUSED callee, no empty-ctor class local), the three factory constructors retail calls (`TTelesaSlot`, `TFence`, `TSunGlass`), the `JUTColor` temp stride, `this`-vs-pool-base swaps whose named-scalar knob is exhausted. They wait for research, not for another agent.
6. **Four Opus agents was the user's choice; the budget cannot sustain it for a week.** If the user asks to cut cost, drop closure and re-pass batches first, keep research and detector passes, and run two agents.
7. **Landing is one bash call per step**; do not narrate between them. Refresh `PROGRESS.md` with `tools/refresh-progress.py` and the timeline after every landing, and report the percentage table to the user after every completion (they asked for it).
8. **Spawn messages are short**: worktree, units, scratch dir, the two or three rules that apply, the link-test procedure with the `.cpp` spelling and the ninja exit-code check. The briefs carry the rest.

## Open leads worth a batch (verified 2026-09-18, late night)

- **Links one function away**: JALModSe `JALSystem::append` (instruction-exact; the 0x68 hole is 3 x 16 + 7 x 8 in the `JALPrmSet`/`JADPrmS<f32>` prologue constructions, unnamed: a research batch on what real C++ in those two constructors reserves it); J3DModel `entryModelData` 99.92 (zero-frame r25/r24 vs r20/r21 rotation).
- **Research seeds with a measured payoff**: the `a = b - c` allocation order (+9 functions, needs a legitimate spelling for retail's (4, 12) geometry); the JGadget stride (eight functions; the lever is in the callers' enclosing levels, not `std-list.hpp`); `getMaxHitPoints()` as a statement-free address binding of `getSaveParam()->mSLHitPointMax` (bosstelesa +3, fireWanwan must stay exact); the constant 4-byte pad at the top of `TSunModel::calcAnim_`'s block; the emergent per-expansion inlining class (`TSelectMenu::perform` vs `initData`, the sharpest same-TU datum); `JPAGetRMtxTVecElement`'s FPR ranking (research 174); the frame-only census (functions with zero opcode markers and stack-offset residues) as a new detector for the research-208 block rule.
- **Data**: the remaining sub-100 units are jump-table owners needing code (GCConsole2 `perform` 14.7 KB rebuild, CardSave `execMovement_`, bossgesso `changeAttackMode`, CardLoad's two `perform` tables, CardManager `setCheckSum` via a sub-9-statement byte-exact `CalcCheckSum`); Koopa's `.sdata2` needs the `KoopaNeckCallBack` tail; LightUtil wants `System/DummyMactorString.hpp` at a bisected include position.
- **Exhausted, do not re-run without a new rule**: closure passes over the one- and two-function game tiers (54 units, batches 206/207) and the eight near-link library units (batch 202); header items on return types measured today (`isHolding`, `getColor`, `getMaxHitPoints`, `operator*` V2/V3, `SMatrix34C` ctor, `TNerveBase` ctor, `TVec2` copy ctor); reloc sites (264 left, nearly all compensated pairs or inlining artifacts); structural census rows (all 594 worked once).
- Symbol-order FAILs on linked Runtime units (ExceptionPPC 26, NMWException 8 MISSING UNUSED bodies): correctness, not links.

## State log

- **2026-09-18 late night** (session `110dc638`, batches 182-208, ~5.6M agent tokens): 489/732 linked (no new links), 54.43 -> 54.86% matched, ~60 functions to exact, ~25 real gameplay bugs fixed, matched data 92.8 -> 95.2% (eleven units to 100% data). Yield per type this session: data passes 240k for 3-5 units to 100% data (best value while units remained); header rounds 110-190k for 1-4 exact plus data; structural passes 320-410k for 5-9 exact and 3-5 bugs, no links; closure re-passes over the one- and two-function game tiers (54 units) and the eight near-link library units: **zero closures**, every residue a recorded known-open class; research 184/186/193/208 each produced a rule (JGadget stride not in `std-list.hpp`; the statement-count table with the named-local +1; MSound's fabricated gate; an inlined callee's class-object locals form their own block above the caller's). The user is at 40% of the weekly budget 12 hours into the week and chose two agents, yield-first; with the cheap tiers exhausted the remaining yield is research only. Open leads: JALModSe `append` is instruction-exact and its 0x68 hole is positioned in the `JALPrmSet`/`JADPrmS<f32>` prologue constructions (3 x 16 + 7 x 8) but unnamed: naming it links the unit; J3DModel `entryModelData` 99.92 (zero-frame r25/r24 vs r20/r21 rotation) links the unit; `TSunModel::perform` has a constant 4-byte pad at the top of `calcAnim_`'s block; `getMaxHitPoints()` wants a statement-free address binding of `getSaveParam()->mSLHitPointMax`; CardManager `setCheckSum` needs a `CalcCheckSum` body under 9 statements that stays byte-exact; the remaining sub-100 data units are jump-table owners (GCConsole2 `perform` is a 14.7 KB rebuild). Worktrees grpfg/grpfh/grpfi/grpfj/grpfk exist, all landed, some behind HEAD (rebase + `ninja` + `ninja baseline` before use). Census scripts take `SMS_ROOT` or the cwd.
- **2026-09-18 night, final** (session `7a7a015e`): 489/732 linked, 54.43% matched, tree quiet. Every batch through 181 is landed; no agent is running. One spare worktree `grpff` exists (branch `wt/grpff`, no commits, built a few commits behind HEAD: `git rebase local/decomp-progress`, `build/venv/bin/ninja`, `build/venv/bin/ninja baseline` before using it). The last four batches (reloc III, structural III/IV, 4-byte re-pass II) lifted ~25 functions and closed 8 but linked nothing: the structural and reloc passes are now the main vein for exact functions, and links come from research results. Land routine for the next session: `tools/worktree.sh land <name> <units>`; `objects.json` conflicts via `python3 tools/resolve-objects-conflict.py config/GMSE01/objects.json`; catalog conflicts keep both hunks.
