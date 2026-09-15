# Priority 1 file completion — batch 27

Baseline: `68fb613c`.
Reread the user's file-completion priorities and the batch 25/26 deferrals.
Completed and source-linked `Map/PollutionObj.cpp` (2,092 code bytes).

## PollutionObj: completed

The last 564-byte function, `getDepthFromMap`, had the right frame and instructions but passed a ground-query output pointer at `0x4C` instead of `0x54`.
Declare that pointer before the coordinate locals, name the `is_near` result, and name the center ground-height result before converting it to pollution depth.
This reproduces the original stack layout and removes the pre-existing `(void)0` expression.
All eight runtime functions now match, as do the reported data symbols.
All ten map definitions are present and correctly ordered/linked, including both size-correct UNUSED `is_near` overloads.
The full relink resolves the reported BSS alignment gap and reproduces the entire original executable byte for byte.

Rejected intermediate trials: moving the pointer alone put it at `0x50`; removing `(void)0` at that stage also caused unwanted inlining into `updateDepthMap`.
Naming the center height alone did not fix the remaining pointer offset.
The final named predicate provides the original local/call structure without artificial padding or inlining pragmas.

Shared-pattern search covered `is_near`, `getDepthFromMap`, and `worldToDepth` across game map source and headers.
The two `is_near` overloads are local to this file; other depth-conversion callers have different surrounding code.
Do not mass-replace them without native caller evidence.

## EffectUtil: improvement retained, file deferred

Normalize both cross-product vectors in place and use them directly as matrix columns.
This eliminates the extra saved float register and brings the instruction count to the original 122.
The last function improves from 96.66393% to 98.7541%.
Frame size, matrix position and float-register assignment remain different.
Using `cross2` for both products regressed similarity to approximately 97.3%; reverted to existing `cross`.
No middleware was edited.

The map still identifies a missing UNUSED `SMS_GetJumpIntoWaterModelData` definition (32 bytes).
Its declaration and the related MarioAccess wrapper do not supply enough body evidence for reconstruction.
This pre-existing map failure remains explicit; no promotion is claimed.

## CameraBck: inspected, unchanged

The 452-byte `updateDemo` function differs in frame size (`0x80` original, `0x68` current) and the final completion predicate's retained boolean.
Position, interest and up-vector calculation otherwise follow the original instruction structure.
Several UNUSED demo-query/restart definitions remain stubs.
No source change or completion claim.

## Validation and next action

Full build, `ninja changes_all`, all-function presence/score comparison, executable byte comparison and expected SHA-1 pass.
All 12,904 functions remain present; zero regressions.
PollutionObj passes the symbol-map check; EffectUtil retains its documented missing definition.
No gameplay test was performed.

There are now 75 source-linked game objects / 79,528 code bytes.
Game coverage is 24.812231% matched / 2.7927492% source-linked.
Aggregate coverage is 38.438206% matched / 2.2068136% source-linked.
The regenerated small-file queue contains 105 authorized candidates / 96,304 unmatched bytes.

Continue Priority 1 with the next unaudited single-function candidates, starting with `CameraWarp`, `areacylinder`, `Butterfly` and `Shimmer` in remaining-size order.
Revisit earlier deferred files when new evidence addresses their recorded blocker.
BossHanachan file completion remains Priority 2; larger reconstructions remain Priority 3.
