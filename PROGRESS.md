# Local decompilation progress

## Target and status

Target: North American English, `GMSE01` revision 0, using the local ISO.
The full decompilation is **not complete**.
The local branch is `local/decomp-progress`.
The upstream starting commit is `ab00c3c9a466152f6e6bc5b9c28aca959d1a8454`.
Before related edits, consult the [shared-fix catalog](docs/MATCHING_CATALOG.md) and search for other callers.

## Latest checkpoint: batch 33 — two whole files completed

Completed and source-linked **DemoBossHanachanBase.cpp** and **SleepBossHanachan.cpp** from the small-file queue.
This adds **21 exactly matching functions/helpers / 2,440 code bytes / 1,496 data bytes**.
Recorded the successful accessor/local-variable patterns and their MapEventSirena exception in the shared-fix catalog.
MapModel, MarDirectorPreEntry and Sirena trials did not complete those files and were reverted.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.951435%** | **2.9231017%** |
| Aggregate | **38.548203%** | **2.3098176%** |

Source linking is now **78 game files / 83,240 code bytes**.
Baseline at `f7a677b8`; full build, changes_all, all 12,904 function comparisons, DOL byte comparison and expected SHA-1 pass.
Zero regressions; both completed files pass all code/data and map checks.
No gameplay test was performed.
The regenerated queue contains 102 authorized small game files / 92,340 unmatched bytes.
See the [batch 33 audit](docs/progress/GMSE01-closure-audit-batch33.md) and [measurements](docs/progress/GMSE01-batch33.json).
Continue the plan's file-completion order, consulting recorded blockers before retries.

## Verified checkpoint: batch 32 — completion queue and shared bounds check

Reread the plan and reaffirmed its file-completion order.
One shared bounds-check change improves sunmgr, lensglow and lensflare; sunmodel requires a direct-member exception.
**No file completed, no newly exact function, and no new source-linked bytes.**
Recorded CameraSecureView, sunmgr-load and egggen trials; revisited CameraInbetween and the deferred string-boundary evidence.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.86575%** | **2.8374174%** |
| Aggregate | **38.48049%** | **2.24211%** |

Source linking remains **76 game files / 80,800 code bytes**.
Baseline at `08c838ba`; full build, changes_all, all 12,904 function comparisons, DOL byte comparison and expected SHA-1 pass.
Zero similarity regressions; sunmgr and lensglow pass map checks.
sunmodel and lensflare retain map failures reproduced with the baseline sources; no gameplay test.
The regenerated queue remains 104 authorized small game files / 94,780 unmatched bytes.
See the [batch 32 audit](docs/progress/GMSE01-closure-audit-batch32.md) and [measurements](docs/progress/GMSE01-batch32.json).
Continue with file-closing hypotheses in the plan's order, consulting recorded blockers before retries.

## Verified checkpoint: batch 31 — MapCollisionEntry close to completion

Matched **three MapCollisionEntry functions / 608 code bytes** and restored its missing 116-byte UNUSED base constructor definition.
The file now passes every map check and has one nonmatching runtime function: its 168-byte initializer retains an eight-byte stack-frame difference.
**No new file was linked from source.**

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.86575%** | **2.8374174%** |
| Aggregate | **38.48049%** | **2.24211%** |

Source linking remains **76 game files / 80,800 code bytes**.
Baseline at `915877d3`; full build, changes_all, all 12,904 function checks, DOL byte comparison and expected SHA-1 pass.
Zero regressions across all header consumers; no gameplay test.
The regenerated small-file queue contains 104 authorized game files / 94,780 unmatched bytes.
See the [batch 31 audit](docs/progress/GMSE01-closure-audit-batch31.md) and [measurements](docs/progress/GMSE01-batch31.json) for retained changes, rejected trials and next files.

## Verified checkpoint: batch 30 — small-file closure audit

Reread the plan and continued its file-completion queue.
MessageLoader's parser now has the original instruction sequence and registers, improving from 99.64336% to 99.94405%; stack offsets and its constructor still differ.
**No new file was completed or linked from source in this checkpoint.**
Recorded blockers and rejected trials for MapXlu, PollutionPos, PerformList, BossHanachanNerve and MovieRumble to avoid repeating work.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.844398%** | **2.8374174%** |
| Aggregate | **38.463623%** | **2.24211%** |

Source linking remains **76 game files / 80,800 code bytes**.
Baseline at `f3694700`; full build, changes_all, all 12,904 function checks, DOL byte comparison and expected SHA-1 pass.
Zero regressions; MessageLoader passes its map check including both UNUSED sizes.
No gameplay test was performed.
The regenerated small-file queue remains 104 authorized game files / 95,388 unmatched bytes.
See the [batch 30 audit](docs/progress/GMSE01-closure-audit-batch30.md) and [measurements](docs/progress/GMSE01-batch30.json).
Continue Priority 1; require full-file verification before any source-link promotion.

## Verified checkpoint: batch 29 — HelpActor file completion

Completed and source-linked **GC2D/HelpActor.cpp**, adding **1,272 source-linked code bytes**.
The US help ID offset and named console locals restore its final two functions.
Also recovered AnimalManager's missing 128-byte UNUSED helper and matched CameraTalk's talk setup and Strategy's constructor.
These three files retain other differences and remain unlinked.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.844398%** | **2.8374174%** |
| Aggregate | **38.463623%** | **2.24211%** |

