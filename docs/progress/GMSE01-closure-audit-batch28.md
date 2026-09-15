# Priority 1 closure audit — batch 28

Baseline: `6bb39882`.
The previous turn completed PollutionObj; this pass continues the remaining single-function queue.
No new file was promoted.

## ProgSelect: US data restored, padding removed

Recovered five original named strings from the local GMSE01 executable and its generated assembly.
`strDispProg`, `strSetProg`, and `strSetInter` are mutable static arrays in `.data`; `strYes` and `strNo` belong in `.sdata`.
Their sizes are 52, 110, 103, 4, and 3 bytes, respectively.
Keep the original escape commands, capitalization, punctuation and line breaks.
The US text draw uses width 360 and horizontal position 145; the existing Japanese values were 300 and 175.
US-specific text and draw parameters are guarded by `VERSION_GMSE01`.

All 328 reported data bytes now match, a gain of 312 bytes.
Constructor relocations now match too; the report already counted its instructions as exact before fixing the referenced data, so do not claim another 500 exact code bytes.
The remaining 792-byte `perform` has the original 198 instructions and differs only in stack layout.

Removed the pre-existing `char trahs[0x10]` padding array.
This exposes the real frame difference: original `0x1F8`, current `0x1E8`.
Color, print and graph slots are twelve bytes below their original locations; conversion/saved-register slots are sixteen bytes below.
The function's similarity decreases from 99.85859% to 99.79798% as a result.
This is an explained score regression that removes artificial source, not lost exact code or a source-link regression.
Do not restore dummy padding to improve the percentage.

Rejected: `u8 prevSelection` has no effect; restore `u32`.
Hoisting three named controller booleans adds eager normalization instructions and still does not restore the frame; reverted.
All three map symbols are present, ordered and linked correctly; no UNUSED entries.

## Shimmer: restore helpers and rendering calls

The original model virtual calls select offsets `0x10`, `0x14`, and `0x0C`.
Use the existing J3DModel methods `calc`, `viewCalc`, and `entry`, respectively.
The previous source selected `entry`, `calc`, and `update`, which used the wrong offsets.
This is a game caller correction; no middleware was changed.
Related game rendering paths in MapStaticObject and MapWire already use the expected methods.

Recovered `near` and `far` from the inlined position-setting branches.
The map records 24 bytes for `near` (Z = 9600) and 20 bytes for `far` (Z = 0).
Both now have exactly those sizes, and `perform` calls them without changing the original position instructions.
The size and caller evidence do not establish a byte match for unavailable dead-stripped bodies.
All ten map definitions pass, including both UNUSED sizes.

Reused the water-filter finding: declare inverse-view, translation and scale matrices before transform info.
The native relative matrix/transform layout is restored; use the existing view-matrix getter and descriptive matrix names.
Remaining frame is `0x170` versus original `0x198`; matrix/transform slots are `0x1C` too low.
Similarity improves from 99.80247% to 99.820984%.

## Deferred trials — all reverted

| Candidate | Evidence and rejected trials |
| --- | --- |
| CameraWarp, 580 bytes | Map clean; original frame `0x58`, current `0x50`. Moving the position vector before look-at fixes relative vector order but leaves all vector slots four bytes low. Named mode/distance results do not help. Copy construction adds integer copies; a named base-vector reference or base-cast construction adds instructions/register changes. |
| areacylinder, 604 bytes | Original frame `0xA0`, current `0x98`, with stream and container temporaries in different positions. `readF32` for the discarded float plus direct `readS32` probability calculation gives the original frame but wrong slots. `s32` count improves relative temporary positions; direct extraction into count introduces loop reloads. Hoisting count, naming discarded float, and splitting radius/height extraction do not close it. Plain `int` cannot bind to this compiler's `s32` stream overload. |
| Butterfly, 640 bytes | Instructions agree, frame `0x108` versus `0x80`. Existing setter-only attempts are already noted in source. Map presence/order/linkage pass but UNUSED receiveMessageFromChild is 256 bytes versus 260. No edit attempted. |
| CameraMultiPlayer, 884 bytes | Extra move when clamping camera distance, exchanged X/Y difference registers, and frame `0x60` versus `0x50`. Initializing directly from MsClamp moves the extra copy; explicit branch clamp adds loads/instructions. Reverted. |

The frame-only candidates remain investigation groups, not justification for blanket edits or padding.
Consult batch 25–27 audits before retrying earlier candidates.
Next uninspected single-function entries include MarDirectorPreEntry; smaller multi-function candidates remain eligible under Priority 1.

## Validation

Full build, `ninja changes_all`, DOL byte comparison and expected SHA-1 pass.
All 12,904 functions remain present; all previously exact functions remain exact.
One explained nonmatching-function score regression is recorded above; zero unexplained regressions.
Both changed files pass symbol-map checks without warnings.
No gameplay test was performed.

Source linking remains 75 game objects / 79,528 code bytes.
Game code is 24.812231% matched / 2.7927492% source-linked.
Aggregate code is 38.438206% matched / 2.2068136% source-linked.
The regenerated small-file queue contains 105 authorized candidates / 96,304 unmatched bytes.
