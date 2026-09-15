# Local decompilation progress

## Target and status

Target: North American English, `GMSE01` revision 0, using the local ISO.
The full decompilation is **not complete**.
The local branch is `local/decomp-progress`.
The upstream starting commit is `ab00c3c9a466152f6e6bc5b9c28aca959d1a8454`.
Before related edits, consult the [shared-fix catalog](docs/MATCHING_CATALOG.md) and search for other callers.

## Latest checkpoint: batch 9

Reconstructed the boss head/body animation setters and hit-message handlers, plus tumble-rate adjustment, circular-shadow submission, and damage fog.
The shared hit-reaction predicate is reconstructed once and used by both message handlers.
Added the main boss's mapped declarations and field layout needed by these parts, including its eight body pointers, head pointer, weak-body index, and parameter pointers.
Owner fields whose meanings are not established retain provisional names.

The tumble-rate function now matches all **188 bytes** exactly.
The other six routines remain nonmatching: shadow submission 99.54%, head animation setter 97.746475%, body animation setter 91.818184%, head hit handler 96.02941%, body hit handler 96.78832%, and damage fog 93.683334%.
Their remaining differences include state-lookup inlining, registers, and stack layout.
Both hit handlers use the shared overturned-state test; the body handler also follows the original six-case hip-drop eligibility table.

Corrected `TWaterHitActor::onWaterHitCounter` to an out-of-line declaration, consistent with its global symbol in `BossHanachanSub.cpp` and the head caller's original call instruction.
The former inline wrote a 32-bit value, while the original function stores a 16-bit counter.
The underlying field declaration still needs a coordinated audit with particle-manager consumers; it was not globally narrowed.
See the [shared-fix catalog](docs/MATCHING_CATALOG.md) for the evidence and pending work.

Aggregate exact code is **1,369,460 / 3,603,748 bytes (38.00099%)**, with **8,147 / 12,904 functions** matching.
Matched data remains 298,283 bytes.
There are still **72 source-linked objects**, covering **73,452 code bytes (2.0382113%)**.
The parts object remains incomplete and is not source-linked.

### Validation and remaining work

Used the saved batch 9 baseline at `f7eb2cf2`; rebuilt all affected units and ran `ninja changes_all`.
Compared every reported function against the baseline, including missing-function detection: zero regressions.
The full build, expected SHA-1, and byte comparison against the original US executable pass.
These executable checks validate the current mixed source/original-object build, not the unfinished parts implementations.
No gameplay test was performed.

The parts object's ordering and linkage pass; its map-presence check still fails on the foot destructor/thunk and `CLBPalFrame<short>`.
Three UNUSED-size warnings remain: the hit predicate (208 versus 196 bytes), `isMarioOn_` (still a stub), and the joint-matrix helper.
The sound object's complete map check passes.
Remaining parts work includes the constructor, collision/foot initialization, animation-state dispatcher, `isMarioOn_`, and exact matching of the partial routines.
The parameter-constructor draft is saved locally for the next layout reconstruction.

See [batch 9 measurements](docs/progress/GMSE01-batch9.json).
Build and change logs are `build/GMSE01-build-batch9.log` and `build/GMSE01-changes-batch9.log`.

## Verified checkpoint: batch 8

Started reconstructing `BossHanachanParts.cpp`, with complete mapped base/head/body method declarations and explicit TODO bodies for unfinished behavior.
Identified the shared motion controller as `TNpcInbetween` from its constructor layout and calls in `BossHanachanMain`.
Reconstructed ground-actor lookup, moving collision translation, getting-up rotation, frame transfer, animation-end checks, and three UNUSED animation helpers.
The first three routines match exactly; frame transfer and animation-end checks remain different only in stack offsets.
The three UNUSED helpers reproduce their original 48-, 12-, and 16-byte sizes.

The derived virtual tables exposed a missing pure virtual animation setter in the base class.
Its null slot was incorrectly called padding in batch 7.
The corrected base table now matches all 256 bytes, and the previously source-linked sound object still reproduces the executable exactly.

