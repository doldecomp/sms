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

## Water-hit payload: particle index and signed countdown

- Status: shared storage and constructor behavior reconstructed, batch 14.
- Search: `rg -n 'TWaterHitActor|mParticleIndex|mWaterHitCounter|mStaticHitActor' src include`.
- Original map: `onWaterHitCounter__14TWaterHitActorFv` is a global 12-byte function in `BossHanachanSub.cpp`, at `0x800ED5B4`.
  It sets 60 with `sth` at offset 0x68; `receiveMessage` also uses halfword stores there.
  The declaration and definition are out of line; batch 15 reconstructs the setter and receiver exactly.
- Signedness is established by `BossHanachanMain` at 0x800EF50C, 0x800EF580, and 0x800EF5F4: `lha` from the head/body/foot payload, compare with one, decrement, and `sth` back.
- Particle consumers require a separate 32-bit view of the same storage.
  `TSmallEnemy::decHpByWater` at 0x8006BAB8 and `TBossManta::receiveMessage` at 0x80112828 load the sender's offset 0x68 with `lwz`, then use it to index the water manager's arrays.
  The particle manager and Mario water-message sender assign that index; all existing sites now use `mParticleIndex`.
- Current reconstruction: an anonymous union at 0x68 containing `int mParticleIndex` and `s16 mWaterHitCounter`, preserving the 0x6C object size.
  This expresses the binary's two proven access patterns; the exact original header declaration, including whether different source headers disagreed, is not established.
  Do not globally narrow the particle index or reinterpret the countdown as a separate field at 0x6A.
- The named receiver constructor calls `THitActor(name)` and initializes only the counter halfword to zero.
  Both boss allocation sites reproduce the original vtable stores and `sth` initialization, at 0x800F432C and 0x800F4564.
  The default sender constructor leaves the payload initialization to the manager.
  Its original static initializer at 0x80280B04–0x80280B30 constructs the base and stores the same water-actor vtable without a payload store.
- All header consumers rebuilt in batch 14 with zero function regressions; existing particle-manager and enemy scores are unchanged.

## Boss parameter constructors and shared literals

- Status: both constructors exact and `BossHanachanSave.cpp` source-linked, batch 10.
- Search: `rg -n 'TBossHanachan.*SaveParams|PARAM_INIT' src/Enemy/BossHanachanSave.cpp include/Enemy/BossHanachan.hpp`.
- Common parameters: 29 `TParamRT` members, size `0x24C`; changeable parameters: 34 members, size `0x2B0`.
- Names are native DOL strings starting from rodata base `0x803820A0`; defaults and types come from constructor stores and vtable references.
- The common block supplies head/body collision dimensions, shadow sizes, and the signed-short motion-blend duration at `0x248`.
- Both constructors initially had correct initialization logic but incorrect string offsets: the original has 32 bytes of shared literals before the first parameter name.
  Including the existing `System/DummyStrings.hpp` restores them and makes both constructors exact, including the first constructor's scheduling.
- The original trailing four zero bytes in `.sdata2` are reproduced by linker alignment; no dummy data definition was needed.
- Full executable SHA-1 and byte comparison pass with the parameter object source-linked.

## Boss joint-matrix lookup: narrow the local index

- Status: verified shared correction, batch 10.
- Search: `rg -n 'CalcMtxPtrFromJointName' src include`.
- Original head/body constructor sites narrow the `JUTNameTab::getIndex` result to 16 bits before computing the matrix address.
- Use `u16 index = names->getIndex(name);` followed by `model->getAnmMtx(index)` in the existing helper.
  This restores its UNUSED 76-byte size and makes both derived constructors exact, including their stack layouts.
- Do not globally narrow `JUTNameTab::getIndex` or change the protected model accessor: this evidence belongs to the caller's local index.
- Completing the base constructor with existing model and blend helpers also emits the exact `CLBPalFrame<short>` implementation.
  The base constructor itself still has an eight-byte stack difference; avoid artificial padding.
- Both changed units and all header consumers rebuilt with zero function regressions in batch 10.

## Boss-part animation dispatcher and standing check

- Status: reconstructed, partially matching, batch 11.
- Search: `rg -n 'considerSetAnm_|isMarioOn_|isCurBckAlreadyEnd_' src/Enemy/BossHanachanParts.cpp include/Enemy/BossHanachan.hpp`.
- Original dispatcher: `0x800F3610`, 1,844 bytes; all six nerve states use an if/else chain.
  The current function is 99.6833%, with the same 461 instructions; its frame is 0xD0 rather than 0x110 and the blending branch has register/load differences.
- States 0/1 share the completion cases 5, 6, 13, 16, 17.
  State 1 additionally checks Mario standing on this part and motion blending.
- State 2 checks animation completion before its switch, including cases that do nothing (9/12).
  Preserve these calls and the two directional chains, 7→8→9 and 10→11→12.
- State 3 distinguishes a positive countdown becoming zero from a countdown already at or below zero.
  Do not merge these with the other timer branches.
- `isMarioOn_` returns `bool`, checks `SMS_IsMarioTouchGround4cm()`, then a non-null ground plane whose actor is this part.
  Reusing `SMS_GetMarioGroundPlane()` and `getActor()` reproduces the inline instructions and the map's UNUSED 100-byte size.
- Blending trial: `bool blending = isMotionBlending() || isForcedBlendRatio()` leaves the second helper out of line here (97.7%).
  Initializing true and setting false when both helpers return false inlines both and reaches 99.6833% without changing the shared NPC helpers.
  A ternary around the OR still leaves the call and adds another normalization group.
  Directly comparing the forced-ratio field removed the call but left scheduling differences; that trial was reverted.
- Stack trials: using `getMActor()` at each access in the completion and frame-copy helpers did not change their stack frames.
  Holding that actor in a local regressed both instruction sequences; splitting the completion-state OR into a named local also regressed the helper and dispatcher.
  All these trials were reverted; do not repeat them as presumed shared fixes.
- `BossHanachanAnm.cpp` was reconstructed in batch 12: its complete map inventory begins at line 59489 and includes four UNUSED wrappers.
  Its methods must be reconstructed together with the wrappers rather than omitting their inline context.

## Owner animation routines and timer families

- Status: all ten routines and four UNUSED helpers reconstructed in batch 12; eight linked functions exact after batch 13.
- Search: `rg -n 'setAnmTimerWhen|setTumbleBckRate_|setHeadAndBodyAnm|changeAnmRateAndFrameUpdate_' src/Enemy/BossHanachanAnm.cpp`.
- Timer loops use `u8` parameter values and signed integer indices.
  Damage/death timers are `delay * CLBAbs(mWeakBodyIndex - i)`, with the head using `mWeakBodyIndex + 1`.
  Both 332-byte functions match with the same loop form.
- Snort timers run from the head (zero) through body indices 0–7 with multipliers 1–8; all 120 bytes match.
- Get-up timers run in the reverse body order, then the head.
  Use an ascending loop `i = 0..7`, access `mBodies[7 - i]`, and multiply by `i`.
  Counting down the index and multiplying by `7 - i` leaves runtime subtraction/multiplication after unrolling (20.3%); the ascending form matches all 116 bytes.
- `considerSetAnm` (116 bytes) and `isFinishedGetUp` (92 bytes) match exactly.
  The latter waits for head animation 9 or 12 to complete.
