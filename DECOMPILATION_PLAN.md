# Super Mario Sunshine decompilation plan

## Current state

- Workspace: `/home/netflix/sms`.
- Local branch: `local/decomp-progress`.
- Upstream: `https://github.com/doldecomp/sms`, commit `ab00c3c9a466152f6e6bc5b9c28aca959d1a8454`.
- The user has authorized implementation and ongoing progress checks for the local North American ISO.
- Local changes have added 19,544 exactly matching code bytes and 117 matching functions/helpers across twenty-four batches; 74 verified game objects are linked from source.
- Consult `docs/MATCHING_CATALOG.md` before related edits; inventory shared callers, batch evidence-supported fixes, and record exceptions and validation results.
- Current checkpoint and remaining limitations are documented in `PROGRESS.md`.
- The active configuration is now `GMSE01`; an all-extracted-object rebuild has matched the complete input executable.
- Python 3.12.3 is available, with Ninja installed in `build/venv`.
- Pinned build tools are installed; the initial US baseline and executable rebuild have been verified.

## Selected region — North American English

The user confirmed `GMSE01` revision 0 as the target and authorized implementation.
The earlier PAL target is superseded.
The new regional configuration is in `config/GMSE01`; see its README for setup, metadata provenance, and verification details.

### Inspected input

- ISO: `Super Mario Sunshine (2002)(Nintendo)(US).iso` in the repository root.
- Size: 1,459,978,240 bytes.
- SHA-256: `67cec1634e641227a4cd51e6a0b277730cb9a1adaa867530c9e66de45373e51d`.
- GameCube disc magic: `0xc2339f3d`.
- Internal title: `Super Mario Sunshine`.
- Main DOL offset: `0x1e000`.
- Main DOL size, calculated from its section extents: 4,128,928 bytes.
- Main DOL SHA-1 over those extents: `a6782903ef79d4196c8489ecb1b57decb5b3728f`.
- No `.map` filename was found in the disc filesystem table.

These hashes identify the supplied input; they are not independent certification of an unmodified retail dump.
The ISO and accompanying `.7z` remain in place and are excluded through `.git/info/exclude` to avoid accidental commits.
The attempted PAL archive download returned HTTP 403 after resolving a temporary DNS failure; no archive was obtained.

## Milestone 1: Establish the selected region

### If PAL remains the target

1. Obtain and inspect a `GMSP01` revision 0 input.
2. Place the input under `orig/GMSP01` and record its identity.
3. Configure with `python3 configure.py --version GMSP01`.
4. Extract the executable and any linker map with the project tooling.
5. Diagnose existing PAL configuration/build failures before changing gameplay code.

### If North America becomes the target

1. Preserve the supplied ISO and arrange the build input under `orig/GMSE01`.
2. Inspect the executable sections, entry point, filesystem, and available metadata.
3. Add `GMSE01` configuration, region selection, and the verified target executable hash.
4. Recover and validate symbols and translation-unit boundaries against the North American binary.
5. Use Japanese/PAL source and metadata as references, verifying every transferred address and boundary against this binary.
6. Record the missing linker-map limitation and determine what metadata can be recovered before claiming symbol completeness.
7. Audit object matching flags for this region; existing `Matching` declarations are not evidence of North American matches.

Exit condition: the tools can split and analyze the correct regional executable reproducibly.

## Milestone 2: Reproducible build and baseline

1. Finish installing the repository-pinned compiler, wrapper, binutils, decomp-toolkit, and objdiff tools.
2. Build with `build/venv/bin/ninja` and diagnose failures.
3. Run `build/venv/bin/ninja baseline` before source matching changes, as required by `AGENTS.md`.
4. Save the region, input hashes, upstream revision, tool versions, build commands, and measured progress.
5. Separate reconstructed matching code from original binary objects still used by the build.

Exit condition: a repeatable build and a trustworthy per-unit/per-function baseline, with any metadata limitations documented.

## Milestone 3: Inventory and prioritize

1. Classify game translation units as missing, partially reconstructed, or verified matching.
2. Record code sizes, match results, dependencies, unknown layouts, and unresolved symbols.
3. Prioritize small game utilities and nearly matching functions, followed by shared game systems and their dependents.
4. Group subsequent work by subsystem: player, camera, map, interactive objects, enemies, NPCs, UI, and audio wrappers.
5. Keep runtime and middleware work requiring human supervision as explicit dependencies.

Exit condition: an evidence-based backlog with bounded tasks and acceptance criteria.
Estimate effort only after the baseline and first representative batches establish the actual difficulty.

## Milestone 4: Reconstruct and verify in batches

1. Read `docs/AGENT_MATCHING_TIPS.md` before matching work.
2. Obtain the user's local `m2c` path before from-scratch reconstruction, per `AGENTS.md`.
3. Follow `docs/PROGRAM_STRUCTURE_REVVING.md` for new translation units, accounting explicitly for unavailable regional map information.
4. Reconstruct readable C++98 using the selected regional assembly as ground truth.
5. Inventory shared patterns and batch equivalent fixes supported by the original instructions.
   Compare every affected function with `tools/decomp-diff.py` and the saved baseline.