This batch adds **1,496 exactly matching bytes and nine functions/helpers**, including three destructors, two adjustment thunks, and static initialization.
Aggregate exact code is now **1,369,272 / 3,603,748 bytes (37.995777%)**, with **8,146 / 12,904 functions** matching.
Matched data is 298,283 bytes.
There are still **72 source-linked objects**; the unfinished parts object is not promoted.

### Validation and remaining work

Captured the batch 8 baseline at `122eab40` before edits; ran `ninja changes_all` and compared every reported function with that baseline.
There are zero function regressions.
The full build, SHA-1 check, and byte comparison against the original US executable pass.
No gameplay test was performed.
The parts object's ordering and linkage pass map checks, but its overall map check fails because the foot destructor/thunk and `CLBPalFrame<short>` are not emitted yet.
There are three UNUSED-size warnings, including two TODO bodies and the joint-matrix helper.
The sound object's map check passes.

The main boss class and parameter declarations still need a full-map reconstruction before completing the parts constructor, collision setup, damage effects, and animation/message behavior that depends on them.
The partially reconstructed constructors are not runnable replacements for the originals yet.
Do not promote this object based on its matching helper functions.

Added a durable [shared-fix catalog](docs/MATCHING_CATALOG.md) with successful cases, exceptions, search terms, and unresolved cross-file work.
For example, the boss frame-transfer routine needs the current animation accessor, while the already-exact NPC blend routine needs the previous-animation accessor; a mass replacement would be incorrect.
See [batch 8 measurements](docs/progress/GMSE01-batch8.json).
Drafts and validation logs remain under `build/GMSE01` and `build/GMSE01-*-batch8.log`.

## Verified checkpoint: batch 7

Reconstructed `Enemy/BossHanachanSound.cpp` from the original executable's 38-entry animation sound table.
The table getter and destructor adjustment thunk now match exactly, and the entire object is linked from source.
Added every mapped `TBossHanachanPartsBase` method declaration, including UNUSED methods, with inheritance checked against its constructor and virtual table.
Constructor and animation-setting instructions establish the current/previous animation fields; collision initialization identifies the water hit actor, moving collision, and joint matrix pointers.
The animation enums remain incomplete, and the motion-controller pointer remains opaque.

There are now **72 source-linked objects**, covering **73,452 / 3,603,748 code bytes (2.0382113%)**.
This batch adds **20 exactly matched code bytes, two matching functions/helpers, and 1,440 source-linked data bytes**.
Aggregate exact code is **1,367,776 bytes (37.95426%)**, with **8,137 matching functions**.
Aggregate matched data increased by 1,032 bytes to 297,687 bytes.

### Validation

Captured `ninja baseline` at `6718c96a`, rebuilt, and ran `ninja changes_all`.
Every reported function was compared against the baseline: zero regressions.
The sound object's map presence, ordering, and linkage checks pass.
The final source-linked executable passes both the expected SHA-1 and a full byte comparison against the US original.
No gameplay test was performed.

At this checkpoint, four bytes after the reconstructed 252-byte table were interpreted as alignment padding.
Batch 8 disproved that interpretation: the final slot belongs to a pure virtual animation setter.
The batch 7 executable matched because padding happened to produce the same null bytes; the class declaration was incomplete.

### Remaining work

`BossHanachanParts.cpp` is still empty and nonmatching.
Its base-class declarations now support reconstructing animation selection and collision behavior, but the head/body subclasses must also be fully scaffolded from the map before implementing that unit.
The new sound-table object does not imply that the boss itself is decompiled.
See [batch 7 measurements](docs/progress/GMSE01-batch7.json).
Build, change, and map-check logs remain under `build/GMSE01-*-batch7.log` and `build/GMSE01/BossHanachanSound-symbol-order-batch7.log`.

## Verified checkpoint: batch 6

Corrected the US crash-reporting filename to `/marioUS.MAP` and its read-only data placement in `System/marerr.cpp`.
Both functions and both data symbols now match in the detailed object comparison, and the object is linked from source.
There are now **71 source-linked objects**, covering **73,432 / 3,603,748 code bytes (2.0376563%)**.
This adds 284 source-linked code bytes and 40 matched data bytes.
The aggregate report already counted the two functions as matched before the data/relocation correction, so aggregate exact code remains **1,367,756 bytes (37.95371%)**, with **8,135 matching functions**.

