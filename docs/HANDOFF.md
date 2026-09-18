# Orchestrator handoff

Written 2026-09-18 by the session that ran batches 92 to 181.
Read this before `docs/ORCHESTRATION.md`; it is the judgement that document does not carry.
Keep it current: every orchestrator appends a dated "State" entry below and rewrites the advice when it stops being true.

## Where things stand (2026-09-18, evening)

| | Matched code | Linked units |
| --- | ---: | ---: |
| Game | 44.49% | 162 / 385 |
| JSystem | 89.93% | 179 / 198 |
| SDK | 95.71% | 146 / 149 |
| All | 54.43% | 489 / 732 |

`docs/TIMELINE.md` has the per-commit history and per-day rates; regenerate it with `python3 tools/progress-timeline.py`.
Per-batch token costs are in `docs/progress/agent-costs-2026-09-18.md`; the batch-by-batch outcome log is `docs/progress/queue-2026-09-18.md`; the census scripts are under `tools/census/`.

## What the numbers say about yield

Measured over 52 batches on 2026-09-18 (about 14.5M agent tokens, 57 links, roughly 75 functions to exact):

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

## Open leads worth a batch (verified 2026-09-18)

- Relocation-target sites remaining after pass III (GCConsole2, bossgesso, SelectMenu, MapObjMamma, MarioSpecial, ModelGate): run `tools/check-relocs.py` and work the real ones.
- Structural census entries 171+ (about 470 functions).
- `operator*` consumption split: V2 (`const TVec3&` parameter, by-value return) is +12 improvements, -1 function only because Tongue loses `__ami__`; if Tongue can keep `__ami__` another way, apply it (research 159).
- Research seeds: `JPAGetRMtxTVecElement`'s FPR ranking (x f31, z f30, y f29) fits neither order while its byte-exact sibling pins the callee (research 174); `JPAVecToRotaMtx`'s memory-resident `axis`; the JGadget stride mechanism (research 161's variant table); `TMBindShadowManager`-style `Vec`-vs-`TVec3` accessor splits.
- Library: `TPolarCamera::perform` (104 dead bytes = 96 + 8), `JAIBasic::initDriver` (8 bytes, a two-word class local in UNUSED `initAudioThread`), J3DModel (22 MISSING map symbols block its link), `bootDSP` UNUSED 0x20 with an empty body.
- Symbol-order FAILs on linked Runtime units (ExceptionPPC 26, NMWException 8 MISSING UNUSED bodies): correctness, not links.
- Catalog contradictions listed by the rules-card agent (queue.md, 2026-09-18): a docs pass when a slot is idle.

## State log

- **2026-09-18 night, final** (session `7a7a015e`): 489/732 linked, 54.43% matched, tree quiet. Every batch through 181 is landed; no agent is running. One spare worktree `grpff` exists (branch `wt/grpff`, no commits, built a few commits behind HEAD: `git rebase local/decomp-progress`, `build/venv/bin/ninja`, `build/venv/bin/ninja baseline` before using it). The last four batches (reloc III, structural III/IV, 4-byte re-pass II) lifted ~25 functions and closed 8 but linked nothing: the structural and reloc passes are now the main vein for exact functions, and links come from research results. Land routine for the next session: `tools/worktree.sh land <name> <units>`; `objects.json` conflicts via `python3 tools/resolve-objects-conflict.py config/GMSE01/objects.json`; catalog conflicts keep both hunks.
