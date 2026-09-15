# Priority 1 closure audit — batch 32

Baseline: `08c838ba`, local GMSE01 revision 0 configuration.
Reread the plan after the user's renewed instruction to prioritize file completion.
No file completed, no newly exact function, and no additional source-linked bytes in this checkpoint.

## Retained shared bounds-check reconstruction

Inventory: `rg -n 'isInBounds' src include/Camera` finds five calls across sunmgr, lensflare (two), lensglow and sunmodel.
Read the complete original/current perform diffs before the header trial.
The external callers retain the address of the first screen-position vector: the original uses `lfsu` at offset 0xF8 and then reads Y at offset 4.
Use a const reference to `unkF8[0]` in the existing fabricated helper to reproduce that address reuse.
This fixes a structural mismatch in three functions without editing middleware.

| Function | Before | After | Remaining |
| --- | ---: | ---: | --- |
| sunmgr perform, 428 bytes | 98.82243% | 99.915886% | All instruction operations/registers match; frame 0x30 versus original 0x60. |
| lensglow perform, 1,328 bytes | 98.665665% | 99.00301% | Floating-point registers, conversion slots and frame still differ. |
| lensflare perform, 1,512 bytes | 76.756615% | 77.309525% | Vector copies, out-of-line trig calls, hidden-ratio loop, registers and frame remain. |

The sunmodel self-call is an exception: the original directly addresses 0xF8/0xFC through the model pointer.
Using the reference there adds an address calculation and a redundant X load; the score falls to 90.9%.
Replace that call with a direct member check and a named floating-point bound.
Its bounds-check instructions match again and its whole-function score is unchanged from baseline.
The frame changes from baseline 0xD0 to 0xC8, against original 0xE0; this does not resolve its existing frame problem.
Without the named bound, the compiler removes the second negation and the score is 91.6%.
A non-const reference does not resolve the self-call or floating-point register differences.
Keep the fabricated-name annotation: the original helper's identity is not established.

sunmgr and lensglow pass all map checks.
sunmodel still lacks CLBScreenFPosToSPos and emits calcDispRatioAndScreenPos_ globally rather than weakly.
lensflare still lacks the mapped JMASSin, JMASCos and vector set definitions.
Rebuilt both units from the baseline sources to reproduce these exact pre-existing validator failures; current edits are restored and rebuilt afterward.
These failures prevent file completion.

## Other camera trials — reverted

CameraSecureView has two runtime functions whose instruction operations/registers already agree, but stack layouts differ.
The 288-byte target calculation uses frame 0x60 versus original 0x68.
The 512-byte executor uses 0x98 versus 0xA8, with conversion slots 0x70/0x74 versus 0x80/0x84.
Existing Mario-angle accessors move the executor to 0xA0 but do not complete either function.
CLBAbs changes fused negation/arithmetic and regresses the executor; naming the front-facing predicate has no effect.
All trials reverted; its two-symbol map check passes.

sunmgr load has 114 instructions, with reversed color-input stack slots, swapped this/rodata registers, and frame 0x70 versus 0xB0.
Reverse declaration order and chain the five stream extractions to recover ascending input slots and frame 0x90, but its score remains unchanged.
An array changes argument scheduling; a four-read loop remains rolled and is substantially worse.
All load trials reverted, including the pending experiment from the previous continuation.

## Revisited higher-priority deferred files

CameraInbetween still has two four-byte UNUSED stubs versus map sizes 188 and 236.
The full caller inventory finds no runtime call to either body.
Its source .sdata2 begins with zero, 0.001, 1.0, 0.1, then the conversion double and extra +/-0.5 constants.
The target begins with 1.0, alignment, the conversion double, then zero, 0.001 and 0.1.
The source also emits a CLBRoundf short helper absent from this target object.
Do not promote based on the exact runtime-code report while these emission and unused-body issues remain.
No source or configuration trial in this pass.

TargetArrow and MarDirectorCreateObjects remain part of the broader imported-map string discrepancy recorded in batches 6 and 25.
Decoded the local DOL section table and compared the MovieDirector string symbols from 0x803A8B90 through the light-manager name at imported address 0x803A8DC8.
The movie filenames, UI/resource names, ending_1.blo and light-manager name are consistently eight bytes before the imported addresses.
The light-manager string is exactly 19 bytes at 0x803A8DC0.
This confirms that correcting only its two split endpoints would cut through neighboring incorrectly labeled strings.
The preceding boundary and following setup/TargetArrow region require a coordinated metadata audit; do not repeat the limited batch 6 relink trial.
No metadata changed and no linker trial in this pass.

## egggen single-function candidate — reverted

Read the entire control diff and inventory existing Yoshi, actor and vector helpers.
The original calls PSVECSquareDistance; TVec3::squared emits scalar arithmetic instead and is unsuitable.
A named Yoshi pointer, BOOL hatched result and existing getMActor accessor preserve the instruction sequence but move the frame only from 0x18 to 0x20, against original 0x30.
Naming the distance result does not move it further.
All trials reverted; no new wrapper or padding introduced.
NpcInbetween was inspected against its prior audit; do not repeat the recorded reciprocal/register trials without new evidence.

## Validation and continuation

Required baseline/changes_all, full build, comparison of all 12,904 function-presence/similarity entries, DOL byte comparison and expected SHA-1 pass.
Zero similarity regressions and no loss of exact functions or data.
Map failures above are reproduced at baseline; no gameplay test.

Source linking remains 76 game files / 80,800 code bytes.
Game code: 24.86575% matched / 2.8374174% source-linked.
Aggregate: 38.48049% matched / 2.24211% source-linked.
Regenerated queue: 104 authorized small game files / 94,780 unmatched code bytes.

Continue the plan's order: exact-code deferred game objects, single-function objects by remaining size, then other small objects.
Consult recorded blockers and rejected trials before choosing the next experiment.
Prioritize a new hypothesis that can close an entire file over additional partial lensflare/sunmodel reconstruction.
Measurements: `GMSE01-batch32.json`; local verification logs: `build/GMSE01-*-batch32.*`.
