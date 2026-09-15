# Priority 1 closure audit — batch 37

Baseline: `b8c96698`, local GMSE01 revision 0 configuration.
Reread the plan and prior closure audits before continuing the single-function completion queue.
Completed and source-linked MarNameRefGen_Map: 824 code bytes and 488 data bytes.
One runtime function becomes exact; 448 additional data bytes match.

## MarNameRefGen_Map — completed

The original factory has PollutionTest, SunModel and SunsetModel branches that were commented out in the source.
Read the full instruction diff, original data, map closure and m2c draft before restoring them.
The allocations establish a 0x10-byte TPollutionTest with only its TViewObj base, and the existing 0x1B0-byte TSunModel layout.
Restore the pollution-test constructor in its game header, using the original Japanese debug name as its default argument.
Calling that constructor without an explicit argument preserves the out-of-line TNameRef and TFlagT constructors and the original pointer spill at 0x14.
An explicit name argument inlines those base constructors too deeply; that trial was replaced.
The sun and sunset branches call the existing constructor with false and true respectively.

Include the existing InfectiousStrings header before SunModel so the original dummy/error/matrix-type strings precede the sun paths.
This restores the 0xE0-byte missing prefix and every factory string offset.
All 206 instructions, the 0x28 frame, and both data sections match.
The one map-listed function passes presence, order and linkage; there are no UNUSED functions in this factory file.
Inventory found no other TPollutionTest construction site, so no unrelated callers were changed.
PollutionEvent's runtime function scores remain unchanged; its nine missing map definitions and ten UNUSED size warnings remain deferred.

## Reproduce archive resolution for the final link

The first source-link attempt failed on duplicate cSunVolumeName and cSunsetVolumeName globals from SunModel.hpp.
The original map explicitly records these globals in lensflare and unreferenced duplicate definitions in MarNameRefGen_Map, sunmodel, sunmgr and lensglow.
This is evidence for preserving the declarations and reproducing archive resolution.

Archiving only the factory resolves the duplicates but moves its sections after the loose objects, breaking executable layout.
Archiving the entire ordered link-input list with the pinned MWLD preserves the order, discards the unreferenced duplicates, and produces a byte-identical executable.
Add the opt-in ProjectConfig.link_dol_as_archive build setting and enable it for GMSE01.
Ninja builds mario.a from the existing ordered source/extracted objects, then links that archive with the same linker script.
The compiled objects used for comparison remain unchanged; no symbol bindings or original binary bytes are rewritten.
Other regions retain their existing default link path.
The archive still includes extracted objects for unfinished files; packaging them does not increase source coverage.

## Other candidates — no retained changes

- MapObjFloat: naming the allocated wave pointer leaves all instructions/registers unchanged; reverted the unfinished experiment.
- ModelUtil: its 404-byte riding routine retains stack differences; map validation also finds missing UNUSED SMS_DumpJ3DModel.
  Do not treat a runtime fix alone as file completion.
- NpcInbetween: reviewed its full diff and previous reciprocal/register trials; no new edit justified.
- NpcColor: passing the two TEV color addresses directly, and separately naming the model-data/material-name chain, both leave frame 0x40 instead of 0x38.
  Both trials reverted.
- MapObjWater: replacing the three Mtx locals with TMtx34f leaves the instruction sequence, branch discrepancy and 0x30 frame gap unchanged; reverted.
- M3UJoint: all instructions and stack slots agree, but the model-field address, parameters and scale flag use different registers.
  Map checks pass with no UNUSED functions; no speculative edit retained.
- M3UModel and MarioBlend: reviewed the sources against previously recorded frame blockers; no repeated getter trials.

## Validation and remaining queue

Required baseline and changes_all pass.
All 12,904 functions remain present, with zero similarity regressions; only the factory changes score.
No unit loses matched data.
Factory map checks, Python syntax checks, Ninja's archive/relink build, incremental no-op build, final DOL byte comparison and expected SHA-1 pass.
No gameplay test was performed.

Source linking: 85 game files / 91,324 code bytes.
Game: 25.02223% matched / 3.206984% source-linked.
Aggregate: 38.604145% matched / 2.5341396% source-linked.
Regenerated queue: 97 authorized game files with 1..2,000 unmatched bytes, totaling 90,324 bytes.
CameraInbetween and PollutionEvent remain the two deferred code-matching objects.
Continue the plan's file-completion order, consulting these recorded exceptions before further trials.
Measurements: GMSE01-batch37.json; verification logs: build/GMSE01-*-batch37.*.
