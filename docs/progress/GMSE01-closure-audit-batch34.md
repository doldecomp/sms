# Priority 1 closure audit — batch 34

Baseline: `7e3e3455`, local GMSE01 revision 0 configuration.
The preceding checkpoint completed two files and was verified progress.
This checkpoint completes and source-links livemanager and objmanager, adding 2,912 source-linked code bytes and 488 source-linked data bytes.
Three runtime functions become exact, totaling 656 code bytes.

## Shared timer context closes livemanager

Consulted batch 25's SnapTimeObj/livemanager blockers and read their full original/current instruction diffs.
Inventoried startTimer/endTimer callers in MarioMain, ModelWaterManager, enemymanager, Application, SnapTimeObj, livemanager and objmanager.
The four-component startTimer overload constructed color through a union; its packed-color overload used a fabricated volatile temporary and then ignored the reload.
The existing JUtility::TColor supplies both constructions and the required integer conversion without modifying middleware.

For the four-component overload, retain the instance load before constructing the color, convert the color before the null check, and name the current time-array pointer after OSGetTick.
endTimer uses the existing instance accessor.
Together these recover the original 0x50 frame and color slot 0x34 in both TLiveManager::perform (252 bytes) and TObjManager::perform (236 bytes).
Using the instance accessor in the four-component start helper instead expands the frame to 0x58; moving color before the instance changes the later end-timer register.
These ordering/accessor choices are verified exceptions, not interchangeable stylistic variants.

All 1,272 code bytes and 96 data bytes of livemanager match.
Its ten map symbols pass presence, order and linkage checks without warnings.
The source-link promotion passes executable byte comparison and SHA-1.

## objmanager loader and missing mapped helper close the second file

After the timer fix, objmanager has only a 168-byte loader mismatch: its name buffer is four bytes above the original slot, with otherwise exact code.
The map-wide inventory also identifies a declared but undefined initObjArray(int), size 60, between manageObj and the constructor in emission order.
Reconstruct it from the capacity assignment/allocation sequence already in load, and place it between the constructor and manageObj in source order.
Its standalone UNUSED body has the exact mapped size.

Calling it with readS32 directly expands the loader frame to 0x148 and moves the name buffer to 0x34.
Read into a named s32 capacity using the existing stream extraction operator, then call initObjArray(capacity).
This restores the original frame 0x140, buffer 0x2C and capacity slot 0x28, matching every instruction.
All 1,640 code bytes and 392 data bytes now match.
All 16 map symbols pass, including the UNUSED helper's size.
Individual and combined source-link builds pass executable byte comparison and SHA-1.

## SnapTimeObj — improved, not complete

The packed-color timer overload now constructs JUtility::TColor before loading the instance through its accessor and uses the converted integer in append.
It retains the direct crTimeAry()[0] append expression; a named array pointer expands the frame.
This removes the volatile workaround and restores all operations/registers plus the original 0x50 frame in SnapTimeObj.
Only the color store/reload slot differs: 0x34 versus original 0x38.
Similarity is 99.96491%; the CLI's rounded 100.0% display is not an exact match.
Do not promote the file.

Rejected trials: explicit toUInt32 versus implicit conversion, direct versus copy initialization, early declarations of the color integer/array pointer, and a named caller color do not complete it.
A named outer recording predicate expands the frame without fixing the slot and was reverted.
Several intermediate instance/array combinations matched the slot but expanded the frame to 0x58, or changed cue/end-timer registers.
The retained helper form improves the original code while preserving all other callers.

## Validation and existing audit debt

Required baseline and changes_all pass; the saved baseline equals the batch 33 snapshot.
Full comparison of all 12,904 function-presence/similarity entries finds zero regressions.
Only the three newly exact functions and the improved SnapTimeObj function change score.
All sections of both promoted objects are exact; both map validators pass without warnings.
Final DOL byte comparison and SHA-1 `a6782903ef79d4196c8489ecb1b57decb5b3728f` pass.
No gameplay test was performed.

TimeRec itself remains runtime-exact and source-linked, but its map validator reports three pre-existing four-byte UNUSED stubs: end (map 52 bytes), drawSyncCallbackSt (92), and suppleGXTime (104).
The source bodies are unchanged in this checkpoint.
These are completion-audit debt; source-link coverage alone does not prove those unused bodies reconstructed.
SnapTimeObj's map check passes without warnings.
HitActor's existing radius diff and sqrt helpers were inspected, but no change or new successful hypothesis was established.

Source linking: 80 game files / 86,152 code bytes.
Game: 24.97447% matched / 3.025361% source-linked.
Aggregate: 38.566406% matched / 2.3906221% source-linked.
Regenerated queue: 100 authorized small game files / 91,684 unmatched code bytes.
Continue the plan's completion order and consult these shared-helper exceptions before retries.
Measurements: `GMSE01-batch34.json`; verification logs: `build/GMSE01-*-batch34.*`.
