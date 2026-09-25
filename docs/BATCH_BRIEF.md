# Batch brief for unit agents

You are decompiling one or more translation units of Super Mario Sunshine (GMSE01, MWCC 1.2.5) in a matching decomp.
Your spawn message names your worktree, branch and units; this file is the rest of the brief.
Read it once, then the documents it points to, then start.

## Where you work

Only inside your worktree `sms-wt/<name>` beside the main checkout (branch `wt/<name>`).
Never touch the main checkout or another worktree.
`cd` there first; every path below is relative to it.
Every command in `docs/AGENT_GUIDE.md` works unchanged inside the worktree (`build/venv/bin/ninja`, `python3 tools/decomp-diff.py`, `tools/validate-symbol-order.py` with the `NM=` and `--map` overrides).

## Read first, once

1. `docs/AGENT_GUIDE.md` (the working guide; `CLAUDE.md` is already in your context).
2. `docs/AGENT_MATCHING_TIPS.md` (general MWCC codegen).
3. `docs/PROGRAM_STRUCTURE_REVVING.md` (scaffolding a TU from the map) only if your unit lacks scaffolding.
4. `docs/catalog/LEVERS.md`, the thirty-lever card; grep `docs/catalog/RULES.md` (the full one-line rules list) only for a tell the card lacks. Open a long catalog topic file (`codegen-tells.md`, `frame-gaps.md`, `linking.md`, `tu-reconstruction.md`) only at the section a rule points you to; never read them end to end.
5. Two finished units in the same subsystem as yours, for style: enemies `src/Enemy/bombhei.cpp` + `include/Enemy/BombHei.hpp` and `src/Enemy/Kazekun.cpp`; MoveBG `src/MoveBG/MapObjBall.cpp` and `src/MoveBG/MapObjWave.cpp`; animals `src/Animal/BeeHive.cpp`.

## Method, per unit

1. **Inventory** every symbol the map lists for the TU: `grep '<Lib>.a <file>.cpp' orig/GMSE01/files/marioUS.MAP`, reading the `.text section layout` lines, the UNUSED entries and the closure section (weak symbols, vtables, `.data`/`.rodata`/`.sdata2` objects, `@NNNN` literals).
   Every class, method, nerve, UNUSED helper, vtable, static and data object must be declared and defined.
2. **Layout** before bodies: vtable order per class via the awk one-liner in `tu-reconstruction.md` (declare virtuals in that order, with the base's return types); param names and defaults from `.rodata` in `build/GMSE01/asm/<path>.s`; animation indices from the `.bas` table rule; the string-pool prefix includes (`InfectiousStrings`, `MapCollisionManager`, the MSound pair for `__sinit`).
3. **Draft** each function with m2c: `build/venv/bin/python3 "$M2C" -t ppc -f <mangled> --globals=used build/GMSE01/asm/<path>.s`, then rewrite with real fields, inlines and enums (GX constants checked against the asm immediates). Nerves use `DEFINE_NERVE`.
4. **Order**: definitions in the map's order reversed (`-inline deferred`); `tools/validate-symbol-order.py` checks it. UNUSED bodies go in the `.cpp` and should compile to their map sizes.
5. **Write the whole TU first, verify, commit.** Only then polish, and budget it: at most **five minutes** per function below 99%, and **skip any function over 2 KB that is below 95%** unless the difference is structural (a missing call, block or branch, not frame size, scheduling or register numbering); note it in the report and move on. Re-reading a 4 KB diff several times is the single most expensive thing a batch does: start every function with `decomp-diff.py -d <fn> --clusters` (mismatch runs only), read the full `--no-collapse` diff at most once per function, and after each trial re-run `--clusters` or check the match percentage, never the whole diff. Apply the catalog rules in this order: structure (calls, control flow, load/store order); inlining; then frame-gap causes (param `.get()` vs wrapper, accessor reads, uninitialised locals, named results).
   The inlining rule that decides most call-vs-expand differences is measured, not guessed: MWCC inlines a plain callee only up to **14 statements at depth 1, 9 at depth 2, 6 at depth 3, 2 at depth 4, never at 5**, and an `inline`/in-class body has **no limit at depth 1**. Definition order in the TU changes nothing. So a plain method retail *calls* at depth 1 had 15+ statements (a hard lower bound on its reconstruction), a helper retail expands at depth 1 despite being large was declared `inline`, and one statement added or removed (a named fetch, a ternary for an if/else, a wrapper) flips a decision. Count statements, then depth, before trying anything else.
   Leave a `// TODO:` naming the remaining difference and move on; register renumbering and frame-only gaps beyond that budget are for a later bulk pass.
6. Commit the unit, then start the next one.

## Rules

- Edit only your units' `.cpp` files and their own headers. Any other header change: leave a `// TODO:` describing it and report it; do not make it.
- Likewise, enumerators that belong in a shared enum (particle ids, camera-shake modes, hit messages) are parked under a **TU-prefixed name** (`BOSSPAKKUN_CAM_SHAKE_BOPA_DOWN`), not the canonical name: a header batch may add the canonical enumerator while you work, and two definitions of one name break every TU that includes both.
- If your unit needs a helper that belongs in a shared header (the map shows it weak, i.e. a header inline), park it as `static inline` **in your `.cpp`**, never in your unit's header: two units parking the same name in two headers broke the build for every TU that includes both.
- No stack padding (`volatile char trash[]`, `u32 pad[]`), no `#pragma dont_inline`, no reinterpret casts, no fakematches. Temporary padding to confirm a body is otherwise exact is fine if removed.
- One change at a time; rebuild and re-diff.
- Do not run `ninja baseline` (one is saved in your worktree). Do not edit `configure.py`. If a unit becomes fully matching in code and data, link-test it when your spawn message says so (the `objects.json` form of `git show be4b773e`, full `ninja` exit 0, SHA-1), otherwise say so in the report.
- Verify each unit before its commit: `build/venv/bin/ninja changes_all` (zero regressions outside your units), `NM=build/binutils/powerpc-eabi-nm build/venv/bin/python3 tools/validate-symbol-order.py -u <unit> --map orig/GMSE01/files/marioUS.MAP`, and `sha1sum build/GMSE01/mario.dol` must print `a6782903ef79d4196c8489ecb1b57decb5b3728f`.
- Commit on your `wt/` branch with a one-line imperative message in the repo's style (`git log --oneline -10`) and the trailer `Claude-Session: https://claude.ai/code/<the trailer named in your spawn message>`. Do not push or merge. Commit early and often; a cut-off loses only what is uncommitted.
- Scratch files go under the scratchpad directory named in your spawn message, never in the repo.

## Report back

Per unit: exact-function count, unit match % for code and data, symbol-order result, commit hashes.
Then the final `changes_all` totals, the SHA-1 result, any shared-header changes needed (file, change, evidence, measured effect), and reusable findings for `docs/catalog/`: codegen rules with the function that proved them and the rejected alternatives, not per-function logs. Describe them; do not edit the catalog.
