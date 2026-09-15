# Matching knowledge and shared-fix catalog

Read this catalog before changing a related declaration, helper, or repeated implementation.
It records local GMSE01 findings so work can resume without rediscovering them.
General compiler guidance remains in [AGENT_MATCHING_TIPS.md](AGENT_MATCHING_TIPS.md).

## Working procedure

1. Search this file and `PROGRESS.md` for the class, helper, symptom, or field.
2. Use `rg` across `src`, `include`, and the original map to inventory related definitions and callers.
3. Compare a representative caller's complete instruction diff before changing a shared definition.
4. Apply a shared fix or a batch of equivalent edits only to cases supported by their original instructions.
5. Rebuild affected units and run `ninja changes_all` against the saved baseline.
6. Check function presence, order, and linkage against the map, and verify the whole executable after source-link changes.
7. Record the successful case, exceptions, remaining candidates, and evidence here.

Similar source text is a search lead, not proof of equivalent code generation.
A full executable match also does not validate bodies in objects that are still linked from the original binary.

## Return types: `TBathtub::getNumGripsDead`

- Status: verified shared correction, batch 5 (`57f560d3`).
- Search: `rg -n 'getNumGripsDead' src include`.
- Change: return `int`, not the previous `u8`.
- Evidence: original counter routine and Bullet Bill caller use a signed count without byte narrowing.
- Results: the counter matches exactly; `TBathtubKiller::generateItemBathtubKiller` improved to 99.926384%, with vector stack offsets still different.
- Exception: the grip constructor stores the count into a `u16` local; that caller's own conversion is separate from the function's return type.
- Rule: inspect all callers before changing a return type; identical mangled names do not establish return types.

## Animation pointers: current animation versus previous blend animation

- Search: `rg -n 'getBckAnm|getBckOldMotionBlendAnmPtr|getOldMotionBlendAnmPtr' src include`.
- Current animation: `MActor::getBckAnm()` checks `mAnmBck` and loads its animation pointer at `0x24`.
- Previous blend animation: `getBckOldMotionBlendAnmPtr()` calls the out-of-line `MActorAnmBck` getter, which accesses the blend controller.
- Verified distinction: the boss's `copyFrameFromOldAnmToNewAnm_` writes the old frame into the current animation and frame controller.
  Using `getBckAnm()` removes the incorrect getter call; only stack offsets remain different.
- Exception: `TNpcInbetween::execMotionBlend` uses the previous-animation getter in both paths and already matches all 476 bytes.
  Do not replace those calls.
- Other caller: `TYoshi::thinkAnimation` uses the previous-animation getter; the boss's evidence does not justify replacing it.
- Related existing current-animation callers: `bosseel.cpp`, `CameraBck.cpp`.

## Virtual-table null entries: boss animation setter

- Search: `rg -n 'TBossHanachanParts|setAnm_' include/Enemy/BossHanachan.hpp` and the original map.
- Base table: `TBossHanachanPartsBase`, 256 bytes, last slot at `0xFC` is null.
- Derived head/body tables: same size; both put their `setAnm_` override at `0xFC`.
- Correction: declare a pure virtual `BOOL setAnm_(EnumBossHanachanAnmKind, EnumBossHanachanStopMotionBlendOnOff)` in the base.
- The batch 7 interpretation of this slot as alignment padding was wrong.
  Padding happened to preserve the executable bytes, but omitted the class interface.
- Validation: after correction, the sound object's entire virtual table compares exactly.
- Rule: inspect derived tables before treating a null tail entry as padding; a byte-identical build alone does not prove the class interface is complete.

## Shared motion-controller type: `TNpcInbetween`

- Search: `rg -n 'execMotionBlend|mInbetween|TNpcInbetween' src include`.
- Boss parts field `0x110` is a `TNpcInbetween*`.
- Evidence: constructor stores match its 44-byte layout, and `BossHanachanMain` loads that field before calling `TNpcInbetween::execMotionBlend`.
- Reuse the existing NPC type rather than creating an offset-compatible private structure.
- Boss UNUSED helpers: `setNonstopMotionBlendRatio_`, `offNonstopMotionBlend_`, and `restartBck_` reproduce their map sizes of 12, 16, and 48 bytes.

## Boolean completion checks

- Search: `rg -n 'isCurBckAlreadyEnd|STATE_COMPLETED_ONCE|STATE_LOOPED_ONCE' src include`.
- Boss check: `(completed || looped) || frame + 0.1f >= end` assigned to a `bool` local reproduces the intermediate boolean groups.
- Changing the local/result to `BOOL` adds normalization instructions; explicit `if` or `?:` trials also differed.
- Current boss result: instruction sequence matches apart from stack offsets; not yet exact.
- `MActor::isCurAnmAlreadyEnd` has a similar check but lacks the boss's outer null-MActor guard.
  Do not substitute the out-of-line function blindly.