Source linking is now **76 game files / 80,800 code bytes**.
Baseline at `ad9a47f2`; full build, changes_all, all 12,904 function checks, DOL byte comparison and expected SHA-1 pass.
Zero regressions; four newly exact runtime functions / 916 code bytes.
All four changed source files pass map checks; no gameplay test was performed.
The refreshed small-file completion queue contains 104 authorized game files / 95,388 unmatched bytes.
See the [batch 29 audit](docs/progress/GMSE01-closure-audit-batch29.md) and [measurements](docs/progress/GMSE01-batch29.json) before related trials.

## Verified checkpoint: batch 28 — US selector data and Shimmer definitions

Restored ProgSelect's five original US English strings and text dimensions: **312 newly matched data bytes**, with all 328 unit data bytes now exact.
Removed its pre-existing padding array; the remaining draw-function differences are stack offsets.
Its similarity decreases from 99.85859% to 99.79798% because the artificial frame padding is gone.
No exact code or source-linked coverage was lost.

Recovered Shimmer's two UNUSED near/far bodies with correct 24/20-byte map sizes, corrected three rendering calls, and restored relative matrix-local order.
Both changed files pass their map checks without warnings.
**No new source-linked file is claimed.**

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.812231%** | **2.7927492%** |
| Aggregate | **38.438206%** | **2.2068136%** |

Source linking remains **75 game objects / 79,528 code bytes**.
Baseline at `6bb39882`; full build, changes_all, all 12,904 function checks, DOL byte comparison and expected SHA-1 pass.
All previously exact functions remain exact; one explained nonmatching-function score regression, zero unexplained regressions.
No gameplay test was performed.
See the [batch 28 audit](docs/progress/GMSE01-closure-audit-batch28.md) and [measurements](docs/progress/GMSE01-batch28.json) before further trials.
Continue the Priority 1 file-completion queue.

## Verified checkpoint: batch 27 — PollutionObj file completion

Completed and source-linked **`Map/PollutionObj.cpp`**, adding **2,092 source-linked code bytes**.
Its final 564-byte function now matches exactly after restoring the original local-variable and inline-call structure.
Removed its pre-existing no-op expression; no stack padding was added.
EffectUtil improves to 98.7541% but remains deferred with a missing UNUSED definition.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.812231%** | **2.7927492%** |
| Aggregate | **38.438206%** | **2.2068136%** |

Source linking is now **75 game objects / 79,528 code bytes**.
Baseline at `68fb613c`; full build, changes_all, comparison of all 12,904 functions, DOL byte comparison and expected SHA-1 pass.
Zero regressions; PollutionObj passes all map checks including both UNUSED sizes.
No gameplay test was performed.
The refreshed completion queue has 105 authorized small files / 96,304 unmatched bytes.
Continue file completion in the plan's order; consult the [batch 27 audit](docs/progress/GMSE01-closure-audit-batch27.md) and [measurements](docs/progress/GMSE01-batch27.json).

## Verified checkpoint: batch 26 — restore missing file definitions

Recovered both missing NPC color helpers from repeated call sites.
Their UNUSED map sizes are **48 and 80 bytes**, and the file now passes symbol presence/order/linkage checks without warnings.
The runtime caller still has an eight-byte frame difference.
Water-filter `perform` improves from **78.38614% to 98.68317%** through existing camera helpers, correct pointer reuse, and relative matrix-local order.
A frame gap and one branch remain unresolved.
**No new source-linked file or exact runtime code is claimed.**

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.792425%** | **2.7192852%** |
| Aggregate | **38.422554%** | **2.148763%** |

Source linking remains **74 game objects / 77,436 code bytes**.
Baseline at `4d0e4561`; full build, changes_all, comparison of all 12,904 functions, DOL byte comparison and expected SHA-1 pass.
Zero regressions; both changed files pass map checks without warnings.
No gameplay test was performed.
See the [batch 26 closure audit](docs/progress/GMSE01-closure-audit-batch26.md) for retained evidence, rejected trials and next candidates, and [measurements](docs/progress/GMSE01-batch26.json).

## Verified checkpoint: batch 25 — file-closure audit

Reread the revised plan and audited the Priority 1 completion queue.
**No additional file reached source-link completion**; coverage remains **74 game objects / 77,436 code bytes**.
Restored the airport clear-sign sound through the existing wrapper, improving its 208-byte `watch` function from 79.88461% to 99.96154%.
Its camera flag stack location and two UNUSED pool definitions still prevent a completion claim.
No new exact code bytes or functions are claimed.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | 706,004 / 2,847,660 (**24.792425%**) | 77,436 / 2,847,660 (**2.7192852%**) |
| Aggregate | 1,384,652 / 3,603,748 (**38.422554%**) | 77,436 / 3,603,748 (**2.148763%**) |

Baseline at `6ed21f59`; full build, `ninja changes_all`, comparison of all 12,904 functions, DOL byte comparison and expected SHA-1 pass.
Zero function regressions.
Airport map presence/order/linkage pass with two pre-existing UNUSED size warnings.
No gameplay test was performed.

