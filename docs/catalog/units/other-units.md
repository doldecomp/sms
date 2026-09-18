# Other units: exceptions and rejected trials

Per-unit notes that do not generalise.
Search this file for the unit name before retrying a stalled function.
Scores are as of the batch noted and may be stale.
Longer evidence for batches 25-39 is in `docs/progress/GMSE01-closure-audit-batch<N>.md`.

## Camera

- **cameragc / CameraChange / cameralib sweep (2026-09-17):** chained assignment puts the inner `operator=` one level deeper (`mPreviousTarget = mCurrentTarget = mTargetBeforeFixedMode`: the ROM inlines the outer off the returned reference and calls the inner, weak `__as__13TTargetCamera`; one wrapper level above the statement flips it, `changeCamModeSub_` 89 -> 98). The depth-2 allowance measured **10** statements here (`checkStatusType` expands at 10, calls at 11). Per-call-site inlining is not `if`-nesting (a copy of `calcExternalData_()` outside all `if`s expands the same helpers). `inline` is *not* how retail got weak-and-not-inlined: adding it to `ctrlGameCamera_` (0x468) expands it into `perform` (95 -> 70); open. Factoring `CLBCalcNearNinePos`'s duplicated up/right transform into one helper: 37 -> 62. `mult33` writes over its argument (the ROM copies the input after `setRotate`); `identity33()` (9 stores) not `identity()`. MWCC extends `s16` returns at the caller: an `s16` accumulator gives the ROM's `mr r3, r5`. A named `const TCameraMapTool* prev = unk70;` was worth 8 bytes (qualifies "pointer locals are worth zero"). `getThing`'s `return unk8[unk4 - 1]` needs an explicit `const int* p;` if/else for the ROM's `subi` + shared `lwz 0(p)`. Fabricated `execLButtonCameraOnProc_` adds the level that makes `changeCamModeSpecifyFrame_` a `bl` at depth 4 while keeping it expanded at depth 2. Two missing behaviours restored in `execCameraModeChangeProc_` (`MSD_SE_SY_NOT_COLLECT` behind `checkFrameMeaning(0x4000)`; `CAMERA_MODE_FOLLOW_D` short-circuit). Open header items: `TMario::checkStatusType(s32)` must be callable from cameragc (five sites; forcing it gives unit 95 -> 97 and `isMarioReadyGun_` size-exact; the one-statement in-class body at depth 2 expands; three extra levels flip it); one-arg `TRotation3::mult33(TVec3&)` should forward as `TVec3 tmp(v); mult33(tmp, v);` (`CLBRotatePosAndUp` 73 -> 84); `MsClamp<f32>` needs to be out of line from `perform`/`changeCamModeSub_` (local 0x20 in both TUs). `CLBIsPointInCube`/`CLBCalcPointInCubeRatio`/`CLBCalc2DFPos` are pure 64/64/24-byte frame gaps.

- **CameraMode** (linked): current-mode check is an out-of-line call, previous-mode check an inline switch. Added a fabricated current-mode predicate analogous to `isLButtonCamera`, used only for the current-mode branch.
- **CameraTalk** (linked): existing Mario-angle accessor restores talk setup.
- **Sun bounds** (`sunmgr`, `lensglow`, `lensflare`): `isInBounds` via a const reference to the first position recovers `lfsu`/offset-4 accesses. Exception: the `sunmodel` self-call addresses members directly, with a named bound.
- **CameraSecureView, sunmgr**: accessor/ABS/predicate and stream-chain/array/loop trials failed (batch 32 audit).
- CameraWarp, area-cylinder, multiplayer-camera frame trials: batch 28 audit.

## Enemy

