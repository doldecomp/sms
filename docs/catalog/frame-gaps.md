# Stack-frame gaps

A function whose instructions all match but whose `stwu r1` frame size differs.
This is the single largest class of near-exact game code.

## What causes a gap (current understanding)

Read these in order; each one was checked before moving to the next.

1. **Raw `TParamRT` reads.**
   A `TParamRT<T>` is 0x14 bytes with its value at **+0x10**.
   If m2c shows a bare field read at an offset where `offset - 0x10` is a 0x14-aligned param slot, the original called `mSomething.get()`, and that accessor's temporary is the missing stack.
   In `tobiPuku`, reads at 0x33C/0x350/0x364 were params at 0x32C/0x340/0x354.
   Declaring them and calling `.get()` made `TNerveTobiPukuPitiPiti::execute` exact and cut `Bound`'s gap from 32 to 8 bytes.
   `TParamRT` members cannot be default-constructed: they need `PARAM_INIT` in the params class constructor, so declare the class properly instead of padding up to the offset.
   Generated `u8 unkNNN[...]` padding blocks hide params.
2. **Uninitialised locals the original declared.**
   A function that is N bytes short has N bytes of *uninitialised* locals in the original that the optimiser never touches.
   MWCC reserves their slots and emits no code for them.
   On `THitActor::calcEntryRadius` (a leaf with 40 unreferenced bytes of stack), adding an uninitialised `Mtx` grew the frame with no new instructions, and 40 bytes made it exact.
   Initialisation is what matters: `TVec3<f32> v(mPosition);` emits stores and ruins the match, while `TVec3<f32> v;` does not.
   Common sizes: `Mtx` 48, `TVec3<f32>` 12, `TQuat4<f32>` 16.
   Declare what the function plausibly needed (a scratch matrix for an unused branch, a vector computed only under a condition) **with a meaningful name**.
   That recovers source; `char pad[40]` is the prohibited trick.
   Adding N bytes of *any* local fixes an N-byte gap, so the byte count confirms the size, not the declaration.
   `egggen` and `CameraTalk` were closed this way because the recovered locals were ones the code plainly wanted (a scratch position next to a distance test; a previous target/yaw pair in a camera).
   If the byte count is the only evidence, leave the gap and say so.
3. **Missing inlined helpers.**
   MWCC reserves an inlined function's local slots even when its values live in registers, so a caller missing an inline can be instruction-identical with a short frame.
   Also see `AGENT_MATCHING_TIPS.md`: "Each inline level in a call chain leaves one dead 4-byte temporary" and "`T x = f();` costs one more stack object".
4. **Inlined accessor reads.** Each member read that goes through an inlined accessor leaves one dead 4-byte temporary (measured on `TMapCollisionMove::init`: one use 0x28, two 0x30, four 0x38 = target).
   Reading members through their accessors is therefore a first-class fix for small gaps, and the accessor can also fix load order (`allocCheckData(getUnkC())` loads the count before `gpMapCollisionData`; the raw field swaps them).
   Scalar naming saturates: on `MSBgm::init`, naming the `new` result, the loop and the table owner reaches 0x40 and a fourth scalar reuses a slot, so the last 8 bytes of a 32-byte gap need an aggregate or address-taken local.