## Imported US map data boundaries

- Status: unresolved; trial reverted in batch 6 (`6718c96a`).
- `MarDirectorCreateObjects`: native light-manager string starts at `0x803A8DC0`; imported map says `0x803A8DC8`.
- Several preceding MovieDirector strings are also eight bytes before their mapped addresses.
- A limited split-boundary correction made the unit compare exactly but left the legacy linker running without producing an executable; that metadata change was reverted.
- Next action: audit the surrounding string table, relocations, and object boundaries together.
- Do not globally subtract eight from map addresses.

## Shared bathtub matrix type

- Status: unresolved; trial reverted in batch 5.
- `TBathtubData` at `0x188`: original constructor calls `SMatrix33R<float>`; current declaration uses a column-major matrix.
- Related consumers: `MapObjCorona`, `BathWaterManager`, `GCConsole2`.
- A type-only correction regressed existing water consumers.
- Next action: coordinate the type and access changes against each caller's assembly.
- Related candidate: `TBathtub::allowsTumble` and `TBathWaterManager::throwMario` share a local-coordinate conversion sequence.

## Boss animation and reaction families

- Status: reconstructed, partially matching, batch 9.
- Search: `rg -n 'setAnm_|receiveMessage|isReactToTrampleOrHipDrop_|changeTumbleAnmRate_' src/Enemy/BossHanachanParts.cpp`.
- The body/head animation tables contain 18 entries each; head BCK/BTP/BTK table offsets are `0x190`, `0x1D8`, and `0x220` from the original rodata base `0x80381E08`.
- Body BCK table starts at `0x80381F50`; weak-body overrides select indices 11/14 for animation kinds 2/3.
- The owner stores eight body pointers at `0x150`, head at `0x170`, weak-body index at `0x174`, and parameter pointers at `0x1BC`/`0x1C0`.
- Keep the candidate body-animation index separate from the mutable index inside the change branch; this reproduces the target's load/move scheduling.
- Head setter is 97.746475% with register differences; body setter is 91.818184% with register/prologue and stack differences.
- `BOOL` result/local and `bool` result/local trials had equivalent setter instruction streams; mixed types added a conversion.
  Do not infer a global return-type correction from these trials.
- Both hit handlers share the same director guard (`isThing()`), overturned-state test, and trample reaction.
  Reuse `isReactToTrampleOrHipDrop_()` and `restartBck_()`.
- The overturned test uses exact comparisons to -179 and 179 degrees, with a ternary boolean result reproducing the target's second normalization group.
- Body hip drops affect animation kinds 2, 3, 5, 13, 16, and 17, as shown by the original 18-entry jump table.
  The head has a distinct hip-drop response and does not use that eligibility switch.
- `getLatestNerve()` remains incorrectly inlined in both hit handlers and damage fog; the original calls the existing weak function out of line.
  Investigate common inline context before adding local pragmas or changing the shared getter globally.
- Exact tumble-rate fix: compute `remaining = end - frame` before loading rate and calling `SMSGetAnmFrameRate()`.
  Passing `end - frame` directly as an argument computed it after the call and spilled extra registers; the named intermediate reproduces all 188 bytes, including stack layout.
- Shadow submission reuses `TCircleShadowRequest`; its full instruction sequence matches apart from stack offsets (99.54%).

## Water-hit counter declaration and field ambiguity

- Status: declaration corrected, field audit pending, batch 9.
- Search: `rg -n 'TWaterHitActor|onWaterHitCounter|mStaticHitActor' src include`.
- Original map: `onWaterHitCounter__14TWaterHitActorFv` is a global 12-byte function in `BossHanachanSub.cpp`, at `0x800ED5B4`.
- Native instructions set 60 and use `sth` at `0x68`; the original receive-message handler also uses halfword stores there.
- Removed the fabricated inline `unk68 = 0x3C` from `ModelWaterManager.hpp`, restoring the out-of-line declaration and the head hit-handler call.
  The function body remains supplied by the original nonmatching sub-object until that unit is reconstructed.
- Do not simply change the existing `int unk68` to a halfword globally: particle-manager code and multiple enemy consumers use it as a particle index.
  Audit their actual load/store widths and static-object initialization together to establish whether separate views/fields or an original declaration discrepancy are involved.
- The original particle-manager static object is `0x6C` bytes and its constructor has no store to `0x68`; the boss foot constructor explicitly zeros a halfword there.
  Avoid inventing a shared constructor until that distinction is resolved.
- All callers rebuilt after the declaration correction; the complete batch 9 function comparison found zero regressions.
