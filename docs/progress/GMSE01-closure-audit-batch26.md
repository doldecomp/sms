# Priority 1 closure audit — batch 26

Baseline: `4d0e4561`.
Continue the file-completion priorities and consult the batch 25 audit before retrying its deferred files.
This pass recovers two missing map definitions and removes structural differences in the water filter, but promotes no objects.

## Retained changes

### NPC color helpers

The map records `InitChangeOneColor_Base` (48 bytes) and `InitChangeTwoColor_Base` (80 bytes) as UNUSED definitions in `NpcColor.cpp`.
Both were missing from the source object.
The existing caller contains their bodies: choose a color packet with or without a constant color, and for the two-color case use TEV registers 1 and 2.
Recovered both as local functions and replaced the three one-color branches and one two-color branch with calls.
Compiler inlining preserves every instruction in the runtime caller.
Both standalone helpers have exactly their recorded map sizes; symbol presence, order and linkage now pass.
Their sizes and caller evidence do not constitute a byte match for unavailable dead-stripped bodies.

The 560-byte runtime caller remains 99.96429%, with original frame `0x38` versus current `0x40`.
Changing `new GXColor()` to `new GXColor`, direct access to the actor's model field, and separate model declaration/assignment did not resolve it.
Changing the material index from `s32` to `u16` adds an early truncation and changes registers; rejected.
Direct access to J3DModel's protected model-data field does not compile and was reverted without modifying middleware.

### Water-filter camera and matrix context

`TMapObjWaterFilter::perform` improves from 78.38614% to 98.68317%.
Use existing `CPolarSubCamera::isDemoCamera()` to preserve the original camera pointer across the demo query and reproduce the original boolean groups.
Use `getUnk124()` and a const reference to retain the camera-position address across `TMapObjWave::getHeight`.
Separate missing-model and director-state guards reproduce the original early branches.

Declare inverse-view, translation and scale matrices before the transform-info local.
This restores their original relative stack order.
The named view-matrix pointer reproduces the original register held across matrix calls; use `MTXScale` consistently with the repository's SDK macro convention.
No padding or unused matrix was added.
Remaining differences are a `0x30` frame gap and the shape of the branch returning when the camera is above the water surface.
A named wave-height result does not change that branch.
All twelve mapped functions are present with correct order/linkage; no UNUSED functions.

## Other inspected candidates — trials reverted

| Candidate | Evidence / remaining work |
| --- | --- |
| `M3UModel::updateInMotion` | All instructions match except frame (`0x80` original, `0x58` current). Existing `getModel()` gives `0x60` without fixing it. Map check passes including both UNUSED sizes. |
| `MapCollisionPlane::checkPlaneGround` | Extra conversion spill/reload plus registers and frame. Writing world-to-grid arithmetic directly reduces frame but does not fix instructions. Shared `worldToGrid` must preserve fractions for `MapObjPlane::depress`; do not globally change its return type to int. |
| `enemyinterp` | No source file. Two linked definitions already exist in shared game headers, but the map also records seven UNUSED enemy-interpreter definitions with no bodies. No artificial emitter or stub-only promotion was added. |
| `MarioBlend::updateInMotion` | Same instructions, frame `0xA0` original versus `0x80`. Investigate with the base M3UModel animation context. |
| `CameraMarioData::calcAndSetMarioData` | Same instructions, frame `0x50` versus `0x48`; three UNUSED bathtub/door predicates remain four-byte stubs. |
| `CameraCodeControl::controlByCameraCode_` | Same instructions, frame `0x78` versus `0x50`. Existing Mario-angle and camera-mode accessors give `0x58`, still wrong. Map check passes, no UNUSED functions. |
| `SplashManager::makeDL` | Correct frame and instruction sequence, but color-construction and argument-copy slots are swapped (`0x54`/`0x58`). Detailed rejected forms below. Map check passes, no UNUSED functions. |
| `MapModel::initUnderpass` | Same instructions, frame `0x70` versus `0x50`. No edit attempted. |

SplashManager color trials: aggregate initialization without the compound-literal cast shrinks the frame by eight bytes; declaring then assigning or binding a const reference preserves the swapped slots.
Initializing white then assigning alpha does not solve it.
Passing the compound literal directly fixes the two color slots but moves alpha calculation after vertex writes and adds reloads.
Constructing `JUtility::TColor` or explicitly copying `GXColor` at the call adds non-native copies or changes scheduling.
All trials were reverted; no fake padding or forced inlining was retained.

## Validation and next action

Full build, `ninja changes_all`, all-function comparison, DOL byte comparison, and expected SHA-1 pass.
All 12,904 reported functions remain present; zero regressions.
Both changed files pass map checks without warnings.
No gameplay test was performed.

No new exact runtime code, matched data, or source-linked object is claimed.
Game coverage remains 24.792425% matched / 2.7192852% source-linked.
Aggregate coverage remains 38.422554% matched / 2.148763% source-linked.

Continue Priority 1 with the remaining uninspected candidates, including `CameraBck` and `EffectUtil`, or revisit the recovered NPC helper context when new evidence can resolve its frame.
Do not repeat the recorded source-shape trials unchanged.