The [closure audit](docs/progress/GMSE01-closure-audit-batch25.md) records missing definitions, frame/register issues, shared investigation groups and rejected trials.
The [completion queue](docs/progress/GMSE01-completion-queue.json) now excludes protected THPPlayer objects: **106 authorized small game candidates / 96,868 unmatched bytes**.
Continue the smallest remaining file closures; next uninspected candidates include `M3DUtil/M3UModel` and `Map/MapCollisionPlane`.
See [batch 25 measurements](docs/progress/GMSE01-batch25.json).

## Verified checkpoint: batch 24 — Priority 1 file completion

Completed source-link promotion of **`GC2D/MessageUtil.cpp`**, closing its batch 2 deferral.
The inherited stream availability API avoids two unwanted emitted getters; both message functions still match all **968 code bytes**.
Removed the parser's two pre-existing stack-padding arrays using the stream position accessor, a signed 32-bit block tag, and a separate 16-bit entry-size variable.
There are now **74 source-linked game objects / 77,436 code bytes**.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | 706,004 / 2,847,660 (**24.792425%**) | 77,436 / 2,847,660 (**2.7192852%**) |
| Aggregate | 1,384,652 / 3,603,748 (**38.422554%**) | 77,436 / 3,603,748 (**2.148763%**) |

Aggregate exact code and function counts are unchanged: this checkpoint moves an already matching file into the source link.

### Validation and next completion targets

Saved baseline at `5d1b2705`; full build, `ninja changes_all`, all-function presence/score comparison, direct executable byte comparison, and expected SHA-1 checks pass.
Zero function regressions; the message unit's symbol-map check passes without warnings.
No gameplay test was performed.

Follow the user's revised priorities in [DECOMPILATION_PLAN.md](DECOMPILATION_PLAN.md): close nearly complete game objects first, then finish the existing boss objects, then resume larger reconstructions.
The next complete-code candidates are camera interpolation, pollution events, director object creation, and target arrow.
Record unresolved evidence and move down the completion queue when a candidate stalls.
The [refreshed completion queue](docs/progress/GMSE01-completion-queue.json) contains 108 unlinked game files with 1..2,000 unmatched code bytes, totaling 98,036 unmatched bytes; these are candidates pending full validation.
Library promotion and edits remain outside the authorized queue.
See [batch 24 measurements](docs/progress/GMSE01-batch24.json) and the [matching catalog](docs/MATCHING_CATALOG.md).

## Verified checkpoint: batch 23

Applied an evidence-supported horizontal vector calculation to the boss and NPC walking predicates and the boss's path-transition check.
Boss walking improves from **76.784% to 95.624%**, and NPC walking improves from **89.435486% to 92.30645%**.
Corrected the NPC's original 10-unit horizontal threshold and absolute yaw difference, and restored an integer-word direction copy.
Both UNUSED walking predicates now have their original 192-byte map size.
The existing `MsAngleWrap` helper restores one original call in boss `perform` and emits **72 exactly matching code bytes / one function**.
The main update improves to **78.874916%**.

Aggregate exact code is **1,384,652 / 3,603,748 bytes (38.422554%)**, with **8,222 / 12,904 functions** matching.
Matched data increased by 56 bytes.
Source linking remains **73 objects / 76,468 code bytes (2.121902%)**.
Both changed objects remain linked from the original binary.

### Validation and remaining work

Used the saved `ninja baseline` at `211b726c`, rebuilt, and ran `ninja changes_all`.
The comparison of all reported functions, including missing-function detection, found zero regressions.
Full build, direct DOL byte comparison, and expected SHA-1 checks pass for the mixed source/original-object executable.
Both changed units still fail the map check for one missing emitted `TVec3::set<float>` helper; their function order, linkage, and all UNUSED sizes pass.
No gameplay test was performed.

Next work: recover the shared walking setter-call context, then address the boss main update's remaining maximum-roll calls, three signed angle-wrap calls, history-loop unrolling, and arithmetic differences.
The sphere-link angle-wrap site is a verified exception to the main-update wrapper correction.
Keep the current best-evidenced history loop; trials that merely raised similarity while adding non-native pointer reloads were reverted.
See [batch 23 measurements](docs/progress/GMSE01-batch23.json) and the [matching catalog](docs/MATCHING_CATALOG.md) for successful patterns, exceptions, and rejected trials.
Validation logs and full instruction diffs are saved under `build/GMSE01-*-batch23.*`.

## Verified checkpoint: batch 22

Reconstructed the boss's full 6,108-byte main update/render routine from its original instructions and m2c draft.
It covers body motion, terrain and sand response, collision handling, animation blending, targeting, shadows, and rendering, and currently has **78.2685% instruction similarity**.
All strong functions in `BossHanachanMain.cpp` now have reconstructed definitions.
The boss destructor and adjustment thunk match exactly, adding **116 exact code bytes and two functions**.
Its full virtual table also matches; aggregate matched data increased by **1,144 bytes**.

Aggregate exact code is **1,384,580 / 3,603,748 bytes (38.420555%)**, with **8,221 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The boss main object remains linked from the original binary.

### Validation and remaining work

Captured `ninja baseline` at `8176629d`, rebuilt, and ran `ninja changes_all`.
The comparison of all reported functions, including missing-function detection, found zero regressions.
Full build, byte comparison, and expected SHA-1 checks pass for the mixed source/original-object executable.
The main symbol-map check has two missing emitted helpers: `MsWrap<float>` and `TVec3::set<float>`.
All strong functions have correct order and linkage; the UNUSED walking predicate retains its size warning.
No gameplay test was performed.