### Validation

Captured `ninja baseline` at `57f560d3` and ran `ninja changes_all` after the final changes.
Comparing every reported function found zero regressions.
The crash-reporting object's map presence, ordering, and linkage checks pass.
The full build, expected SHA-1, and byte comparison against the original US executable pass.
This remains a mixed source/extracted-object build; no gameplay test was performed.

### Data-layout investigation

The native light-manager name starts at `0x803A8DC0`, eight bytes before its address in the imported map.
Several preceding movie strings are also eight bytes before their mapped addresses.
A limited object-boundary correction made the light-manager object compare exactly, but left the legacy linker running for over five minutes with an empty output ELF.
That experiment was stopped and reverted, and the restored configuration passed the executable checks.
Audit the surrounding string table and relocations together before attempting another source-link promotion of `MarDirectorCreateObjects.cpp`.
Matching trials in `MarioAccess.cpp` and `MapObjPollution.cpp` were also reverted; the egg-generator control routine still differs in stack layout.

See [batch 6 measurements](docs/progress/GMSE01-batch6.json).
Build and change logs remain under `build/GMSE01-*-batch6.log`.

## Verified checkpoint: batch 5

Reconstructed the missing bathtub grip family in `MoveBG/MapObjCorona.cpp` and its header.
This includes the grip, collision-part base, hard parts, fragile parts, and all 26 bathtub parameters.
Class inheritance and fields follow constructor stores and virtual tables; descriptive field names are inferred from their uses.
All four new grip-family virtual tables match exactly.
Corrected `TBathtub::getNumGripsDead` from `u8` to `int`, eliminating the wrong narrowing operation in the Bullet Bill item-drop caller.

This batch adds **1,376 exactly matched bytes and 20 matching functions/helpers**.
These include the grip counter, animation update, collision removal, message forwarding, matrix getters, four destructors, four adjustment thunks, and an emitted existing nozzle getter.
Exactly matched code is now **1,367,756 / 3,603,748 bytes (37.95371%)**, with **8,135 / 12,904 functions** matching.
Matched data increased by 1,476 bytes to 296,615 bytes.
There are still **70 source-linked objects**; neither bathtub unit is complete or newly source-linked.

### Validation

Captured `ninja baseline` at `218d1312`, rebuilt the changed units, and ran `ninja changes_all`.
Every reported function was compared against the baseline: **zero regressions**.
The full executable build, expected SHA-1, and byte comparison against the local US executable all pass.
This checks the mixed source/extracted-object build, not completion of the new decompilation bodies.
No gameplay test was performed.

The bathtub file's function ordering and linkage now pass map validation.
The overall map check still fails because two expected template symbols are not emitted: `SMatrix33R<float>`'s constructor and `TVec3<float>::set<float>`.
There are also 17 UNUSED-size warnings.
The Bullet Bill unit's map check passes with its existing warnings.
No library implementation was changed.

### Remaining work and next checkpoint

| Routine | Comparison | Remaining issue |
| --- | ---: | --- |
| Grip control | 99.951965% | Stack frame differs by 16 bytes |
| Bullet Bill item drops | 99.926384% | Temporary vectors are four bytes low on the stack |
| Bathtub parameter constructor | 99.84395% | String offsets depend on unfinished TU data |
| Grip-part matrix lookup | 99.72222% | Stack frame differs by 16 bytes |
| Grip constructor | 99.36323% | Stack layout, model-flag registers, and string offsets |
| Bathtub constructor | 86.833336% | Missing nested matrix constructor call |
| Tumble eligibility | 82.83871% | Local-coordinate helper and template inlining |
| Grip messages | 78.30846% | Stubbed quake, hipdrop, and demo callees; parameter loads |
| Nearest-grip query | 70.982605% | Vector temporaries and `std::fmodf` inlining |

These are partial comparison scores, not exact matches.
The standalone reset, collision removal/setup, part constructors, and `startBreak` helpers have the original UNUSED sizes.
`startCrack`, `isCracking`, and `trample` still differ in size.
Other bathtub behavior and several direction/demo helpers remain TODO stubs.