- **BossEel::init**: tooth-model stores use indices 0, 1, 2; eye/heart loader flag is 0x10240000; post-init collision uses virtual slot 0x1c (`setUpTrans`). A named skin-deformer allocation local and one shared resource pointer help. Open: frame 0x300 vs 0x310, eye/heart copy registers. Rule: inspect differing immediates, stack stores and virtual slots even at 99%.
- **SleepBossHanachan** (linked): fall nerve declares position before a named `BOOL` animation result via `getMActor`, assigning inside the success branch.
- **egggen** (linked). A vector `squared` method removes a required SDK call.
- **seal / effectEnemy**: see `../codegen-tells.md` bool rules.
- **hanasambo**: `TSamboFlower(const char* name = "サンボフラワー")` with `new TSamboFlower` in `createEnemyInstance`. The map lists the constructor as a weak out-of-line symbol (duplicated in `MarNameRefGen_Enemy`); spelling the name at the call site inlines it. Open: UNUSED `hide` is 0xb8 vs map 0x50 and `isUseCallBack` 0x150 vs 0x140.
- **Spider, Beam**: see vectors in `../codegen-tells.md`. Beam's `coneInPlane` UNUSED body must stay 348 bytes.
- **AnimalNerve**: two `MsRandI(hi, lo)` calls corrected to `(lo, hi)`. Open: frame 0x118 vs 0xE8.
- **popo / rocket**: the emit-matrix normalisation has a real bug that is in the ROM: the three zero guards are rotated one step against the divisors (`if (lenZ != 0) col0 /= lenX;` ...). Both objects' disassembly agrees; do not "fix" it. The two units share the nozzle-possession protocol and statics.

## GC2D

- **CardSave::waitForChoice** and siblings: `updateCenteredSize` sites use 0x18/0x14 (`mOffsetInterpolator`), not 0x30/0x2C. Naming width/height or expanding to `setPaneSize`/`setPaneOffset` regresses; `pos.set` and reversed additions don't fix particle scheduling. `CardLoad::selectFunction` accesses members 0x20 above the current layout; check the constructor first.
- **GCConsole2::load**: health-pane pairs use `i*2`, `i*2+1`. At 0xec04 `stwu` updates `unk2AC`, but the following blend calls use `unk2A0` (r25); only hide uses `unk2AC`. Default-constructed colour local and named texture allocation rejected.
- **SelectDir::rsetup**: explicit `TDStageDisp("<DStageDisp>", 0)` places the flag temporary better. Naming the three camera vectors before allocation regresses (98.4%). Open: frame 0x610 vs 0x648.
- **ProgSelect**: removed old selector padding (explained small regression). Hoisted controller booleans add normalisation; `u8` selection local no effect.
- **MessageLoader**: tag/length declarations before the outer stream, typed advancing INF1 cursor, named discarded entry-size read. Chained extractions and extra locals regress.
- **MessageUtil** (linked): tag `s32` (matches `readS32`), `u16 entrySize` declared with parser locals before the payload stream.

## Map

- **MapCollisionEntry**: name the translation-only flag in `move` and warp `setUp`; warp also declares its vector before the predicate and uses `set`. Remaining: see `../frame-gaps.md`.
- **MapCollisionPlane** (linked): fabricated `worldToGridIndex` computes `mOneOverScale * (v + mExtent)` into a named `int`. `MapObjPlane::depress` keeps the fractional API.
- **MarNameRefGen_Map** (linked): `TPollutionTest` inline ctor initialises only `TViewObj` (size 0x10); call with no name argument.
- **PollutionObj** (linked): ground-query pointer at slot 0x54 declared first; named `is_near` and centre-height results.
- **Water-filter / Shimmer**: reuse `isDemoCamera()`/`getUnk124()`; declare inverse-view, translation and scale matrices before transform info. Shimmer needs model `calc`, `viewCalc`, `entry` (vtable 0x10, 0x14, 0x0C).
- **SplashManager** (linked): colour at 0x54, by-value copy at 0x58 via a compound literal at `requestCol`; alpha named before vertex writes.
- **MapModel, MapXlu, PollutionPos, PollutionManager, MapObjWater, MapObjFloat, MapEventSirena**: trials in batch 30/31/33/37 audits. Sirena: named flag-manager result only partially shifts the slot.

## MSound

- **MSModBgm**: repeated zero-load mismatch survives bool/u8, integer-zero, assignment-order and early-return trials. `getTiming` optional-output behaviour lacks evidence.
- **MovieRumble**: `init`/`checkRumbleOff` share a missing pointer move inside `readCurInfo`; getter placement, validity locals, signed group, const pointer all fail.

## NPC