5. **Named locals and statics.** Cases where the fix was a source-level declaration:
   - `rocket`: `unk18[i]` -> `getObj(i)` alone closed `TRocketManager::initSetEnemies`; `getManager()` over `mManager` and `getSaveParams()` over `mSaveParams` each bought 8 elsewhere, but `getSaveParams()` in `getGravityY` broke a match, `getObjNum()` in a loop bound moved the wrong local, and naming a `TMsRange::rand()` result shrank the frame by 8. Measure per site.
   - `fruitsboat`: accessor temporaries stop accumulating after two or three uses (`init` stayed at 0xf0), and `setGroundCollision` needed **two** named locals (`J3DModel* model` and `MtxPtr mtx`): `mtx` alone put the fetch after the null test.
   - Ruled out: `MsAngleDiff(const f32&, f32)` helps `fruitsboat::moveObject` (+0.3) but regresses nine functions in six units (`getRotationFlyToDir` 100 -> 95.5, `turnToCurPathNode`, `calcRideMomentum`, ...); spelling it out as `rot.x - MsWrap(...)` is worse. The pitch block's reload of `rot.x` after `MsWrap` is unexplained. `std::min`/`max` landed (see `codegen-tells.md`).
   - `TUtil<f32>::one()` is a +4 lever: an inlined call returning a constant binds a 4-byte temporary a literal does not (`mQuat.set(0, 0, 0, TUtil<f32>::one())` took `TKazekun::reset` to exact; +8 on two `set()` fallbacks with no instruction change).
   - `GCConsole2::countShine` (0x228 vs 0x2A8): the ROM's frame has an 80-byte hole between a dead `JUTRect` and the five inlined `changeNum` locals, exactly one extra 16-byte rect per expansion, plus 48 below; that register pressure is why the ROM recomputes `value % 10` at two sites instead of parking it. It also confirms the dead-initialised-local pattern (`JUTRect rect(0, 0, 0, 0)` nothing reads, `bl JUTRect::set` with four zeros).
   - Measured correlation, not yet a cause: each `theNerve()` expansion beyond the first leaves 16 bytes we do not reproduce (`attackToMario` 2 -> -16, `bind` 3 -> -32, `TNervePopoPossessedNozzle::execute` 2 -> -16).
   - `bombhei`: the accessor step is **8** and a named call result is **16**. `p->mSLFoo.get()` instead of the params class's `getSLFoo()` wrapper is one inline level shallower (-8); a named `f32` holding the fetch is +16. Since frames are 8-aligned, a -8 with a +16 is often the only way to land (`TBombHei::getGravityY`, `TNerveBombHeiExplosion::execute`). Named **pointer** locals (`MActor*`, `MtxPtr`, `MSound*`, `getHolder()`) are worth zero; only a local whose initialiser carries a conversion takes a slot, and `getHitPoints()` over `mHitPoints` or `SMSGetMarDirector()` over `gpMarDirector` are real +8 levers.
   - `yunbo`: naming a param fetch used in a comparison (`s32 recoverTimer = ...->getSLRecoverTimer(); if (recoverTimer < spine->getTime())`) added the missing 8 bytes of `TNerveYumboFreeze::execute`; declaring a scalar *before* the vector it is compared against fixed `TNerveYumboAttack::execute`, because an inlined helper's locals lay out in reverse, so the inline site and the UNUSED out-of-line copy want opposite orders (declare for the inline site; the size check still passes).
   - **Aggregate locals cost a slot per inline expansion; scalars mostly do not.** Measured on `JGeometry::TQuat4<f32>::rotate`: its two `TQuat4` temporaries cost 32 bytes per expansion, `vx/vy/vz` 4, `w/z/y/x` none. When a frame is over by a multiple of a class size, look for an aggregate temporary in an inlined callee. Rewriting `rotate` with seven `f32` locals makes `TYumbo::shotSeeds`' frame exact (95.7 -> 96.7) but renumbers float registers at six other sites (1 improvement, 6 regressions), so it stays as is; the trial table is in the header's TODO. `shotSeeds`' residual is a `v.z` load retail parks in `f31` across `matan`/`sinf`/`cosf`, pointing at `MsGetRotFromZaxisY`'s `const&` parameter, not the quaternion.
   - Before rewriting a header inline, check for a retail out-of-line copy: `rotate` is weak in `Kumokun.o` (0x80117d08, 0x98, a zero-frame leaf) and our body reproduces its 38 instructions. Grep the map with the full mangled stem (`rotate__Q29JGeometry9TQuat4<f>`); the shorter `TQuat4` stem finds nothing and reads as "never emitted".
   - `NpcAnm::sunflowerReviving`: `int kind = unkD0->getCurrentAnmKind()` named inside the flag guard fixed 0x20 vs 0x28.
   - `TMarDirector::loadParticleMario` touches only globals; declaring it `static` removed the implicit `this` slot and fixed 0x20 vs 0x18.
     An unused `this` alone does not prove a method is static; check callers and member-pointer use.
   - Named `SMSGetMarDirector()->getConsole()` result (HelpActor, EventWatcher) and a named `TFlagManager::getInstance()` result (SleepBossHanachan `startFall`) restored frames.
   - `MapCollisionPlane`: computing into a named `int` and returning it gave frame 0x88; returning the float helper directly gave 0x80, naming its int result gave 0x90.