The original bathtub constructor calls `SMatrix33R<float>` at the shared data matrix at `0x188`.
The current `TBathtubData` declaration uses a column-major matrix.
A trial correction exposed transposed-access assumptions in existing water routines and was reverted; update that type and its consumers together after checking each caller's assembly.
`allowsTumble` and `TBathWaterManager::throwMario` share the same original local-coordinate conversion sequence, which is a useful next reconstruction target.
The quake and hipdrop drafts reference `TKoopa`, whose class scaffolding is also missing; scan its full map before introducing declarations.

See [batch 5 measurements](docs/progress/GMSE01-batch5.json).
The m2c drafts, baseline/build/change logs, and both map validation logs remain under `build/GMSE01` or `build/GMSE01-*-batch5.log`.

## Verified checkpoint: batch 4

Reconstructed five more previously stubbed routines in `Enemy/BathtubKiller.cpp`, together with its mushroom-spawning and break helpers.
Recovered manager fields at `0x60`, `0x64`, `0x68`, and `0x69` from the load and item-drop instructions.
Their descriptive names are inferred from the lives comparisons and mushroom-spawning behavior.
The compiled `generateMushroom` helper now has the original map's 100-byte size.

| Routine | Previous comparison | Current comparison |
| --- | ---: | ---: |
| Model and material setup | 1.09% | 88.07% |
| Item drops | 0.61% | 98.94% |
| Water reaction | 1.43% | 54.46% |
| Break state | 2.75% | 99.94% |
| Manager load-after | 1.41% | 84.49% |

These percentages describe partial instruction comparisons, not completed functions.
Exactly matched code remains 1,366,380 bytes (37.915524%); none of these five routines is newly exact.
Matched data increased by 808 bytes to 295,139 bytes.
The build still links 70 source objects, and `BathtubKiller.cpp` remains nonmatching.

### Verification and remaining issues

Captured a fresh `ninja baseline` at `1ccbf133`, rebuilt, and ran `ninja changes_all`.
Comparing every reported function found zero regressions.
Symbol validation passes with weak-symbol ordering warnings and 16 remaining UNUSED size mismatches.
The final executable passed SHA-1 verification and a full byte comparison against the local US executable.
No gameplay test was performed.

The item-drop routine still differs in temporary-vector stack offsets and the declared return type of `TBathtub::getNumGripsDead`.
Its caller compares a signed integer in the original, while the existing declaration returns `u8` and adds a narrowing operation.
The grip class currently has only a forward declaration, so reconstructing the grip-count body requires further class work.
The break state has the same four-byte temporary-vector offset discrepancy as the explosion state.
The water reaction still needs its original inlined state-check boundaries recovered.
Manager loading retains unexplained null comparisons in the original, and model setup needs further scheduling and register-allocation work.

See [batch 4 measurements](docs/progress/GMSE01-batch4.json).
The current logs are `build/GMSE01-build-batch4.log`, `build/GMSE01-changes-batch4.log`, and `build/GMSE01/BathtubKiller-symbol-order-batch4.log`.
The m2c drafts remain under `build/GMSE01/BathtubKiller-*-m2c.c`.

## Verified checkpoint: batch 3

Reconstructed the bathtub Bullet Bill initialization, reset, matrix update, gravity query, active-count loop, personality parameters, and explosion state from the local executable.
The original symbols already existed as stubs.
The five personality fields are now grouped in the existing `TBathtubKillerPersonality` class, whose three parameter-copy methods reproduce the inlined instructions and the map's 44-byte UNUSED sizes.
Field names describe their corresponding parameters; they are inferred names.

Five game functions now match exactly: `init`, `reset`, `calcRootMatrix`, `getGravityY`, and `countActiveKillers`.
The reconstructed explosion state also emits the previously missing 92-byte integer-vector helper with an exact match.
This batch adds 632 exactly matched bytes and six matched functions, bringing the total to 1,366,380 of 3,603,748 bytes (37.915524%) and 8,115 of 12,904 functions.
Cumulative gains over the initial regional baseline are 1,272 bytes and ten functions.