Next work: resolve the main routine's helper inlining, loop unrolling, and floating-point expression differences, then finish matching the remaining boss units and verify source linking.
The two maximum-roll calls and four angle-wrap calls currently inline, while the original keeps calls; the slipping routine correctly needs the maximum-roll helper inline.
These differences need caller-specific investigation before shared changes.
See [batch 22 measurements](docs/progress/GMSE01-batch22.json) and the [matching catalog](docs/MATCHING_CATALOG.md) for verified patterns and exceptions.
The current instruction diff is `build/GMSE01-boss-perform-diff-batch22.txt`; validation logs are `build/GMSE01-*-batch22.*`.

## Verified checkpoint: batch 21

Reconstructed boss initialization, Mario throwing, the local rotation-position helper, and both UNUSED animation helpers.
The three newly reconstructed linked routines cover **2,456 original code bytes**: initialization is **99.07692%**, throwing **96.03125%**, and rotation-position adjustment **95.57692%**.
The animation helpers reproduce their original map sizes of 280 and 368 bytes and inline into initialization with the expected calls.
Corrected the body's position and roll history declarations and both existing callers together; the body constructor remains exactly matching.
A shared trig lookup ordering correction also improved slipping to **99.89655%**.

This batch adds no new exact functions or source-linked objects.
Aggregate exact code remains **1,384,464 / 3,603,748 bytes (38.417336%)**, with **8,219 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.

### Validation and remaining work

Captured `ninja baseline` at `ded6891c`, rebuilt all affected consumers, and ran `ninja changes_all`.
The all-function comparison, including missing-function detection, found zero regressions.
Full build, byte comparison, and expected SHA-1 checks pass for the mixed source/original-object executable.
The parts symbol-map check passes with its prior UNUSED size warning.
The main map check now has five missing symbols: `perform`, emitted `MsWrap<float>` and `TVec3::set<float>`, and the boss destructor/thunk.
Existing strong definitions have correct order and linkage; its weak-order and walking-predicate size warnings remain.
No gameplay test was performed.

Next work: reconstruct the 6,108-byte `perform` routine, using the corrected position/roll histories and the recovered animation helpers.
Remaining instruction differences include temporary stack locations, parameter loads, throwing-ratio evaluation, and the rotation helper's final arithmetic scheduling.
The initializer's resource string offsets also depend on strings in the missing main routine.
See [batch 21 measurements](docs/progress/GMSE01-batch21.json) and the [shared-fix catalog](docs/MATCHING_CATALOG.md).
The original full draft remains `build/GMSE01/BossHanachanMain-batch20.c`; new validation logs are `build/GMSE01-*-batch21.*`.

## Verified checkpoint: batch 20

Reconstructed 21 linked functions and the UNUSED walking predicate in the previously empty `BossHanachanMain.cpp`.
The new code covers manager/model setup, walking, slipping, damage transitions, collision binding, recovery-path selection, body-roll checks, and head movement.
Recovered the manager class and three stored position/velocity vectors from their original reads and writes.
Fourteen functions match exactly, adding **2,364 exact code bytes** and **220 matched data bytes**.
Slipping is **99.63793%**, collision binding is **99.55385%**, and damage handling is **96.304344%**.

Aggregate exact code is **1,384,464 / 3,603,748 bytes (38.417336%)**, with **8,219 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The boss main object remains linked from the original binary.

### Validation and remaining work

Captured `ninja baseline` at `3afc3252`, rebuilt all affected header consumers, and ran `ninja changes_all`.
The comparison of all reported functions, including missing-function detection, found zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

The main unit's map check still fails with ten missing symbols: initialization, Mario throwing, main update/render handling, two UNUSED animation helpers, the local rotation-position helper, two compiler-emitted helpers, and the boss destructor/thunk.
All currently present strong functions have the correct order and linkage.
The manager destructor has a weak-order warning, and UNUSED `isCanWalk` is 164 bytes versus the map's 192.
Walking remains 76.784% because vector construction, squared-length evaluation, and inline context differ.
Stack/register differences remain in other reconstructed routines; no artificial padding or middleware edits were introduced.

Next work: reconstruct `CalcRevisionPosByRotateZ`, `init`, `throwMario_`, and the two animation helpers, then the 6,108-byte `perform` routine.
The [shared-fix catalog](docs/MATCHING_CATALOG.md) records the reusable vector, loop, and boolean patterns and the rejected trials.
See [batch 20 measurements](docs/progress/GMSE01-batch20.json).
The map inventory, full draft, instruction overview, and validation logs are under `build/GMSE01-boss-main-map-inventory-batch20.txt`, `build/GMSE01/BossHanachanMain-batch20.c`, and `build/GMSE01-*-batch20.*`.

## Verified checkpoint: batch 19

Reconstructed the missing US `MSound::getDistPowFromCamera(const Vec&)` using the original three `powf` calls and evaluation order.
All **136 bytes match exactly**.
Corrected sequence callback commands 40 and 123–126 to fall through to the base audio callback, and case 110 to return 0xFFFF for scene 8 in episodes 6 or 1.
The complete US switch routing now matches the original; the callback improved from 95.40247% to **98.75232%**.
Other regions retain their existing behavior through version guards.

This batch adds **136 exact code bytes and one matching function**.
Aggregate exact code is **1,382,100 / 3,603,748 bytes (38.351738%)**, with **8,205 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.