- Preserve the mapped owner wrappers: frame copying (100 bytes), forced blend ratio (112), and clearing the ratio (116).
  All three reproduce their UNUSED sizes and inline into the main frame-update routine.
  The blend wrappers currently access the existing controller fields directly; investigate nested part helper boundaries when resolving the caller's stack gap.
- `changeAnmRateAndFrameUpdate_`: 99.89781%, 411 instructions.
  Current frame 0xC0 versus original 0x118; the first tumble-loop counter and head actor use r28 instead of r26.
  Walk/run cases are 0/1; preserve frame-copy calls and the separate ratio complements for previous-animation cases.
  The original calls `CLBCalcRatio<float>` out of line and updates animation sound before each model's `frameUpdate()`.
- `isAllBckAlreadyEnd`: 100%, 184 bytes, batch 13.
  Compare each compound condition explicitly with `false`: `(animationMatches && part->isCurBckAlreadyEnd_()) == false`.
  This preserves the original normalization groups and the 0x28 stack frame.
  Named `bool` head/body completion locals reproduce the normalization but enlarge the frame to 0x30.
  Direct negation removes groups; ternary boolean conditions add extra groups; splitting declaration from assignment has no effect.
  Exception: using the analogous `(isMotionBlending() || isForcedBlendRatio()) == false` in the parts dispatcher causes the forced-ratio helper to remain out of line and regresses that caller; reverted.
- `setTumbleAnm`: 100%, 420 bytes, batch 13.
  In the shared `setTumbleBckRate_` helper, use one float for the signed rotation difference, then `frames = (1.0f / unk198) * (frames >= 0.0f ? frames : -frames)`.
  Preserve explicit reciprocal multiplication and nested `2 * (40 * SMSGetAnmFrameRate())`.
  Both inline sites match with the original 0x60 stack frame.
  Separate distance/frames locals with `CLBAbs` produce a 0x78 frame; one float retaining `CLBAbs` produces 0x70.
  Direct ternary absolute value with one reused float resolves the inline stack difference; separate ternary and reciprocal assignments give the same instructions.
  Naming the nested frame-rate expression increases the caller frame to 0x80; ordinary division changes instructions and reduces similarity to 87.8%; reverted.
  The helper itself is now 180 bytes versus the UNUSED map's 176 (previously 172); do not claim its standalone body is exact from the matching callers.
- `setHeadAndBodyAnm`: 91.75%, correct 0x68 frame but register allocation and integer-to-float conversion slots/scheduling differ.
  BCK uses an integer remainder; BTP and BTK use a second conversion of that integer to a shared float.
  Preserve both conversions while investigating the original inline context.
  Removing the shared texture float and passing the integer to both texture setters regresses the routine to 74.1%, with a 0x60 frame and additional conversions; reverted.
  A separate BCK float local or splitting texture-float declaration/assignment does not change instructions.
  Moving texture-float initialization before the BCK setter reverses the two conversion slots to the original order but leaves scheduling/register differences (about 91.9%); reverted pending structural evidence.
  The frame controller's frame field is protected; direct field access is not a viable game-code fix, and no library change was made.
- All mapped constants (32 bytes) match; no source-link promotion until all linked functions and the whole executable are verified.

## Shared boss animation setter result: caller establishes bool

- Status: declaration and both implementations corrected together, batch 12.
- Earlier homogeneous `BOOL`/`bool` trials in the setters could not distinguish the return type.
  The newly reconstructed owner caller at `0x800F2ECC` uses `clrlwi.` on the low byte immediately after the virtual setter call, establishing the `bool` interface.
- Changed the base virtual declaration, both overrides, and both result locals to `bool`.
  Existing parts function scores are unchanged, and all header consumers rebuilt with zero function regressions.
- Rule: use a caller's result handling to resolve types when callee instruction streams alone are indistinguishable.

## Boss collision and foot initialization

- Status: both previously empty initialization routines reconstructed, batch 14; all parts map symbols now present.
- Search: `rg -n 'initMapCollisionAndHitActor_|initFootHitActor_|mFeet|mSLFoot' src/Enemy/BossHanachanParts.cpp include/Enemy/BossHanachan.hpp`.
- `initMapCollisionAndHitActor_`: 99.76471%, 544 bytes, with only stack offsets differing (0xE8 frame versus 0xF0).
  Start with body collision parameters and use the original signed actor-type switch to select the head values.
  Bind the `center` joint, initialize a moving map collision with flag 0x8000, allocate the named water receiver, register it with the actor group, enable collision, and place it at the joint translation minus its vertical offset.
- `initFootHitActor_`: 99.55238%, 420 bytes, same 105-instruction length as the target.
  Keep the two joint indices in an `int[2]`, narrowing each index to `u16` when fetching its matrix.
  Fetch foot parameters directly at the `initHitActor` call; their reference-returning `get()` accessors let MWCC hoist the four field addresses in the original order.
  The `foot_L`/`foot_R` local static array is eight bytes in `.sdata`; each string occupies eight bytes in `.sdata2` including padding.
  The current frame is 0xE0 versus 0xF0; name-table and loop-counter registers are swapped (r20/r19 versus r19/r20).
- Use `mMActor->getModel()` in these two initializers.
  `TLiveActor::getModel()` remains out of line here, adding calls absent from the target.
  Do not generalize this to the head/body constructors, whose original call sites use that out-of-line helper.
- Use `group->getChildren().push_back(actor)`.
  The existing fabricated `group->add(actor)` wrapper changes iterator inlining and regresses both functions (about 95.7% and 74.9%); reverted.
- Holding the four foot parameter references in separate locals before the loop alters register ordering; direct arguments are closer.
  Putting the parameter pointer before the model/name-table locals restores the original setup-load registers.
  Reading the foot matrix back from the assigned member adds a load absent from the target; keep the computed matrix in a local and use it for both assignments.
  Using an `int` joint local plus an explicit `u16` cast in the base initializer gives the same instructions as a `u16` local.
  Splitting the model/name-table chain and naming the base placement matrix do not resolve the remaining stack gap.
- The foot destructor and adjustment thunk are now emitted and match all 156 and 8 bytes.
  The only remaining parts map warning is the existing hit-predicate UNUSED size (208 versus 196); all presence/order/linkage checks pass.
- All `.rodata`, `.sdata`, and `.sdata2` bytes in the parts unit now match; `.data` remains incomplete.

## Boss sphere-chain movement and collision: batch 15

- Status: all seven retained strong functions and the UNUSED collision helper reconstructed in `BossHanachanSub.cpp`; five of eleven retained functions exact, with matching data sections.
- Search: `rg -n 'TSphereLink|TSpherePoint|BHSCalc|TWaterHitActor' src include`.
- Full map inventory: `BossHanachanSub.cpp`, lines 59420–59432; includes the UNUSED 188-byte `execMapCollision_` helper.
  `TSpherePoint` has only an empty constructor in the map; `TSphereLink` has its constructor, `moveHead`, `setDegreeZAndRevisionPosXZ`, and `execMapCollision_`.
  The class field names are inferred descriptions, not recovered original names.
