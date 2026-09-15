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