### Validation and remaining work

Captured `ninja baseline` at `19b7f21f`, rebuilt header consumers, ran `ninja changes_all`, and compared all reported functions including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
The sound unit's symbol-map check now passes: all 67 mapped functions, including nine UNUSED entries, are present.
It retains compiler-controlled weak-order warnings and seven existing UNUSED size warnings, including stubbed routines; the unit is not complete or source-linked.
No gameplay test was performed.

Remaining callback differences concern register allocation, a child-track pointer move, and a 0x58 versus 0x88 stack frame.
Direct child-array access and splitting the local declaration/assignment did not resolve the mismatch and were reverted.
The camera-distance helper needs global `powf`; the existing `std::powf` wrappers delayed summation and added spills.
The neighboring animation-distance helper has evidence for different scheduling and was left unchanged.
The [shared-fix catalog](docs/MATCHING_CATALOG.md) records these patterns and exceptions.
Next work: continue the missing boss main-unit reconstruction, with the sound callback's inline context and UNUSED routines retained in the backlog.
See [batch 19 measurements](docs/progress/GMSE01-batch19.json).
Draft and validation logs are under `build/GMSE01/MSound-distance-batch19.c` and `build/GMSE01-*-batch19.*`.

## Verified checkpoint: batch 18

Corrected the shared US `MSound` layout using the original constructor, water-filter routine, timer routine, and camera/boss callers.
The game-side class now declares the water-filter override byte at 0x98 and timer sequence parameter at 0x9A.
This places the tempo controller at 0x9C, crossfade controller at 0xA0, and later sound fields at their observed US offsets.
Restored the filter override check and its constructor initialization order, and updated all seven timer writes plus the sequence callback read together.
Other regions retain their previous layout and accesses through version guards.

**Forty-one functions improved across seven units; fifteen now match exactly, adding 2,060 exact code bytes.**
The exact gains include camera/player sound setup, stage sound entry, sound gating, reset, water filtering, and both inner-camera demo setters.
Aggregate exact code is **1,381,964 / 3,603,748 bytes (38.347965%)**, with **8,204 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.

### Validation and remaining work

Captured `ninja baseline` at `399978d6`, rebuilt affected consumers, ran `ninja changes_all`, and compared all reported functions including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
The boss nerve unit passes its symbol-map check.
The sound unit retains its baseline map failure: missing `getDistPowFromCamera`, weak-symbol ordering warnings, and seven UNUSED size warnings.
Its complete map-check output is unchanged from the baseline.
No source-link promotion or gameplay test was performed.

The constructor still lacks the previous-voice-ID word initialization at 0x94; properly recovering that field requires resolving ownership of the existing `JAIBasic` tail under the middleware supervision constraint.
`playTimer` is 99.881355% and boss Snort is 99.9186%; their field offsets now agree with the original, leaving stack-layout differences.
The sound callback also has remaining US behavior differences in cases 40 and 110, recorded in the catalog for the next focused pass.
Next work: reconstruct the missing game-side sound distance helper and callback differences, then continue the boss main-unit reconstruction and remaining nerve stack differences.
See [batch 18 measurements](docs/progress/GMSE01-batch18.json) and the [shared-fix catalog](docs/MATCHING_CATALOG.md).
Validation logs are `build/GMSE01-*-batch18.*`.

## Verified checkpoint: batch 17

Reconstructed all seven boss behavior states in the previously empty `BossHanachanNerve.cpp`: graph wandering, tumbling, down, get-up, damage, snort, and death.
The existing nerve macro supplies the singleton accessors; all 22 mapped functions, including destructors and initialization, are present in the correct order and linkage.
Twenty functions match exactly, along with all 444 bytes of data sections.

This batch adds **3,156 exact code bytes, twenty matching functions, and 444 matched data bytes**.
Aggregate exact code is **1,379,904 / 3,603,748 bytes (38.290802%)**, with **8,189 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The nerve object remains original-linked pending its two nonmatching functions.

### Validation and remaining work

Captured `ninja baseline` at `e3d8ae6b`, ran the full build and `ninja changes_all`, and compared every reported function with missing-function detection: zero regressions.
The nerve unit passes symbol presence, order, and linkage checks with no map warnings.
Expected SHA-1 and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

`TNerveBossHanachanTumble::execute` is 99.901405%, differing only in its 0x38 versus 0x40 stack frame.
`TNerveBossHanachanSnort::execute` is 99.89535%, with a 0x28 versus 0x40 frame and a shared sound-layout discrepancy.
The original snort routine and `MSound` constructor agree that the tempo controller is at 0x9C; the current header's `MSModBgm* unk98` is at 0x98.
The constructor also writes a byte at 0x98 and a word at 0x94, which must be considered during a dedicated shared-layout audit.
No raw-offset cast, arbitrary padding, or protected middleware change was introduced to hide that discrepancy.

The [shared-fix catalog](docs/MATCHING_CATALOG.md) records the transition rules, exact helper pattern, sound evidence, and reverted stack-layout trials.
Next work: audit the US sound layout and its game-code consumers, resolve the two nerve differences, and continue the boss main-unit reconstruction.
See [batch 17 measurements](docs/progress/GMSE01-batch17.json).
Draft and trial source are under `build/GMSE01/BossHanachanNerve-batch17*`; validation logs are `build/GMSE01-*-batch17.*`.