- `TSpherePoint` stride is 0x2C: previous position at 0, current position at 0xC, velocity at 0x18, segment length at 0x24, and roll angle at 0x28.
  `TSphereLink` is 0x1C: u16 count at 0, point-array pointer at 4, velocity multiplier at 8, vertical position increment at 0xC, collision radius at 0x10, roll-to-displacement multiplier at 0x14, and head yaw at 0x18.
  Owner field 0x178 is a pointer, demonstrated by the `setDegreeZAndRevisionPosXZ` call at 0x800EEC20.
- Movement applies the vertical increment directly to each position, then adds its velocity; it pins the first point to the supplied head position and resolves walls/ground.
  Each following point is projected to its segment length from its predecessor and then checked against the map.
  The final pass derives damped velocity from current minus previous position and updates previous position.
  A near-zero displacement becomes the up vector; otherwise normalization calls `PSVECNormalize` directly.
- Angle conversions differ by context.
  The constructor truncates `yaw * (65536.0f / 360.0f)` to a short.
  Both BHS rotation helpers use the existing rounded `CLBDegToShortAngle`/`CLBRoundf<s16>` chain.
  Do not replace the constructor's truncation with the rounding helper.
- `BHSCalcCentrifugalForce` uses squared XZ distances and a 0.001 threshold.
  Existing `TVec3::sub(first, second)` reproduces the original scalar load/register ordering; explicit component constructor arguments do not.
  Its signed angle subtraction stays an int for the final sign test, but is narrowed to s16 before absolute magnitude is formed as an int.
  `CLBAbs<s16>` adds another narrowing after negation, unlike the original, and mishandles the magnitude of -32768; reverted.
  The final body is 99.023254%, with a 0x80 versus 0xA0 stack frame and one extra integer move.
- `BHSCalcRevisionDistXZByRotateZ` preserves both zero-valued products in the rotation matrix.
  Literal scalar `0.0f * sine/cosine` expressions are optimized away (89.7%); initializing a vector then reading its components preserves them.
  A single three-component distance vector and cosine-before-sine locals reach 94.02857%; one constant-load scheduling difference and a 0x50 versus 0x58 frame remain.
  Adding a second rotated vector inflates the frame to 0x70 without fixing scheduling; reverted.
  The shared roll-position caller is 98.19259% and inlines this helper.
- The constructor is 99.53333% with all 120 instruction positions accounted for; its frame is 0x98 versus 0xB8, and two multiplication operands are exchanged.
  Putting sine/cosine calculations inside the loop prevents the original hoisting and introduces fused arithmetic (81.2%); reverted.
  Replacing the two scalar offsets with a scaled vector changes saved-float allocation and sine/cosine ordering (98.8%); reverted.
  Splitting offset calculations into assignment and multiplication changes length's saved register (99.0%); reverted.
- `moveHead` is 85.69796%, with collision predicate inlining, vector-copy/scale context, registers, and stack layout unresolved.
  The current source emits the UNUSED collision helper at the map's exact 188-byte size, but **does not emit the mapped weak `isIllegalData` function** because it inlines it into the caller.
  This is an outstanding map-check failure, not a completed symbol inventory.
  Passing through `isLegal`, chaining it to `isIllegalData`/`checkFlag`, and rewriting the latter with explicit returns did not establish the required call; all shared-header trials were reverted.
  Explicit `inline` on the cpp collision helper dropped its UNUSED definition; reverted.
  Replacing the collision ternary with an equality-to-false expression reduced the UNUSED helper to 168 bytes and lost the original normalization sequence; reverted.
  Splitting the final velocity expression into named delta/velocity locals restores copies but inlines `scale` where the original calls it (83.5%); using compound multiplication does not restore the call; reverted.
- The original water receiver uses `gpMarDirector->isThing()` (modes 1, 2, or 4), not the broader talk/demo predicate.
  It accepts only the water-spray message, clears the counter during those modes, and otherwise invokes the 60-frame counter setter.
  Receiver, setter, water destructor/thunk, and point constructor are exact; all data sections total 232 exact bytes.
- Full affected rebuild and baseline comparison found zero function regressions.
  The full mixed executable passes byte comparison and SHA-1; no source-link promotion or gameplay validation was performed.

## Boss effects: shared layout and inline context, batch 16

- Status: all four routines reconstructed; particle loading and static initialization exact.
- Search: `rg -n 'TFootHitActor|mLegMtx|mSandPillar|emitCamShake_|emitParticle_|MsRandF|MsSqrtf' src include`.
- Complete function inventory: `BossHanachanEffect.cpp`, map lines 59671–59676, five linked functions and no UNUSED routines.
  Define the four methods in reverse order under deferred inlining; MSound headers generate the fifth (`__sinit`).
- Move the complete `TFootHitActor` declaration into `BossHanachan.hpp` for actual cross-unit use of offset-0x6C `mJointMtx`.
  This is a shared class-layout recovery, not movement of a known cpp helper to force inlining.
- Body offsets 0x14C and 0x150 are `MtxPtr mLegMtx[2]`: the effect loop indexes them by the foot number.
  Both constructor calls were changed together; the body constructor and other parts scores remain unchanged.
- Owner offset 0x19C is `MActor* mSandPillarActor`; offsets 0x1A0–0x1A8 are its position vector.
  The sand-pillar routine averages X/Z with the sand actor, takes its Y, emits particle 0x7E, updates the model base translation, and starts BCK 0x25/BTK 2/BRK 2.
- Foot-step frames are 14 and 34; snort-step frames are 21, 36, and 55.
  The special snort emission occurs at frame 134.
  All sixteen particle filenames/IDs are recovered from the binary and loaded through `SMS_LoadParticle`.
- Camera distance uses `MsSqrtf`, whose one double-precision reciprocal-square-root refinement and rounded float spill match the original instructions.
  Do not substitute the JGeometry float arithmetic or the MSL three-refinement sqrt.
  The distance ratio uses the existing out-of-line `CLBCalcRatio<float>` and inline `MsClamp`.
- Particle probability uses `MsRandF() < mChangeParams->mSLParticleProbability.get()`.
  The inline comparison binds the parameter address before `rand()` and reads its value afterward, matching the original.
  Direct `(1.0f / 32768.0f) * rand()` reads the parameter owner afterward and loses an instruction (96.7%).
  A named const reference restores the instruction shape, but the existing random helper expresses it without the extra alias and yields 98.038315%.
- Tumble/damage dust checks sphere point `i`, not `i + 1`, before binding to body `i`'s offset-0x154 position.
  Keep the shared local position vector and its integer-word assignment copies: they are present in both original loops.
- Remaining: sand-pillar frame 0x38 versus 0x40 (99.93507%); camera frame 0x60 versus 0x78, square-root spill 0x30 versus 0x40, and swapped first-loop counters (99.70303%); particle frame 0x80 versus 0xC8, register allocation, and `cmpwi` versus original redundant `extsh.` after loading the signed water counter (98.038315%).
  No artificial stack padding or fabricated getter was added.
- Reverted trials: one shared `int i` across loops did not recover the original particle registers and worsened camera allocation; explicit `(s16)` on the already signed water counter did not change the comparison.
  Do not repeat these as presumed fixes.
- All five effects map functions pass presence/order/linkage checks; parts also passes with its pre-existing UNUSED hit-predicate size warning.
  Full rebuild and baseline comparison report zero regressions; mixed executable byte comparison and SHA-1 pass.
  Two exact functions add 1,584 bytes; all 844 data bytes match; the object remains original-linked.

