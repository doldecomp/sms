# Priority 1 closure audit — batch 29

Baseline: `ad9a47f2`, active local GMSE01 revision 0 configuration.
Reread the plan and shared matching catalog; continue file completion in the listed order.
Read full original/current instruction diffs before trials.

## Completed: GC2D/HelpActor

All 1,272 code bytes now link from source, including 508 newly exact bytes in `load` and `perform`.
The original US load adds `0x33` to the serialized help index; the prior `0xE0030` needs an extra instruction and selects a different message.
Use a GMSE01 conditional to preserve the existing other-region expression.
Search for the old literal found no additional occurrence in GC2D/System.

Both balloon branches now name the result of `SMSGetMarDirector()->getConsole()` before making their console call.
This reuses the previously recorded EventWatcher inline-context finding: it restores the original `0x58` frame without changing the instruction sequence or adding padding.
Apply the same change to both equivalent sites and verify the entire function.
Other balloon callers in GCConsole2 already use their own console or a local; MarDirectorDirect uses its own member and different fixed message IDs.
Do not transfer this local-variable change without checking the caller's original instructions.

All code and data comparisons pass.
All twelve map symbols are present, with correct order/linkage and the 136-byte UNUSED `getHelpID` body.
Added `GC2D/HelpActor.cpp` to the source-link allowlist; the complete executable byte comparison and expected SHA-1 pass.

## Animal/AnimalManager

Recovered missing `TAnimalManagerBase::loadSaveParams_(const char*)` from the body in TMewManager::load and the whole-map class scan.
Its 128-byte compiled size agrees with the UNUSED map entry; calling it retains the exact 124-byte runtime load.
Place the definition between the base constructor and clipping function to preserve reversed emission order.
The nine-symbol map check now passes without warnings.

Clipping improves from 94.984375% to 99.90625%.
Name the near plane before evaluating camera arguments; use existing `getFovy`, `getObjNum`, and derived `getObj` accessors with an `int` count.
All 64 instructions and register choices now agree; the vector occupies `0x28` instead of original `0x24`, with the correct `0x58` frame.
The 60-byte loadAfter still has a `0x18` frame instead of `0x28`.
No source-link promotion.

Rejected trials:

- A base-qualified object getter or direct object count changes the actor/count registers.
- Direct camera FOV access reduces the frame and moves the vector too low.
- Naming FOV reverses its load order with aspect and leaves the vector mismatch.
- Naming the live-flag predicate expands the frame to `0x60` and moves the vector to `0x30`.
- Vec versus TVec3, declaration versus assignment, actor type and declaration-order changes do not resolve the remaining slot.
- Using getObjNum in loadAfter has no effect and was reverted.

## Camera/CameraTalk

`SMS_GetMarioAngleY()` restores the original `0x28` frame in makeMtxForTalk, matching all 224 bytes.
makeMtxForPrevTalk retains an exact instruction sequence but needs a `0x38` frame instead of `0x28`.
All three map symbols pass.
No source-link promotion.

Rejected: the director accessor has no effect in the previous-talk function.
A shared changeCamMode_ trial with a named s16 frame introduces a call/extra instructions; using int retains the old frame.
The shared header was fully restored.
Caller inventory covers CameraChange, CameraTalk and cameragc; the full build and regression check include these dependents.

## Strategic/Strategy

The constructor's loop must use `s32 i` with the signed bound `16` to preserve the original initial compare/branch before the unrolled sixteen stores.
This also restores vtable instruction scheduling: all 184 bytes now match.
Changing the bound alone with an int counter has no effect.
Search other fixed-array loops before related changes; ObjHitCheck's constructor is already exact, so it is an explicit exception to mass replacement.

The 280-byte load still has the original 70 instructions and local offsets, but its frame is `0x68` instead of `0x70`.
All nine map symbols pass; no UNUSED definitions are missing.
No source-link promotion.

Rejected: an s32/readS32 count shifts the count and stream four bytes low without fixing the frame.
Default memory-stream constructor arguments have no effect.
A separately named base-class genObject result adds a compare/move and changes registers.
Only the matching constructor edit remains.

## Other deferred candidates inspected

- MarDirectorPreEntry: all 223 instructions agree; original frame/viewport rectangle are `0xE8`/`0xB8`, current `0xB0`/`0x80`.
  Map check passes its sole symbol.
  Five searches use the existing name-reference wrapper; no new supporting evidence justifies changing middleware or inventing helpers.
- TalkCursor: loadAfter frame `0x28` versus original `0x30`; associate frame `0x60` versus `0x78`, with vector/matrix locals `0x1C`/`0x28` versus `0x30`/`0x3C`.
  Full instruction sequences inspected; no source edits.
- WoodBarrel: kill has frame/vector `0x28`/`0x10` versus original `0x30`/`0x1C`; appeared `0x20` versus `0x28`; appear `0x18` versus `0x20`.
  All three instruction sequences otherwise match.
  Its TParamVec lacks a setter; do not invent one just to force the frame.
  No source edits.
- ObjManager: load/source inspected; perform shares the already recorded timer issue.
  No new trial or full-file closure claim.

## Validation and next work

Full build, required baseline/changes_all, all 12,904 function-presence/score comparisons, executable byte comparison and expected SHA-1 pass.
Zero function regressions; four newly exact runtime functions, 916 code bytes.
All four edited source files pass map checks.
No middleware changes or gameplay test.

Source linking: 76 game files / 80,800 code bytes.
Game code: 24.844398% matched / 2.8374174% source-linked.
Aggregate: 38.463623% matched / 2.24211% source-linked.
Refreshed small-file queue: 104 authorized game files / 95,388 unmatched bytes.
Next unaudited small multi-function files include MapXlu, PollutionPos, PerformList and BossHanachanNerve; revisit recorded deferrals only with new evidence.
Measurements: `GMSE01-batch29.json`; local validation logs: `build/GMSE01-*-batch29.*`.