The reset helper is at 99.944954%, and the explosion state is at 99.95%; both still differ in stack layout.
The manager's load routine is at 81.03704%, with unexplained null comparisons and stack space still to reconstruct.
Other behavior in this unit remains stubbed, so `BathtubKiller.cpp` remains nonmatching and is not promoted into the source-link manifest.
There are still 70 source-linked objects.

### Validation

- Captured `ninja baseline` at `21fd1969` before changes, then rebuilt and ran `ninja changes_all`.
- Compared every reported function against that baseline: zero regressions.
- Checked individual instruction differences and validated map symbols, ordering, and linkage.
  The map check passes with warnings for weak-symbol order and 17 remaining UNUSED size differences.
- Rebuilt the executable and passed both the expected SHA-1 and a full byte comparison.

See [batch 3 measurements](docs/progress/GMSE01-batch3.json).
Drafts from m2c, the build log, the regression log, and the map validation log are preserved under `build/GMSE01` or as `build/GMSE01-*-batch3.log`.
The original disc image remains excluded from Git.
No gameplay test has been performed; the matching executable still uses extracted objects for unfinished units.

### US audio layout dependency

Several game-side audio routines access fields four bytes later in the US binary than in the current declarations.
Examples include `MSMainProc::toInnerCameraDemo` (target byte `0xCE`, current `0xCA`) and `MSound`'s camera array (target `0xB0`, current `0xAC`).
The constructor also writes additional fields at `0x94` and `0x98` and uses `0x9A` for a field currently at `0x94`.
This indicates a regional layout change around the end of the `JAIBasic` base class, requiring supervised library review under `AGENTS.md`.
No library source or speculative padding was added.

## Verified checkpoint: batch 2

The build now links 70 verified game objects from source.
Exactly matched code is 1,365,748 of 3,603,748 bytes (37.897987%), with 8,109 of 12,904 functions matching.
The two batches together added 640 exactly matching bytes and four matching functions to the initial regional baseline.
The complete decompilation remains unfinished.

### Changes and verification

- `TNpcThrow::throwMario` now matches all 324 bytes after removing an unnecessary intermediate yaw variable.
  Its source object was added to the verified link manifest.
- `TTalkCursor::associateNPC` now uses the existing matrix translation operation, reproducing the original load/store sequence.
  Its comparison improved from 93.62857% to 99.6%; the stack layout still differs, so the unit remains nonmatching.
- `THitActor::calcEntryRadius` and `initHitActor` now have inferred `void` return types, removing the invented height-squared return value.
  No callers consume their result.
  The radius routine improved from 97.6129% to 98.258064%, and the affected `TEMario::init` improved from 93.97248% to 97.76147%.
  These routines remain nonmatching where indicated by the comparison report.

A fresh baseline was captured at commit `1ccb911a` before these source changes.
All changed source units passed symbol presence, order, and linkage checks.
A full rebuild of the affected callers and `ninja changes_all` found zero function regressions.

The entire `build/GMSE01` output directory was then moved aside, and Ninja successfully rebuilt from the stored source/configuration and extracted input.
The clean rebuild passed the expected SHA-1 and a full byte comparison with the input executable.
A deliberately modified copy of the input was rejected by the configured hash check.
No original game input was changed during this test.

See [batch 2 measurements](docs/progress/GMSE01-batch2.json).
The clean-build log is `build/GMSE01-clean-build.log`; the current regression log is `build/GMSE01-changes-batch2.log`.
The prior build, intermediate experiments, and reports remain in `build/GMSE01-before-clean-v1`.

### Deferred source-link investigations

Further testing confirmed the three deferred objects still break the complete binary match.
`CameraInbetween` changes constant layout and reduces `.sdata2` by eight meaningful bytes; shared weak-symbol selection also needs investigation.
`MessageUtil` emits stream getters before `SMSMakeTextBuffer`, shifting its address.
`PollutionEvent` emits a destructor before `loadAfter`, shifting the latter's address.
The diagnostic logs are preserved in `build/GMSE01-before-clean-v1/diagnose-*.log`.
These are pending layout/source-emission tasks, not verified source-link improvements.