## Boss nerve transitions and US sound layout: batch 17

- Status: all seven states reconstructed; twenty of twenty-two mapped functions exact.
- Search: `rg -n 'TNerveBossHanachan|pushAfterCurrent|startAppearBalloon|changeTempo' src/Enemy include`.
- Full map function inventory: `BossHanachanNerve.cpp`, lines 59466–59488, twenty-two linked functions and no UNUSED functions.
  Define states in source order GraphWander, Tumble, Down, GetUp, Damage, Snort, Dead using `DEFINE_NERVE`.
  This emits strong functions, singleton guards/instances, destructors, and vtables in their original order.
  The existing MSound include sequence also matches the 764-byte static initializer.
- `spine->pushAfterCurrent(&NextState::theNerve())` reproduces the original inlined singleton initialization and bounded stack push at all six transitions.
  Return TRUE after scheduling the successor, even when the bounded push cannot append it.
- Walking initializes animation 0 on entry, executes walking, and schedules tumble when the fall check succeeds.
  Tumble initializes its animation on entry, slips until stationary and fully fallen, shows balloon 7, then schedules Down.
  Down waits `mSLDownFrames`; Damage also requires zero march speed and waits `mSLDamageFrames`; both initialize get-up timers before scheduling GetUp.
  GetUp randomizes the weak body and sets snort timers after recovery finishes.
  Snort returns to the initial recovery graph node after animation 14 finishes, then schedules walking.
  Death removes map collision once animation 15 finishes, guarding the action with live flag 0x40000.
- Tumble uses `0.0f == boss->mMarchSpeed`, `isTumbleCompletelyAllBody()`, and `SMSGetMarDirector()->getConsole()->startAppearBalloon(7, true)`.
  Current instructions match except for frame 0x38 versus 0x40.
  Direct `gpMarDirector` access produces frame 0x30; adding named console/director locals to the accessor chain does not increase 0x38 and was reverted.
  No artificial stack objects were added.
- Snort checks timer 200 and flag 0x20000 before clearing that flag and starting BGM 0x80010029.
  Health 2 changes tempo with (0,1); health 1 uses (1,1); preserve switch case order.
  Its frame is currently 0x28 versus 0x40, with two controller loads from 0x98 instead of 0x9C.
- Shared layout audit needed: `include/MSound/MSound.hpp` declares `MSModBgm* unk98` at 0x98 and `MSBgmXFade* unk9C` at 0x9C.
  Follow-up: batch 18 below corrects the US controller offsets through recovered game-side fields; the 0x94 ownership issue remains.
  Original `MSound` constructor at 0x800150BC stores the eight-byte tempo-controller allocation at 0x9C, and 0x800150D8 stores the four-byte crossfade allocation at 0xA0.
  It writes -1 as a word at 0x94 and zero as a byte at 0x98, then reads the byte to choose a water-filter value.
  Current `JAIBasic.hpp` has a u16 at 0x94 and ends before that byte; it explicitly notes uncertainty about which tail fields belong to MSound.
  Do not swap controller pointer types or use a raw-offset cast at the snort call site.
  Inventory the affected game-code fields/callers and respect the restriction on autonomous middleware edits.
- Full build and all-function baseline comparison pass with zero regressions; symbol map passes without warnings.
  All 444 data bytes match; exact code gain is 3,156 bytes across twenty functions.
  Mixed executable byte comparison/SHA-1 pass; the unit remains original-linked until both execute routines match.

## US sound layout: water filter, timer, and shared controller offsets

- Status: verified shared correction, batch 18.
- Inventory `MSound` fields across `src`, `include`, and the full `MSound.cpp` map entries before changing the header.
  Relevant direct consumers are `MSound.cpp`, `MSoundSE.cpp`, `MAnmSound.cpp`, `MSoundMainSide.cpp`, `MarDirectorDirect.cpp`, `MarioSound.cpp`, and `BossHanachanNerve.cpp`.
  Search globals `MSGMSound`, `gpMSound`, and accessor `SMSGetMSound()` as well as unqualified member accesses.
- In GMSE01, 0x98 is a byte checked for equality with 1 by `MSSeCallBack::setWaterCameraFir`.
  Declare it as `u8 mWaterFilterOverride` in the game-side class.
  Keep the original enabled/disabled filter store, then apply the override to 0x78; replacing both steps with a logical OR would change the instruction sequence.
  The restored 56-byte routine matches exactly.
- In GMSE01, 0x9A is a halfword written by every branch of `MSound::playTimer` and read by sequence callback case 15.
  Declare it as `u16 mTimerParameter` and update the seven writes, constructor zero, and callback read as one family.
  Existing non-US code continues using the base class's `unk94` through version guards.
  `playTimer` now differs only in stack layout (0x18 versus original 0x20), at 99.881355%.
- These actual fields place the tempo controller at 0x9C, crossfade controller at 0xA0, flags at 0xAC, camera array at 0xB0, camera sound handle at 0xC8, demo flags at 0xCC, and scene bytes at 0xD1/0xD2.
  Existing legacy field names and offset comments retain Japanese spelling; the header explains the US shift.
  Do not compensate with per-caller casts or swap controller types.
- Constructor evidence: 0x800150BC stores the tempo allocation at 0x9C; 0x800150D8 stores crossfade at 0xA0.
  Initialize the filter override and call `setWaterCameraFir(false)` after zeroing handles 0x7C/0x80, where the original inlines that filter check.
  The previous early filter store before `initDriver` is retained only for non-US builds.
  Constructor improves from 83.15311% to 86.89952%, with remaining register/stack and previous-voice-ID initialization differences.
- Unresolved: original constructor stores -1 as a word at 0x94, and `startMarioVoice` uses that word as the previous voice ID.
  Current protected `JAIBasic.hpp` declares a halfword at 0x94 and notes uncertain base/derived ownership.
  This batch leaves that middleware declaration untouched and records a constructor TODO; adding raw-offset casts or writing only the halfword would misrepresent the evidence.
- Remaining callback evidence from the complete original diff: case 40 has no US branch, while current source sets `unkD1` and returns zero.
  Follow-up: batch 19 below corrects these cases and also commands 123–126.
  Case 110 returns 0xFFFF for scene 8 with episode 6 or 1; current source checks only episode 6 and reloads the scene for the fallback.
  These behavior changes were not included in the layout batch; read the full diff again before the next focused edit.
- Results: forty-one improved functions across seven units, fifteen newly exact functions totaling 2,060 bytes, zero regressions including missing-function detection.
  Both boss Snort controller loads now match without changing the callers; its remaining frame difference is 0x28 versus 0x40 (99.9186%).
  Full build, `ninja changes_all`, and mixed-executable byte/SHA-1 checks pass; no source-link promotion.
- Map accounting: 67 MSound symbols, including nine UNUSED entries, were inventoried.
  Its pre-existing missing `getDistPowFromCamera(const Vec&)`, weak-order warning, and seven UNUSED size warnings are unchanged after this batch.
  Boss nerve presence/order/linkage passes.
  Measurements and exact-function names are in `progress/GMSE01-batch18.json`; logs and the full map inventory are under `build/GMSE01-*-batch18.*`.

