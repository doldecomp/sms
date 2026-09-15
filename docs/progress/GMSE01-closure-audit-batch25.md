# Priority 1 closure audit — batch 25

Baseline: `6ed21f59`, following the user-revised `DECOMPILATION_PLAN.md`.
No object was promoted in this pass.
Only the airport source change was retained; other experimental edits were reverted.

## Complete-code candidates

| File | Evidence preventing closure | Next evidence needed |
| --- | --- | --- |
| `Camera/CameraInbetween` | All 1,672 linked code bytes match, but `.sdata2` ordering differs and includes extra rounding constants. Two UNUSED bodies remain four-byte stubs instead of 188 and 236 bytes. | Recover original inline/literal emission context. |
| `Map/PollutionEvent` | Runtime code and reported data match, but weak emission precedes `loadAfter`. Nine map definitions are missing and ten UNUSED bodies have incorrect sizes. Debug classes have placeholder declarations. | Recover class layouts and dead-code evidence. |
| `System/MarDirectorCreateObjects` | Existing 24-byte data discrepancy and batch 6 string-boundary investigation remain unresolved. | Verify boundaries against the DOL before another relink. |
| `System/TargetArrow` | Strings start eight bytes before imported symbol labels. The assigned end includes the next object's string. | Reconcile adjacent object boundaries; blanket address subtraction is insufficient. |

The imported map is useful evidence but was not extracted from this ISO.
The original DOL remains authoritative when map-derived boundaries disagree with actual referenced bytes.
For TargetArrow, the red model path begins at `0x803A92B8`, directory at `0x803A92E4`, and animation name at `0x803A92FC`.
No artificial padding or guessed dead bodies were introduced.

## Small single-function candidates inspected

Listed by remaining function size, following the plan.
Unlisted candidates were not resolved or cleared by this audit.

| File / function | Bytes | Finding and rejected trials |
| --- | ---: | --- |
| `MarioAccess::SMS_IsMarioOnWire` | 72 | Native code reloads Mario's holder before checking actor type. `getActorType()` does not restore the reload; `isTaken()` adds boolean normalization. Eleven UNUSED wrappers are missing. |
| `egggen::control` | 120 | Instructions agree except stack offsets: original frame `0x30`, current `0x18`. Map presence/order/linkage pass. |
| `HitActor::calcEntryRadius` | 124 | Reciprocal-square-root estimate and float spill differ in registers and frame (`0x40` versus `0x18`). `std::sqrtf` adds Newton refinement absent from the original; rejected. |
| `MSoundBGM::init` | 136 | Same instructions, original frame `0x48` versus `0x28`. Six UNUSED pause/mute/volume/check functions remain undersized. |
| `MapObjPollution::loadAfter` | 172 | Width/height and texture/interval argument scheduling, registers and frame differ. Dimension getters restore height-before-width evaluation but do not match the rest. Repeated `getLayer()` calls expand the frame without fixing registers. Reverted. Map check and three UNUSED sizes pass. |
| `MapObjAirport::watch` | 208 | Restored missing clear-sign sound calls. Now 99.96154%, with only camera flag stack location different (`0x34` versus original `0x3C`). Pool constructor and `appear` remain undersized UNUSED definitions. |
| `NpcInbetween::execPosInbetween` | 220 | Interpolation factor occupies native `f1`, current `f2`. Splitting/reversing reciprocal multiplication, const/local declaration changes and arithmetic regrouping do not fix it. `CLBLinearInbetween` introduces out-of-line calls; reverted. Map check passes, no UNUSED functions. |
| `SnapTimeObj::perform` | 228 | Timer wrapper frame differs by `0x10`; color spill/reload uses different registers. Existing `startTimer(u32)` contains a fabricated volatile temporary. No edit retained. |
| `livemanager::perform` | 252 | Instructions agree except frame and color stack offsets, also `0x10` apart. Investigate alongside SnapTimeObj. |
| `enemytable::getMatchedInfo` | 276 | Native code retains separate addresses of container begin/end fields across `rand`; current code retains the enclosing object. Missing UNUSED `TVector_pointer<TStageEnemyInfo*>` destructor is a protected middleware dependency. |
| `MapEventSirena::watch` | 280 | Named camera flag gives the original frame but places the passed flag eight bytes too low. Implicit conversion from zero does not fix it. Reverted; map check passes. |
| `MapObjFloat::initMapObj` | 288 | Same instructions and frame, different object/table/index registers. Moving index declaration has no effect; for-loop with explicit break regresses. Reverted. Map check passes, no UNUSED functions. |

## Shared fixes and exceptions

Airport uses existing `MSound::startSoundSystemSE` and `MSD_SE_SY_CLEAR_SIGN_BIG` (`0x484D`).
The wrapper emits the original `gateCheck`, conditional branch, and system-sound call.
Search `rg -n 'MSD_SE_SY_CLEAR_SIGN_BIG|startSoundSystemSE' src/MoveBG include/MSound` for existing callers.
Other map objects already use this wrapper; no evidence-supported mass replacement was found.

Removed airport's fabricated local `getMarDirector` and `getPollution` functions.
Existing `SMSGetMarDirector` and `gpPollution` access reproduce the original instruction sequence.
A named camera flag preserves the native frame but still differs in argument location.
The CLI's rounded `100.0%` display is not exact: the report records `99.96154%`.

Airport and Sirena share a flag-temporary issue but have different surrounding inline contexts.
Timer users share a separate header issue.
These are investigation groups, not blanket replacement rules.

## Validation and next pass

Full build, `ninja changes_all`, all-function presence/score comparison, DOL byte comparison and expected SHA-1 pass.
All 12,904 reported functions remain present, with zero regressions.
Airport map presence/order/linkage pass, with two pre-existing UNUSED size warnings.
No gameplay test was performed.

The refreshed queue excludes all five THPPlayer units despite their game-category metadata.
It contains 106 authorized game candidates with 1..2,000 unmatched bytes, totaling 96,868 unmatched bytes.
Coverage remains 24.792425% matched / 2.7192852% source-linked game code, and 38.422554% matched / 2.148763% source-linked aggregate code.

Continue Priority 1 after consulting these deferrals.
Next uninspected small candidates include `M3DUtil/M3UModel` and `Map/MapCollisionPlane`.
Revisit audited files when new inline, layout, or shared-caller evidence is available.
Do not repeat unchanged trials or treat unused stubs as completed source.