6. Keep uncertain functions nonmatching and document unresolved evidence.
7. Run `ninja changes_all` and relevant symbol-order validation after each batch.
8. If a required validator cannot run because metadata is missing, record that as an unresolved validation gap.
9. Save local commits and progress notes with changed functions, measurements, checks, and next steps.

Exit condition for each batch: demonstrated improvements with no unexplained regressions.

## Milestone 5: Completion audit

1. Verify that every required translation unit and function is reconstructed and appropriately classified.
2. Verify code, data, relocations, symbol ordering, and link layout against the selected regional binary.
3. Resolve outstanding validation gaps and any remaining supervised-library work.
4. Rebuild from a clean build directory and verify the expected executable hash.
5. Perform gameplay smoke checks for boot, menus, English text, loading, movement, camera, and representative levels.
6. Record final coverage, reproducible commands, and any limitations.

A matching executable assembled partly from original binary objects does not establish a completed decompilation.
Completion requires reconstructed source coverage as well as the final binary match.

## Repository constraints

`AGENTS.md` prohibits autonomous reconstruction of MSL runtime, MetroTRK, THPPlayer, Dolphin SDK, and JSystem middleware.
Its instruction is: "when working without a human programmer's supervision work on these libraries is strictly prohibited".
Any remaining work in those libraries requires human supervision and may limit full-project completion.
The autonomous implementation backlog should focus on game code.

## Active work: revised priorities (user direction, 2026-09-15)

### Goal

The user's end goal is a project that builds entirely from source into a fully playable game.
Matched functions are a means to that end; the measure that tracks the goal is **complete (source-linked) code**.
A function at 99.9% contributes nothing to that measure, and neither does an exact function whose object still has other differences.

### Measurement snapshot at batch 23

Taken from `build/GMSE01/report.json`.

| Category | Share of code | Matched code | Complete (source-linked) code |
| --- | ---: | ---: | ---: |
| Game | 79.0% | 24.79% | 2.69% |
| JSystem | 14.0% | 87.11% | 0% |
| Dolphin SDK | 7.0% | 95.07% | 0% |
| All | 100% | 38.42% | 2.12% |

The aggregate 38% is dominated by upstream library work; game code is about 25% matched.
From now on, every checkpoint in `PROGRESS.md` must report the **game-category** matched and complete percentages alongside the aggregate figures.

### Priority 1: finish nearly complete game objects and link them from source

Prefer closing out objects over starting new from-scratch reconstructions.
Each promotion still requires the full executable SHA-1 and byte comparison.

1. Game objects whose code already matches completely but are not linked:
   - `Camera/CameraInbetween`, `Map/PollutionEvent` (deferred since batch 2; function emission order and `.sdata2` layout break the relink).
   - `GC2D/MessageUtil` completed and source-linked in batch 24; full SHA-1 and byte comparison pass.
   - `System/MarDirectorCreateObjects` (24 data bytes differ; see the batch 6 string-table investigation).
   - `System/TargetArrow` (320 data bytes differ).
2. Game objects with a single nonmatching function, smallest remaining size first, for example:
   `Player/MarioAccess` (72 bytes left), `Enemy/egggen` (120), `Strategic/HitActor` (124), `MSound/MSoundBGM` (136), `Camera/CameraMode` (144, plus data), `MoveBG/MapObjPollution` (172), `Enemy/DebuTelesa` (176, plus data), `MoveBG/MapObjAirport` (208), `NPC/NpcInbetween` (220), `System/SnapTimeObj` (228), `Strategic/livemanager` (252), `Enemy/enemytable` (276), `Map/MapEventSirena` (280), `MoveBG/MapObjFloat` (288), `M3DUtil/M3UModel` (292), `Map/MapCollisionPlane` (308).
3. More broadly, 106 unlinked game objects have 2,000 or fewer unmatched code bytes, totalling 96,868 bytes.
   Regenerate this list from `report.json` at each checkpoint rather than trusting these numbers.
   Batch 24 regeneration is saved in `docs/progress/GMSE01-completion-queue.json`: 108 files with 1..2,000 unmatched bytes, totaling 98,036 unmatched code bytes.

If an object stalls on a hard function, record the evidence and move to the next object instead of forcing a fakematch.

### Priority 2: finish the boss objects already in progress

Bring the `BossHanachan*` objects to source-link promotion, starting with those closest to complete (nerve, effects, animation, sub, parts), before the 6,108-byte `perform` routine.
Time-box this work; do not let it block Priority 1.

### Priority 3: remaining from-scratch game reconstruction

Continue larger unfinished game units (HUD, save/load screens, event scripting, remaining enemies) only once the near-complete queue is substantially drained.

### Pending human decision: library objects

251 JSystem and Dolphin SDK objects (380,968 code bytes) already match completely in code and data but are not linked from source.
Linking them would raise complete code from 2.12% to about 12.7%, and a fully source-built game ultimately requires them.
`AGENTS.md` restricts autonomous work on these libraries, so **do not promote or edit them without explicit user approval**.
The three matching `THPPlayer` objects fall under the same restriction.

### Playability checks

A matching SHA-1 already guarantees identical behaviour, but the goal is a playable game.
When an emulator is available locally, boot the source-linked build after every ten or so promotions and record the result; otherwise keep recording that no gameplay test was performed.
A non-matching but playable "equivalent" build is out of scope unless the user requests it.