## US sound distance and sequence-command routing

- Status: distance exact, callback behavior corrected with remaining instruction differences, batch 19.
- `MSound::getDistPowFromCamera(const Vec&)` is a 136-byte global function at 0x800151C8.
  Its only retained original caller is in `MoveBG/MapObjBianco.s` at 0x801C6D98.
  Declare/define the US method between `getDistFromCamera` and the constructor in source order, respecting the reversed emission order.
  The routine captures the camera-position pointer once, calls `powf` on y, x, then z deltas with exponent 2.0f, and sums x+y before the z call.
  The ordinary expression `powf(dx,2) + powf(dy,2) + powf(dz,2)` using global functions reproduces the entire original, including its 0x20 frame.
- Failed distance trial: `std::powf` wrappers delay x+y summation until after the z call, retain another floating-point register, and grow the frame to 0x38 (81.4%).
  Naming the x+y intermediate does not fix that scheduling while the wrappers remain.
  Do not replace the calls with multiplication or a vector squared-distance helper.
- Inventory exception: `MSMarioPosVolume::getDistFromMario` is an UNUSED helper in `MAnmSound.cpp` that is inlined in `MAnmSoundNPC::startAnimSound`.
  That original caller delays the sums until all three `powf` calls have completed, unlike the new camera helper.
  Its original also calls `std::sqrtf` out of line, whereas current source inlines it.
  Leave the animation helper unchanged until its own inline context is understood; a global replacement of `std::powf` is not supported.
- Sequence callback US dispatch: handled commands are 0, 1, 12, 13, 15, 20, 30, 110, 120, 121, and explicit 127.
  Commands 1 and 127 fall through to `JAIBasic::setParameterSeqSync`.
  Commands 40 and 123–126 also use the base callback, rather than the extra branches present in the non-US source.
  Keep explicit case 127: after the non-US-only labels are excluded, the original decision tree and its `bge` for values at least 122 emerge naturally.
- Case 110 loads the scene and episode bytes, returns 0xFFFF for scene 8 with episode 6 or 1, and otherwise returns the loaded scene.
  The complete switch-routing and scene-check instruction sequences now match.
  The function improved from 95.40247% to 98.75232%; remaining differences are a 0x58 versus 0x88 frame, register allocation, and one extra child-track pointer move in case 20.
- Reverted callback trials: direct `mChildren[i]->mChildren[j]` removed a narrowing instruction and reduced the frame to 0x50, lowering the score to 98.3%.
  Separating the child pointer declaration from its assignment did not resolve the register/frame mismatch.
  Keep the existing `getChild` chain until stronger evidence supports a change.
- Full build, all-function regression comparison including missing entries, `ninja changes_all`, and mixed-executable byte/SHA-1 checks pass.
  Exact gain: 136 bytes and one function, zero regressions.
  The MSound map now passes presence/order/linkage, retaining weak-order and seven UNUSED size warnings from the baseline.
  Several UNUSED bodies are still stubs despite being present; a map pass does not establish completion.
  See `progress/GMSE01-batch19.json` and the saved `m2c` draft `build/GMSE01/MSound-distance-batch19.c`.

## Boss main movement, manager, and collision reconstruction: batch 20

- Status: partial main-unit reconstruction; 21 linked functions and one UNUSED helper are present, fourteen functions exact.
- Inventory: the imported US map lists 32 functions including three UNUSED routines.
  The extracted original object contains 29 linked functions and 14,712 code bytes.
  Its actual `.text` extent is 0x3978; do not substitute the imported map's aggregate extent.
  The full map inventory and m2c draft are saved under `build/GMSE01-boss-main-map-inventory-batch20.txt` and `build/GMSE01/BossHanachanMain-batch20.c`.

### Shared fields and APIs

- Search: `rg -n 'TBossHanachanManager|mCollisionPosition|mPreviousLinearVelocity|mDeathSoundPosition' src include`.
- Manager size is 0x64: common parameters at 0x54, three change-parameter pointers at 0x58.
  Its resource table, constructor, destructor, collision flag, and clipping routine match exactly.
  Reuse `TModelDataLoadEntry`, `TModelDataKeeper`, `SDLModelData`, and the existing material/texture APIs.
- Boss offset 0x17C is the collision-adjusted position vector; `bind` updates it before ground/wall queries and copies it for the displacement correction.
- Boss offset 0x188 is the previous linear velocity vector; `moveObject` stores its three components from 0x94 before calling the base movement routine, and `execSlip` copies it with integer loads/stores.
  Preserve component stores in `moveObject`; replacing them with vector assignment would change the original instructions.
- Boss offset 0x1AC is the death-sound position vector; `execDamage` copies Mario's position and passes its address to sound 0x28E6.
  The three recovered vectors retain the original constructor initialization order.
- Reuse `TSpineBase::reset` and `setNext`, `TGraphTracer::setGraph`, `THitActor::onHitFlag`, and existing camera/rumble APIs.
  Damage disables head/body/foot collision on death and switches both parameters and graph for the remaining hit-point count.

### Loops, vector temporaries, and boolean results

- `getBodyMaxRotateZ`: direct `fabs(mBodies[i]->mRotation.z)` comparison followed by direct member assignment yields the original fully unrolled loop, all 264 bytes exact.
  Keeping a local `angle` eliminated eight native address calculations and dropped the score to 64.9%; a rotation getter with that local did not repair it.
  Reuse the matching helper in `execSlip`; its entire inlined eight-body scan matches too.
- `isTumbleCompletelyAllBody`: retain the explicit boolean ternary for the +/-179-degree test, a result initialized to true, and result assignment plus `break` when body rolls differ.
  Both boolean normalization stages appear in the original; all 252 bytes match.
- `execSlip`: store `direction.isZero()` in a boolean local before testing its negation.
  This reproduces the original `mfcr`/bit-extraction sequence.
  Copy the current position to a local goal, add the direction, then call `setGoalPath`.
  The compact `setGoalPath(mPosition + direction)` introduced an out-of-line vector-add call and a larger stack frame.
  These two changes improve slipping from 93.2% to 99.63793%; trig registers and two temporary-vector stack locations remain different.
- `bind`: the final displacement has its own vector local before adding the collision correction.
  The compact `(nextPosition - mPosition) + correction` omitted six original copy instructions.
  Restoring the named displacement improved binding from 96.5% to 99.55385%; stack locations and two argument loads remain different.
- Ground validity reuses the prior sphere-chain pattern: `ground && (ground->isIllegalData() == true ? false : true)`.
  It reproduces both native boolean conversions without changing the shared collision-data declaration.

### Rejected trials and remaining work

- Walking still differs in template `TVec3::set<float>` emission and squared-length inlining.
  Replacing `delta.squared()` with three calls to `CLBSquared` introduced three unwanted out-of-line calls; reverted.
  Replacing the explicit set with the vector constructor did not improve the instructions; reverted.
  Existing `direction.isZero()` in slipping already emits the native unfused squared-length sequence, so a blanket floating-point contraction change is not justified.
- Fall setup needs a named absolute rotation-change value before reading the fall-speed parameter, and a result variable plus loop break.
  It reaches 99.742424%; a floating-point operand order and stack frame still differ.