## Verified checkpoint: batch 16

Reconstructed the previously empty `BossHanachanEffect.cpp`: particle loading, state-dependent particle emission, sand-pillar animation and sound, and camera shake/rumble.
All four routines and compiler-generated initialization are present in the correct map order and linkage.
Particle loading (820 bytes) and static initialization (764 bytes) match exactly, along with all 844 bytes of the unit's data sections.

Recovered the body's two-element leg-matrix array and the owner's sand-pillar model pointer and position vector from original field accesses.
Moved the foot-hit actor declaration into the shared boss header because both parts and effects use its joint matrix.
The two body-constructor matrix lookups were updated together and remain exact.

This batch adds **1,584 exact code bytes, two matching functions, and 844 matched data bytes**.
Aggregate exact code is **1,376,748 / 3,603,748 bytes (38.203228%)**, with **8,169 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The effects object remains original-linked pending three nonmatching routines.

### Validation and remaining work

Used the saved `ninja baseline` at `d78586d8`, rebuilt affected header consumers, ran `ninja changes_all`, and compared all reported functions with missing-function detection: zero regressions.
Effects and parts pass symbol presence, order, and linkage checks.
Parts retains its previously recorded UNUSED hit-predicate size warning (208 versus 196 bytes).
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

`emitOneTimeSandPillar_` is 99.93507% (only an eight-byte stack-size difference).
`emitCamShake_` is 99.70303% (stack layout and two loop-counter registers).
`emitParticle_` is 98.038315% (stack/register allocation and a water-counter sign-extension comparison).
The existing `MsSqrtf` and `MsRandF` helpers reproduce the original arithmetic and probability-load order; direct random arithmetic did not.
Remaining differences and reverted trials are recorded in the [shared-fix catalog](docs/MATCHING_CATALOG.md).
Next work: resolve effects inline contexts where evidence supports them, then continue boss main/nerve reconstruction and the pending sub-unit collision helper.
See [batch 16 measurements](docs/progress/GMSE01-batch16.json).
The draft and trial backups are under `build/GMSE01/BossHanachanEffect-batch16*`; build, regression, and map logs are `build/GMSE01-*-batch16.*`.

## Verified checkpoint: batch 15

Reconstructed the previously empty `BossHanachanSub.cpp`: sphere-chain initialization and movement, wall/ground collision, rotation-dependent position corrections, centrifugal force, and water-hit handling.
Recovered `TSpherePoint` (0x2C bytes) and `TSphereLink` (0x1C bytes), with declarations in `BossHanachanSub.hpp`.
Corrected the owner's offset-0x178 field to a sphere-link pointer using its original call sites.
All seven retained strong functions and the map's UNUSED collision helper have bodies.

Five linked functions match exactly: the water-actor destructor (132 bytes), its thunk (8), the sphere-point constructor (4), water-message handling (112), and counter reset (12).
All 232 bytes of the unit's data sections match.
This batch adds **268 exact code bytes, five matching functions, and 232 matched data bytes**.
Aggregate exact code is **1,375,164 / 3,603,748 bytes (38.15927%)**, with **8,167 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The sub-unit remains original-linked pending its remaining differences.

### Validation and remaining work

Captured `ninja baseline` at `6e2992e9`, rebuilt affected header consumers, ran `ninja changes_all`, and compared all reported functions with missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

**The sub-unit's symbol-map check still fails one presence check:** `TBGCheckData::isIllegalData()` is defined in the existing header but is inlined where the original calls and emits it.
Function order, linkage, and the UNUSED collision helper's 188-byte size pass.
Do not promote this object while that emission issue or its five nonmatching routines remain.
Those routines are `setDegreeZAndRevisionPosXZ` (98.19259%), `moveHead` (85.69796%), the sphere-link constructor (99.53333%), `BHSCalcRevisionDistXZByRotateZ` (94.02857%), and `BHSCalcCentrifugalForce` (99.023254%).

The catalog records recovered field semantics, angle rounding versus truncation, signed angle-difference behavior, matching helper patterns, and reverted trials.
Next work: resolve the sub-unit's collision/vector inline context and rotation-helper stack layouts, then continue the boss main/nerve reconstruction and pending source-link investigations.
See [batch 15 measurements](docs/progress/GMSE01-batch15.json) and the [shared-fix catalog](docs/MATCHING_CATALOG.md).
The `m2c` draft is `build/GMSE01/BossHanachanSub-batch15.c`; build, regression, and map logs are `build/GMSE01-*-batch15.log`.

## Verified checkpoint: batch 14

Reconstructed the last two TODO routines in `BossHanachanParts.cpp`: head/body map-collision setup and the two foot collision actors.
The code recovers the original actor names, joint names, collision dimensions, actor-group registration, collision flags, matrix bindings, and initial positions.
`initMapCollisionAndHitActor_` is **99.76471%** (544 bytes); only stack offsets differ.
`initFootHitActor_` is **99.55238%** (420 bytes), with stack offsets and two register assignments still different.
The foot destructor (156 bytes) and its pointer-adjustment thunk (8 bytes) now match exactly.
All mapped functions in the parts unit, including UNUSED functions, are present for the first time.

