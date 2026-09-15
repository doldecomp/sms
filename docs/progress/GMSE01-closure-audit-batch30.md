# Priority 1 closure audit — batch 30

Baseline: `f3694700`, local GMSE01 revision 0 configuration.
Reread the plan and matching catalog; prioritize whole-file source linking.
This checkpoint completes no additional file or exact runtime function.
The retained MessageLoader improvement is preparatory work, not source-linked progress.

## GC2D/MessageLoader — retained parser reconstruction

Hoist the block tag and length before the outer memory stream and use signed stream extraction.
Name the current INF1 pointer, advance a typed length cursor, and read the discarded entry-size field into a named u16 before reading the format.
The parser improves from 99.64336% to 99.94405%: all 143 instruction opcodes and registers agree, with the original 0xB8 frame.
The outer stream, block length and tag occupy the original 0x5C/0x70/0x74 slots.
The remaining differences are the outer constructor-bound pointer at 0x50 instead of 0x54, the inner stream at 0x34 instead of 0x38, and entry size at 0x30 instead of 0x34.
The 108-byte constructor still has a 0x18 frame instead of 0x20.
All eight map symbols pass presence/order/linkage; both UNUSED bodies have the original sizes.
Do not promote this file yet.

Rejected trials, so later work does not repeat them:

- Hoisting tag/length alone restores their relative order but leaves offsets too low.
- Signed extraction instead of raw reads does not independently change instructions.
- Declaring entry size after the inner stream gives the wrong relative layout; declaring it before the stream is retained.
- The typed cursor restores the original r18 pointer reuse; a signed-long length instead of int shifts the outer slots down again.
- Naming payload size, a separate entries pointer, or the format result prevents readInfoBlock inlining and drops the parser to about 59.1%.
- Swapping the entry-size and length declarations has no effect.
- Chaining the first two inner extractions matches some slots but changes registers, expands the frame to 0xC0 and shifts other slots; separate extractions are retained.
- Hoisting the constructor's two header outputs has no effect and was reverted.

## Map/MapXlu — unchanged

changeNormalJoint and changeXluJoint have the original instruction sequences: normal is 256 bytes, translucent is 280 bytes.
Both original frames are 0x88; current frames are 0x98 and 0x90 respectively.
Inventory includes the map/root-model accessors, child traversal and both priority-index arrays.
Using signed-long loop counters or naming child/grandchild indices has no effect.
A priority-group reference inside the inner loop changes address-expression reuse and makes both functions worse without resolving their frames.
All trials reverted; all five map symbols pass.

## Map/PollutionPos — unchanged

getEdgeDegree retains register/index-expression differences and a 0x20 frame instead of 0x38.
isSame has the original 53 instructions, but its frame is 0x38 instead of 0x58 and conversion temporary is 0x30 instead of 0x50.
All ten map symbols are present with correct order/linkage.
UNUSED subtractFromYMap is still a four-byte stub against a 112-byte map body.
No reliable inline body was identified in this audit; no implementation was guessed.
The existing index helper is shared, so its layout formula must not be changed from this one caller alone.

## System/PerformList — unchanged

load has the original 88 instructions and 0xC8 frame; its value-read temporary is 0x58 instead of 0x54.
perform has the original 54 instructions, but a 0xC0 frame instead of 0xE8 and different iterator-copy slots.
Its by-value iterator machinery is in protected JGadget code; no middleware edits were made.
The existing extra game list base also affects already-matching destructors and MarDirector construction.
All eleven map symbols pass, including the 164-byte UNUSED forEachPerform.

Rejected load trials:

- Changing u32 to int or hoisting the read result has no effect.
- Direct stream extraction adds a write/reload and reduces the frame to 0xC0.
- Extracting to a separate raw value removes those extra instructions but retains the wrong frame and 0x50 slot.

## Enemy/BossHanachanNerve — unchanged

Tumble and Snort retain original instruction sequences, with frames 0x38/0x28 instead of 0x40/0x40.
All 22 map symbols pass.
Consulted the batch 17/18 findings before trials; did not repeat the previously rejected Tumble director/console locals.
The sound accessor and march-speed getter have no effect.
The hit-point getter grows Snort's frame only to 0x30, still nonmatching.
Naming the next nerve introduces an extra instruction and changes registers in both callers.
All trials reverted; do not retain getter substitutions merely for a closer frame.

## GC2D/MovieRumble — unchanged

Full diffs for init, perform and checkRumbleOff show 748 unmatched bytes in total.
init and checkRumbleOff share an inlined readCurInfo mismatch: the original preserves a separate tool-data pointer move, while current code eliminates it and assigns different registers.
Their frames are respectively 0xB0/0x38 instead of 0xC0/0x48.
perform has the original instruction sequence but a 0x28 frame instead of 0x30.
All nine map symbols pass presence/order/linkage.
UNUSED readCurInfo is 212 bytes against 208, and movement is 120 against 180; these remain closure blockers.

Moving the existing getToolData local before the group index improves register assignment but does not recover the missing move.
A named validity result changes the frame without fixing the move.
Signed-long group, const tool-data pointer, calling the getter inside isValid, and assigning the pointer in the condition do not resolve the shared mismatch.
All source/header trials reverted.
The existing dont_inline pragma was left unchanged; its relationship to the UNUSED movement body needs further evidence.

## Verification and next work

Required baseline/changes_all, full build, comparison of all 12,904 function entries, complete DOL byte comparison and expected SHA-1 pass.
Zero regressions, zero newly exact functions, zero new source-linked files.
Only MessageLoader source changes remain; its map validation passes without warnings.
The other audited map warnings are recorded above, not treated as completion.
No gameplay test was performed.

Source linking remains 76 game files / 80,800 code bytes.
Game code: 24.844398% matched / 2.8374174% source-linked.
Aggregate: 38.463623% matched / 2.24211% source-linked.
The regenerated queue remains 104 authorized small game files / 95,388 unmatched bytes.
Continue the plan's completion order; revisit deferred files only when new evidence addresses their documented blockers.
Further unaudited small multi-function candidates include MSModBgm and MapCollisionEntry.
Measurements: `GMSE01-batch30.json`; local validation logs: `build/GMSE01-*-batch30.*`.
