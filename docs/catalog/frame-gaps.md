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
   Scalar naming saturates: on `MSBgm::init`, naming the `new` result, the loop and the table owner reaches 0x40 and a fourth scalar reuses a slot, so the last 8 bytes of a 32-byte gap need an aggregate (see [The last 8 bytes](#the-last-8-bytes)).
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

## The last 8 bytes

Once every accessor and naming lever has saturated, a residual **8**-byte gap is
one 8-byte stack object at the very **bottom** of the local area, below every
referenced local.
Positional signature: each `r1` displacement in retail is exactly 8 higher than
ours, the LR slot included.
Confirmed on `TModelWaterManager::drawWaterVolume` (0xa8 vs 0xa0: retail
44/48/51/.../88/144/172, ours 36/40/43/.../80/136/164).
`MActor::MActor` is *not* this shape - its low group moves one way and its top
group does not move at all, so that gap is two objects, not one.

Three spellings put exactly that object there with **zero** instruction change.
All measured in a scratch TU with the game flags, against a referenced `f32
marker[3]` used to read off the insertion point.

| variant | frame | insns | where the 8 bytes land |
| --- | --- | --- | --- |
| baseline | 0x20 | 37 | - |
| `u32 tail[2];` / 2-word struct declared **last** in the body | 0x28 | 37 | below `marker` |
| 8-byte class with a user ctor/dtor, local of an **inlined callee** | 0x28 | 37 | below `marker` |
| same 8-byte local declared **first** in the body | 0x28 | 37 | *above* `marker` (wrong shape) |
| `f64 tail;` declared last | 0x30 | 37 | +16, not +8 |
| 2-word struct **by value** to an inlined callee | 0x30 | 37 | +16 (copy + source) |
| 2-word struct **returned** by an inlined callee | 0x30 | 37 | +16 |
| `T x = <inlined call returning 2 words>` | 0x38 | 37 | +32 |

So: to add 8 and nothing else, declare a two-word **aggregate** (array or
struct) as the **last** local of the body, or give an inlined callee an 8-byte
**non-trivial** local.
Do not measure with `f64`: it is 8-aligned, so it is worth 8 only when the
bottom of the local area already happens to be aligned and 16 otherwise.

### What is worth nothing (do not retry)

- **Any scalar or trivial POD local of an inlined callee.** `u32`, `f32`,
  `f64`, `u64`, `u32[2]` and 4/8/12-byte structs with no user ctor or dtor are
  all +0 there; MWCC drops them. Triviality is the whole switch: the same
  struct with an empty `~T() {}` is +8. (In the *caller's* own body the
  opposite holds - triviality is irrelevant and only the size counts.)
- **A 4-byte local anywhere.** The base frame already carries 8 bytes of slack
  at the bottom, so the first 4 bytes of locals are free.
- **A dead int-to-float conversion.** `f32 x = (f32)someU32;` inside an inlined
  callee whose result is unused is +0. A *live* `u32`/`s32` to `f32` or `f32`
  to `s32` conversion is +8, but its `0x43300000` pair sits at the **top** of
  the local area and it costs instructions, so it can never be the silent
  residue. Hypothesis refuted.
- **`sqrtf`, `sqrt` and `f32 * 1.0`**: +0. Only the double `fabs()` of a float
  adds 8 at the bottom, and it costs an instruction.
- **An extra inline level once the ladder has saturated.** A TU-static
  `getMapNo()`/`getDirector()` forwarder above `SMSGetMarDirector()` buys
  nothing and emits a stray symbol.

### The two closure cases, measured exhaustively

Neither has any evidence for an 8-byte object, and retail references no stack
slot in either, so there is no positional evidence to name one. Both stay
nonmatching; the trial tables live in the source TODOs.

- `TMapObjWave::perform` (0x38 vs 0x40). Each
  `SMSGetMarDirector()->getCurrentMap()` use is +8 and saturates at three uses;
  `gpMarDirector->getCurrentMap()` and `->mMap` are +0. Two uses is 0x38, three
  is 0x40 but 36 instructions instead of 34. A `switch` on the map is 38
  instructions. 35 combinations of five guard spellings by seven map spellings
  give no 0x40 at 34 instructions. The gap is `perform`'s own: an uninitialised
  scalar in `movement()` is +0 there (it is an inlined callee), +8 in `perform`.
- `MSBgm::init` (0x28 vs 0x48). +8 each for naming the `JAIData*`, taking a
  `JAISoundTable&`/`*`, naming the `new` result, and writing the three track
  clears as a `u32`/`int` loop (a `u8` index is 39 instructions, not 34). All 72
  combinations of 6 table x 3 new x 4 clear spellings cap at **0x40**.

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
| `MSound/MSoundBGM` | `MSBgm::init` | Body exact (padding of 25-32 bytes gives 100%). Scalar naming ceiling 0x40 vs target 0x48 over all 72 spelling combinations; needs one 8-byte aggregate with no evidence for one. All trials are in the source TODO. |

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

## Closure batch 67: levers that cut both ways, and the dead-low-region family

- **A cast inside an accessor vs at the call site changes register allocation, not just the frame.** `TAnimalManagerBase::clipEnemies`: raw `unk18[i]` 0 slots, `(T*)TObjManager::getObj(i)` 1, `(T*)TLiveManager::getObj(i)` 2, three-level `getObj(i)` 2 (saturating); with the cast in the initialiser the loop index and object pointer swap callee-saved registers, with it inside the accessor they come out right (header item: `TEnemyManager::getObj` as `(TSpineEnemy*)TObjManager::getObj(i)`). `getObjNum()` 0; `gpCamera->getFovy()/getAspect()` +8 each; `checkLiveFlag` family 0; `actor->getPosition()` +16.
- **An accessor retail did not use is a real +8/+16 over-frame in a hot loop**: `TJointObj::getChildrenNum()` cost 16 bytes in `TMapXlu::changeNormalJoint` and 8 in `changeXluJoint`; the raw `mChildrenNum` read closed the unit (const-ness irrelevant, the level is the cost).
- **A dead inline temporary is reserved in expansion order downward from the named locals**, so a misplaced stream-read buffer localises a missing level *before* it: `TPerformList::load`'s `readU32` buffer at 0x58 vs 0x54 was the fabricated `search<T>` template; `getInstance()->getRootNameRef()->search(name)` spelled out closed it.
- **Probe a pure callee-saved rotation with a throwaway normalisation**: `bVar5 != 0` at one argument of `M3UMtxCalcBlendAux` reproduces retail's whole ranking and leaves only the instructions it adds, proving "body right, allocator ranking wrong" and naming the live range responsible.
- **Dead-low-region family: solved as a dead non-trivial local in an inlined callee (batch 69).**
  See [The dead low region](#the-dead-low-region) below for the rule, the probe
  table and which member each measurement closes.

## The dead low region

*Measured in batch 69 in a scratch TU with the game flags, on the
`TMewManager::loadAfter` shape (base frame 0x18, 15 instructions throughout).*

An **uninitialised class local of an inlined callee** is the one construct that
buys low-region bytes with **zero** instruction change, and its price is the
local's size rounded up to 8:

| local of the inlined callee | frame delta |
| --- | --- |
| trivial POD, any size | **0** |
| non-trivial, 4 B | +8 |
| non-trivial, 8 B | +8 |
| non-trivial, 12 B (`JGeometry::TVec3<f32>`) | **+16** |
| non-trivial, 16 B | +16 |
| non-trivial, 20 B | +24 |
| non-trivial, 24 B (`TVec3<f32>[2]`) | +24 |
| two non-trivial 8 B locals | +16 |

- **Triviality is the whole switch, and any one of four things flips it**: a
  user-declared default ctor, a user-declared dtor, a user-declared copy ctor,
  or a virtual function. A struct with none of them is worth 0 at any size.
- **Nesting depth is irrelevant.** The same 12-byte local reached at depth 1, 2
  or 3 costs +16 once. The cost is per *expansion*, not per level, so N call
  sites of the same callee each pay.
- Two expansions of one callee, by how the second argument is spelled:

  | local size | 2 exp, by-value 2nd arg | 2 exp, pointer 2nd arg |
  | --- | --- | --- |
  | 4 B | +16 | +8 |
  | 8 B | +24 | +16 |
  | 12 B | +32 | +24 |
  | 16 B | +40 | +32 |

  (Frames are 8-aligned, so the same local can read 8 higher or lower depending
  on what else sits in the low region; use the closest shape as the probe.)

### One 12-byte local explains all four Animal `loadAfter`s

Each body is `Base::loadAfter();` plus one or two `MSRandPlay` calls. Assume one
dead 12-byte non-trivial local (a `TVec3<f32>`) in an inlined callee and every
frame falls out, at the right instruction count:

| function | expansions | ours | retail | S=12 predicts | insns |
| --- | --- | --- | --- | --- | --- |
| `TMewManager::loadAfter` | 1, by-value arg | 0x18 | 0x28 | 0x28 | 15 |
| `TAnimalBase::loadAfter` | 1 guarded, ptr arg | 0x18 | 0x28 | 0x28 | 18 |
| `TAnimalBird::loadAfter` | 2, ptr arg | 0x18 | 0x30 | 0x30 | 17 |
| `TAnimalBirdManager::loadAfter` | 2, by-value arg | 0x18 | 0x38 | 0x38 | 19 |

S=8 and S=16 each miss two of the four, so 12 is the unique fit. What the callee
*is* stays unknown: `MSound::setPlayerInfo` matches with six `MSRandPlay` calls,
so there is no wrapper around those, and the four classes share no base below
`TNameRef`. Ruled out here (all zero, and `&getPosition()` also adds an
instruction): `&TPlacement::getPosition()` over `&mPosition`, `getObjNum()` /
`getCapacity()` / a named `u16` / an explicit `(u16)` cast, inline forwarder
levels above the call, and 1-, 2- or 3-parameter inline wrappers with no local
(parameter binding is free here, contrary to the per-parameter rule elsewhere).

### The carrier has to be a callee with no matching out-of-line copy

This is what decides whether the rule can be *applied*, and it is worth checking
before any spelling work. The dead local grows the callee's own frame too, so a
callee whose out-of-line copy is emitted and already matches cannot carry it.
An **UNUSED** callee can: its size is an instruction-byte count, and an
uninitialised non-trivial local emits nothing.

- `TPerformList::perform` (0xc0 vs **0xe8**): 36-40 B in `forEachPerform` lands
  0xe8 exactly, with `perform` still 54 instructions and `forEachPerform` still
  41 (UNUSED size 0xa4 preserved). `forEachPerform` is UNUSED, so this one is
  legal — it just has no evidence for a 36-40-byte object.
- `TPollutionPos::isSame` (0x48 vs **0x58**): 28-32 B in `worldToDepth` lands
  0x58 exactly at 53 instructions, but `worldToDepth`'s own copy is emitted and
  100% at frame 0x18, so it breaks. `index()`, `isInArea()` and `getDepth()` are
  shared with `getDepthWorld`, `isProhibit` and `getEdgeDegree`, which match
  already. **No carrier exists**, so the 32 bytes have to be `isSame`'s own body
  locals.
- `TTalkCursor::associateNPC` (0x60 vs 0x78, +24) and `loadAfter` (0x28 vs 0x30,
  +8): the only inlined callees are `JGeometry::TPosition3::translation`,
  `MActor::getModel` and `TFlagT::on`/`off`; `TBaseNPC::getCursorPos` returns
  12 bytes by value and is a real `bl`. Every carrier is a shared header.


## Closure batch 66: ladders worth three digits, and slot order as evidence

- **A ladder can be worth 136 bytes with zero instruction change** (`TButterfloid::load`): `getRealoid(i)`+`getBoidNum()` +16/+24 per loop, a boid-leader accessor over raw `unk150` on ten writes +48, nine `TBoidLeader` setters +40, `getMActor()` +8, and a forwarding `TRealoid::getBoidNum()` stacked on the leader accessor the last +8 (a forwarder on the owning class over a member accessor pays a third time). Read a three-digit low-region gap as a missing accessor *set* once the inlined helpers' UNUSED sizes are exact (`loadItem` 0x104, `initBoids` 0x74).
- **Slot order identifies the declaration form** (`TAreaCylinder::load`): explicitly declared locals form a block above the compiler/inline-temp pool, first-declared highest; inline-callee locals and spills fill the pool in source order. A C-style block `rot; A; B; count;` with two dead scalars, `stream >> count` (address-taken), discarded `readF32()/readS32()` values, and `s32 num = count;` for the loop bound (an escaped local is reloaded across every call, so it cannot be the ROM's r29). Two function-scope value-returning stream reads cost 8 bytes of low region, one costs nothing; splitting `int x = s.readS32();` into declaration plus assignment recovers 4.
- MarDirector ladder, per site: `SMSGetMarDirector()->mMap` +4, `gpMarDirector->getCurrentMap()` +8, `SMSGetMarDirector()->getCurrentMap()` +12 (Shimmer; a +4 low lever exists). `gpCamera->getUnk124()` +12. Mixing an accessor and a raw member for one field inside a basic block costs two instructions: apply levers per block (ProgSelect). `getGamePad()` +8 low; `u32 prevSelection = getSelection();` fixes the low region alone.
- An extra `JDrama::TNameRefGen::search<T>` expansion is +24 of frame, a by-value `TRect`/viewport pair +16 (`TMarDirector::preEntry`, 56 low bytes short with one referenced local: retail has ~7 search expansions to our 5, either a deeper `search<T>` chain or two `push_back(search<T>(name), cue)` that CSE; open, needs a JDRNameRefGen.hpp probe).
- `ctrlMultiPlayerCamera_` (99.1): uniform 16 low delta, but retail's `MsSqrtf` slot sits above 28 bytes of inner-loop temporaries (ours 4), plus an f3/f4 swap on the loop's `fsubs` and one `fmr f31, f0` (retail's `fmadds` writes `camDistance` into f31). `trash[16]` fixes the frame and leaves both register issues, so the lead is the inner loop's extra named scalar, not the size. Three named squares are required (`squared()`/component differences contract to `fmadds`).

## Closure batch 70: aggregates for folded out-parameters, parameter temporaries

- **An out-parameter pair that an inlined callee writes must be one aggregate.** When the callee is inlined and its stores fold away, MWCC drops two separate scalars' slots entirely; `u32 header[2]` closed `TMessageLoader::TMessageLoader(const char*)` and `f32 target[2]` closed `CPolarSubCamera::execSecureView_` (+8 each). Scope does not matter, only aggregation.
- **An inlined callee's parameter temporary is a placeable 4-byte slot**: `readInfoBlock(stream.getCurrent())` creates it, a named local for the same value is register-allocated and creates nothing (`parseBlock` closed; MessageLoader linked). `readInfoBlock` sits at the depth-1 statement budget — one added statement flips it out of line.
- **A matching sibling is the best control for a slot-position gap**: `TRailMapObj::setGroundCollision` (exact, 0x50/0x18) vs `TRideCloud::setGroundCollision` (0x50/0x14) localises the missing slot to the guard's inlined accessor reads.
- **Naming a value that lives across calls is a register-count fix**: `TGraphWeb* graph = unk138->getGraph()` reused for the second node fetch restored r28-r31 in `TRideCloud::control` (96.3 -> 99.8); re-reading after a call costs a load and a callee-saved register.
- Steps measured: an `MSound` member accessor +8/site (`getModBgm()` parked in BossHanachanNerve, header item); `getConsole()` +8, `SMSGetMarDirector()` stacked +8, saturating at +0x10 for a one-site chain; `const TVec3&` member accessor +8; `TMapCollisionManager::getUnk8()` +8; `SMS_GetMarioX/Z` +8 (pair); `SMS_GetMarioAngleY()` +8 in the inlining caller, +0 in the callee. `sunmgr::load`: colour inputs declared in reverse read order so the reads go to ascending slots; still ~64 low. `sunmgr::perform`: a 48-byte uninitialised local (a scratch `Mtx`) gives 100% but nothing wants a matrix — left open. Open "last 8 bytes" members: `TNerveBossHanachanTumble::execute`, `calcSecureViewTarget_` (must be a trivial aggregate of its own body).

## Closure batch 74: `MSound::startSoundActor` carries the sound-site 8 bytes

`MSound::startSoundActor` (a header inline in `include/MSound/MSound.hpp`,
marked *fabricated*) holds **one small stack object**.
The map has no `startSoundActor__6MSound...` symbol at all — consistent with a
header inline that inlined at every site and so never got an out-of-line weak
copy, exactly like `MSound::startSoundSystemSE` and `startSeRandPlay`.
So the object is invisible to `validate-symbol-order` and can only be measured.

The honest spelling is a **named local carrying the callee's return value**.
`MSoundSESystem::MSoundSE::startSoundActor` really does return `JAISound*`
(`include/MSound/MSoundSE.hpp`), so a wrapper that hands the handle back is what
a developer would have written:

```cpp
JAISound* startSoundActor(u32 id, const Vec* position, u32 ground_no,
                          JAISoundHandle* out_handle, u32 fade, u8 camera_idx)
{
	JAISound* sound = nullptr;
	if (gateCheck(id))
		sound = MSoundSESystem::MSoundSE::startSoundActor(
		    id, position, ground_no, out_handle, fade, camera_idx);
	return sound;
}
```

Measured project-wide, each variant against one fixed baseline, counting
per-function `fuzzy_match` moves (the whole tree, not one unit):

| variant inside `MSound::startSoundActor` | improved | to 100% | regressed | from 100% | `matched_code` |
| --- | --- | --- | --- | --- | --- |
| unchanged (reference) | - | - | - | - | 47.78% |
| 8-byte non-trivial local | 85 | 59 | 34 | 14 | 48.10% |
| two 4-byte non-trivial locals | 85 | 59 | 34 | 14 | 48.10% |
| 8-byte non-trivial local, inside the `if` | 85 | 59 | 34 | 14 | 48.10% |
| 12-byte non-trivial local | 58 | 37 | 33 | 14 | 48.00% |
| 4-byte non-trivial local (dtor only) | 54 | 38 | 11 | 2 | 48.04% |
| 4-byte non-trivial local (default ctor only) | 54 | 38 | 10 | 2 | 48.04% |
| 4-byte non-trivial local, inside the `if` | 54 | 38 | 11 | 2 | 48.04% |
| 4-byte non-trivial local, declared last | 54 | 38 | 11 | 2 | 48.04% |
| **named `JAISound*` result, returned** | **54** | **38** | **10** | **2** | **48.04%** |
| named `JAISound*` result, discarded inside the `if` | 54 | 38 | 10 | 2 | 48.04% |
| `return` inside the `if`, no named local | 0 | 0 | 0 | 0 | 47.78% |
| 8-byte **trivial** POD local | 0 | 0 | 0 | 0 | 47.78% |

What the table settles:

- **The named local is the slot, not the return type.** Spelling the wrapper
  `if (gateCheck(id)) return MSoundSE::startSoundActor(...); return nullptr;`
  is worth **zero** everywhere. Only binding the result to a local reserves
  anything. This generalises the "named call result is 16" rule from `bombhei`
  down to 4 bytes: the cost is the *binding*, and a pointer binding that is live
  across a branch merge is not free even though a plain named pointer is.
- **The object is 4 bytes, not 8.** A 4-byte object lands as +8 in callers with
  no low-region slack and +0 in callers that have four spare bytes; an 8-byte
  object costs +8 in *every* caller. 8 bytes closes 21 more callers but breaks
  12 more, so 4 is the fit.
- **The raw spelling is a different frame.** `if (gpMSound->gateCheck(id))
  MSoundSESystem::MSoundSE::startSoundActor(...)` spelled out at the call site
  is +0 (measured on `TDebuTelesa::receiveMessage`). A site's frame therefore
  tells you which of the two spellings retail used, and it is the only evidence
  that does.

### Which spelling each site wants

38 functions go to 100% with the wrapper form, among them
`TDebuTelesa::receiveMessage` (which makes `DebuTelesa` a 100/100 unit),
`THamuKuri::behaveToFindMario`, `THaneHamuKuri::attackToMario`,
`TFireHamuKuri::changeTevColor`, `THinokuri2::updateAnmSound`,
`TStayPakkun::setBehavior`, `TSmallEnemy::setAfterDeadEffect` and
`generateEffectColumWater`, `TTelesa::changeByJuice` and `initItemAttacker`,
`TFireWanwan::receiveMessage`, `TTobiPuku::behaveToWater`, `TMoePuku::hitWater`,
`TGorogoro::setDeadAnm`, `TTamaNoko::setAfterDeadEffect`,
`TBombHei::walkBehavior`, `TNerveKazekunHitWater::execute`,
`TNerveAnimalBirdTakeoff::execute`, `TNerveBeeHiveFall::execute`,
`TGessoPolluteObj::calcRootMatrix`, `TWoodBarrel::appear`,
`TMapObjBase::startSound`, `TItem::appeared`, `TSuperHipDropBlock::receiveMessage`,
`TRoulette::switchStop`, `TPanelRevolve::receiveMessage`,
`TBalloonKoopaJr::kill`, `TMareCork::drawObject`, `TBigWatermelon::touchWaterSurface`,
`TBellDolpic::control`, `TDptMonteFence::touchPlayer`, `TMareGate::control`,
`TMapObjNail::receiveMessage`, `TBreakHideObj::receiveMessage` and
`TMario::stayWall`.

Ten functions want the **raw** spelling and regress under the wrapper form.
Two of them are currently exact, so the change cannot land until they are
converted:

| function | file | pre-change |
| --- | --- | --- |
| `TNerveKazekunPreAttack::execute` | `src/Enemy/Kazekun.cpp:531` | 100% |
| `TMapEventSink::control` | `src/Map/MapEventSink.cpp:106` | 100% |
| `TMareEventDepressWall::depressing` | `src/Map/MapEventMare.cpp:325,356` | 99.94% |
| `THaneHamuKuri::walkBehavior` | `src/Enemy/hamukuri.cpp:1490` | 99.90% |
| `THamuKuri::jumpToSearchActor` | `src/Enemy/hamukuri.cpp:824` | 99.78% |
| `TEffectObjBase::moveObject` | `src/Enemy/effectObj.cpp:149` | 98.84% |
| `TMario::oilSlip` | `src/Player/MarioRun.cpp` | 98.49% |
| `TYoshiTongue::movement` | `src/Player/Tongue.cpp:307` | 78.32% |
| `TItemManager::resetNozzleBoxesModel` | `src/MoveBG/ItemManager.cpp:34` | 78.57% |
| `TBossTelesa::forceHide` | `src/Enemy/bosstelesa.cpp:2121,2124` | 49.63% |

Conversely `TEffectEnemy::setDeadAnm` and
`TBossHanachan::emitOneTimeSandPillar_` currently spell the gate check out and
are 8 bytes short, so they want the **wrapper**; switching them and landing the
wrapper closes both, and `setDeadAnm`'s fabricated `JGeometry::TVec3<f32>
effectPos` should be deleted in the same move.

**Why this batch did not land it.** Reaching zero regressions needs those ten
conversions, and four of the TUs involved (`hamukuri`, `MapEventMare`,
`Tongue`, `MarioRun`) do not yet include `MSound/MSoundSE.hpp`; adding a header
to a TU perturbs BSS and static-init order, so each has to be converted and
re-measured on its own. The header edit alone is +54/-10 and must not be
committed by itself.

### Problem B addendum: the Animal `loadAfter` carrier is not `MSRandPlay`

`MSoundSESystem::MSRandPlay::registerTrans(u32, const Vec*)` (0x54) and
`createRandPlayVec(u32, u16)` (0xa4) are both **emitted, matching, out-of-line
statics** reached by a real `bl` in all four `loadAfter` diffs, so neither can
hold the dead 12-byte local itself — the same "carrier has to be a callee with
no matching out-of-line copy" test that killed `TPollutionPos::worldToDepth`.
The carrier must be a wrapper that inlines everywhere and so leaves no map
symbol, like `MSound::startSoundActor` above. The Animal TUs' weak lists
(`AnimalBase.cpp`, `Bird.cpp`, `AnimalManager.cpp`) contain no `MSRandPlay`- or
sound-related entry, so the map cannot name it either.
New lead from this batch: since a *named* local reserves a slot with zero
instructions, the Animal carrier's 12 bytes need not be an explicitly dead
object — a named by-value 12-byte result inside such a wrapper would read the
same. Untested.

## Closure batch 82: the sound sites' second level is a two-argument `startSoundActor`

Batch 74 found that ~37 sound sites are 4 bytes short and that a named
`JAISound*` result inside `MSound::startSoundActor` supplies them; header round
13 rejected putting it there, because `TMapEventSink::control` is byte-exact
with its vector temporary at `0x28 + 4 * bindings = 0x30`, i.e. exactly two
bindings (`this`, `position`).
The missing 4 bytes are one level up: a **two-argument overload** in
`include/MSound/MSound.hpp` that carries the four constant arguments and binds
the callee's result,

```cpp
JAISound* startSoundActor(u32 id, const Vec* position)
{
	JAISound* sound = startSoundActor(id, position, 0, nullptr, 0, 4);
	return sound;
}
```

so a site pays the 4 bytes only by spelling the short form.
The six-argument body keeps its two bindings and the eleven sites that need
them stay byte-exact; making it return `JAISound*` with a direct `return` is
codegen-neutral whole-tree (zero functions moved), which is how the return type
was confirmed.

**The slot is the binding, not the level.** Measured in `smallEnemy` against
`generateEffectColumWater` (0x18 vs 0x20) and `setAfterDeadEffect`:

| second level between the site and `MSound::startSoundActor` | result |
| --- | --- |
| plain forwarder `(u32, const Vec*)` | +0 |
| forwarder taking `const Vec&` or `const JGeometry::TVec3<f32>&` | +0 |
| member wrapper `(u32 id)` binding only `this` (the `TMario::startSoundActor` shape) | +0 |
| binds the result, discards it, returns `void` | +0 |
| **binds the result and returns it** | **+8, both exact** |
| dead 4-byte **non-trivial** local | +8, both exact (sizes the slot) |
| `JAISoundHandle handle = ...; releaseSoundHandle(handle)` | new instructions |

A constant argument binds nothing, `this` and a `&member` address each bind 4,
so a member wrapper cannot be the carrier: its only fresh value is already the
inner call's.
That also retires the "per-class `startSound`-style helper" lead — the map's
only two, `startSoundActor__6TMarioFUl` and `startSound__11TMapObjBaseFUs`, are
emitted, and `TMario::startSoundActor` is byte-exact at frame 0x20 with one
raw expansion, which pins the six-argument body's two bindings a second time.

**Apply it per site, never per file.** A function with several sound sites in
mutually exclusive branches pays per expansion and overshoots, so those stay on
the six-argument spelling: `TBellDolpic::control` +0x10 (four sites),
`TRoulette::switchStop`, `TDptMonteFence::touchPlayer`,
`TMapObjBase::startSound` +8 (two sites each), and the single-site
`TFireWanwan::receiveMessage`, `TNerveKazekunHitWater::execute` and
`TTelesa::changeByJuice` (already 8 over). Those want the binding one level
deeper and are still open.

Landed (26 functions byte-exact, no regressions, DOL unchanged):
`TDebuTelesa::receiveMessage`, `THamuKuri::behaveToFindMario`,
`THaneHamuKuri::attackToMario`, `THinokuri2::updateAnmSound`,
`TStayPakkun::setBehavior`, `TSmallEnemy::setAfterDeadEffect` and
`generateEffectColumWater`, `TTelesa::initItemAttacker`,
`TTobiPuku::behaveToWater`, `TMoePuku::hitWater`, `TGorogoro::setDeadAnm`,
`TTamaNoko::setAfterDeadEffect`, `TBombHei::walkBehavior`,
`TGessoPolluteObj::calcRootMatrix`, `TEffectEnemy::setDeadAnm` (its fabricated
`effectPos` deleted), `TBossHanachan::emitOneTimeSandPillar_`,
`TNerveAnimalBirdTakeoff::execute`, `TWoodBarrel::appear`, `TItem::appeared`,
`TSuperHipDropBlock::receiveMessage`, `TPanelRevolve::receiveMessage`,
`TBalloonKoopaJr::kill`, `TMareCork::drawObject`,
`TBigWatermelon::touchWaterSurface`, `TMapObjNail::receiveMessage`,
`TBreakHideObj::receiveMessage`, `TMario::stayWall`.
`Enemy/DebuTelesa` and `MoveBG/WoodBarrel` are now 100/100 and ready to
source-link.
`TMareGate::control` passes a real handle (`&sound->unk7C`) so the
two-argument overload cannot reach it; it needs a three-argument sibling or the
deeper carrier.

## Closure batch 78

- **A four-corner quad is readable off the store offsets**: twelve `stfs` to consecutive slots with two CSE'd sums is `TVec3 vtx[4]` at function scope plus `vtx[i].set(x±size, y±size, z+size)` in the loop (the `TSplashManager::makeDL` idiom); the twelve by-value `set` parameter temporaries are ~48 bytes of low region (`TQuestionManager::makeDL` 84 -> 100; question linked; its `#pragma dont_inline` was masking a 19-statement body).
- **A named `bool` per block saturates like an accessor**: one of two blocks +0, both +8 (`TQuestionManager::perform`). `f32 dx; f32 dz;` named in that order so `fmadds` gets register operands (`request`).
- **The dead-low-region carrier can be sized even when it cannot be named**: a dead non-trivial 4-byte local in `MSBgmXFade::getTimingForce` closes `xFadeBgmForce`, an 8-byte one in `getTiming` closes `xFadeBgm`'s frame (both UNUSED, so legal carriers; not fabricated). `getTiming`'s UNUSED size 0x94 vs our 0x60 is the optional `param_2` output path.
- **A `JUtility::TColor`-from-`GXColor` conversion temporary is 8 bytes for us and 4 for retail** (Menu: four temps at an 8-byte stride vs retail's 4; header suspect: the commented-out `TColor(const TColor&)` copy ctor in JUTColor.hpp).
- Hoisting a receiver local above its guard is a register lever (`TMovieRumble::readCurInfo`); retail still copies the ToolData pointer into a second callee-saved register that the guard's `isIndexValid` consumes (+8 in both callers, open). `movement`'s UNUSED 0xb4 vs our 0x78 says fifteen instructions are unrecovered (why `checkRumbleOff` still needs its pragma).
- `TPollutionManager::load` 0x58 vs 0x60 with the ladder exhausted (the last 8 is a zero-instruction object); `SMSGetPollutionLayer(i)` is the wrong rung there (+0x20, +1 instruction). `cleanedAll` is 8 too big: a const `getLayer(i)` casting `mJointModels[i]` directly (one level) is the header candidate. `TWoodBarrel::kill`: a second dead `TVec3` (the emit direction at its `.prm` default) after `vec`, positional evidence; `appear` closes only with the queued `startSoundActor` item. A signed loop bound shows as `cmpw` vs `cmplw` against an `int` member. Trial scripts must grep for `FAILED`/`build stopped`, not `error` (sjiswrap prints "Shift JIS encoding errors" on success).

## Closure batch 77

- **Slot-allocation order names the source's term order**: for `A*a + B*b + C*c` MWCC gives the second term the `fmuls` and the lowest conversion slot; reading the magic-double slots identified `start*omt^2 + control*mid + end*t^2` (`TBoundPane::update` 55 -> 92.7, then closed with `out.set(nx, ny)` — `JUTPoint::set(int, int)` inlined is worth 16 low bytes where two field stores are 0 — and two named `s16` results; BoundPane linked; its 0x14/0x1C are two `JUTPoint`s, not a `JUTRect`).
- **One reused `f32 cross[2]` beats two arrays** (0x40) or four scalars (0x30) for the last-declared two-word aggregate (`checkLinesCollision` closed).
- **Named locals can be worth both the frame and the schedule**: in `TMario::hangPole` computing `b` before `a` plus four named scalars (`sinY`, `cosY`, `nx`, `nz`) was the missing 16 bytes and the `jmaSinTable`/`jmaCosTable` load order. `hitNormal`: a parked `getCurrentNozzleIndex()` returning `s32` (signed `cmpwi` on the `u8` field; header item) fixed the frame; one volatile register left.
- **An outgoing-parameter area is 8 bytes per `f32` argument** (`matan(f32, f32)` costs 16): when a dead local in an inlined callee lands the size but moves a `volatile` temp, the gap is parameter area, not a local (`MsGetRotFromZaxis`, retail reserves 32). `MsGetRotFromZaxisX2`'s degenerate cases were sign-swapped (real bug; `@1673 = 90.0f`, `@1674 = -90.0f`).
- **Five stacked accessor levels on one receiver chain are additive to the byte** (`changeAnmRateAndFrameUpdate_`: 32 + 24 + 24 + 8 + 8 = the 0x58 gap; parked `BHA_*` accessors, header items) but per block: the same `getSL*` wrapper that buys 24 bytes at ten sites swaps two float registers at the eleventh. `fuzzy_match` can read lower while differing instructions fall (13 -> 10) because it weights the `stwu` immediate less than register operands.
- `polygonIsInGrid`: routing `pointIsInPolygon`'s twelve point reads through `getPoint1/2/3()` is worth 296 of 608 dead low bytes; open. `TMarDirector::setup2` (99.9, every instruction exact): exactly 256 bytes = `sizeof(JDrama::TGraphics)` of dead low region below every named local, owner unknown. Restored two behaviours there: the event loop's `u16 eventId` written to `TMapObjBase::mEventId` (0x134; a `clrlwi r0, rN, 16` + `stw` with the increment outside the guard is the tell for an id-assigning search loop) and the pause/talk `mGamePad` assignments. `extsh` before a `stw` into an `int` field means an `s16` intermediate.

## Closure batch 81

- **Each chained `>>` continuation costs 8 bytes of low region** (`TGenerator::load`: 9 unchained reads 0x58, three chains of three 0x88, one chain of six plus one of three 0x90 = exact; `#continuations = #reads - #statements`). A discarded `stream.readS32()` and `stream >> x` place their read target in different pools. Three discarded scale floats are a `TVec3 scale`. `s32 count = stream.readS32(); s32 num = count;` with a named `interval` assigned after the loop gives retail's r31/r0 choice. generator linked.
- **A named `f32` is worth 8, not 4, in a leaf with an `f31` save** (`TWireBinder::isEndWire` +8 by adding one, `getDirAtPos` -8 by removing one). Two UNUSED sibling helpers with an `f32` parameter next to a predicate are that predicate's target-value ternaries (`getStartRangePos`/`getEndRangePos`, `static` because the callers are const and the mangled names carry no `C`).
- **An inlined callee's dead non-trivial local can be sized by two callers with different base frames**: `TFileLoadBlock::pushed` (UNUSED 0x9c) needs 13-16 bytes (12 closes only `touchPlayer`, 20 only `receiveMessage`); `JDrama::TDisplay::getRenderMode()` (header inline, no map symbol) needs one dead 4-byte non-trivial local to close all four `SMSSetup*RenderingInfo` with zero regressions (unique carrier; header item — the object cannot be named).
- `MTXCopy(src->getX(i), dst->getX(i))` vs `dst->setX(i, src->getX(i))` is readable off the register order (right-to-left evaluation puts the destination's base in r3 first; `TMirrorActor::perform`, whose second loop bound is `mWEvlpMtxNum`, and `checkIsInMirror` had a missing `&& !(unk1A & 0x20)` term and a mirror number read through a `const int&` — header item `TMirrorModelManager::getUnk18()`). `getModelData()` is the standard +8/+4 rung in M3DUtil (three SDLModel closures). `SMSGetCamera()->getUnk1EC()` is +0 in `SDLModel::viewCalcSimple` where it was +8 in `TSky::perform`: the global fork is per site. `enemy->unk124->setGraph()` over `getTracer()` was -4.
- **The JGadget `push_back` temp-pool spacing is off by 8 per group gap in two unrelated units** (`SDLModel::entry` 0xa8 vs 0xb8, `TMirrorActor::init` frame exact but pairs shifted) with everything else exact — one shared-header cause (`std-list.hpp` iterator / `JDRViewObjPtrList::insert`); research item. A `static void dummy(Vec*)` forcing literal emission shifted MapObjOption's whole string pool by 0x18 (removed).

## Closure batch 84

- **A named pointer local is +8 when its initialiser goes through an indexed inline accessor** (`TSunGlass::load`: `SMSGetMarDirector()->getGamePad(1)` +16 plus the named `TMarioGamePad*` +8); refines "named pointer locals are worth zero".
- **Dead-local carriers pay per expansion; accessor forks saturate**: an 8-byte dead non-trivial local in `getShineAlpha()` pays +8 at each of three expansions, `SMSGetMarDirector()`/`SMSGetFlagManager()` pay once. When a caller with N expansions needs Nx8 and a caller with one needs 8, the lever is a local, not a level. A genuinely dead read (`s32 shineCount = getFlag(0x40000)`) is a named top slot.
- **A `bl` between two reads of the same inline accessor is a statement boundary**: retail's reload of `getWire(i)` after `getPosInWire` proves two statements with a named `f32 posInWire` (`getPointPosInNthWire` 57 -> 100). **An inlined method's guard reached through a back-pointer identifies its owner**: `lwz r3, 0x78(r30)` (`unk4.unk74->unk7C`) proved the take-check block is `TMapWireActor::checkTakingActor` (its body is 40 bytes short of the map's 0xe8 = three `TVec3` locals; `doActorToWire` 0x30 vs 0x58 closes with them).
- **A fabricated sound call can be the residue**: `TItemManager::resetNozzleBoxesModel` had no `startSoundActor` in retail (78 -> 100); check for a `>`-only gate-check run first. `makeShineAppearWithTimeOffset` UNUSED restored at 0xc8.
- A receiver materialised as `addi rD, rBase, off` means one more accessor level above the inline (`getUnkF4().getPoint()`), per site (`execWalk` wants it, `execUTurn`/`isCanWalk` do not). Binding an `s16` return turns `cmpwi rX, 0` into `extsh. rX, rX`; an explicit cast at the compare does not (`getWaterHitCounter()`, header item). Hoisting the inner loop counter above the outer `for` swaps their callee-saved registers (`emitCamShake_`). `TFlagManager::getID()`/`getStatus()` over raw members are +8 per site (MSHandle; real bug: `setSeDistanceParameters` calls `setSeDistanceFxmix`, vtable 0x18). Two sibling UNUSED symbols of identical size are not necessarily wrapper and body. `mr` vs `addi rX, rY, 0` flips per site with no correlate — not worth chasing.
- Open: `TTakeActor(const char*)` must be both inlined and emitted (UNUSED 0x50 in MapWireManager; header); `TList_pointer<THitActor*>::insert` 16 bytes too big (JGadget); `TVec3::set<f>` reached one level deeper from `isCanWalk` (NpcWalkTurn MISSING); `emitParticle_` 0x90 vs 0xc8, `emitCamShake_` 0x68 vs 0x78 pure frame.

## Closure batch 87

- **A 4-byte named local declared after a 12-byte one lands at the bottom of the named block**: the block fills bottom-up in reverse declaration order with any 8-alignment pad at the top, so a `+4 named` lever can move a `TVec3` up by 4 (`TCameraOption::TCameraOption`: `f32 yaw = tool->getYaw()` after `origin`; CameraOption linked, its `cLoadCamName` is "左サイドカメラ").
- **A global-accessor level is +4 per read site, not +8** (`SMSGetCameraOption()`: 4 sites +0x10, 8 +0x20, 12 +0x30); the 8-byte-step rule is about member/receiver chains. One extra inline level above `TMario::mModel` (`getM3UModel()`, parked) is +8 per site and closed both MarioInit functions (linked; `initValues` does `new MAnmSoundMario`; the param string is `mHPMax`).
- **A function-local static's mangling is an oracle for its enclosing function's linkage**: `name$localstatic0$<fn>` means the function was in-class/inline (weak), `name$NNN` ordinary; a weak static also re-materialises `lis/addi` per basic block where a local in `.data` gets one CSE'd base. `calcTowerCenterPos_` is weak with `sPositionNameTable$localstatic0$`, and `inline` cannot give "weak and still called at depth 1" (second confirmation) — the tower branch is an unreconstructed inlined wrapper (`ctrlTowerCamera_`; header item). Fixed the table to `[5]`.
- `fneg` off a single parameter read is the tell for a symmetric range test `[-r, +r]`; a second `.get()` of the parameter displaces the `.sdata2` pool. A pool longer than the target's whose extra entries are angle factors (`182.04445f` where retail has `0.005493164` or `3.0517578e-05`) is a direction bug (CameraNotice: three real fixes, data 100). Three named squares beat `TVec3::squared(other)` where retail keeps three `fmuls` (third confirmation; parked `CameraNoticeSquaredDist`). A dead 8-byte non-trivial local in an UNUSED callee expanded four times is +32 (`chaseOptionCamera_`), sized and not committed.
- `MarioEffect::init` is a clean "pure callee-saved rotation with an exact frame": the lever must be zero-frame; one `addi r4, r25, 0` vs `mr` says the operand came from an accessor's returned address. `FeetInvCalc` (92.3): every `r1` displacement exactly 0x68 higher in retail, LR and save slots included — 104 bytes of pure dead low region below every local, plus four batch-86 FPR sites.

## Lever sweep batch 95 (2026-09-18): 30 functions to exact

- **A global-accessor fork buys frame only over a raw global *variable* read.** Over an existing inline accessor it is codegen-identical: `SMSGetFlagManager()` over `TFlagManager::getInstance()` (already `return smInstance;` in-class) gave a byte-identical object at all 13 sites of Guide.cpp. This retires header round 16's "194 raw `getInstance()` sites, +4 each" note (that +4 came from a named result).
- **Two site shapes accept a fork, by different mechanisms**: (a) frame — a small `load`/`loadAfter`/`reset`/`init`/`execute`/`receiveMessage` with a single raw site and a gap of exactly 8 (25 of 49 kept groups; apply on sight); (b) register allocation — a receiver in front of an argument-heavy call (`fireStartDemoCamera`'s nine arguments, `startDemoCamera`), where the fork fixes the receiver's register and the argument scheduling with the frame unmoved. One spelling per function: 45 of 49 kept groups took all sites; bisect only when the full set overshoots. Hit rates: Pollution 20%, Camera 17%, MarDirector 8.6%, CameraOption 4.5%, MSound 1.9% (`gpMSound->` is nearly always already right; the MSound sites' missing 8 bytes come from the call form).
- **The six-argument `startSoundActor(id, pos, 0, nullptr, 0, 4)` in a function at 99.7-99.95% is one +8 short**: the two-argument overload closes it (21 of 26 keeps were >= 99.73, 15 hit 100). Below 98% it only nudges; multi-site functions overshoot; `MSoundSE::startSoundActor`'s 42 static sites have no overload. Driver: `lever.py`/`lever6.py` in the batch-95 scratchpad (groups raw sites by function, searches subsets, keeps only strict unit gains with zero per-function regression).

## Second-pass batch 98

- **A params ladder has three independent +-8 rungs** (global -> object, object -> params wrapper, named result); landing a frame means picking the combination, and a wrong pair shows as one misplaced argument load, not a frame error (`TEnemyPolluteModelManager::perform` closed with `gpConductor->unk84.getEnemyFarClip()` plus a named `100.0f`).
- **Lever pairs, third confirmation**: `int time = spine->getTime()` + `f32 distToMario = seal->getDistToMarioSquared()` are +0 each and +8 together (`TNerveSealWait::execute` closed). Try pairs before declaring a "last 8 bytes" case unnameable.
- **An inlined callee's local and a block-scope local allocate identically** (both below every function-scope local), and a function-scope object with an empty ctor has that ctor inlined away while a block-scope one calls it at the use site — why `generatePolluteModel` ("matrix allocated first, constructed lazily") cannot be reproduced.
- **An UNUSED symbol bigger than our out-of-line copy is usually a depth artifact, not missing statements**, when its expansion elsewhere already matches instruction-for-instruction (`TEnemyPolluteModel::generate` 0x178 vs 0xf8; `addCheckDataToList` 0x27c vs 0x124). Conversely a named local inside an UNUSED helper can be sized by the map (`drawModelStamp` 0x5c only with the draw buffer bound to a local; its duplicated copy in `countTexDegree` re-reads the member).
- MWCC fully unrolls a constant-trip clearing loop whose stores fold to constant offsets from `this`; retail's unroll-by-8 + `ctr` shape means the base was not foldable (`TNpcParts` ctor; three real index bugs fixed there; header item: `TNpcModelData`'s first two words are `const char* unk0[2]`). Free-function vs member linkage in the map decides which register an inlined callee's result must land in (`addGroundNode` etc. are `__F...` free functions => r3). `SMSGetPollutionLayer(i)` is +16 at a `getLayer(i)` site inside a loop (second unit). `getListRoot` (UNUSED) sized for a dead non-trivial 12-byte local (0x108 exact), not committed.

## Third-pass batch 103: five of the earliest stuck units closed

- **An UNUSED symbol left as a stub is a frame-gap suspect, not just a size warning**: `TCameraBck::isDemoFinished` (0x70) was a `void` stub; restored as `bool` and returned from `updateDemo`, it put the result in r31 as retail and re-created the +8 the caller needed (CameraBck linked). Check a unit's UNUSED-size mismatches before hunting levers.
- **Lever pairs, fifth and sixth confirmation** (`getTexture()` + a named cue bool in `TMapObjWave::perform`; `getTime()` + `getMarchSpeed()` in `TNerveBossHanachanTumble::execute`), and **a fourth lever must be on a class the first three do not touch**: three same-class rungs saturate (`MSBgm::init`'s 0x40 ceiling over 72 spellings fell to one level on `JAIBasic::unk0`, parked `MSBgmGetAudioData`; header item `JAIBasic::getData()`). MapObjWave, BossHanachanNerve, MSoundBGM linked.
- **A TU-local `static inline` free function taking a pointer and returning a value binds 8 bytes only when the argument is a fresh global load** (`MSBgmGetAudioData(MSGMSound)` +8; the same shape with `this` +0 — TalkCursor). Binding the helper's own result inside it is a further +8. **A free function wrapping a real `bl` can be spelled to preserve argument order**: `CameraSecureViewDistX(this)` keeps retail's min-then-max loads where a helper taking `(this, min, max)` reverses them — take parameters out of the wrapper (CameraSecureView linked).
- `TMewManager::loadAfter`'s carrier confirmed: a `static inline` wrapper around `createRandPlayVec` holding an uninitialised `TVec3` gives 100% (a dead 12-byte local in the function's own body is +8, in an inlined callee +16); it must be shared (`MSound.hpp`) because Bird's two `loadAfter`s need the same level — header item closing four Animal functions. Open cross-unit question: `AudioDecoderForOnMemory` and `TMapObjRevivalPollution::loadAfter` both have retail interleaving a source local *above* the compiler-generated temporaries while every spelling puts both temporaries on top (research). `SMS_IsMarioOnWire` (17 vs 18 instructions): every documented CSE breaker fails.

## Lever sweep batch 99 (2026-09-18): 126 functions improved, 34 to exact

- Hit rates by lever: `getMActor()` over the bare member 10.4% (the best lever, and every large jump: `TPakkun::perform` 93.7 -> 100, `setHeadAndBodyAnm` 92 -> 98.6, `THauntLeg::init`, `TBossWanwan::shakeCamera`, `TGorogoro::init` — all `mMActor` receivers in front of argument-heavy calls, gap 0: it fixes the receiver's register, not the frame); `getScaling()`/`getRotation()` 8.1%; own-object `getPosition()` 6.4% (94 keeps — the bare `this`-relative form is where the yield is); explicit-receiver `getPosition()` 5.2%; the current-map ladder 4.8% (per function: `getCurrentMap()` alone closed `TShellCup::loadAfter`, the `SMSGetMarDirector()` rung only `TTurboNozzleDoor::touchPlayer`). `getModel()` on an explicit `->mModel` receiver is inert everywhere; the flag family is inert (byte-identical) except in one 0x448-frame function. `&actor->getPosition()` as `startSoundActor`'s `const Vec*` is a real lever (+0x28 `calcAtkParticleAndSE`) — per site, contradicting the Animal-`loadAfter` note.
- **Named-result lever (lever 6) is exhausted**: all seven hand trials moved the frame onto the target and cost 4-16 points — retail genuinely re-reads the parameter/predicate, so the residual 8 bytes in the ">= 99.5% and 8 short" population (313 functions) is an unnamed object, not a named result.
- Process: a sweep's worktree must be re-based before landing when units were linked meanwhile (MapObjBlock's edits had to be reverted; chuuhana/MapStaticObject lost 0.2 to interaction with the depth sweep and were restored), and after reverting a hand trial with `git checkout` the stale `.o` makes the driver's "before" snapshot the broken build (rebuild before measuring). Driver `lev.py` (occurrence-based, write-context filter, full-set-first) in the batch-99 scratchpad.

## Closure batch 109: the `a = b - c` pool residue and allocation mechanics

- **Five "depth-sweep 99.6-99.9%" functions share one residue** (koopajr/limitkoopa `bind`, `TLeanMirror::loadAfter` -12; `TFlyEnemy::fly`, `TCannon::moveObject` -32 through an inlined `updateAttachPos`-style callee): our `a = b - c` expansion reserves 16 dead bytes *below* the `operator-` by-value parameter temp; retail reserves ~4 below and a 12-byte dead object *above* it. Frame totals identical; only the `addi rD, r1, N` and six loads/stores around the `bl` differ. Refuted in a scratch TU: `operator-` returning by value (+6 instructions), a by-value wrapper, an inlined callee taking `TVec3` by value (+6), prvalue/cast/block-scope/`getVel()` left operands (slot-identical), a dead named `TVec3` after the local (right structure, 12 too high), a dead `TVec3` in the UNUSED callee (moves the temp the wrong way). Research item.
- **Allocation mechanics measured**: the inline-temp pool fills downward from just below the function-scope named locals in expansion order; function-scope named locals fill downward from the frame top in declaration order (first-declared highest); a non-leaf with only register arguments reserves no outgoing-parameter area (locals start at 0xc). **Slot order names the declaration form**: `TCannon::moveObject`'s zeroing vector above the velocity copy proves both were function-scope locals with the zero one declared first (`zeroVel.set(0,0,0); mVelocity = zeroVel;`).
- Rungs: `this` accessors (`getGroundPlane()`, `getRotation()`, `isGold()`) saturate against each other; a class forwarder for the inherited `mSpine` (`getSpine()`) is a separate rung (+8 per two-level `getTime()` site); a params-class `getXxx()` wrapper is +8 per nerve where `getParam()` is out of line, and an owning-class forwarder above it does not stack; `SMSGetMSound()` is a genuine +4 after the two-argument `startSoundActor`'s +8 (`TKiller::bind` closed); `SMS_GetMarioSpeedX/Z()` over the raw globals +8 as a pair; `getPosition()` on two of three `TBGWallCheckRecord` arguments (the `y` argument stays raw). Closed: `TKiller::bind`, `TKoopaJrSubmarine::moveSwing`, `TNerveKoopaJrSubmarineLaunchKiller::execute`. Real fix: `TNerveLimitKoopaGetDown` read `tumbleWeight` (0x1e8) for `downSpeed` (0x1d4). `TBossHanachan::execWalk` is 16 over with five permuted stack objects — `target` is probably a temporary of an inlined distance-test helper.

## Second-pass batch 110: the binding level (three units closed)

- **The single most productive lever: a TU-local `static inline` free function that takes the receiver by pointer and binds its result is one extra inline expansion, worth +8 of low region.** The binding pays — the same forwarder returning directly is +0, and a thin level above a real `bl` is +0 at any spelling. Generalises batch 103's "fresh global load" note: with `this` it is +0 without a binding and +8 with one. **Nested binding levels stack additively; multiple bindings inside one level do not** (`PollutionPosIndex`: two nested one-binding levels +24, one level with two bindings +16). **A binding level can be a register lever independent of its frame cost** (one on `index()` restored `getEdgeDegree`'s whole scratch-register assignment and `lbzx` base).
- Closed and linked: `TSnapTimeObj::perform` (one level above `TTimeRec::endTimer()` at the DRAW_INIT site moves the inlined `startTimer(u32)` colour temporary +4 with the frame unchanged — the (+4, 0) header round 18 could not find); `TPollutionPos::isSame` (three binding levels, one per member read: `isInArea`/`getDepth` +0 each and +16 together, `worldToDepth` +8) and `getEdgeDegree` (two nested levels on `index()`); `TTalkCursor::loadAfter` (any single binding level) and `associateNPC` (model fetch +0x10, NPC receiver +8).
- `TSunMgr::perform` closed (`SMS_GetMarioX/Z()` +8 as a pair, bound `SMSGetMSound()` +8, the BGM handle at three sites +8 each); `load` now frame- and instruction-exact with an r30/r31 swap (retail ranks the `.rodata` pool address above `this`; the liveness rule does not cover it). `TRideCloud::setGroundCollision` closed (`mMapCollisionManager` through one binding level); `control` frame exact with a 3-register FPR permutation left (the 300.0f literal in the freed register). `PerformList::perform`'s residue is the JGadget iterator temp-pool *grouping*, not a frame gap. A level above an inlined callee moves the caller's pool (direction depends on expansion order); above a real `bl` it is free. Chained `>>` continuations reconfirmed at 8 each. `TSunModel::isInBounds` carrying a dead 48-byte non-trivial local is the only lead for `TLensGlow::perform`. Note: editing `Map/PollutionPos.hpp` breaks the DOL (linked Pollution TUs depend on `index()`'s codegen).

## Research batch 113: the `a = b - c` temp sits 4 above the local floor in retail, 16 in ours

Measured in a scratch TU against the real `JGVec3.hpp` with the game flags, on the `TKoopaJrSubmarine::bind` shape (frame 0x40, 55 instructions, `next` at 0x28 in both builds).
The reference point is the **local floor 0xc** (a non-leaf whose calls take only register arguments reserves no outgoing-parameter area), and the number that matters is the `operator-` by-value parameter temp's distance above it.

- **Retail puts that temp 4 bytes above the floor, at 0x10, in every function whose calls are register-only** — `TKoopaJrSubmarine::bind`, `TLiveActor::bind`, `TBaseNPC::bind` and `TEnemyAttachment::bind` all have `addi r3, r1, 0x10`. Ours puts it 16 (or more) above, immediately below the function-scope named locals, and the dead bytes end up *below* it instead of between it and the locals. Batch 109's "12-byte dead object above" is that gap, not an object: the per-function delta is whatever else the pool holds (12 koopajr/LeanMirror, 16 liveactor, 20 enemyAttachment, 24 NpcCollision, 52 chuuhana), so it is a **pool-order swap**, not a missing declaration.
- **The 12 extra bytes decompose, and 8 of them are the `operator-=` level.** From the floor: the reference return that the caller copies out of is +4 (unavoidable for `a = b - c`); routing `operator-` through `operator-=` instead of calling `sub` directly is a further +8. `void operator-=` and a void-returning intermediate (`fst.subOf(snd)`) both land 8, i.e. halfway. The last 4 was not isolated.
- **`operator-=` must keep its `TVec3&` return.** The `void` spelling was measured project-wide: -1 function, ~35 regressions and no gain, and it breaks the member's own out-of-line copy — `__ami__` (weak, 0x34, Tongue.cpp) drops 100 -> 98.5 because retail loads `other.x` before `this->x` and only `return *this;` orders the two `lfs` that way. Recorded at the declaration; the 8 bytes have to come from somewhere else.
- **The only construct that puts the temp *at* the floor is a callee that takes the source by value and writes the destination it was handed**: `inline void f(V* d, V a, const V& b) { a.sub(b); *d = a; }` gives `addi r3, r1, 0xc`, one instruction count identical to the reference-returning form and a frame 8 bytes smaller — and it stays at the floor even when the intermediate returns `V&` and even when the left operand is a named local. Copy-out from a *returned reference* is what lifts the temp. No spelling of `a = b - c` through `operator-` can avoid it, so if retail's sites are really `a = b - c` the lever is elsewhere; if they are not, this helper shape is what to look for.
- Refuted here, all slot-identical to the stock header (frame 0x40, temp 0x1c, 55 instructions): `operator-` as a namespace-scope inline instead of a friend; `return fst -= snd;` and `return fst.operator-=(snd);`; `operator=` returning `void` or taking `const Vec&`; `operator-` returning `const Vec&` or non-const `TVec3&`; `operator-=` returning `*(TVec3*)this`; wrapping the site in a bare block, an `if`, or one or two inline helper levels; `mLinearVelocity.set(next - mPosition)`. Costing +6 instructions (all forms of a by-value return or a by-value `operator=`): `TVec3 operator-`, `TVec3& operator=(TVec3)`, a helper returning `TVec3`, a named `TVec3 d = next - mPosition`. A dead `TVec3` in an inlined callee, before or after the statement, always lands *below* the temp (+8 frame), which is why batch 109 saw it move the wrong way.
- **Census — the residue is worth ~40 near-exact functions.** Retail has 102 `bl TVec3::sub(const TVec3&)` sites in 57 units (`fly`, `bind`, `loadAfter`, `moveObject`, nerve `execute`s, eight `TMario` wire functions). **Not one of them is byte-exact in our build**, and 40 are at 99.3% or better, so a single header or call-site spelling closes them together. Full list in the batch-113 scratchpad (`subsites.json`); probe driver `drv.py`/`cc.sh` there.

## Own-class accessor sweep 112 (2026-09-18): 49 functions to exact, game code 39.6 -> 40.25%

- Population: 589 unlinked functions at >= 99.5% short by 8 (388) or 16 (201); 187 had a raw own-member read with an existing getter (830 in-class accessors over 310 classes, resolved through the base chain); 70 of 183 groups kept (38%), 181 sites across 40 units. **A lever saturates per member, not per function**: 45 of 74 keeps took all sites, and bisection only paid on multi-member groups where one member's sites won. Members that paid: `mPosition` 17 groups, `mMActor` 10, `mStatus` 9, `mGroundPlane`/`mVelocity`/`mScaling` 6, `mModel`/`mManager` 5. The own-class fork is exhausted on this population (a widened getter index and a wide subset pass added four).
- **Missing getters ranked for a header round** (`missing_final.txt` in the batch-112 scratchpad, 741 pairs): `TLiveActor::getSpine()` (31 functions across 24 derived classes; TTinKoopa/TBossHanachan/TSeal already declare a private one), `TMario::mForwardVel`/`mFaceAngle`/`mVel`/`mInput`/`mModelFaceAngle`/`mStatusTimer`/`mFloorPosition`/`mSlopeAngle`/`mStatusState`/`mWallPlane`/`mSlideVelX/Z`, `JPADraw::mDrawCtx`, `MActor::mAnmByType`, card-code `unk*`s, and const overloads (`TTakeActor::getHolder()` blocks six const callers; `TPollutionLayer::isPolluted`, `TMapCollisionBase::mCheckDataNum`, `TCoin::perform`, `TBathtub::load`). Drivers `getters.py`/`own.py`/`gen_gap.py` there. MarioCollision's pre-existing MISSING `TVec3` copy-ctor/`__amu__` come from a still-nonmatching function in that TU.

  Superseded in part by batch 116 below: the count is 130 sites, and the "+4 unavoidable, +8 for `operator-=`" decomposition is really `8 x consumed reference returns` plus a pool-ordering difference.

## Research batch 116: the `bl sub` slot is a pool *ordering* difference, and prefix 4 is off our ladder

Same scratch TU and flags as batch 113, on the strictest model available: `TCoasterEnemy::bind` is 55 instructions in both builds, frame 0x40 in both, `next` at 0x28 in both, and the *only* difference in the whole function is `addi r3, r1, 0x10` (retail) against `0x1c` (ours), plus the six loads/stores that use it.
Writing the temp's offset as **floor (0xc) + prefix**, ~200 spellings were measured.

- **The ladder is `prefix = 8 x (reference returns the caller copies out of)`, and inline levels themselves are free.** Stock (`operator-` returns `const TVec3&`, `operator-=` returns `TVec3&`) is 16; `void operator-=` is 8; a destination-pointer helper that consumes no returned reference is 0. Inserting one or two extra void forwarders between `operator-` and `sub` changes nothing and `sub` still goes out of line at depth 5, so **depth is not the lever** — only the number of consumed reference returns is. The floor is 0xc and is insensitive to callee arity up to 8 register arguments (9 arguments move it to 0x14).
- **Retail is at prefix 4, which is off that ladder**, and it also has **12 bytes of pool *above* the temp** (0x1c..0x27 in `TCoasterEnemy::bind`) that our build does not have. Both builds reserve the same 28 bytes of pool, so nothing is missing — the entries are ordered differently.
- **Prefix 4 is reachable exactly once**, with `friend const TVec3& operator-(const TVec3& fst, const TVec3& snd) { TVec3 r; r = fst; r -= snd; return r; }` plus `void operator-=` *and* `void operator=` (55 instructions, temp at 0x10). But that shape's gap above the temp is 4, not 12, and **every construct that adds bytes above the temp also adds 4 to the prefix** (a bound `const TVec3&` or `const TVec3*` in `operator-` is +4/+4; a second binding is +0/+4; a dead named `TVec3` after the caller's local is +0/+12 but costs 8 of frame). (4, 12) was not produced by any combination of `operator-`, `operator-=`, `operator=`, copy-ctor, call-site and dead-object spellings tried.
- **Multi-site functions name the mechanism.** Retail spaces consecutive `a = b - c` temps 20-24 bytes apart (`TYoshiTongue::movement` 0xd0/0xe8/0x100, `TEnemyMario::emReplayJumpToNearestNode` 0x11c/0x130/0x148); ours packs them 12 apart and hoists every level byte to the bottom of the pool (scratch three-site probe: temps at 0x3c/0x48/0x54 over a 48-byte prefix). So retail allocates *per statement* — the by-value temp first, then that statement's level bytes — while we allocate all the level bytes for the whole function first. The per-function delta batch 113 saw (12/16/20/24/52) is that hoisting, not a missing declaration.
- **Also refuted here.** The map's two-argument `sub__...RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>` (weak, 0x34, limitkoopa.cpp) *is* emitted, and its 13 instructions are the plain `x = fst.x - snd.x;` body we already have — it is not `set(a); sub(b);` and it is not what the one-argument sites go through. Removing the user-declared `operator=` (letting MWCC generate it) loses the level and makes `sub` inline, so `operator=` must stay a real level. Both operands by value, `operator-=` by value, `operator-`/`operator=` returning by value, an explicit `TVec3(...)`/cast at the site, and `getPosition()`-style accessors on either operand all cost +6 instructions or move the block the wrong way.
- **Census reclassified** (130 one-argument `bl sub` sites, 57 units; batch 113's 102 was after filtering). By the copies bracketing the call: **112 have a copy in and a copy out** (`a = b - c` into a member, a named local or a temp), **16 have a copy in only** (the difference consumed as a value — `TBossGesso::doAttack*`, `TAmenbo::control`, `TNerveTabePuku*`), **2 have neither**. **122 of the 130 already emit the `bl` at the right point in our build**; only 8 differ in count (`TCannon::killerShoot` 2/0, the two `TNerveBossTelesa*::execute` 1/0, `TSphereLink::moveHead` 2/1, and four `TMario` wire functions plus `TBossGesso::doAttackSingle` where we emit more than retail). So the depth work is done and the entire population is blocked on this one slot.

## Binding-level pass 115: two units closed

- **A dead low-region gap can be a named local in the wrong place**: scalars held in an inlined callee are trivial PODs worth 0, while the same scalars as the caller's own block locals reserve 8 above the pool (`TBGPolDrop::perform`: the X-rotation block is the map's weak `MsMtxSetRotX(Mtx, f32)`, 0x7c, identified by compiling it out of line at 124 bytes on the nose; bgpoldrop linked; header item next to `MsMtxSetRotRPH` in MathUtil.hpp). The inverse family has a -8 lever: a named intermediate pointer one statement earlier in an accessor chain (`J3DModelData* modelData`; `SMS_InitChangeNpcColor` closed, NpcColor linked; the raw chain is -16).
- **`search<T>` binds its result, `search2` does not; a binding level on `search<T>` is +12 per expansion**, and the `search2`+cast temporary is one shared 4-byte slot however many sites use it (`TMarDirector::preEntry` frame exact with four binding levels plus one plain `search2`; residue one 4-byte slot; header item: `JDRNameRefGen.hpp`'s `search<T>` should bind). **Global forks are not independent of other levers**: `SMSGetMarDirector()` was +0 alone and +4 once two binding levels were in place (`EffectUtil`, frame and every displacement now exact; residue a three-way FPR rotation with results ranked descending by component) — measure global forks last.
- **A binding level is a frame lever but not always a register lever** (three counter-examples: `TMapObjRevivalPollution::loadAfter`, `NPCNeckCallBack` — two `gpCurrentNpc` levels land 0x190 but in the wrong slots — and `SMS_IsMarioOnWire`, where a binding level on the holder fetch is byte-identical: it is not a CSE breaker); batch 110's register claim is narrowed to chains the caller's own receivers go through. A parameter binding is free where a global binding is +12. A zero-parameter level can be free (`TUtil<f32>::one()` in `execPosInbetween`) — the lever there must change liveness ranking. The liveness probe costs one `stw` and a use that constant-folds does not rank the local (`AudioDecoderForOnMemory`). Diagnostic: a dead 4-byte named local declared last lands the frame and all named slots, isolating a residue to one expansion temporary (`warpPosAndAt`: one 4-byte temp after the `getUsualLookat` sret).

## Research batch 133: an **implicit** derived-from-base conversion on a `return` reserves 16 bytes

Model: `JGadget::TList_pointer<T*>::insert(iterator, const T&)`, a weak 0x4c symbol whose retail copy lives in `Map.a MapWireManager.cpp`.
It is 19 instructions in both builds and the only difference is the frame: retail 0x30 with its three pool words at 0x18/0x1c/0x20, ours 0x40 with them at 0x2c/0x30/0x34.
Forcing an inline member out of line for measurement is easy: take its address in a scratch TU (`L::iterator (L::*p)(L::iterator, THitActor* const&) = &L::insert;`).

- **The lever is the spelling of the conversion on the `return` statement.**
  `return Base::insert(where, what);` — where the callee returns the base iterator and the function returns the derived one — reserves **16 extra bytes at the bottom of the frame**, with the whole pool riding 20 bytes higher and 4 fewer dead bytes above it.
  `return iterator(Base::insert(where, what));` removes them and lands retail exactly, instruction stream unchanged.
  The signature of this residue — **+16 frame, pool 20 low / 4 high** — is the one `SDLModel::entry` shows after `++it`, so read a "+20 low / -4 high, +16 frame" gap as a missing explicit conversion first.
- It is the *implicitness* that costs, not the conversion or a named result: `iterator r(Base::insert(...)); return r;` is 0x48, worse again, and `insert` taking `const iterator&` does not compile against `TList_pointer_void`'s fixed signature.
  `begin()`/`end()` in the same class were already spelled `iterator(Base::begin())` and are byte-exact, which is the style evidence for `insert`.
- **Everything else in `std-list.hpp` is frame-inert for this function** (all 19 instructions, all frame 0x40): stubbing `TList::insert`/`CreateNode_`/the destructor/`erase`, deleting `TList_pointer<T>::begin` or either iterator's `operator++`/`operator*`/`operator==`, public instead of private inheritance at either level, a user copy ctor or `operator=` on `TList::iterator`, a one-word `TNode_`, and renaming the shadowing `typedef Base::iterator Base;`.
  A stripped micro-model reproduces the fork in nine lines, so the property is the conversion and nothing about JGadget.
- **Post-increment's cost on a JGadget iterator is its by-value return slot, not its copy local.** Respelling `operator++(int)` as `TNode_* p = p_; p_ = p_->pNext_; return iterator(p);` — no `iterator copy` at all — is byte-identical to the stock `iterator copy = *this; operator++();`, and the extra pool word survives. Since retail has no such word, **retail's list walks are spelled `++it`** (`SDLModel::entry`'s grouping with `++it` is retail's exactly).
- Open: the `TSeal::init` / `TMirrorActor::init` / `TPerformList::perform` gaps are *not* this lever (applying it leaves all three byte-identical), and no `std-list.hpp` or `singlelinklist.hpp` spelling moved them. Retail's inter-group gaps are 4 (seal, one word) and 8 (MirrorActor, two words per gap) smaller than ours with the group *sizes* already right. Measured and rejected on the four callers: `push_back` with a named `end()`, `insert`/`iterator`/`operator==` taking `const&`, named returns everywhere in the chain, implicit `begin()`/`end()`, explicit by-value slicing of `where`, `push_back` bypassing `TList_pointer<T>::insert`, and dropping `getChildren()` at the site (-8 frame, gaps unmoved). For `TPerformList::perform` the bottom `[4]` splits into retail's `[2]+12 [2]` shape only by deleting the derived iterator's `operator==` (13 slots, gap 8 not 12) or its `operator!=` (11 slots, wrong count) — recorded at the declaration, not applied.

## Research batch 119: the per-statement geometry is a **by-value return**, and it costs 6 instructions

Same scratch TU, flags and model as batches 113/116 (`TCoasterEnemy::bind`: 55 instructions, frame 0x40, `next` at 0x28 in both builds; retail `addi r3, r1, 0x10` against our `0x1c`).
Retail's pool is written here as **(below, above)** = dead bytes under the live `bl sub` temp and between it and the named locals.
Retail is **(4, 12)**; the stock header is **(16, 0)**.

- **Retail's `a = b - c` allocates two 12-byte vector slots per statement, and the lower one is the live temp.** `TYoshiTongue::movement`'s retail pool is a *dense* run of 12-byte objects (0xb8, 0xc4, 0xd0, 0xdc, 0xe8, 0xf4, 0x100), of which the three `bl sub` temps are 0xd0/0xe8/0x100 — the 24-byte spacing batch 116 measured is two slots, not a slot plus level bytes.
- **The property that produces that geometry is `operator-` returning `TVec3` *by value*.** The return slot is allocated per statement, immediately above that statement's by-value parameter temp, and the pool then fills downward statement by statement. On the three-site scratch probe this reproduces retail's spacing exactly: stock temps 0x3c/0x48/0x54 (12 apart, 48-byte hoisted prefix) against by-value 0x18/0x30/0x48 (**24 apart**), frame 0x90 in both.
- **Adding exactly one bound `const TVec3*`/`const TVec3&` inside `operator-` supplies the 4 bytes at the pool bottom, and the single-site model then lands on retail's layout precisely**: `friend TVec3 operator-(TVec3 fst, const TVec3& snd) { const TVec3* p = &snd; fst -= *p; return fst; }` gives frame 0x40, temp **0x10**, `next` **0x28**, (4, 12). That geometry is invariant under `operator-=`/`operator=` returning `TVec3&` or `void`, dropping the user copy-ctor, `operator=(const Vec&)`, and the call site spelled `m = a - b`, `setLinearVelocity(a - b)` or `m.set(a - b)`.
- **But every by-value return costs +6 instructions (61 against 55), and nothing removes them.** MWCC 1.2.5 has no NRVO and no parameter-to-return elision: returning the by-value parameter, a copy-constructed or copy-initialised named local, a constructor expression, a member `operator-` instead of a friend, a `const`-qualified by-value return, and letting the copy-ctor or `operator=` be implicit all emit the three-word copy into the return slot. **In our build the second 12-byte slot and 55 instructions are mutually exclusive** — every reference-returning spelling allocates exactly one slot and can only put dead bytes *below* it. Retail reserves the return slot and elides the copy into it; that is the whole remaining residue for the 130-site population.
- **New lever, and a project-wide trap: a user-declared `~TVec3() { }` collapses the inline-temp prefix to 0** (temp at the floor 0xc, frame 0x30, still 55 instructions) and makes callee-side dead objects free. It is the only construct found that reaches prefix 0 without changing the instruction stream. **Do not ship it**: measured project-wide it is total fuzzy_match 97.36 -> 95.87 and matched_code 50.85 -> 48.82, because it also makes `TVec3` non-trivial everywhere — `__dt__17TMarioPositionObjFv` and `__dt__18TModelWaterManagerFv` 100 -> 46.5, `loadYBBMtx__7JPADrawFPA4_f` 99.8 -> 10.1, most of JPADraw/JPAField/JPAEmitter and every `TVec3` array member's implicit dtor. Reverted.
- **Dead-pool sizing law (useful for reading any frame gap).** A dead *named local in the caller* adds its own size **above** the temp — `TVec3`/`Vec` 12, `u8[8]` 8, `u8[4]` 4, `f32` 0 (register-allocated) — while a dead object inside an inlined callee adds **below**, and is dropped entirely once `TVec3` has a destructor.
- **Also refuted here, all slot-identical to the stock header** (frame 0x40, temp 0x1c, (16, 0), 55 instructions): `TVec3` as the **primary class template** with the operators in class rather than an explicit `template <> class TVec3<f32>` specialisation (hypothesis from batch 104 — the specialisation is not the problem); `operator-` as a **namespace-scope `inline` function template** `template <class T> TVec3<T> operator-(TVec3<T>, const TVec3<T>&)` or as a namespace-scope non-template, both with a reference and a by-value return (the by-value forms give the same (0, 12) and the same +6 as the friend); a memberwise copy-ctor; no user copy-ctor at all; and up to four bound references inside `operator-` when a destructor is present (with a destructor the prefix stays 0 whatever is added below).

## Closure batch 118

- MarioWait closed and linked (`stopCommon`: one binding level on `mYoshi`; header item `TMario::getYoshi()`). **A trailing dead 4-byte scalar is a real "+4 named" lever when the frame is already exact and the whole named block sits 4 low** — declared last (first, it is absorbed by frame padding; check for 8 bytes of slack first: `drawRefracAndSpec` is the counter-case). `TMapObjTree::controlLeaf`'s own copy-paste bug named it (`f32 absAngularVelocity`).
- Lever pairs across classes (a search-chain binding level +8 and an own-class `getAttackRadius()` +0 alone, +16 together — `TMapObjTreeScale::loadAfter`). **Declaration order of a `TVec3` against a `MtxPtr` is a +-4 positional lever** (`emitSweat`: `pos` before `mtx`). **`getM3UModel()` over `mModel` is worth +8 per *pair* of sites** (`warpOutEffect`, bisected over twelve: any exact-frame subset of a large identical site set is arbitrary — the real lever is elsewhere). A float truthiness test (`if (p.get())`) vs `!= 0.0f` swaps the `fcmpu` operands (second unit). **`if/else if` vs two sequential `if`s is readable off the frame**: mutually exclusive arms kill the receiver's live range across the intervening call (`TNerveNameKuriLand::execute` retail 0x8 vs our 0x20). The const-accessor CSE rule cuts the other way for `checkLiveFlag`/`onLiveFlag` in one statement pair (raw `mLiveFlag` read, `TNameKuri::isHitValid` — which also had a whole missing `onLiveFlag(HIDDEN)` block). `(T*)TLiveManager::getObj(i)` can *remove* 8 where the three-level accessor overshoots. A binding level applies per call site within one function (`TTobiPuku::changeOut`: `mJuiceBlock` bound at the `kill()` site only).
- Real bugs: `kickRoofEffect` reads `mJointIdChnFootR`, `kickFruitEffect` writes `mGlobalTranslation`. Open: namekuri's `setDeadAnm`/`setMeltAnm`/`init` short by exactly 40 (a dead 12-byte named local above the unnamed `setVelocity(TVec3(0,0,0))` temporary at 0x48 plus 28 low); `drawRefracAndSpec` (seven GXColor temporaries 4 high); `TTobiPuku::TTobiPuku` (retail spills `this` to `8(r1)`). **Tooling trap: `decomp-diff.py -d` prints "100.0% match" for a function that is not exact** (slot offsets differed) — trial drivers must count `~|<>` markers, not read the header; `sweep.py` in the batch-118 scratchpad does.

## Binding-level sweep 121 (2026-09-18): 53 functions to exact, game code 41.0 -> 41.4%

- Driver `bind.py` (with `acc.py`: 1697 zero-argument in-class accessors, 9777 fields, 157 global pointers) generates the level verbatim per site — `static inline RET PrefixMeth(const Cls* p) { RET v = p->meth(); return v; }` — full same-receiver set first, singles, then cross-receiver pairs; 507-function population, 542 + 70 trials, 80 groups kept over 56 units. **A single binding level at a single site is worth +8 in 53 keeps but +16 in 12** (batch 110's +8 is a floor). Saturates per receiver (12 keeps took more than one site; three sites still bought only +8 twice); six keeps needed a cross-receiver lever pair. Accepting sites: local/parameter receivers 55, implicit `this` 32, own member pointer 12, global 6; by method `getMActor()` 14, `getModel()` 5, `getPane()` 4 (the batch-99 ranking, now for the level above the accessor). Nested levels are a thin second rung (3 of 15; one stack gave only +8). **A mutator forwards as well as an accessor** (`unsetUnk165` through a level closed a nerve) — the lever is the binding, not the getter.
- Interaction rule: a sweep started before a header round must be re-verified per function after landing — header round 20's getters had already closed `TBubble::init`, `startAppearCoin`, two CardSave waits, `readRailFlag`, `boardJumping` and `calcCreateParticle`, and the sweep's levels on top double-levered them (restored to the getter spelling). Headroom: 57 unswept lowest-score files, 227 functions with no resolvable candidate (template/namespaced bodies), the nested round on the 8-short population. Kill trial waiters by pid, not `pkill -f`.

## Closure batch 120

- **A missing `void` return type is a register-allocation bug**: `TMarioGamePad::read()` declared `u32` with no `return` kept the callee's r3 alive across the body, shifting every scratch register (both functions now exact — but the unit still rests on the pre-existing `u32 stackAlloc[83]` padding in `updateMeaning`, 336 bytes of dead low region over 26 inlined expansions; **do not link MarioGamePad** until that is honest). **The dead-4-byte-local-after idiom generalises beyond path buffers** (`TLensGlow::TLensGlow`, `read()` after `s32 resetPort`, `TMario::drawSyncCallback` after the `GXPeekARGB` target): one named slot 4 low at an exact frame, moved only by a local declared after it. **A loop counter declared in the `for` init vs ahead of the preceding local swaps their scratch registers** (`int i;` before `u16 dc`, `updateMeaning`'s unroll-by-8 loop).
- **A named pointer local is a zero-instruction "+4 named / -4 low" lever** at an exact frame (`M3UMtxCalcBlendAux`); a compiler-generated global-address temp's callee-saved rank floats against the source values' live ranges (shortening one promotes it) and inline levels are inert on it. **Rogue-include order is load-bearing for string-pool offsets and objdiff scores it 100%**: `System/DummyStrings.hpp` must precede `Player/MarioDirtyStrings.hpp` (`MarioCap`'s `cDirtyTexName` 0x20 low). **Classify a large gap by inter-slot deltas**: uniform => low region (levers); growing between an `operator-` copy and its consumer => the batch-113/116 pool-ordering residue (`TMarioCap::perform`, levers wasted); a hole in the named block => a missing declaration (`TMario::receiveMessage`'s 68-byte object).
- Binding levels: over the raw `gpMapObjManager` +8 per expansion, three exclusive branches paid it (`TMario::getGesso` closed; header item `SMSGetMapObjManager()` binding); a binding fludd accessor +16/expansion at 8 of 9 `mWaterGun` sites plus `getM3UModel()` landed `TMario::perform`'s 0x168 (the real `getFludd()` is only +16 total: the rung is the binding). `MarNameRefGen_BossEnemy`'s last residue is one level between `TSleepBossHanachan`'s ctor and `TVec3(f,f,f)` (no defaulted-argument forwarder: the map has no `__ct__18TSleepBossHanachan` at any arity). `TLensGlow::perform`'s 96 missing pool bytes are two inlined shared callees (`TSunModel::isInBounds`, `J3DFrameCtrl::update` x2, 48 each). MSoundScene: retail re-derives one indexed load independently (`lwzx` for `getDistFromCamera(mMaxTrans[d][rank])`) and keeps `rank`/`rank << 2` callee-saved; its three empty UNUSED stubs are the frame candidates.

## Binding-level sweep continuation 124: +32 exact, JPAParticle closed

- **The resolver, not the lever, was the limit**: the previously-unresolvable functions gave 21 keeps from 43 (49% vs 22% for the unswept low-score files). Two parser bugs hid them — a one-line `RET C::f() const { return x; }` definition swallowed the following function (51 of 65 NOFUNC cases, every `DEFINE_NERVE` after a one-line getter), and namespaced bodies (`int TTrack::noteOn` indented inside `namespace JASystem {`) failed a column-0 test. **One-argument indexed/predicate accessors accept a level at least as readily as zero-argument ones and dominate the keeps** (`checkFlag` 5, `isState` 4, `isBckAnm` 4, `checkLiveFlag` 3, `checkMapObjFlag`, `checkSdlFlag`, `checkStatus`, `getLightSet(i)`). **A two-level stack on an 8-short frame is usually the whole gap** (13 of 69, eleven landing exactly, eight of them indexed accessors), whereas on a 16-short frame it is a partial step. `getCurrentMap()` is a receiver that paid three times (`SunGlass`, `MapMirror`, `MapObjMare` load/init to 100). JPAParticle 100/100 and linked. Headroom: 217 population functions with no accessor call site at all (a level over a raw member read is untried there) and the header-resident `MActorAnmDataEach<T>::loadAnmPtrArray` instantiations. Drivers `bind2.py`/`acc2.py` in the batch-124 scratchpad; wait on the python pid, not `pgrep -f`.

## Raw-member binding sweep 127 (2026-09-18): 100 functions to exact, game code 42.2 -> 43.2%

- **A binding level over a raw member read pays like one over an accessor**: `static inline T PrefixFoo(const Cls* p) { T v = p->mFoo; return v; }` (+8, occasionally +16, per expansion; saturates per member — 121 of 142 keeps took one member's full site set). Receivers: implicit `this` 112 sites, pointer local/parameter/member 29, **the fork-plus-binding form over a global 42** (`gpMarioParticleManager` 9, `gpMSound` 7, `gpMarDirector` 7, `gpMap`, `gpCameraShake`, `gpItemManager`) — the single most reliable shape. Members: 100 pointers, 52 scalars (including `unkNN` placeholders). **The lever is independent of the accessor lever**: the already-swept accessor-bearing population gave 47 keeps from 74 (64%). Two-level stacks are a thin rung on raw reads (5 of 226). **The register lever is real but rare** (6 of 141 frame-exact functions, every one straight to 100%: `TGesso::pollute`, `TSunShine::perform`, `TBellDolpic::control`, `TFireWanwan::receiveMessage`, `TNPCManager::makePartsModelData_`, `TBossEelHeartCoin::perform`). Struct members are excluded (a by-value copy is +6 instructions). ItemManager, NpcManager, JDRDisplay closed and linked. Headroom: 111 frame-exact accessor-bearing functions below the top-80 cut, 82 no-accessor functions with only struct/array/private members (a private field needs a member level). Drivers `raw.py`/`acc3.py`/`unlev.py`/`gen_gap2.py` in the batch-127 scratchpad; the driver refuses trials that move the frame further from retail's. Traps: killing a trial mid-build leaves the trial text in the source; a `.c` TU breaks a prefix derived by stripping four characters.

## Second pass 128 on the Sonnet units (Opus): six functions closed

- **A named result can be a "+8 low" lever where the whole global-accessor ladder is +0** (`TMapObjFlagManager::load` closed: `char* name = stream.readString(buffer, 8)` and `u8 map = getCurrentMap()` for the switch, +8 each — retires "unnamed 16-byte object" readings for stream-read + switch bodies). **A required named local can be paid for by unbinding a sibling value** (`FifoSetFog` closed: `c_hex` must be named for r27 across `__cvt_fp2unsigned`; two of the five FIFO words are written straight into `GXWGFifo.u32` — which two is undetermined). **The named-scalar rule closes clamp loops** (`f32 y = pos.y; if (y > 0.0f) pos.y = 0.0f;`, `TFishoid::perform` closed). `TJumpBase::initMapObj` closed (`getUnk8()` +8); `TMushroom1up::perform` closed (a lever pair: `getStateTimer()` at both sites +8 together, plus a binding level over the director's frame counter +16); `~TSelectDir` closed (one binding level over the gamepad member; header item `TSelectDir::getGamePad()` binding).
- **A named pointer local through an indexed inline accessor is per-expansion, so it can be unpayable**: `TBoid* boid = getBoid(i)` closes the out-of-line `TRealoid::clipBoids` but costs the byte-exact `TRealoid::perform` which inlines it — the exact sibling wins. **Classify before levering** (delta histogram over all single-slot `~` markers): `SelectDir::direct` uniform +0x80 = a carrier ladder (fader x6 +0x38, `unk20` x7 +0x40, gamepad pair +0x20 land 0xd0 exactly but leave the pool 4 bytes off — not committed); `rsetup` deltas spread 28-64 over 233 slots = several misplaced objects in a 0x600 area. A missing store pair can belong to a different block than the diff offset suggests (`TJumpBase::control`'s `unk13C = 0; unk138 = 2;` is in the ground-plane block; 326/326 instructions now, r29/r31 `this`/pool swap left — fourth instance, no lever). `MSoundStruct::startSoundSetDyna`'s r23/r24 swap is not a declaration-order property; every restructuring of its shared `li r0, 1` adds instructions. `JMASSin`/`JMASCos` shared-index CSE is not steerable from the caller (negative-confirmed). `TMushroom1up::control` is a genuine +4 pool with a dead 12-byte named slot (positional only). `TMapObjFlag::draw`'s 80 bytes are dead low region below every local (twelve GX writers + `getViewMtx()` expansions).

## One-function pass 129: three more units closed

- **A member accessor can be a -8 anti-lever at depth 2 while free at depth 1** — the level's cost is paid by the inlining caller (`TPollutionLayer::getPollutionDegree()` inside the loop `cleanedAll` inlines; raw `mCounter` closed it; PollutionManager linked). **A non-void return type with no `return` reserves r3 for the whole body** (second confirmation: `createMActorFromAllBmd` -> `void`; ObjModel linked). **Cast-outside vs cast-inside on an indexed accessor is a +-4 low-region lever at equal depth** (`(TMapObjBase*)TObjManager::getObj(i)` closed `makeObjAppear`; MapObjManager linked). **"+4 low / -4 named" after a stack buffer is often a split-declared pointer**: `T* p; ... p = f(buf);` reserves the slot below the buffer where `T* p = f(buf);` is register-allocated (NpcManager's `makePartsModelData_`, already linked via the sweep's spelling). `newAndRegisterCoin`'s carrier is one dead 12-byte non-trivial local in `TMapObjBase::setEventId(u32)` (header inline, measurable only — ItemManager was linked through the sweep's spelling meanwhile).
- **Post-increment on a JGadget iterator costs exactly one 4-byte pool word in the caller**: `++it` reproduces `SDLModel::entry`'s pool grouping exactly and reduces the SDLModel/MirrorActor/seal push_back family to a pool-base translation (retail's pool more compact with more dead bytes below) — seed for a `std-list.hpp` research batch. `MovieSubtitle::setupResource` is sized to one dead 12-byte non-trivial local in the UNUSED `makeBmgName`. Naming the product of a literal-times-member (`f32 x = K; x = x * m;`) decides the literal's FPR and both multiply destinations (`TRideCloud::control` half solved; the member loads' f0/f1 order is immovable). Binding levels were frame-neutral and register-inert on `TMapObjTree::initMapObj` (fourth counter-example).

## Sweep batch 130 (2026-09-18): the address-binding lever (7 exact, 5,853 trial builds)

- **A binding level over the *address* of a struct or array member pays exactly like batch 127's pointer/scalar lever**, and it is what the 8/16-short residue was waiting for: `static inline const TVec3<f32>* PoihanaPosition(const TPoiHana* p) { const TVec3<f32>* position = &p->mPosition; return position; }` is worth +8 or +16 of low region per expansion and lands the frame exactly (10 of 11 keeps hit retail's frame on the nose). Hit rate 10/37 of the 8/16-short functions with such a site; batch 127's by-value form was excluded for aggregates because the copy costs +6 instructions, but binding the address copies nothing. Exact: `TNerveFireWanwanHungTail::execute`, `TGorogoro::boundSE`, `TPoiHana::moveObject`, `TBombHei::calcRootMatrix` (two stacked address levels, one alone moved nothing: the 0x20-vs-0x30 TODO), `TRailMapObj::resetPosition` (+`TNormalLift::resetPosition`, same inline body), `TMarDirector::initECTMir`.
- **It is a frame lever only, never a register lever**: 0 keeps from 335 frame-exact functions with sites. The pointer/scalar fork behaves the opposite way (its one keep this batch, `TCardLoad::titleDraw`, was a frame-exact register gain) and is now exhausted on the 8/16-short population (0 of 186, cross-member pairs included).
- **The reference form is inert**: `const S& s = p->mFoo; return s;` gave 0 keeps from 189 functions / 1,243 trials, despite batch 116's "8 bytes per consumed reference return" ladder. The lever is the bound *pointer*, not the binding of a reference.
- **Rejected as a fakematch**: `TMapObjBase::throwObjToFrontFromPoint` lands its frame exactly (99.94) only by binding `mRotation` at the `.y` read of a three-component `MsMtxSetRotRPH` call; binding all three or naming a `const TVec3* rotation` local is worse. Left as a TODO at the site.
- **Headroom**: of 186 resolvable 8/16-short functions, 100 have no member-read site of any kind (accessor-only bodies or work through locals), 14 are unparseable (template/namespaced bodies), 63 pointer/scalar and 31 struct/array sites are both swept. The "private members" blocker from batch 127 does not exist (9,789 of 10,499 fields resolve public). Drivers: `grpdf/raw2.py` (`NESTED=n`, `REF=1`), `acc4.py` (field index with 724 array members); `unlev.py` cannot parse the address form, revert with `git checkout`.

## One-function pass 131 (2026-09-18): SunGlass and CameraWarp closed, signed accessor knob, FPR ranking

- **Read the direction off the slot map, not the frame.** `CPolarSubCamera::warpPosAndAt(f32, s16)` was 4 bytes *over* (sret 0x24 vs 0x20), so no accessor lever could close it; dropping `getCamMode()` for the raw `mMode` at that overload's three sites was the -4 (the sibling `(const Vec&, const Vec&)` overload keeps the accessor and stays exact). Accessor levels are a signed knob; check which way before sweeping.
- **A single-site binding level can be +16, not +8**: `TSunGlass::startFade` (0x58 -> 0x68) closed by one level over `SMSGetFlagManager()` at the dead `shineCount` read. The frame is the only way to tell +8 from +16 apart.
- **FPR ranking rule**: the callee-saved FPR each component of a scalar-replaced `TVec3` local gets follows the declaration order of the producing helper's temporaries, f31 to the last-declared. Retail's descending x/y/z ranking means `cross()` declares `_z, _y, _x` (proved on `SMS_EmitSinkInPollutionEffect` with a frame-neutral TU-local clone, 17 -> 13 markers; reversing the stores alone fixes one component). Header item for `JGVec3.hpp`, untested tree-wide.
- **A 4-byte pool item and an 8-aligned frame are not independent**: with no slack every +4 of pool rounds to +8 of frame, so a 4-byte slot difference at an identical frame proves an ordering difference, not a missing object (`TMarDirector::preEntry`, `linGetSRT`).
- **Dead-object sizing refinements**: a 4-byte non-trivial struct is +4 per expansion inside a level, +8 of frame in a single-expansion level, +0 in the caller's own body; `u8 pad[4]` or a dead `f32` beside a dead `TVec3` in an inlined callee is +0 (only class objects count; a POD array is dropped entirely). An inlined callee's reserved size can be 16 for a 12-byte object, payable as a pair with a named caller result worth +0 alone (`sqDistance` + `maxDist` in `ctrlMultiPlayerCamera_`, 61 -> 6 markers; not committed, the dead local is not evidence).
- **A lone params-pointer member level can be worth 0x40** when taken at all ten reads (`ctrlNormalOrTowerCamera_`'s `mCurrentParams`), large enough to look like a missing inlined callee; check for a `bl` and a map symbol first. Retires batch 87's `ctrlTowerCamera_` theory.
- **An explicit base-class upcast is a real inline level**: `*(J3DTevOrderInfo*)material->getTevOrder(0)` instead of the implicit slice is -4 in `SampleCtrlMaterial`'s ctor (8 over -> 4 over). Three byte-exact spellings exist, each routing one of 24 `getColorChan(i)`/`getTevStage(i)` sites through `getColorBlock()`/`getTevBlock()` directly; not committed as a mixed path at one line looks artificial (open judgement call: it would link the unit).
- **Classify before levering `bl sub` members**: `NPCNeckCallBack`, `TSpider::bind` and `linGetSRT` are all the per-statement by-value-return pool residue (a real `bl TVec3<f32>::sub`, pool differing by ordering not total). Stop spending lever budget on them. `sunmgr::load` is the fourth `this`-vs-pool-base register swap with no lever; `M3UMtxCalcBlendAux` confirms levels are inert on that ranking.
- Tooling traps: `decomp-diff.py -d` prints "100.0% match" for a function two markers from exact; trial drivers must rebuild before reading the `.o`.

## Library pass 135 (2026-09-18): five library objects linked

- **A named pointer for a global singleton accessor is a +8 low-region lever**: `JAIBasic* basic = JAIBasic::getInterface(); basic->mX = v;` closed `setParamSoundOutputMode` (JAIGlobalParameter linked) where the raw chain was 8 short and nothing else in the function had a lever.
- **Naming an intermediate of a two-step address computation is +8 with no instruction change**: `u32 aligned = ALIGN_NEXT((u32)content, align); u32 offset = aligned - (u32)content;` landed `JKRExpHeap::allocFromHead`'s 0x38 frame; an explicit mask local before the loop stops the mask folding into the `and` (~80 operands). Residue: the hoisted `~(align-1)` sits in our prologue (r6) and after the `size` alignment in retail (r4).
- **Test a shared-header carrier against a linked sibling before believing it**: `J3DNBTScale`'s two ctors each accept a +4 binding and two of them close both `newNBTScale`s exactly, but they break `J3DMaterial::createTexGenBlock` (byte-exact today) and the DOL. A byte-exact caller elsewhere pins an inline's body as hard as a map size. Batch 127's parked `J3DMaterialFactoryNBTScaleIdx` level is the wrong lever (frame right, object 4 high); the natural sibling idiom plus one 8-byte aggregate declared last is what lands both.
- **A uniform per-site accessor lever makes the landing subset ambiguous**: `JAInter::StreamLib::callBack` needs 32 of 24 x 8 available bytes from `TDSPChannel::getNumber()` sites and three different 5-site subsets give byte-identical objects (JAIGFrameStream linked with one, recorded at the site so a consistency cleanup does not break it).
- Recorded residues: `JAIBasic::initDriver` (8 short, every callee `void`, nothing to bind); `JDrama::TActor::load` (needs -4 pool and +4 named local at once; each `>>` continuation is 8 pool bytes); `IssueGXCopyDisp` (scratch permutation on a dead `rlwinm.` inside inlined `IssueGXSetCopyFilter`, whose out-of-line copy is exact and pins the body); `TFrmGXSet::perform` (frame solved by a named `GXRenderModeObj` copy, blocker is a leaf-allocator start register); `TDSPChannel::updateAll` (three fabricated +8 header levers land the frame but score lower; not committed); `TRKSuppAccessFile` (r28/r29 swap, no declaration order fixes it); `TPolarCamera::perform` (two live zeros in callee-saved FPRs = more `setTrans` expansions than ours).

## Closure batch 136 (2026-09-18)

- **An accessor retail did not use reserves a dead 12-byte slot per aggregate-returning call, not 8**: `TMapWire::getStartPoint()/getEndPoint()` were `TWireBinder::init`'s whole 24-byte over-frame and `TLiveActor::getPosition()` `bind`'s 16; raw member reads closed `init` and took `bind` to 99.96 (residue: one `a = b - c` temporary at 0x3c vs 0x24, the by-value-return family; ten spellings recorded at the site).
- **The ladder's modulus**: when the referenced local sits at `0xc + low` and the required low region is not congruent to the baseline modulo 8, no accessor or binding level lands it (nine forms measured on `ctrlNormalDeadDemo_`, all multiples of 8). The odd rung is a binding *around a real `bl`*: `static inline f32 W(Vec* v) { f32 m = MsVECMag2(v); return m; }` is +12. `ctrlNormalDeadDemo_` closed with `mInbetween` fetch (+8) + `&mPosition` binding (+8) + that wrapper (+12) = 28. A dead unwritten 4-byte local reaches the frame with the padding on the wrong side; named `f32`/pointer locals reserve nothing.
- **Refuted: MWCC does not size the outgoing-parameter area by argument count.** On `MsGetRotFromZaxis`, calls with 3 or 4 `f32`s, 1 pointer + 3 `f32`s, or 8 pointers leave the frame at 0x48; only a ninth pointer adds 8. Batch 77's "8 bytes per f32 argument / retail reserves 32" is retired; that gap is a dead pool object of the right size in the wrong position (a dead 12-byte non-trivial local lands 0x58 from five placements, MsSqrtf temp still 0x10 low). MathUtil carries three pre-existing `char trash[]` paddings and is not honestly linkable.
- **Two inlined bindings inside an UNUSED helper pay +8 each in every caller**: `TFileLoadBlock::pushed()` with `startStateTimer(120)` (the real `MapObjBase.hpp` accessor) in place of raw `mStateTimer` stores and an in-class `getEffectPos()` binding `&unk144` at the second `emit` only closed both `touchPlayer` and `receiveMessage` (MapObjOption linked; the earlier "dead 13-16-byte local" reading is superseded). `updateDemoCamera_` stays 99.7: two `origin + offset` temporaries 36 bytes high behind three 12-byte pool objects retail allocates first.

## Sonnet library sweep 138 (2026-09-18)

- **The library 8/16-short population has no lever sites**: of 32 frame-short JSystem functions (24 unlinked units), 29 show zero opcode differences and no accessor chain, singleton call or address computation to bind; the three lever attempts were inert. The two gains were statement order, not levers: `JAIBasic::checkEntriedSeq` sets `mLoadingFlag = true` *before* `Vload::loadFileAsync` (98.79 -> 99.93), and `TTrack::writeRegParam` case 2 names the u16-truncated value before the second `writeRegDirect` (99.36 -> 99.98). Both keep a pure-frame residue.
- **Tooling trap**: `objdiff-cli diff` defaults to `functionRelocDiffs=data_value` (what `decomp-diff.py` and the earlier sweep drivers use) while `report generate` defaults to `none`; they disagree by tenths of a percent on sda21-relocated functions and produce false regressions in a driver that compares against `report.json`. Pin `functionRelocDiffs=none` for live comparisons against the report (`grpdo/lev.py`).

## Header round 22 (2026-09-18)

- **A carrier candidate is disproved by its exact callers, not by frame arithmetic**: a dead 12-byte local in `TMapObjBase::setEventId` costs six 100% functions (`TCoinBlue::loadBeforeInit`, three `*HideObj::load`, `TShellCup::loadAfter`, `evSetEventID`) for one 0.12 gain; rejected. `TShine::loadBeforeInit`'s layout (retail `name` 0x24 / `eventId` 0x20 / `v` 0x18: 12 bytes low region plus a dead 4-byte slot at 0x1c) is recorded at the site.
- **The binding ladder is strictly additive, so the lever set is over-determined**: if no subset of the measured steps hits the target frame, the lever in hand is the wrong one. `TSelectDir::direct()`: 0x50 baseline, gamepad binding at both sites 0x70, fader+menu on top 0xe8; only fader+menu+stage lands retail's 0xd0, so retail reads the pad raw there. `getGamePad()` accepted as a de-fabrication only (dtor exact).
- **When the residue is the total size of the inline-temporary pool, every equal-cost expansion is an interchangeable knob and none is evidence**: `SampleCtrlMaterial`'s ctor is 4 over and each of the 24 `getColorChan(i)`/`getTevStage(i)` forwarder expansions contributes 4, so any single one routed through its block matches. Not linked; `unk38` in an initialiser list 98.0.
- `TAfterEffect`'s `TVec3` member at 0x5C accepted (`TModelGate::screenBlur` 83.74 -> 87.07); the fifth stack vector (`blurDir.set(viewDir)`) rejected at 84.94; remaining 0x30 = 12 vector + 8 second `stfd` slot + 28 unexplained. `isInBounds` carrier and the level above `TVec3(f,f,f)` stay rejected (round 21 measurements).

## Library closure pass II, batch 140 (2026-09-18)

- **Naming an intermediate is a +8 frame lever with zero instruction change** (third confirmation, library side): `f32 max = 1.0f; f32 min = 0.0f;` clamp bounds closed `HardStream::volFloatToU8`; `f32 cos = v.y; f32 sin = v.z;` closed `JPADraw::loadYBBMtx`; binding a `new` result into a local before `append` closed `JALSystem::appendGrpMember` (retail double-checks `found` only in the last four arms). Try it before calling a frame residue unnameable.
- **Declaration order is inert for callee-saved allocation**, not only for inlining: three hoists in `createEmitterBase` and a shared `int i` in the JPAEmitterManager ctor all +0 or worse.
- **An uninitialised class local of an inlined callee is +sizeof of pure low region only if the class has no destructor**; a ctor+dtor class adds 8 more above the caller's named block (measured at 8/16/20/24 bytes on `fileOpen`; `startFirst`/`startSecond` need a 24-byte ctor-only class the map lacks).
- Recorded residues: `JPADraw::initialize` (16 bytes of temp pool, unlocalisable), JPAEmitterManager (two register rotations at exact frames), `expf` (pure FPR permutation: retail keeps long-lived values in f6-f9), `powf` (frame 0x18 too large + scheduling), `J3DDeformer::deform` (two 4-byte gaps + `rlwinm` pair order; the first `Vec deform` is confirmed), `initMtxIndexArray` (one `add` operand order), JAIGFrameSe (callee-saved rotation over twelve up-front locals; `sendPlayingSeCommand` needs -8 pool and +6 named). `JALSystem::append` needs 0x68 = 13 x 8 of temp pool against twelve in-class `JALSeModXxx` ctors in a shared header (parked). objdiff prints `__log2_F@ha` and `...rodata.0@ha` for the same address.

## Closure batch 139 (2026-09-18): MapObjGrass and PollutionAction linked

- **Dead-local carriers in UNUSED callees are legal zero-code levers and their sizes are fixed**: an uninitialised `JGeometry::SMatrix34C<f32>` in `TMapObjGrassManager::draw()` is +48 (`perform` closed; `initDrawFar()` is indistinguishable as the host); the first uninitialised `TVec3` is +16 and each further one +8 (`TPollutionActionManager::action` +40 = one in `electric()`, two in `spread()`). The split across callees decides where the live slots land.
- **Bindings pair super-additively**: `mPollutionMap` and `SMS_GetMarioPos().y` are +8 each alone and +24 as a pair (`getPollutedPosNear` closed); `mWidth` +16 and `j3dSys.getViewMtx()` +8 closed `initDrawNear`. For `BossHanachanEffect::emitParticle_` any single binding is +8 but n in combination cost 24 + 8n, so exactly five reach 0xc8 (not committed: frame-exact only drops 103 -> 90 markers, the `this`-vs-pool-base rotation remains).
- **No 4-byte lever exists** (refuted again): global forks returning directly, a `one()` constant helper and a director-state accessor are all +0 or +8. MapWarp's `init` residue (+4 from the `Vec&` base binding retail needs for eager address materialisation) is therefore open.
- **Stack arrays' offsets read off their declaration order** (named locals descend from the top, first declared highest; `TMapWarp::init`, 93.8 -> 97.2 with the frame exact). `readU32()` gives each expansion its own low temp; one named `u32` + `operator>>` shares one slot placed above the named block. Each chained `>>` continuation is 8 low bytes and one loop-hoisted `addi rN, rStream, 0`; groupings "3,3"/"4,2"/"5,1" are codegen-identical, only the count is evidence (`loadWarpPointPos`: six).
- **A flat pointer walk with `++slot` in the `for`'s third clause is the only spelling giving an unroll-by-8 `bdnz` with ctr = 3** (`TNpcParts` ctor 84.8 -> 94.2, 24 slots).
- Parked shared-header need: `MActor::initSimpleMotionBlend` wants a `frame = -1` defaulted wrapper resolving to `TBaseNPC::mPtrSaveNormal->mMotionBlendFrame` (retail keeps a dead `li r4, -1; cmpwi r4, -1`); a TU-local `static inline` does not reproduce it, since MWCC folds the default through one level, so a second inlined level is involved. riccohook `init` +8 sits *between* two sub-expansions of an inlined JGadget insert (every binding lever puts it at the bottom instead); `emitCamShake_`'s residue is MsSqrtf's `volatile float y` at 0x44 vs 0x40.
## Research batch 142 (2026-09-18): the `a = b - c` slot is an allocation-*order* difference, and retail's geometry needs dead bindings

Model: `TCoasterEnemy::bind` rebuilt in a scratch TU (private `JGVec3.hpp`, game flags, a `Model` with `mPosition` 0x10 / `mLinearVelocity` 0x94 / `mVelocity` 0xac), which reproduces our object byte for byte.
Geometry is written **(below, above)** = pool bytes under and over the live `bl sub` temp; retail is **(4, 12)** and the stock header **(16, 0)**, both frame 0x40 with the named local at 0x28 and 55 instructions.

- **Retail's instruction stream holds one 12-byte temp and twelve copy instructions — exactly ours — so batch 119's "two 12-byte slots per statement, from a by-value return" is wrong as a *source* hypothesis.**
  Both builds reserve the same 28 pool bytes (12 temp + 16 dead); retail puts 4 of the dead bytes below the temp and 12 above, we put all 16 below.
  The residue is an allocation **order** difference inside one statement, not a missing slot.
  Every by-value return still costs +6 (61 against 55), reconfirmed on three spellings.
- **The `below` ladder is finer than batch 116's "8 per reference return": the callee's return type prices it — reference 8, pointer 4, void 0 — and the prices are additive over the levels inside `operator-`.**
  So `TVec3* operator-=` gives below 12 and `void` gives 8; the by-value-parameter family (`operator-(TVec3 fst, const TVec3&)`) has a floor of 8 because `operator-`'s own reference return is always 8.
- **below 4 needs a named result *and* both inner steps void-returning**, and it is reachable with `operator-=` and `operator=` left alone:
  `friend const TVec3& operator-(const TVec3& fst, const TVec3& snd) { TVec3 r; *(Vec*)&r = *(const Vec*)&fst; r.subV(snd); return r; }` plus `void subV(const TVec3& o) { sub(o); }` gives (4, 4) at 55 instructions.
  A raw `Vec` assignment is the only free copy: `r = fst` costs 8, `r.set(fst)` and `r.set<f32>(fst)` cost 4 (the derived-to-base conversion), `TVec3 r(fst)` costs 4.
- **The `above` region is fed only by a binding declared *after* the subtraction and never used, at 4 bytes each.**
  Two of them land retail exactly — (4, 12), frame 0x40, temp 0x10, named 0x28, 55 instructions — and `const TVec3*`, `const TVec3&`, `Vec*`, `const f32*` and a pointer to `snd` are interchangeable.
  Any binding that is *consumed* (returned through, or used as the `sub` receiver) costs 4 **below** as well and misses.
  A dead trivial POD (`u8[4..16]`, `f64`, `Vec`) is worth 0 here; a dead `TVec3` after the subtraction is (+4, +12), before it (+12, +0).
- **The prize is measured and real, and it is still not committable.**
  The exact-geometry shape is **+9 functions to 100%** (`TEnemyAttachment::bind`, `TChuuHana::bind` and `attackToMario`, `TAmiNoko::bind`, `TKoopaJrSubmarine::bind`, `TCoasterEnemy::bind`, `TLimitKoopa::bind`, `TLeanMirror::loadAfter`, `TLiveActor::bind`), 24 improved against 17 regressed, matched_code 53.57 -> 53.67, total fuzzy flat at 97.40.
  Reverted: the two bindings are dead padding, and routing `operator-` past `operator-=` drops `__ami__` (weak 0x34, Tongue.cpp) from 100% to MISSING, with `TMario::isTakeSituation` and `toroccoEffect` 100 -> 99.8 and `TSphereLink::moveHead` 85.7 -> 82.2.
  Read it as the size of the bounty, not as a lever.
- **Also refuted here, all slot-identical to the stock (16, 0) at 55 instructions.**
  The **declaration order** of `sub`, `operator-=`, `operator=`, the copy constructor and `operator-` inside the class body (eight permutations, completely inert).
  The whole caller side: the site as `setLinearVelocity(a - b)`, `m = a - b`, `m.set(a - b)` or `TVec3 d = a - b`, a setter body of `m.set(v)`, a setter taking `TVec3` by value, a setter with two levels, and a setter calling `operator=` explicitly — none of it moves the temp.
  `operator=` returning `const TVec3&`, `TVec3*` or `void`, or with a memberwise or `set`-forwarding body; `sub` declared out of class with `inline`; `operator-` returning `TVec3&`; a member `operator-`; `return *fst.subP(snd)`.
- Census notes: retail's `below` is 4 only for the nine single-statement minimal-frame functions, and over all 130 sites it takes 57 distinct values from 4 to 760, because retail allocates per statement in source order — so "temp at the floor + 4" is not a universal target, the *intra-statement* order is.
  `__mi__` appears nowhere in the map (0 hits), so `operator-` is inline at all 130 sites and its shape cannot be read off a symbol size.

## Research batch 144 (2026-09-18): the callee-saved allocation order, and `sunmgr::load` closed

The `this`-vs-pool-base swap was a known-open register class (pass 131).
It is now explained, and one of its four instances is closed at zero frame cost.

**The ranking rule, from a scratch TU compiled with the game flags** (drivers `grpds/rdrv.py`, `t6`-`t14`; `run(int n, C* q)` with three string literals to force a `.rodata` pool base, one `bl` per use so every value is live across a call).
Callee-saved GPRs are handed out **r31 downward in reverse order of value introduction**: the compiler's pool/base-address temp first, then locals in reverse declaration order, then parameters in reverse parameter order, with `this` last.

| variant | ranking, r31 first |
| --- | --- |
| pool + `q` + `n` + `this`, seven permutations of first-use order | pool > q > n > this |
| same, use counts 5/1/1/3 and 1/1/1/9 | pool > q > n > this (unchanged) |
| pool materialised only at the end of the body | pool > this (the `lis` sinks, the rank does not) |
| two literals only | no pool base: each literal gets its own `lis`/`addi` |
| a loop added around the uses | pool > i > n > this |
| two pointer locals from calls, either declaration order | b > a > this (later-declared higher) |
| reference parameter driven by an inlined `operator>>` chain | pool > stream > this |
| three disjoint ranges packed into one register | chain > pool > this |

**First-use order, use counts, loop depth and range length are all inert** (that is the one thing every variant agrees on), so a swap is never fixed by moving a use, shortening a range or hoisting a materialisation — which is why every level and binding tried on these sites was inert.

**What *does* move the pool temp's position in that list is how many named scalar locals the frame holds.**
A stub-level mimic of `TSunMgr::load` (same 93 instructions and same frame in every variant) ranks pool > this > stream with five or six named scalars and **this > pool > stream with seven**, and putting four of them into a `u32 v[4]` array puts it back to pool-first.
Inert in the mimic: naming or not naming the search result, declaring the temporaries before the input locals, an extra dead named local at either end, computing the pair through an inlined helper, and storing the pair through an inlined two-argument setter.

- **`TSunMgr::load` closed (99.12 -> 100, unit 77.69 -> 100 matched_code)**: the four colour words are read into `u32 color[4]`, not four named scalars, and the two colour temporaries stay named — they are what hoists all four loads above the first store (dropping them fixes the registers but sinks the stores and moves the named block 4 bytes, 14 markers). The array is frame-neutral, so the low-region decomposition recorded at the site still holds. The batch-32 "array plus a four-read loop" rejection stands; it is the array *without* the loop that lands.
- **`M3UMtxCalcBlendAux` stays open and is a different animal**: retail's permutation is a one-step rotation of r25-r30 that puts the `&j3dSys + 0x38` address temp on top and ours puts it at the bottom, with relative order otherwise preserved. Eleven spellings measured, all inert or worse than the stock 55 markers: dropping `pQuat`, dropping the `Vec& currentS` reference, the pointer walk as an indexed loop, a named `J3DModel*` at the first `getModel()`, named blended-scale components, a named `u16` joint index, and `bVar5` at all seven declaration positions (55 at its current position 1, then 64, 64, 65, 67, 69, and 92 at position 0). Declaration order is therefore a real but coarse knob on this ranking, contrary to batch 140's "inert" reading — it moves the rotation without ever landing it.
- **`sunmgr` is 100% matched_code and still not linkable**: adding `Camera/sunmgr.cpp` to `config/GMSE01/objects.json` gives a DOL of `934a10aa`, so the unit's remaining difference is section content, not instructions (`validate-symbol-order` passes; the `.ctors` gap symbols are the suspects). `configure.py`'s `Matching` flag is inert for GMSE01 — the linked set is that json — and a bare `python3 configure.py` regenerates `build.ninja` for GMSJ01 and breaks the build; always pass `--version GMSE01`.
- Not retried here: `TBossHanachanEffect::emitParticle_` (frame 24 + 8n short, so not the frame-exact case) and `TDSPChannel::updateAll`; neither has a group of named scalars to group into an aggregate.

## Closure batch 143 (2026-09-18)

- **A binding written into the unit's own accessor is +4 per expansion, not +8**, and materialises the callee-saved copy retail keeps: `Koga::ToolData* d = unk14; return d;` in `TMovieRumble::getToolData()` closed `perform` and `checkRumbleOff` (with a TU-local level over `TTHPRender::getFrameNumber()` for the +8 both frames needed). Rejected: the local inside the `if` (-1.7), the guard re-reading `getToolData()` (+12), `const`/reference variants, `&buf[0]`, dead `u8[4]`. Corollary confirmed twice: a TU-local level over a *shared* class's getter is +8 per expansion and can be the whole residue (`MovieFrameNumber`, `JAIActorTrans` in MAnmSound: 0x88 -> 0x90).
- MAnmSound real bug: the gate divisor is `(entry.unk10 >> 24) + 1`, not `mLoopCount >> 24`; naming the masked `flags` local restores the entry reload (NPC anim sound 81.0 -> 84.3). Residue: `std::sqrtf` inlined where retail calls it (weak-plus-`bl` class).
- Recorded: `MSHandle::calcPan`'s 8 bytes are tied to the third callee-saved FPR, not a statement (nine spellings inert); `Menu`'s two `perform`s are the JUTColor `x = c.get()` temp stride (4 retail / 8 ours), its ctor reloads `this` from the 8(r1) spill; `MarDirectorSetup2::setup2` writes `graphics.unk0` (268 bytes of inline-temp pool missing, `MSStage::init` args right to left); MSModBgm `modBgm` shares one `li r31, 0` between the null test and the `stb`; MarioCap ctor reloads `unk30` through `this + idx*4`.
## Targeted batch 145 (2026-09-18): the ranking rule's declaration-order knob is class-bounded

Batch 144's rule holds, but its "declaration order is a real but coarse knob" reading (from `M3UMtxCalcBlendAux`) is narrower than it looked.
**Declaration order moves the callee-saved ranking only among function-scope named locals that live in callee-saved registers.
It is inert on the rank of `this`, of parameters, of the compiler's pool/base and constant temps, and of inner-block locals; block scope is inert everywhere.**
That is the whole explanation of why `bVar5` moves `M3UMtxCalcBlendAux`'s rotation (a function-scope local competing with other function-scope locals) and why none of the four sites below move at all: in each of them the two values that must swap belong to *different* classes.

- **`TRKSuppAccessFile` is the one site where order is a knob, and it is now exhausted.** All 120 orders of its five register-held function-scope locals (`done`, `exit`, `error`, `replyBuffer`, `length`; the other five declarations are address-taken and stack-homed) were built and measured. The present order is the unique minimum at 12 differing operands; every other order gives 19, 20, 23, 27 or 28. Retail's ranking is `length > replyBuffer > done > exit > error`, ours `length > done > replyBuffer > exit > error`, and no permutation reaches retail's. Do not re-sweep this function's declarations.
- **`TBossHanachanEffect::emitParticle_`, `JAIBasic::checkNextFrameSe` and `TNpcParts::partsPerform` are fully inert to both knobs.** emitParticle_: a shared `int i` hoisted to function scope, `int i` plus `int foot` hoisted, a block-scoped `int i` for the first two loops, and `position` declared above `waterHeight` — all exactly 103 markers. checkNextFrameSe: 25 orders of its twelve up-front locals (reverse, all eleven rotations, twelve random) and eight relocations of subsets of them into the `for (i...)` body — all exactly 129. partsPerform: `u16 j` hoisted to four positions among `starglowMatIdx`/`matNum` and `Mtx44 mtx` declared ahead of `mactor` — all exactly 31.
- **`checkNextFrameSe` is the pool-vs-local boundary with a four-wide pool block, not a permutation.** Retail puts all four of its base/constant temps on top (r31 `&candidates`, r30 the 0x4330 int-to-float magic, r29 `&JAIConst::dummyZeroVec`, r28 `0x7fffffff`) and `this` at r24 *below* both parameters — batch 144's rule verbatim. Ours puts the same four at the bottom (r23-r20) with `this` at r28. The pool block and the local block trade places wholesale, so the only known mover is still the named-scalar count, and it cannot be exercised: all twelve locals are load-bearing (`k` is read after its loop, `j`/`l` are assigned their bounds as early exits).
- **`partsPerform` shows a second boundary: inner-block locals.** Retail ranks them *below* `this` (r30 `data`, r28 `i`, r27 `it`, then r26 `param_2`, r25 `param_1`, r24 `this`, then r23 `starglowMatIdx`, r22 `j`, r21 `matNum`), which is the rule's order with inner-block locals appended after `this`. Ours lifts `matNum` and `j` above the parameters (r26/r25) and drops `starglowMatIdx` to r21. The +40 frame gap (0xf8 vs 0xd0) is separate: 24 bytes below `Mtx44 mtx` and 16 above it.
- **`TDSPChannel::updateAll`'s register instance is closed and the recorded note is stale.** `delta` is r29 on both sides now; the function is 17 markers, every one an r1 displacement, no opcode, insert or delete. All that is left is the 24-byte low-region gap, whose three measured levers are header-side (batch 74/136), so it stays parked.

## Header round 23 (2026-09-18): JASDSPChannel linked

- **Accessor bindings are +8 only on u16 member reads**; u8/bool accessors (`getStatus`, `getPriority`, `DSPBuffer::isFinish`) and a u16 accessor over another class's member are +0. `TDSPChannel::updateAll`: two u16 bindings in `JASDSPChannel.hpp` (0x38 -> 0x48) plus `decCBInterval()` spelled `setCBInterval(getCBInterval() - 1)`, a getter/setter composition codegen-identical to `mX--` that buys the third expansion of the getter when the class saturates at two. 100%, linked (478). A frame lever that introduces a callee-saved register is the wrong lever (`getHandle(u32)` with a binding landed 0x50 but pushed `delta` r29 -> r28).
- **Promoting a TU-local level into a new reference-returning accessor keeps its +8; promoting it into a binding inside an existing accessor drops to +4 and hits every other expander** (`JAIActor::getTranslation()` promoted, neutral; `TTHPRender::getFrameNumber()` rejected: `checkRumbleOff` 100 -> 99.97 and `TMovieSubTitle::perform` breaks).
- `JALSystem::append` (0x68 pool gap): `TFlagManager::get()` binding +8/expansion breaks three functions; a named pointer per arm +4; 0x68 is not divisible by twelve so part is not per-arm; a dead 8-byte local in `JALSeModData<T>`'s ctor is +0. `MActor::initSimpleMotionBlend` defaulted-wrapper hypothesis refuted (the `20` site has `li r4, 0x14` with no compare, so the -1 comes from an inlined accessor returning the constant, not a default argument).

## Library re-pass 150 (2026-09-18): three links from the new rules

- **A plain u16 accessor (`return mX;`) is +4; the +8 needs the binding form (`u16 x = mX; return x;`)**, both measured on `getMaterialID`. Two binding expansions of the same u16 read cost +12, not +16 (CSE merges part); mixing a binding accessor and a raw read on one member keeps the offsets but breaks CSE (one extra reload). The +8 binding survives being parked as a TU-local `static inline` wrapping the class's own accessor by pointer; in the header it drops two linked `J3DModelLoader_v26` functions, so it stays parked. `J3DMaterialFactory::newNBTScale` and `_v21` closed this way (plus map-exact `getMaterialInitData` bodies); both linked.
- **The inline-temp price rule closes pool-high residues**: `TNameRefGen::search<T>`'s pointer return was one 4-byte level too many in `JDrama::TActor::load`; `getInstance()->getRootNameRef()->search(str)` directly gives -4 and 100% (JDRActor linked).
- Still open with notes: `JAIBasic::initDriver` (needs a dead 8-byte class local in `initAudioThread`), `IssueGXCopyDisp` (scratch permutation), `TFrmGXSet::perform` (allocator start), `allocFromHead` (scheduling; a hoisted `alignMask` does not move the `nor`), `TPolarCamera::perform` (66%: retail holds 0.0f in f31 and f30, 1.0f in f29, `-unk44` in f28; needs a `TPosition3f` setup rewrite with named scalars, its own batch).

## Closure batch 151 (2026-09-18)

- **Try removing an existing lever before adding one**: batch 127's TU-local binding over `mRevivalTexStampNum` overshot by a step once the pool changed; a plain `getRevivalTexStampNum()` in the unit's own header closed `drawRevivalTexStamp`. On a u16 read a binding inside the class's own accessor and the TU-local one give the same +8.
- **A reference-returning accessor costs frame per expansion, and mixing by-reference and by-value reads at one site is a lever**: `TParamT<T>::get()` fabricated as `const T&`; `TCameraShake::startShake` six reads = 0x60, three (the f32 amplitudes) = 0x50 = retail, zero = 0x48, so the three s16 velocities are by-value reads. Header item: for an integer parameter `get()` returned `T` by value. `startShake`/`keepShake` frame-exact, one FPR coalesce left. Wrapping a by-value read in a TU-local getter is not neutral (99.9 -> 99.7): read `.value` directly.
- **"Pointer return = +4" is not universal**: `ECTSearch` stacked twice is +8 per level in `setupPerformList_console` (4 low; overshoots).
- Recorded: `MSSetSoundTL` ctor instruction-exact with 104 = 13 x 8 bytes of dead low region (one 8-byte dead local per `JADPrm` ctor expansion, carrier in shared JSystem audio headers); enemymanager `copyAnmMtx` r27/r28 swap unreachable by declaration order (and an UNUSED `TPosition3f::TPosition3()` in the map is a lead), `createEnemies` is the JGadget grouping class, `performShared` the JUTColor stride; emario `perform` is the batch-142 order class (a function-scope `s32 i` is byte-identical: block scope inert), `init` needs a 40-byte shared carrier; boid `calcGoalForce` 4 over (`getPoint()`'s reference return is worth 8).

## Closure batch 149 (2026-09-18)

- **An accessor's inline temp is +4 of low region per expansion; bisect sites first**: several individually +0 sites sum (six `getCollision(i)` sites gave +16, plus `getCollision(i)->getPosition().x/.z` +16) and only some are codegen-neutral; closed `execNpcObjCollision_`. `setVariableDamageRadius_` closed by dropping a one-use local and inlining `SMSGetNpcInitData(...)` into the product, which alone flips retail's `fmuls` operand order.
- **A leaf callee that has a matching out-of-line copy can still carry dead locals** (`getTipPoints`: its own copy allocates no frame for them), so "carrier must have no out-of-line copy" is too strong. **Dead locals price as floor(total/8) x 8**, not per-local rounding.
- **Inline temps allocate statement by statement in reverse statement order and by level within a statement**; a named intermediate adds a third copy per step, a pure chain elides one. `TWarpInCallBack::execute` 38 -> 74 with five named scalars (f31-f27 in declaration order), `factor += 1.0f` as its own statement (stops the `fmadds`) and a `set(x, y, z)` tail. Residue: one extra 12-byte copy per scale step; header item: a member `TVec3 operator*(f32) const` instead of the by-value friend has exactly retail's six temp slots there.
- walker `calcFarthestVertex` reaches 100 only via a dead `TVec3` plus two asymmetric `getPoint1()` expansions (rejected as an accounting coincidence). MarDirectorLoadResource (+64/+24) and NpcInitPrg (+112, `TBaseNPC::init` wants a dead non-trivial local in an inlined NpcBase ctor) are pure dead-low-region gaps with every callee a real `bl`. `TTakeActor(const char*)` is an in-class inline in TakeActor.hpp (cannot also be defined in the .cpp); MarioParticle's 8 UNUSED stubs need declarations in shared Mario.hpp.