- Manager `loadAfter` is 99.74286% with only stack-frame differences (original 0x40, current 0x30).
  Boss construction is 99.82758% with frame differences; changing adjacent scalar fields to the recovered vectors did not remove those differences.
- Damage is 96.304344%: collision-loop registers, manager array-index addressing, and the 0x60 versus 0x30 frame remain different.
- Missing map entries: `init`, `throwMario_`, `perform`, `execHeadCalcAnim_`, `execBodyCalcAnim_`, local `CalcRevisionPosByRotateZ`, emitted `TVec3::set<float>` and `MsWrap<float>`, and boss destructor/thunk.
  The complete original 208-byte rotation-position helper and its m2c draft were read; implement it with its initialization/animation callers next.
  `perform` is 6,108 bytes and remains entirely absent.
- Validation: all affected consumers rebuilt, `ninja changes_all` and the all-function comparison show zero regressions, and the mixed executable passes byte/SHA-1 checks.
  Gain: fourteen exact functions, 2,364 code bytes, 220 data bytes.
  The main map check remains a recorded failure for ten missing symbols, with a weak-order warning and UNUSED `isCanWalk` size warning (164 versus 192).
  No source-link promotion or gameplay test.
  Measurements are in `progress/GMSE01-batch20.json`; full logs are `build/GMSE01-*-batch20.*`.

## Boss initialization, history layout, and animation helpers: batch 21

- Status: initialization, throwing, local rotation-position adjustment, and both UNUSED animation helpers reconstructed.
  The three linked additions cover 2,456 bytes but are not yet exact.
  Twenty-seven of the main unit's 32 mapped functions are now present.

### Body history layout correction

- Search: `rg -n 'unk120|mPreviousPosition|mOlderPosition|mPreviousRoll|mOlderRoll' src/Enemy/BossHanachan* include/Enemy/BossHanachan.hpp`.
- The previous three-vector interpretation starting at 0x120, 0x12C, and 0x138 was wrong despite an exact zero-initializing constructor.
  Initialization copies position into 0x124..0x12C and then into 0x130..0x138.
  Main-update instructions copy those same vectors forward, and separately copy roll at 0x13C to 0x140 before saving the actor's current roll.
- Correct layout: scalar 0x120 (meaning still unknown), previous position 0x124, older position 0x130, previous roll 0x13C, older roll 0x140.
  Updated the body constructor and fall check together; all existing function scores, including the exact 260-byte constructor, are preserved.
  The batch 20 reference to `body->unk138.y` is superseded by `body->mPreviousRoll`.
- Rule: zero stores prove field extent and initialization order, but do not establish vector boundaries.
  Copy sites and mixed scalar/vector operations provide stronger type evidence.

### Initialization and UNUSED animation helpers

- Initialization reuses manager/model-keeper, spine, graph, sphere-chain, and hit-actor APIs.
  Body names, head name, and enemy-group name were decoded from the original Shift-JIS resource strings.
- Reuse a single converted short yaw for both initial sphere-chain sine and cosine lookups.
  Calling `JMASin(mRotation.y)` and `JMACos(mRotation.y)` independently repeated the conversion; saving the angle improved initialization from 95.9% to approximately 99%.
  A reference to the head-length parameter reproduces its native address calculation; some float register/load differences remain.
- `execHeadCalcAnim_` builds the head's base transform directly, then calls `MActor::calc`.
  `execBodyCalcAnim_` builds a local matrix for each body, copies it to the model's base transform, and calculates animation.
  Both call local `CalcRevisionPosByRotateZ` before constructing the transform.
  Their out-of-line bodies inline the rotation helper, while their inlined bodies in `init` retain the original calls.
- The rotation helper uses `fabsf` when storing the absolute roll in a float local.
  Global `fabs` added a `frsp` instruction and inflated both UNUSED helpers by four bytes.
  With `fabsf`, their sizes are exactly the map's 280 and 368 bytes.
  This size agreement is not an instruction-match claim for dead-stripped code.

### Reusable trig lookup order and throwing arithmetic

- Declaring the cosine local before the sine local reproduces the native sine/cosine table register assignments in both `CalcRevisionPosByRotateZ` and `execSlip`.
  Verified both callers individually; slipping improves from 99.63793% to 99.89655%, leaving only two temporary-vector stack locations.
  The rotation helper remains 95.57692% with one load-order difference and final arithmetic registers.
- Replacing the rotation helper's temporary-X assignment with a three-component `set` increased its frame from 0x58 to 0x60 without resolving arithmetic differences; reverted.
- Throwing reuses `MsGetRotFromZaxisY`, rounded short-angle conversion, and `MsClamp`.
  The native sequence measures the signed short-angle difference, takes its absolute value as an int, scales it by 1/32768, and subtracts from one before applying throw parameters.
  Keeping the absolute result as an int preserves the 32768 result at the opposite-angle boundary.
- Global `abs` introduced an unwanted call; `CLBCalcRatio` also remained out of line.
  Separate normalized-difference and `1 - ratio` assignments reproduce the native unfused arithmetic better than one expression.
  An int ternary and `CLBAbs<int>` have the same essential branch behavior here; the final source reuses the existing helper.
  Throwing is 96.03125%, with remaining scheduling and stack differences.

### Validation and next work

- Full affected rebuild, `ninja changes_all`, all-function regression comparison including missing entries, and executable byte/SHA-1 checks pass with zero regressions.
  No new exact functions, data, or source-linked objects this batch.
- Main map failure is reduced to five missing symbols: `perform`, emitted `MsWrap<float>` and `TVec3::set<float>`, and boss destructor/thunk.
  Strong order and linkage are correct.
  The remaining UNUSED warning is `isCanWalk`, 164 versus 192 bytes.
  Parts map still passes with its prior trample-predicate size warning.
- Next: reconstruct the complete 6,108-byte `perform` using the recovered helpers and corrected history fields.
  Its resource strings must be included before judging initialization's final string offsets.
  Measurements and logs: `progress/GMSE01-batch21.json`, `build/GMSE01-*-batch21.*`.

## Boss main update and rendering, batch 22

- Status: all strong main-unit functions reconstructed; `perform` is 78.2685% and remains original-linked.
- Search: `rg -n 'perform|mPreviousPosition|mOlderPosition|mPreviousRoll|mOlderRoll|unk120|unk144|unk148' src/Enemy/BossHanachanMain.cpp include/Enemy/BossHanachan.hpp`.
- Evidence: original `perform` at 0x800EE624, 6,108 bytes, plus the full m2c draft `build/GMSE01/BossHanachanMain-batch20.c`.
  The full original function and initial instruction comparison were reviewed; current diff is `build/GMSE01-boss-perform-diff-batch22.txt`.
- Body history copies at 0x800EE930 confirm the batch 21 layout: previous position 0x124, older position 0x130, previous roll 0x13C, older roll 0x140, current centrifugal force 0x144 and previous force 0x148.
  The scalar at 0x120 stores the signed terrain/sand roll target.
  The wave calculation uses previous rolls from neighboring segments and mirrors the neighbor at each endpoint.
- The head-anchor offset in `perform` converts the yaw twice using `JMASin`/`JMACos`, with a parameter load between them.
  Keep the batch 21 initializer's single converted yaw: its original instruction sequence differs.