Audited the water-hit payload across the boss and particle users.
The binary uses offset 0x68 as a signed 16-bit countdown in boss receivers and a 32-bit particle index in senders.
The shared class now represents those views with a union, and all existing particle-index uses were renamed together.
The named constructor initializes the receiver counter; the default constructor preserves the static sender's original lack of a payload store.
This is an evidence-based representation of the binary's accesses, not proof of the original header's exact spelling.

This batch adds **164 exact code bytes, two matching functions, and 800 matched data bytes**.
Aggregate exact code is **1,374,896 / 3,603,748 bytes (38.151836%)**, with **8,162 / 12,904 functions** matching.
Matched data is 300,595 bytes.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The parts unit remains original-linked until all remaining instruction/data differences are resolved.

### Validation and remaining work

Captured `ninja baseline` at `918bf10e`, rebuilt the shared header's consumers, ran `ninja changes_all`, and compared all reported functions, including missing-function detection: zero regressions.
Existing particle-manager and enemy function scores are unchanged.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

The parts unit passes map presence, order, and linkage checks; its only map warning is the existing 208-versus-196-byte UNUSED hit predicate.
The new strings and joint-name array also complete the unit's `.rodata`, `.sdata`, and `.sdata2` section matches.
The `.data` section still differs.

Next work: reconstruct `BossHanachanSub.cpp` using the audited counter field, and resolve the parts/owner animation stack and register differences before source-link promotion.
The sub-unit map inventory and both initialization drafts were inspected; the draft files remain in `build/GMSE01/BossHanachan{Foot,Collision}-batch14.c`.
See [batch 14 measurements](docs/progress/GMSE01-batch14.json) and the [shared-fix catalog](docs/MATCHING_CATALOG.md).
Build, change, and map logs are saved under `build/GMSE01-*-batch14.log`.

## Verified checkpoint: batch 13

Matched `isAllBckAlreadyEnd` (184 bytes) and `setTumbleAnm` (420 bytes) exactly.
The completion check compares each compound condition explicitly with `false`, preserving the original boolean normalization without the extra stack slots of named boolean locals.
The tumble helper reuses one float and expresses absolute value directly with a ternary; both inline sites now match the original 0x60-byte stack frame and instruction sequence.

This adds **604 exact code bytes and two matching functions**.
Aggregate exact code is **1,374,732 / 3,603,748 bytes (38.147285%)**, with **8,160 / 12,904 functions** matching.
Matched data remains 299,795 bytes.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
Eight of the animation unit's ten linked routines now match; the unit remains original-linked pending the other two.

### Validation and remaining work

Used the saved `ninja baseline` at `75133a1f`, ran `ninja changes_all`, and compared every reported function, including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.
The animation unit passes map presence, order, and linkage checks.
The UNUSED tumble helper is 180 bytes versus the map's 176; its two retained inline sites are exact, but the standalone size remains unresolved.

The analogous boolean-expression change in the parts dispatcher regressed inlining and was reverted.
Frame-conversion trials in `setHeadAndBodyAnm` did not establish a match and were reverted too.
The shared-fix catalog records these exceptions to prevent repeating them or applying the successful forms blindly elsewhere.

Remaining owner routines are `changeAnmRateAndFrameUpdate_` (99.89781%, stack/register differences) and `setHeadAndBodyAnm` (91.75%, conversion scheduling/register differences).
Resolve these before testing source-link promotion of the animation unit.
The existing collision/foot initialization and water-hit field discrepancy remain outstanding in the parts/sub units.
See [batch 13 measurements](docs/progress/GMSE01-batch13.json) and the [shared-fix catalog](docs/MATCHING_CATALOG.md).
Logs and trial backups are saved under `build/GMSE01` and `build/GMSE01-*-batch13.log`.

## Verified checkpoint: batch 12

Reconstructed `BossHanachanAnm.cpp`: all ten linked routines and all four UNUSED helpers from the map.
The routines coordinate walk/run blending, frame preservation, directional tumble rates, animation completion, and staggered head/body transitions.
The recovered parameter fields are reused throughout.

Six routines now match exactly: `considerSetAnm`, `isFinishedGetUp`, and the four `setAnmTimerWhen*` methods.
They add **1,108 exact code bytes** and six matching functions.
All 32 bytes of the unit's mapped constants match.
Four routines remain nonmatching: `changeAnmRateAndFrameUpdate_` (99.89781%), `isAllBckAlreadyEnd` (99.891304%), `setTumbleAnm` (99.85714%), and `setHeadAndBodyAnm` (91.75%).
Their remaining differences are documented in the shared-fix catalog.

The owner's `setHeadAndBodyAnm` caller tests the low byte of the virtual setter's result, providing new evidence that `TBossHanachanPartsBase::setAnm_` returns `bool`.
Corrected the base declaration and both head/body overrides together, including their result locals.
Existing parts function match scores did not regress.

Aggregate exact code is **1,374,128 / 3,603,748 bytes (38.130524%)**, with **8,158 / 12,904 functions** matching.
Matched data is 299,795 bytes.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The animation and parts units remain linked from the original executable until their remaining differences are resolved.

### Validation and remaining work

Captured `ninja baseline` at `50f5cffc`, rebuilt affected consumers, ran `ninja changes_all`, and compared all functions, including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

The animation unit passes map presence, order, and linkage checks.
Its three forwarding/blend helpers have the recorded 100-, 112-, and 116-byte sizes; `setTumbleBckRate_` is still 172 rather than 176 bytes.
The parameter and sound objects pass all map checks.
The parts unit retains the missing foot destructor/thunk and its hit-predicate UNUSED-size warning.