Hypothesis 2 superseded an earlier conclusion (batch 61) that gaps were opaque compiler artifacts to be treated as blocked.
It also explains why adding *initialised* copies to `TEggGenerator::control` (frame 0x38, 55.1%) looked like evidence against the locals theory.

## Two regions, and which levers feed which

Measured on the Player sweep. The gap splits into the **low region** (outgoing-argument and inline-temporary slots, growing up from 0xc) and the **named-local region** (growing down from the top). Every referenced slot's distance from the frame top tells you which region is short: if all slots share the frame delta, the whole residue is low and accessor levers work (`soundMovement` 0x100 -> 0x1b0, `MarioCap::perform` 0x140 -> 0x178, `MarioMain::perform`); if slots in the middle are missing (`receiveMessage`: 68 bytes between `diff` and the first `tmp`, 48 more below the `tmp` pair, while the low region is only 44 short), accessors push the low region *over* while the middle stays empty. Measure both before applying any lever; the full table is in `MarioReceiveMsg.cpp`'s TODO.

Scalar accessor levers saturate: `getStatus()`, `getIntendedMag()`, `getHeldObject()`, `getHolder()`, `getGroundPlane()`, `getColNum()` are +8 each, any pair +16, a third nothing (exhaustive on `waiting`). Indexed accessors do not saturate: `getCollision(i)` over `mCollisions[i]` is +56 in `checkCollision` (one `hitHipDrop(mCollisions[i])` must stay raw or the base lands in `r3`); `&mYoshi->getTranslation()` over `&mYoshi->mTranslation` is +112 in `soundMovement` across ~30 sites with no instruction change.

Anti-levers (measured): an accessor for a raw `unk114 &` test materialises a bool retail lacks; `getHealth()` as `startMarioVoice`'s argument hoists the `lha` above the vtable load; a named `s16` for `IConverge`'s result adds a sign extension; naming a call result is worth zero once scalars saturate.

## Diagnosing

- Validate first with a temporary `volatile char trash[N]`.
  If the function reaches 100% with no instruction differences, the body is correct and only the frame is wrong.
  Never commit the padding.
  `TNerveSealWait::execute` validated this way.
- A diff whose only differences are the `stwu` and save/restore offsets is not an instruction problem: do not rewrite the body.
- Leaf frame arithmetic: locals start at `0xc(r1)` (back chain, LR slot, a dead word at 0x8) and `frame = align8(0xc + locals + 1)`, so 36 bytes of locals give 0x38 and 40 give 0x40.
- Two local regions grow in opposite directions: inline-expansion temporaries grow **up from 0xc** in expansion order, named body locals grow **down from the top** in declaration order. So once the temporaries' offsets are exact, the last 4-8 bytes can only come from a named local, typically `T x = <inlined call>;` (`THitActor::calcEntryRadius`: `f32 diag = HitActorFastSqrt(rad2)` took 0x38 to 0x40 without moving anything else).
- To locate *where* the missing locals were declared, put the temporary `volatile char trash[N]` at the **end** of the function (last-declared locals get the lowest offsets), so the named locals land on the target's offsets, then compare the two sets of `r1` displacements; each divergence is a local the original declared there. Inline-expansion locals are prepended, so they sit below everything the body declared: a slot lower than later-declared locals belongs to an inlined helper (`FeetInvCalc`'s cross-product vector).
- Frame gaps are a source property, not the build: `-prefix`, `nostrength`, `,p`, `-inline` variants, `noschedule`, `-sym on` and `-O3,p` all left `calcEntryRadius` at 0x18 (target 0x40); only `-O0` changed it, and changed every instruction too.
- Gap sizes do not correlate with call-site count, nerve pushes or `startSoundActor` within a file (seal: `perform` -8, `SealSleep` -8, `receiveMessage` -8, `SealWait` -16, `SealDie` -32, `init` +8).
- Scripting: `-d` with a symbol that is not present prints `Symbol not found`, which a marker count reads as a perfect match; confirm the header shows a real percentage.
  In Python, `''[:1] in "|<>"` is `True`; use `l and l[0] in "|<>"`.