- Terrain probing rotates a 200-unit side vector, probes both sides 500 units above the body, and compares ground-height deltas.
  Construct the opposite vector from `(-side.x, -side.y, -side.z)`.
  Copying `side` then calling `negate()` introduces integer-word copies, forces both vectors to stack, and blocks the original scalar optimization.
  Named `f64 absoluteLeft = fabs(left)` and `absoluteRight` reproduce the original two `fabs` operations without narrowing or repeated absolute-value evaluation.
- Sand response compares horizontal distance to `CLBSquared(50.0f)` and zeroes the terrain roll for relative angles within 15 degrees of the front or back.
  Its direction is relative to the boss position, as the native subtraction at 0x800EEF24 shows.
  Outside those ranges the signed roll is 70 times `SMS_GetSandRiseUpRatio`.
- Collision loops at 0x800EF53C, 0x800EF5B4, and 0x800EF624 directly compare the actor ID to 0x80000001.
  Use `getActorType() == 0x80000001` at all three sites.
  The existing `isActorType` helper adds a boolean normalization group absent from these originals; do not change the shared helper globally.
- Head/body collision transitions at 0x800EF7F0 and 0x800EF8F4 have the opposite requirement: `(checkHitFlag(0x80000000) ? true : false)` restores a normalization group missing from a direct helper condition.
  While walking, collision actors and both feet set that flag, and the head/body map collision is removed.
  Otherwise the flag is cleared and the map collision is set up at its joint translation.
- Reuse `execHeadCalcAnim_` and `execBodyCalcAnim_`: both inline in the new routine with the expected calls to the local rotation-position helper and matrix functions.
  The render phases also submit shadows, damage fog, object drawing, and view calculations; the sand-pillar actor follows its live flag.
- The new definition emits the exactly matching 108-byte destructor, 8-byte adjustment thunk, and 276-byte boss virtual table.
  Its two Japanese shine/camera strings also improve initialization resource offsets (99.08876%).
- Initial draft: 72.4%; component construction, native actor-ID comparisons, hit-flag normalization, named absolute values, and direct squared wave parameters improve it to 78.2685%.
- Remaining: `getBodyMaxRotateZ` incorrectly inlines twice here, while `execSlip` correctly needs its inline scan.
  `MsWrap<float>` also inlines where the original calls its local 72-byte symbol.
  Do not globally disable these helpers' inlining without checking their other callers.
  The history loop fully unrolls eight iterations rather than the original four-iteration group repeated twice.
  Horizontal distance currently lacks the native fused operation; register and stack allocation differences remain.
- Symbol-map check: only emitted `MsWrap<float>` and `TVec3::set<float>` remain missing.
  Strong order and linkage pass; `isCanWalk` retains its UNUSED 164-versus-192-byte warning.
- Validation: full build, baseline/changes_all, all-function presence and score comparison with zero regressions, expected DOL SHA-1, and byte comparison passed.
  Added 116 exact code bytes/two functions and 1,144 matched data bytes; source linking remains 73 objects/76,468 code bytes.
  No gameplay test was performed.

## Shared horizontal walking checks and angle wrapping, batch 23

### Walking predicate inventory and original behavior

- Search: `rg -n 'isCanWalk|CLBSquared|2.5625f' src/Enemy src/NPC src/Animal include`.
  The two `isCanWalk` definitions are `TBossHanachan` and `TBaseNPC`.
  Both original inline sites construct `(target.x - position.x, 0, target.z - position.z)` and compare squared length against `CLBSquared(10.0f)`.
  Both UNUSED predicates are 192 bytes in the map.
- NPC evidence: original `execWalk` at 0x80216E68, predicate construction at TU offsets 0x7A0..0x824, and `.sdata2` constant `@2354` at 0x80415224 explicitly contains float 10.
  The previous full three-dimensional subtraction and 2.5625 threshold were incorrect for this US target.
- Shared change: invoke `.squared()` directly on the horizontal vector temporary.
  This keeps the vector in memory and reproduces separate component multiplies and adds before the threshold call.
  A named vector was scalarized, eliminated the zero component, and fused arithmetic across the threshold call.
  Applied to both predicates and the boss's separate 100-unit path-transition check, whose complete original diff independently supports the same arithmetic.
- Boss walking improves from 76.784% to 95.624%.
  NPC walking, including the additional yaw corrections below, improves from 89.435486% to 92.30645%.
  Both UNUSED predicates now reproduce the 192-byte map size.
  Size agreement does not prove their original instruction bodies, which were dead-stripped.
- Remaining shared issue: both original inline predicates call `TVec3::set<float>`; the current construction still inlines the setter.
  The boss's second, 100-unit check correctly needs the setter inline and now reproduces its arithmetic apart from stack locations.
  Do not globally disable setter inlining.
- Related candidate: `TNerveNPCGraphWander::execute` in `NpcNerve.cpp` uses a horizontal named vector plus an existing `(void)&local_58` workaround.
  Its function was not edited or fully investigated in this batch; compare its complete original before reusing the temporary-expression form.

### NPC yaw completion and vector copies

- Original NPC offsets 0x6A8..0x6AC subtract yaw and take its absolute value before wrapping to 0..360.
  Restored `fabsf(mRotation.y - angle)` before the existing `MsWrap` call.
  The exact original `fabs` instruction is restored without an extra narrowing instruction.
- Original offsets 0x5D0..0x5FC copy the direction through integer-word vector assignments twice.
  Changed the existing float-component `.set(direction)` to vector assignment, recovering one missing copy.
  The remaining copy and register differences are explicitly noted in source.
- Rejected: rewriting direction as `unkF4.getPoint() - mPosition` recovered both copies but introduced a non-native out-of-line `TVec3::sub` call.
  Kept the explicit in-place subtraction and the evidenced assignment instead.

### Existing angle wrapper and caller exception

- Search: `rg -n 'MsAngleWrap|MsWrap\(MsGetRotFromZaxisY' src include/MarioUtil/MathUtil.hpp`.
- In boss `perform`, use existing `MsAngleWrap(MsGetRotFromZaxisY(axis))` for the first body-yaw wrapping site.
  Its additional inline boundary retains the original call and emits the missing local `MsWrap<float>` with all 72 bytes matching exactly.
  `perform` improves from 78.2685% to 78.874916%.
- Exception verified against the complete diff: `TSphereLink::setDegreeZAndRevisionPosXZ` in `BossHanachanSub.cpp` correctly inlines its direct `MsWrap` call.
  Do not apply the main-update wrapper change there.
  The main update's three signed wrapping sites still inline incorrectly and need their own source-context investigation.

### Rejected construction and loop trials

- Explicit assignment or copy initialization from a constructed vector emits the missing setter call but adds a non-native integer-word vector copy; UNUSED predicate size becomes 216 bytes.
  Using `.set(temporary)` also retains the setter call but fuses the squared-length calculation incorrectly.
- Binding the temporary to a const reference, or naming its squared-length result, preserves the unfused arithmetic but does not recover the missing setter call.
  They add stack slots compared with the simpler direct-expression form and were not retained.
- Const-qualified named vector construction still scalarizes and does not help.
  Naming the boss's acceleration parameters leaves their reversed load order unchanged; reverted.