- **NpcAnm::npcWetting**: naming the sunflower predicate or final switch value doesn't fix registers and can grow the frame (0x178 vs 0x160).
- **NpcCallback**: `checkLiveFlag` inside the conditional bool; named Mario Y across the range call.
- **NpcColor**: direct two-colour arguments and named material lookup don't fix the frame.
- **NpcInbetween**: ratio accessor recovers float registers but grows the frame by 8.
- **isCanWalk / execWalk**: horizontal `(dx, 0, dz)` against `CLBSquared(10.0f)` (not 3D with 2.5625). `fabsf(mRotation.y - angle)` before `MsWrap`. Direction copied by vector assignment. `unkF4.getPoint() - mPosition` adds an out-of-line `sub`.
- `TNerveNPCGraphWander::execute` has an existing `(void)&local_58` workaround; not investigated.

## System

- **TTimeRec::startTimer/endTimer**: existing `JUtility::TColor` gives four-byte colour storage; convert before the null check and `OSGetTick`. Four-component overload loads the instance before constructing colour. Fixed livemanager/objmanager `perform` (both linked). TimeRec has three UNUSED stubs.
- **objmanager**: `stream >> capacity` then `initObjArray(capacity)`; direct `readS32` as the argument grows the frame.
- **SnapTimeObj**: packed-colour overload; open colour slot 0x34 vs 0x38.
- **Strategy / ObjHitCheck**: `s32` counter keeps the initial branch; ObjHitCheck already matches.
- **PerformList**: `load` differs by one stream-read slot; by-value iterator in `perform`.
- **MarioGamePad**: see padding list in `../frame-gaps.md`.

## MoveBG and Animal

- **MapObjAirport**: `0x484D` clear-sign sound via `MSound::startSoundSystemSE`; use the director accessor and pollution global. Open: passed camera flag 0x34 vs 0x3C; pool ctor and `appear` UNUSED bodies undersized.
- **MapObjBall**: per-call-site inlining, see `../codegen-tells.md`. Scores: `hold` 48%, `touchWall` 75%, `TBigWatermelon::touchActor` 61%, `TResetFruit::control` 52%, `receiveMessage` 69%.
- **MapObjPollution**: accessor/loop trial reached the right frame with wrong registers (batch 39 audit).
- **AnimalManager**: `loadSaveParams_` restored; named near-plane input for clipping.
- **EffectUtil**: missing one UNUSED definition; two `cross2` calls regress.
- **enemyMario / bgtentacle (batch 58):** `TEnemyMario::getStickPower()` (UNUSED 0x8) returns `0.0f` and `setStickToAngle` is `power * (JMASSin(angle) * getStickPower())`: a literal `0.0f` folds away, the inlined accessor keeps the product (the retail enemy Mario's stick is always zero). `initEnemyValues`' three if/else-if chains are `switch`es with grouped cases (`case 0: case 1:` first builds retail's `beq`/`bge` tree and puts `"/../map/pad/Setting.prm"` ahead of the param names in the pool). `matan(dz, dx)` with `dx` named first. `TBGTentacleMtxCalc::calc` subtracts `col3(i-1) - col3(i-2)`; the old `char trash[0x10]` is gone. `unk84 += v * (f32)i * 80.0f` by-value chain is worth 128 bytes of frame and two `scale()` calls (retail elides the first result into the named vector; open). Own-class accessor reads (`getState()`/`getOwner()`) are the frame lever for the 88-136-byte-short functions. Ruled out: by-value `TNode::getPosition()`, `char` replay letter (right `extsb`, permutes every register), `.sub()` for `-=`, `SMS_GetMarioPos()`. Open header items: `MAnmSound` dtor in-class (UNUSED copy in enemyMario), `M3UMtxCalcSIAnmBlendQuat` default ctor in-class (weak, `__construct_new_array`), `TVec3::cross()` store order (x, y, reload, z), and retail's `consider()` calling const `TGraphTracer::getGraph()` and `TPathNode::getPoint()` out of line (frame 0x250 vs 0x220; not the caller-size family). `calcAttackGuideAnm` and `MtxCalc::calc` share a residue: retail promotes the subtrahend's `.y`/`.z` into f30/f31 and re-reads only `.x`.