## Worklist

`docs/progress/GMSE01-frame-gaps.md` lists 138 game functions (182,932 bytes) that are instruction-identical and differ only in frame size.
Regenerate it by scanning the 99-100% band, discarding anything with a `|`, `<` or `>` marker, and comparing the two `stwu r1` operands.

Units one function short of source linking (last checked 2026-09-16; `egggen` and `CameraTalk` have since linked):

| Unit | Blocking function | Notes |
| --- | --- | --- |
| `Player/MarioAccess` | `SMS_IsMarioOnWire` | Not a frame gap. The original loads `mHolder` twice (r0 to test, r3 to dereference); MWCC CSEs it. `&&` (93.8%), separate null check (90.0%), assigning the comparison (54.7%) and casting the holder (93.8%) all fail. |
| `Strategic/HitActor` | `THitActor::calcEntryRadius` | 40-byte gap plus register numbering. The `frsqrte`/`frsp`/`stfs`/`lfs` sequence has no Newton step, so it is **not** `JGeometry::TUtil<f32>::sqrt`. |
| `MSound/MSoundBGM` | `MSBgm::init` | Body exact (padding of 25-32 bytes gives 100%). Scalar naming ceiling 0x40 vs target 0x48; needs an aggregate local with no evidence for one. All trials are in the source TODO. |

Also validated: `JGeometry::TQuat4<f32>::slerp` is exact modulo one 16-byte aggregate with no evidence (`= *this` + `normalize()` and the constructor form both give 35%).

Closed since: `Map/MapCollisionEntry` (accessor reads, linked), `Enemy/egggen`, `Camera/CameraTalk` (uninitialised locals, linked).

## Ruled out

Do not retry these.

- `TSolidStack::push` taking `T` instead of `const T&`: frame unchanged, **65 regressions** against 1 improvement. Always `ninja baseline` before touching a shared header like `SolidStack.hpp`.
- Holding the nerve in a local before `pushAfterCurrent`: frame unchanged, 99.9% -> 98.0%.
- Pushing a nerve correlates with a gap but does not set its size.
- Expanding `SMS_LoadParticle` calls directly (loadParticleMario): no frame change. Do not mass-rewrite its 29 callers on this evidence.

## Pre-existing padding

These inherited functions reach their score with fabricated padding, so their scores are not evidence of a reconstruction:
`GC2D/SelectMenu.cpp` (`volatile s32 const unkArr[]`), `System/MarioGamePad.cpp:16` (`u32 stackAlloc[83]`), `Enemy/bgtentacle.cpp:204`, `MarioUtil/MathUtil.cpp:236,297,323`, `Player/MarioDraw.cpp:472`, `Enemy/graph.cpp:55`, `Map/MapData.cpp:11`.
`TMarioGamePad::updateMeaning` also has a genuine r4/r5 swap (old button bits vs loop counter) that is fixable on its own.

## Accessor ladders measured on zero-delta functions (batch 56)

- `TMario::barClimb` (instructions exact, 0x60 vs 0x80): null test plus two `getActorType()` receivers via `getHolder()` = +8; `SMSGetMarDirector()` over `gpMarDirector` = +8; the `.x`/`.z` copies plus `mPosition.y - holder.y` together = +8 (saturating); the five `mPosition.y + mHolderHeightDiff` sites the remaining 0x18. Routing everything through `getHolder()` lands 0x80 exactly but flips the `fadds` FPR pairs. `mHolder->getPosition().y` is +16 per site.
- "+4 low / -4 named" family (frame total right, one local 4 bytes too high): `TWaterGun::setBaseTRMtx` (scratch `Mtx` 0x20 vs 0x1c) and `TMario::doRoofMovingProcess` (`newPos` 0x38 vs 0x3c). Model-fetch ladder in `setBaseTRMtx`: `mFluddModel->mModel` 8, `getModel()` 12, `mFluddModel->getModel()` 20, forwarding `getModel()` 24; retail wants 16. Retail's 4-byte hole is alignment padding under the double-conversion temporaries, not a local.