- Main-update history-loop experiments from the same batch were all reverted.
  Direct repeated `mBodies[i]` access partially unrolls the loop but adds non-native pointer reloads; a reference local or unsigned index still fully unrolls eight iterations.
  A do/while loop does not reproduce the original four-body group repeated twice using CTR.
  The original single-body-pointer loop remains the best evidenced source until its unrolling context is resolved.

### Validation

- Saved baseline at `211b726c`; full build, `ninja changes_all`, all-function presence/score comparison, direct DOL byte comparison, and expected SHA-1 check pass.
  Zero function regressions; gain is one exact function, 72 code bytes, and 56 matched data bytes.
- Both changed units retain one missing map symbol, emitted `TVec3::set<float>`.
  All present strong functions have correct order and linkage; all UNUSED function sizes now agree with the map.
  These are recorded map failures, not complete-unit claims.
- No new source-linked object or gameplay test.
  Measurements: `docs/progress/GMSE01-batch23.json`; original/current diffs, map checks, and regression results: `build/GMSE01-*-batch23.*`.

## Message utility source-link closure, batch 24

- Priority: close complete game files before pursuing isolated function scores, following the revised `DECOMPILATION_PLAN.md`.
- Status: both `MessageUtil.cpp` functions, totaling 968 code bytes, now link from source and reproduce the full executable exactly.
  This resolves the batch 2 deferral.

### Shared stream helper emission

- Search: `rg -n 'isNotDrained\(' src include/JSystem/JSupport`.
  The message parser was the only game-code caller of this fabricated wrapper; no additional source replacements remain.
- The original loop calls the stream's length and position virtuals through offsets 0x18 and 0x1C, then tests their difference.
  Calling inherited `getAvailable() != 0` reproduces the same instructions as `isNotDrained()` but avoids emitting the memory-stream getter overrides in this object.
- The former source emitted `getPosition__20JSUMemoryInputStreamCFv` and `getLength__20JSUMemoryInputStreamCFv` between the two message functions.
  Their original selected definitions belong to `MarioPositionObj.cpp` at 0x80280FD8 and 0x80280FE0.
  Selecting the earlier copies during the mixed link shifted `SMSMakeTextBuffer` by 16 bytes and broke the DOL.
  The inherited API fixes this in game source without changing middleware, linker rules, or symbol metadata.
- Rule: identical caller instructions do not prove equivalent whole-file emission.
  Inspect the object's additional weak definitions when an otherwise exact object shifts later code after promotion.

### Remove the existing parser padding

- Removed both pre-existing `char trash[4]` stack-padding declarations.
  The parser now uses the real `getPosition()` accessor instead of direct field access.
- Keep the block tag as `s32`, matching the return type of `readS32()`.
  Keep the INF1 entry size in a separate `u16 entrySize`, declared with the parser locals before the payload stream, and compute the message's `u32` entry offset after the four-byte skip.
  The original halfword load, multiplication, and stack-slot order support this separation.
- These changes reproduce all 744 parser bytes exactly with no dummy stack objects.
  `SMSMakeTextBuffer` retains its exact 224 bytes.
- Rejected: chaining the two header reads changes bound temporary offsets; a named DAT1 position intermediate adds a misplaced temporary; changing the data offset to `s32` also shifts temporary offsets.
  Declaring `entrySize` inside the INF1 case shifts the payload stream by four bytes, while declaring it before that stream matches the original layout.

### Validation and queue

- Baseline at `5d1b2705`; full build, `ninja changes_all`, all-function presence/score comparison, direct DOL comparison, and expected SHA-1 pass.
  Zero function regressions; the message unit's map check passes without warnings.
- Added `GC2D/MessageUtil.cpp` to `config/GMSE01/objects.json`: 74 source-linked game objects, 77,436 code bytes.
  Game code is 24.792425% matched and 2.7192852% source-linked; aggregate code is 38.422554% matched and 2.148763% source-linked.
  Aggregate exact code/function counts are unchanged because both functions already matched before the emission fix.
- Refreshed `docs/progress/GMSE01-completion-queue.json` from the final report.
  It lists complete-code candidates, one-function candidates, and 108 unlinked game objects with 1..2,000 unmatched code bytes (98,036 unmatched bytes total).
  Candidate scores do not establish promotion readiness; original relocation, data, map, and whole-link evidence still applies.
- Next Priority 1 candidates: camera interpolation, pollution events, director object creation, and target arrow, followed by the smallest single-function closures.
  Camera and pollution-event deferrals have been re-read; their existing layout/emission issues remain unresolved.
  No library object was promoted or edited, and no gameplay test was performed.
- Measurements: `docs/progress/GMSE01-batch24.json`; validation/diff logs: `build/GMSE01-*-batch24.*`.

## Priority 1 closure audit and airport sound, batch 25

- Read the [closure audit](progress/GMSE01-closure-audit-batch25.md) before revisiting small nearly complete files.
  It records full-file map gaps, original/current stack layouts, shared timer and camera-flag issues, and rejected source-shape trials.
- Restored airport's missing `0x484D` clear-sign sound through existing `MSound::startSoundSystemSE`.
  Searched other MoveBG callers; they already use this wrapper, so no mass edit was warranted.
  Removed two fabricated airport singleton getters in favor of the existing director accessor and pollution global.
- Airport watch improves from 79.88461% to 99.96154%; its passed camera flag remains at `0x34` instead of `0x3C`.
  The CLI rounds this to `100.0%`, but it is still nonmatching.
  Its pool constructor and `appear` UNUSED bodies remain undersized.
- Full build, baseline comparison of all 12,904 functions, changes_all, DOL byte comparison and SHA-1 pass; zero regressions.
  No exact-code gain or source-link promotion is claimed.
  Game code remains 24.792425% matched / 2.7192852% source-linked; aggregate 38.422554% / 2.148763%.
- Queue refresh excludes protected THPPlayer despite game-category metadata.
  Measurements: `progress/GMSE01-batch25.json`; validation logs: `build/GMSE01-*-batch25.*`.

## NPC color helper recovery and water-filter context, batch 26

- Restored `InitChangeOneColor_Base` and `InitChangeTwoColor_Base` from repeated branches in `NpcColor.cpp`.
  Their signatures come from the map; their compiled sizes match the UNUSED 48/80-byte entries.
  Three one-color callers and one two-color caller reuse them without changing runtime instructions.
  The map check now passes; the runtime frame remains eight bytes too large.
- Water-filter camera logic reuses existing `isDemoCamera()` and `getUnk124()`.
  These retain the original camera pointers across calls and restore register allocation and boolean groups.
  Declare inverse-view, translation and scale matrices before transform info to restore relative stack order.
  The 404-byte function improves from 78.38614% to 98.68317%; frame and one return branch remain different.
- Search covered existing demo-camera helpers and transform construction in MapObjWater/Shimmer.
  Shimmer has different surrounding rendering work and was not changed without full caller evidence.
- Read [batch 26 audit](progress/GMSE01-closure-audit-batch26.md) before retrying animation frame, ground-plane conversion, camera-code, splash-color or NPC-color trials.
  No stack padding or middleware edits were retained.
- Full build, changes_all, all-function comparison, DOL comparison and SHA-1 pass; zero regressions.
  Both changed units pass map checks without warnings.
  No source-link promotion or exact runtime-code gain is claimed.
  Game code remains 24.792425% matched / 2.7192852% source-linked; aggregate 38.422554% / 2.148763%.