## Verified checkpoint: regional setup and batch 1

The new regional configuration rebuilds `mario.dol` byte-for-byte identically to the executable extracted from the supplied disc.
Both the expected SHA-1 and a direct `cmp` passed.
That checkpoint linked 69 verified game objects from source and retained extracted objects for the remaining units.

| Measure | Initial baseline | Batch 1 |
| --- | ---: | ---: |
| Matched code bytes | 1,365,108 | 1,365,424 |
| Matched functions | 8,105 | 8,108 |
| Overall matched code | 37.88023% | 37.888996% |
| Verified game objects linked from source | 0 | 69 |

Exact machine-readable measures are in [the initial snapshot](docs/progress/GMSE01-initial.json) and [the batch 1 snapshot](docs/progress/GMSE01-batch1.json).
Category totals changed when previously unclassified US-only metadata was assigned to the correct libraries; this is not a source regression.

### Source improvements

- `TNpcBalloon::updateBalloon`: use the director accessor and a named console pointer to reproduce the original inline call and stack layout; 108 bytes now match.
- `TProcessMeter::TProcessMeter`: preserve the US-specific field value `0x1C2`, while retaining the existing value for other versions; 64 bytes now match.
- `TTimeRec::flip`: use a reference to the current timing entry so the loop reproduces the original register allocation; 144 bytes now match.

The batch gained 316 exactly matching code bytes and three matching functions.
All three changed units also passed incremental source-link tests against the complete executable.

### Checks performed

- Repository-pinned toolchain installed and compiler execution verified.
- Disc region, revision, filesystem, and extracted executable inspected.
- Regional map layout checked against the executable.
- `ninja baseline` captured before source changes.
- Per-function `tools/decomp-diff.py` comparisons reached 100% for the three changed functions.
- Symbol presence, order, and linkage checks passed for all three changed source units.
- `ninja changes_all` and a function-by-function baseline comparison found zero function regressions.
- Full `ninja` build passed the expected executable SHA-1.
- `cmp orig/GMSE01/sys/main.dol build/GMSE01/mario.dol` passed.
- No emulator gameplay test has been performed.

### Known limitations

The symbol validator reported existing incomplete UNUSED functions: `TProcessMeter::draw`, and `TTimeRec::suppleGXTime`, `drawSyncCallbackSt`, and `end`.
Those functions are absent from the retail executable, and their current stubs do not have the sizes recorded in the linker map.
The warnings remain unresolved and must not be treated as reconstructed code.

Three otherwise matching game objects changed the executable when linked from source and remain deferred: `Camera/CameraInbetween.cpp`, `GC2D/MessageUtil.cpp`, and `Map/PollutionEvent.cpp`.
Their per-object code/data comparison alone was insufficient to establish a full link match.
`Enemy/enemyinterp.cpp` is now recognized in configuration but still has no source file.
The repository's restrictions on autonomous runtime/middleware reconstruction remain applicable.
Japanese and PAL binary regression checks are unavailable because their inputs have not been supplied.

## Local artifacts and tools

- Source build manifest: `config/GMSE01/objects.json`.
- Metadata provenance and reproducible commands: [regional README](config/GMSE01/README.md).
- Work plan: [DECOMPILATION_PLAN.md](DECOMPILATION_PLAN.md).
- Initial game-code inventory: `docs/progress/GMSE01-backlog.json`.
- Full comparison reports: `build/GMSE01/baseline.json` and `report.json`.
- Build and regression logs: `build/GMSE01-build.log` and `build/GMSE01-changes-batch1.log`.
- Incremental promotion results and logs: `build/GMSE01/promotion-results.json` and `promotion-logs/`.
- `m2c`: `/home/netflix/m2c`, commit `e07f7e1c1d1d52f33ab3f7fedbe74b634404b943`, installed in `build/venv`.

Build artifacts and original game data are kept locally and excluded from Git.
Source, configuration, progress snapshots, and notes are versioned locally.

## Next work

Continue with small game-code mismatches, capture a fresh baseline for the next batch, and investigate the three deferred source-link candidates.
The largest unfinished game units in the initial inventory include the HUD, save/load screens, boss logic, and event scripting.