## The "one 8-byte object" family (closure batch 60)

When every accessor and scalar lever saturates and the frame is still 8 bytes short with all instructions intact, the last 8 bytes come only from an 8-byte aggregate or `f64` (`MSBgm::init` 0x40 -> 0x48 with `u32 arr[2]`/`char buf[8]`/`f64`; `TMapObjWave::perform` 0x38 -> 0x40 with `f64 unused`). Scalars saturate at three in `init` and the whole accessor chain saturates at 0x38 in `perform`. Candidate sources still to be probed: the 8-byte magic temporary of an integer-to-float conversion inside an inlined body whose result folds away, a 64-bit intermediate, a two-word struct passed by value. `MSBgm::init` has no string pool and its closure lists only `JALList<MSBgm>::JALList`, so the object is not a name buffer.

## Closure batch 63: six units closed by slot-map triage

Read the offsets of every named local before choosing a lever: a delta shared by every slot is low-region (accessor levels apply); a hole between two named locals is a missing named scalar. Measured steps: `getModel()` +8 (worth 0 in MarioBlend: per site); an indexed accessor on an array-of-structs member +16 per use (`M3UModelCommon::getMtxCalcBasicAnm`, `M3UModelCommonMario::getMtxCalcSIAnmBQ`; two of three sites was exact, all three overshot); `const TVec3&`-returning `getRotation()` +12; a global-pointer accessor stacked on a member accessor +16 where the member accessor alone is +8/+12; a named intermediate for a cast (`TCubeGeneralInfo* general = &(*list)[i]; TCubeCameraInfo* info = (TCubeCameraInfo*)general;`) +4 low with identical instructions; named `f32` locals +4 each in the named region. Levers saturate per region: a fourth same-class lever adds nothing once three are in place, but a level on a different class or a second level on the same chain keeps accumulating (`controlByCameraCode_`: five levels for 40 bytes). `M3UModel::updateInMotion`: `getFrameCtrl()` for the reference is +8 but turns retail's `mr r3, r25` into `addi r3, r25, 0` (a receiver copy spelled `mr` came from a plain variable, `addi` from an accessor's returned address).

## Closure batch 61: named-slot and low-region rules

- **Naming a discarded `new` result reserves a named slot**: `TObjHitCheck* objHitCheck = new TObjHitCheck();` (self-registering ctor, variable unused) closed `TStrategy::load`. A hoisted declaration that is later assigned reserves nothing; only an unused/uninitialised one does (measured twice).
- Named locals are allocated above inline-expansion temporaries, so a short low region can never be paid for with a recovered local (`TAirportEventSink::watch`).
- Inline-expansion temp cost is per parameter, not per call: `this` or one parameter +8, two parameters +16, return value free (`TNpcInbetween::execPosInbetween`, a pure FPR permutation whose fix costs 8 bytes a leaf frame cannot hold; open).
- Ladder for `gpPollution->getLayer(i)->startDecay()`: `startDecay()` +4, `getLayer(i)` +4, `getJointModel(i)` +12 (an indexed one-parameter accessor is worth three slots where a 0-parameter one is worth one; two stacked 0-parameter levels regress). A TU-local `SMSGetPollution()` over `gpPollution` is +4 and saturates; `SMSGetMSound()` +4; `SMSGetMarDirector()` 0; a named `TFlagManager::getInstance()` result +4.
- `JDrama::TFlagT<u16>` is passed by invisible reference: the `stw rX, 8(r1)` at a `fireStartDemoCamera` call is the address of a 2-byte temporary; the unnamed `TFlagT<u16>(0)` form puts it at the top of the temporary region, a named local adds 8 and puts the copy 8 lower. `MapObjAirport`/`MapEventSirena` `watch()` are 12 bytes short (24 in `TMapEventSinkInPollution`/`Bianco`), all inside the pollution-layer chain; the `getJointModel` level that completes them breaks the linked `Map/MapEvent` when applied globally.
- `TSnapTimeObj::perform`: the `startTimer(u32)` overload needs a named `TTimeArray* timeArray` (+4 slot, but +8 frame) with a compensating -8; `endTimer` is right as it stands (changing it moves the linked `TLiveManager::perform`).
