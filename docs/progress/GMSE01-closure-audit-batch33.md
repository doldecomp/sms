# Priority 1 closure audit — batch 33

Baseline: `f7a677b8`, local GMSE01 revision 0 configuration.
Reread the file-completion priorities and prior closure audits.
Completed and source-linked two files from the small-file queue: DemoBossHanachanBase and SleepBossHanachan.
Together they add 21 exactly matching functions/helpers, 2,440 code bytes and 1,496 data bytes.

## DemoBossHanachanBase — completed

Reconstructed the previously empty file using the full original assembly, m2c drafts and map-wide class/symbol inventory, following PROGRAM_STRUCTURE_REVVING.md.
The actor derives from TSpineEnemy without new fields; the manager's parameter pointer is at 0x54.
The save parameters at 0x08/0x1C retain the original mSLViewClipFar/mSLViewClipRadius names and 25000/3500 defaults.
Initialization recovers actor registration, MActor creation, collision dimensions, live flags, ground query and light type using existing fields and helpers.
All seven runtime functions (728 bytes) and all 656 data bytes match.
All seven map function symbols are present, ordered and linked correctly; none is marked UNUSED.
The two synthetic gap labels in objdiff need no source definitions: natural linker alignment reproduces those bytes in the final executable.
No padding emitter was introduced.

## SleepBossHanachan — completed

Recovered the actor/manager hierarchy, shine position at 0x150, mirror actor at 0x15C and both nerve classes from original code and map evidence.
The explicit two-entry animation table occupies .sdata as in the original.
Initialization uses the existing float-argument MsMtxSetXYZRPH overload, preserving rotation conversions and argument loads.
The fall nerve creates the shine with its demo camera, marks the boss dead, hides its mirrored model and queues the sleep nerve.
The four-byte calcRootMatrix body is empty in the original; this is a verified implementation, not an unresolved stub.

The first draft matched 12 of 14 functions.
For the fall nerve, the existing getMActor accessor and a named BOOL animation result recover the original 0x40 frame.
Declaring the position before that result, then assigning it inside the successful branch, places the vector at original slot 0x28.
For startFall, naming the TFlagManager::getInstance result restores the 0x48 frame and completes all 236 bytes.
A named next-nerve pointer/reference or spine pointer did not resolve that frame and was reverted.
These are useful source-context patterns, not a rule to name every intermediate value.

All 14 runtime functions (1,712 bytes) and all 840 data bytes match.
All 14 map function symbols pass presence, order and linkage checks; none is marked UNUSED.
Both new files use the existing header helpers; no library implementation changed.

## Shared-pattern inventory: MapEventSirena — reverted

Searched the single-nonmatching-function candidates for TFlagManager::getInstance callers after completing startFall.
MapEventSirena was the only candidate in that group.
Read all 70 watch instructions and its prior batch 25 trials before experimenting.
Its baseline frame is 0x48 versus original 0x50, and the camera-flag slot is 0x38 versus 0x44.
A named flag-manager result moves the flag slot to 0x3C without changing the frame.
Adding a named camera flag restores the frame but leaves its slot at 0x40.
A named SMSGetMarDirector result moves that slot to 0x44 but expands the frame to 0x58.
Splitting the flag-manager declaration/assignment does not help.
Using gpMarDirector directly for the director local returns to frame 0x50/slot 0x40.
No combination completed the file; all Sirena edits were reverted after this bounded pass.
Do not blindly apply the successful SleepBossHanachan local-variable form across callers.

## Other higher-priority closure trials — reverted

MapModel's 420-byte initUnderpass already has matching operations/registers, with frame 0x50 versus original 0x70.
Using existing getModelData accessors moves it to 0x58.
Existing child getters and setInfo move it to 0x68, still eight bytes short.
A named joint-name table does not help; a second named model-data pointer changes registers.
An explicit default tex-matrix argument does not move the frame further.
All trials reverted; its map check passes all 13 symbols including one UNUSED body.

MarDirectorPreEntry's 892-byte function also has matching operations/registers, with frame 0xB0 versus original 0xE8 and rectangle slot 0x80 versus 0xB8.
Changing its five typed name searches to search2 plus casts moves the frame only to 0xB8.
Direct root-name searches move it to 0xA0 instead.
All trials reverted; its single-symbol map check passes.

## Validation and continuation

The saved baseline equals the batch 32 measurement snapshot.
Individual promotion builds and the combined final build pass executable byte comparison and SHA-1 `a6782903ef79d4196c8489ecb1b57decb5b3728f`.
Required changes_all and all 12,904 function-presence/similarity comparisons pass with zero regressions.
All code/data sections of both promoted files are exact, and both map validators pass without warnings.
No gameplay test was performed.

Source linking is now 78 game files / 83,240 code bytes.
Game code: 24.951435% matched / 2.9231017% source-linked.
Aggregate: 38.548203% matched / 2.3098176% source-linked.
Regenerated small-file queue: 102 authorized game files / 92,340 unmatched code bytes.
The whole decompilation remains incomplete.

Continue exact-code deferred game objects, then single-function objects by size, then other small files.
Use new evidence to address recorded blockers; avoid repeating exhausted frame-only trials.
Measurements: `GMSE01-batch33.json`; verification logs and m2c drafts: `build/GMSE01-*-batch33.*`.
