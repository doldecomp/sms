# Closure brief for unit agents

You are closing translation units of Super Mario Sunshine (GMSE01, MWCC 1.2.5): each unit in your spawn message has one to three functions left that are not byte-exact, and everything else in it already matches.
The goal is not a higher percentage; it is **zero non-exact functions and 100% data**, so the unit can be source-linked.
`docs/BATCH_BRIEF.md` applies in full (worktree, reading list, rules, verification, commits) except where this file overrides it.

## What is different from a sweep

- **No time cap per function.** The five-minute polish budget does not apply; the remaining function *is* the batch. Budget roughly 30-40 minutes per unit, then write the report and move to the next unit.
- **Read the unit's history first.** `git log --oneline -- src/<path>.cpp`, the unit's entry in `docs/catalog/units/*.md` and `docs/catalog/frame-gaps.md`. Previous agents recorded rejected trials there; do not repeat one unless you have a new reason.
- **Classify the residue before editing.** Start with `decomp-diff.py -d <fn> --clusters` (mismatch runs, marker counts, frame); read the whole diff with `--no-collapse` at most once per function, and after each trial re-run `--clusters`, never the full diff. Write down, in the scratchpad, which class it is:
  1. structural (missing/extra call, block, branch, load/store order) — always fixable, fix first;
  2. inlining (a call where retail expands or the reverse) — count statements and depth per the inlining table in `docs/catalog/codegen-tells.md`;
  3. frame size only (`stwu`/`addi r1` and the save/restore offsets differ, bodies identical) — see the two-region frame model in `frame-gaps.md`; the candidates are named results, `.get()` vs wrapper, accessor by value vs reference, an unnamed temporary, a `TVec3` copy;
  4. register or float-register permutation with identical instruction sequence — usually a declaration-order or evaluation-order difference; try reordering declarations and operands before anything else;
  5. scheduling only — usually a hidden dependency (a store the compiler must order before a load); look for a missing `const`, a by-reference argument or an aliasing pointer.
- **Probe in a scratch TU when a hypothesis is cheap to isolate.** Copy the function into `<scratchpad>/probe.cpp` with minimal stubs, compile it with the game flags (`build/compilers/GC/1.2.5/mwcceppc -O4,p -fp_contract on -str reuse,readonly -opt all,nostrength -inline deferred -nodefaults -proc gekko -enum int -Cpp_exceptions off -RTTI off -I include ...`; copy the exact command from `build/GMSE01/build.ninja` for the unit) and compare frame size and instruction count between variants. Ten probe compiles cost less than one full re-read of a 2 KB diff.
- **Data counts.** If the unit's data is below 100%, fix that too: `python3 tools/decomp-diff.py -u <unit> -t object -s nonmatching` and the `.s` file show which object differs (const-ness and section, float literal pool order, string pool order, vtable contents, `.sdata` vs `.data`).
- **Do not link it yourself.** When code and data both reach 100%, say so at the top of the report; the orchestrator adds the unit to `config/GMSE01/objects.json` and checks the DOL SHA-1.

## Report back

Per unit: closed or not; if not, the residue class from the list above, the exact remaining difference (instruction offsets, frame sizes), every trial made with its measured effect, and your best hypothesis for the next attempt.
A precise "not closed" report is worth almost as much as a closure: it becomes the unit's catalog entry and the seed of a research batch.