Next work: resolve the owner animation routines' remaining stack/register differences and the tumble helper's size, then test source-linking the whole animation unit.
The frame-update routine's short stack suggests missing inline context; the map-visible wrappers are present, but their current direct blend-field accesses may omit a nested helper boundary.
Do not add artificial stack padding.
Collision/foot initialization and the water-hit field discrepancy remain outstanding in the parts/sub units.
See [batch 12 measurements](docs/progress/GMSE01-batch12.json) and the [shared-fix catalog](docs/MATCHING_CATALOG.md).
Logs and the full `m2c` draft are saved under `build/GMSE01` and `build/GMSE01-*-batch12*.log`.

## Verified checkpoint: batch 11

Reconstructed all six branches of the boss-part animation dispatcher, including Mario standing on a body segment, animation completion, directional get-up sequences, and delayed damage/snort/death transitions.
`considerSetAnm_` improved from a stub (0.21691974%) to **99.6833%**, reproducing the original 461-instruction length (1,844 bytes).
Remaining differences are the stack frame (0xD0 versus 0x110), register allocation, and one constant-load/move choice in the blending check.
The existing animation-completion and motion-blending helpers are reused.

Reconstructed `isMarioOn_` from the original dispatcher inline site and corrected its result to `bool`.
Its inline instructions match and its emitted UNUSED body now has the mapped 100-byte size.
Added the missing enum values; descriptive animation-state names remain provisional.

This batch adds no fully exact linked functions or source-linked objects.
Aggregate exact code remains **1,373,020 / 3,603,748 bytes (38.099777%)**, with **8,152 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The unfinished parts object remains linked from the original executable.

### Validation and remaining work

Captured `ninja baseline` at `90cba87f`, rebuilt affected consumers, ran `ninja changes_all`, and compared all functions, including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass.
These executable checks cover the mixed source/original-object build; the dispatcher is not yet used by that executable.
The parameter and sound objects pass all map checks.
The parts object's order and linkage pass; the foot destructor/thunk are still missing, and the hit predicate has the sole remaining UNUSED-size warning (208 versus 196 bytes).
No gameplay test was performed.

Shared accessor/local-variable trials did not resolve the stack differences and were reverted.
Their outcomes and the successful blending-expression form are recorded in the [shared-fix catalog](docs/MATCHING_CATALOG.md).
Next work: reconstruct collision/foot initialization after resolving the water-hit field discrepancy, then continue exact matching and the owner's animation routines.
The owner animation unit's complete map inventory is at `marioUS.MAP:59489`; it remains empty and has linked and UNUSED wrapper functions suitable for grouped reconstruction.
See [batch 11 measurements](docs/progress/GMSE01-batch11.json).
Logs and the dispatcher draft are saved under `build/GMSE01` and `build/GMSE01-*-batch11*.log`.

## Verified checkpoint: batch 10

Completed `BossHanachanSave.cpp` and linked it from source after verifying the full executable.
Both parameter constructors match exactly: 1,608 bytes for the changeable parameters and 1,408 bytes for common parameters.
Recovered all 63 parameter names, defaults, types, and field offsets from native strings, constructor stores, and parameter virtual-table references.
Reused the existing `System/DummyStrings.hpp` for the leading shared literals; this fixed the remaining differences in both constructors together.

Completed the previously missing base-part constructor logic using the recovered parameters and existing model/motion helpers.
It now reproduces the original instruction sequence apart from an eight-byte stack-frame difference (99.90598%).
Correcting the shared joint-matrix helper to hold a `u16` index made both head/body constructors exact (192 and 260 bytes) and restored the helper's mapped 76-byte size.
The existing `CLBPalFrame<short>` template is now emitted and matches all 92 bytes.

This batch adds **3,560 exact code bytes and five matching functions/helpers**.
Aggregate exact code is **1,373,020 / 3,603,748 bytes (38.099777%)**, with **8,152 / 12,904 functions** matching.
Matched data is 299,763 bytes.
There are now **73 source-linked objects**, covering **76,468 code bytes (2.121902%)** and 51,024 data bytes.
The unfinished parts object remains linked from the original executable.

### Validation and remaining work

Captured `ninja baseline` at `58f3e2d0`, ran `ninja changes_all`, and compared all functions, including missing-function detection: zero regressions.
The complete executable passes its expected SHA-1 and byte comparison after linking the parameter object from source.
The parameter and sound objects pass map presence, order, and linkage checks.
The parts object passes order and linkage but still lacks the emitted foot destructor/thunk; its two remaining UNUSED-size warnings concern the hit predicate and `isMarioOn_`.
No gameplay test was performed.

Remaining parts work includes collision/foot initialization, the animation-state dispatcher, `isMarioOn_`, and exact matching of the partial routines.
The water-hit counter/particle-index layout discrepancy recorded in batch 9 must be resolved before adding a shared water-hit constructor.
The base constructor now contains model and blend setup, but the parts unit is not yet a runnable replacement as a whole.
See [batch 10 measurements](docs/progress/GMSE01-batch10.json) and the [shared-fix catalog](docs/MATCHING_CATALOG.md).
Logs and drafts are saved under `build/GMSE01` and `build/GMSE01-*-batch10*.log`.

## Verified checkpoint: batch 9

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
