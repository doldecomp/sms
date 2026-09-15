# Priority 1 closure audit — batch 36

Baseline: `8f7a03e5`, local GMSE01 revision 0 configuration.
The preceding checkpoint corrected regional metadata and source-linked two deferred files; it was verified progress.
This checkpoint completes two single-function candidates: SplashManager and CameraMode.
Together they add 3,904 source-linked code bytes and 3,684 source-linked data bytes.
Two runtime functions become exact, totaling 536 code bytes.

## SplashManager — completed

Read the full 98-instruction makeDL diff and the rejected batch 26 color trials.
The original constructs color at stack slot 0x54 and passes its copy at 0x58; the previous source swaps those slots.
The old direct-compound-literal trial recovered the slots but computed alpha too late, after vertex writes.
Name the u8 alpha result before writing vertices, then construct the compound-literal GXColor at the requestCol call.
This preserves the original integer division and floating-point scheduling while recovering both color slots and the 0xE0 frame.
All 392 function bytes match on the first new trial.

Searched requestCol and related color construction sites in game rendering code.
SplashManager is the only requestCol caller found outside its definition; other color sites have different consumers and do not establish a mass-edit opportunity.
Do not repeat the earlier local-color/explicit-copy/constructor variants without accounting for alpha scheduling.

All 2,752 code bytes and 308 data bytes match, including every section.
All eight map symbols pass presence, order and linkage checks; there are no UNUSED functions.
The source-link build passes executable byte comparison and SHA-1.

## CameraMode — completed

The 144-byte isNormalCameraCompletely originally calls isNormalCameraSpecifyMode for the current mode, then inlines the previous-mode switch after isNowInbetween.
The previous source inlines both switches, creating extra instructions and an extra jump table.
An existing isLButtonCamera helper provides the corresponding current-mode predicate design in the same class.
Add the analogous isNormalCamera predicate and call it for the current mode only.
Mark its name fabricated because the map does not establish that inline helper's original name.
This recovers the original asymmetric inlining, 36-instruction body, 0x18 frame and jump-table emission without a no-inline pragma.
The previous-mode predicate remains direct, as supported by the original switch instructions.

Inventoried related calls in CameraChange, CameraBGCheck and cameragc.
Those other translation units call the separately defined predicate and remain unchanged.
The full camera-header rebuild introduces no other function-similarity changes.

All 1,152 code bytes and 3,376 data bytes match, including every section.
Correct table emission restores 1,432 reported matched data bytes.
All 14 map symbols pass presence, order and linkage checks; there are no UNUSED functions.
The source-link build passes executable byte comparison and SHA-1.

## Other candidates and validation

Rechecked enemyinterp's absent source against its recorded UNUSED-method blockers; no artificial emitter was added.
Read MapObjFloat's complete initializer diff and prior trials; it retains register-allocation differences with an otherwise correct instruction sequence and frame.
No changes to either candidate were retained.

Required baseline/changes_all and all 12,904 function-presence/similarity comparisons pass with zero regressions.
The baseline equals the saved batch 35 snapshot.
Only the two newly exact functions change similarity; no unit loses matched data.
Both promotion builds and the final executable byte comparison/SHA-1 pass.
No gameplay test was performed.

Source linking: 84 game files / 90,500 code bytes.
Game: 24.993294% matched / 3.1780477% source-linked.
Aggregate: 38.581276% matched / 2.5112743% source-linked.
Regenerated queue: 98 authorized small game files / 91,148 unmatched code bytes.
Continue the plan's file-completion order, consulting the saved successes and exceptions before further trials.
Measurements: GMSE01-batch36.json; verification logs: build/GMSE01-*-batch36.*.
