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

## Game re-pass 152 (2026-09-18): MapArea linked

- **Dead-carrier price is 12 bytes per `TVec3` per expansion**, not "first 16, each further 8": MapArea's 288 dead pool bytes are three reserved `TVec3` vertex locals in each of the UNUSED `pointIsInPolygon` and `checkLinePolygonCollision` (12 x 3 x 4 expansions x 2); the apparent 16 at single-expansion sites is frame alignment. Both UNUSED sizes stay exact; MapArea linked. A carrier is disproved when its own out-of-line copy breaks while the caller's frame does not move (`checkLinesCollision`): a one-build test.
- **Class-object locals of an inlined callee are allocated after the scalar temps** regardless of where the callee expands (CameraMultiPlayer: `sqDistance`'s dead vector, now legal, lands above the later MsSqrtf slot; 61 -> 5 markers, frame exact).
- **The +4 binding form is u16-specific**: a binding in a new accessor over a pointer member is +8 (MovieSubtitle's `getScreen()`), so no 4-byte lever exists yet (MovieSubtitle needs exactly 4; `makeBmgName` compiles to 84 vs map 0x4c). MarDirectorPreEntry needs a paired +4 below / -4 above the TViewport statement. lensglow requires +96 below `avg` and -8 above, carrier in shared `SunModel.hpp`. BathtubBinder's `dir` is a `TVec3` (44-byte named region) whose z/x retail ranks in f31/f30 above the products. THPAudioDecode/MapObjTree/M3UJoint: ranking knob exhausted.

## Header round 25 (2026-09-18)

- **A ctor that is only an initialiser list holds no dead local** (generalising round 24's JADPrm test from map size to source shape), and a byte-exact caller elsewhere excludes a shared ctor as a carrier outright: `TBaseNPC::init`'s 112 bytes would need seven +0x10 carriers and every candidate (`TNpcInbetween`, `TNpcSink`, `TNpcUnk22CStruct`, `TNpcUnk230Struct`, `TMultiMtxEffect`) is excluded.
- **MWCC 1.2.5 does no NRVO**: a by-value member `operator*(f32)` is inert (the return temp survives, nine slots stay); only a reference return collapses a per-step temp, and that shape (`friend const TVec3&` / member `const TVec3&`, the header's `operator+`/`-` idiom) gives retail's six slots in `TWarpInCallBack::execute` (73.9 -> 92.5) and ~12 other gains, but drops four weak out-of-line copies to MISSING (`div`, `dot`, `TUtil<f>::sqrt` in boid; `__ami__` in Tongue) and loses ~13 functions: tree-wide -0.02, rejected with the trial table beside the operator.
- **An UNUSED size equal to a sibling's body plus ~4 instructions proves a per-call-site inline split** (`emitSweatSometimes(s16)` = `emitSweat` + a 4-instruction guard, 0x30 vs 0xe4), which is what a `#pragma dont_inline` in the same TU stands in for; a one-argument forwarder's UNUSED size says which parameter passes through (pass-through in r4 = one `li`, a swap = `mr` + `li`; `emitFootPrint` 0x24 vs 0x28). MarioParticle's eight UNUSED bodies defined (five size-exact), symbol order 8 MISSING -> PASS.
- lensflare's three-deep `set(const Vec&)` chain is already present (two of three `bl set` and both `JMAS{Cos,Sin}` reproduce); the third is lost to the nine-scalar lerp whose products fp_contract fuses, so retail built the two difference vectors as objects (0x90 frame gap, four extra callee-saved FPRs).

## Research batch 159 (2026-09-18): `operator*`'s return type is a **consumption** split, not an overload split

Round 25 left the `TVec3<f32> operator*(f32)` return type open because one header shape gained ~12 functions and lost ~13.
Both populations are now characterised from the source and from the retail stream, and the discriminator is not overload resolution.

| site | source shape | retail objects | reference return |
| --- | --- | --- | --- |
| `TEffectColumWater::generate` | `mScaling = param_2 * 1.3f` | 1 (0x18 copied straight into `mScaling`) | 91.95 -> 99.79 |
| `THamuKuri::forceRoll` | `local_20 = local_20 * k` | 1 | 90.23 -> 99.66 |
| `TRope::moveHead` | `mPoints[i].unk18 = v * scale` | 1 | 93.60 -> 99.80 |
| `TIgaiga::setMeltAnm` | `scale = mScaling * 0.5f` | 1 | 92.14 -> 96.06 |
| `SMS_MakeJointsToArc` | `TVec3 a = dir * t` (copy-init) | 1 | 84.56 -> 88.83 |
| `TWarpInCallBack::execute` | `TVec3 v1 = *vel * k`, three steps | 2/step: scale target + copy-out | 73.92 -> 92.47 |
| `TYoshiTongue::emit` | `mInitialVelocity = dir * mInitialSpeed` | 1 | 93.64 -> 93.95 |
| `TTamaNoko::landEffect` | `setGlobalScale(mScaling * 0.8f)`, four sites | 2 (0x40 copied to 0x74, 0x74 read) | 94.64 -> 82.97 |
| `TNameIndParCallback::execute` | `setGlobalScale(local_7c * 0.5f)` | 2 | 82.05 -> 79.55 |
| `TConeBeam::calcVertices` | `local_11c += local_140 * c` | 2 | 95.67 -> 88.48 |
| `TMapObjPuncher::touchPlayer` | `dest += dir * 100.0f` | 2 | 99.72 -> 92.86 |
| `TMario::keepDistance` | `mPosition += diff * step` | 2 | 93.96 -> 91.20 |
| `TBoidLeader::calcForces` / `calcBoids` | `force += v * k`, `+= d / d2 * radius` | 2 | 99.55 -> 92.95 / 95.53 -> 85.50 |
| `TWalker::bind`, `TMario::gunExec` | `pos + dir * k` | 2 | 92.34 -> 86.01, 94.54 -> 92.30 |

Gains are exactly the sites that **copy the product into a destination object** (`x = a * k`, `TVec3 v = a * k`); losses are exactly the sites that **consume it by reference** — a function argument, or the right operand of `+=` or `+`.
`div`, `dot` and `TUtil<f>::sqrt` (boid), `__ct__` (MarioCollision) and `__ami__` (Tongue) dropping to MISSING, plus `damageExec` -2.6 and the four wire* functions, are secondary: those functions hold no product of their own.

- **At a destination-copy site retail materialises one object, at a consuming site two, so no single overload reproduces both.**
  `generate` copies `operator*`'s object straight into `mScaling` with nothing in between; `landEffect` copies 0x40 into 0x74 and reads 0x74 for the inlined `setGlobalScale`.
  A reference return gives one object everywhere and a by-value return two, and a member/friend by-value form is inert (round 25), so the return type is decided per *consumption*, not per operand type.
- **Two candidate splits are refuted by the map without a build.**
  `__as__Q29JGeometry8TVec3<f>FRCQ29JGeometry8TVec3<f>` and `__ct__Q29JGeometry8TVec3<f>FRCQ29JGeometry8TVec3<f>` both exist, so `operator=` and the copy constructor take `const TVec3&`: a by-value `operator=` (whose parameter could double as the return temp) and an implicit copy constructor are both impossible.
  `__ml__` has 0 hits in the map, so no `operator*` signature can be read off a symbol size.
- **Direct-initialisation is inert**: `TVec3 v1(*vel * k)` is byte-identical to `TVec3 v1 = *vel * k` (73.92, identical markers), so MWCC does not elide the return temp into the destination — the caller-side counterpart of round 25's no-NRVO result.
- **The parameter form is an orthogonal knob**: `friend const TVec3& operator*(const TVec3&, f32)` with an internal local reproduces the by-value-parameter reference return to within 0.1 on every member of both populations.
  It also warns `function result is a pointer/reference to an automatic variable` in all 377 TUs, which the `operator+`/`operator-` form does not — a small argument that retail's reference-returning operators return their *parameter*.
- **The only legal C++98 selector found is operand order**: by-value `operator*(TVec3, f32)` for `v * k` beside `const TVec3& operator*(f32, TVec3)` for `k * v` (equivalently a member/friend pair).
  It reproduces both populations mechanically, but which order a site spells is unfalsifiable from the asm, so choosing it per site is picking codegen rather than following evidence. Not applied.
- **Two of the regressions are our call sites, not the return type.**
  `TTelesa::behaveToWater`'s retail stream copies `operator*`'s object into `local_20` and only then into `mVelocity`, i.e. retail wrote `local_20 = local_20 * fVar1; mVelocity = local_20;` — exactly its sibling `THamuKuri::forceRoll`; with the reference return that spelling is 99.9% with **zero** structural markers.
  `TMapObjPuncher::touchPlayer` recovers to 99.65 with `TVec3 ofs = dir * 100.0f; dest += ofs;`, but the named local lands in the high region where retail's second object is a pool temp (+0x10 frame): the consuming population wants a *temporary*, and only a by-value return makes one.
- **Near-miss worth re-trying: the parameter form alone.**
  `friend TVec3 operator*(const TVec3& fst, f32 snd) { TVec3 r(fst); r *= snd; return r; }` keeps the by-value return, so neither population moves, and is +12 improvements — `TBoidLeader::calcBoids` 95.53 -> 98.79, `TMario::damageExec` 95.71 -> 97.83, `wireSWait` 93.09 -> 97.00, `wireHanging` +1.7, `wireWait` +1.1, `wireRolling` +0.6 — against six tenth-of-a-point regressions (`generate`, `landEffect`, `touchPlayer`, `emit`, `calcForces`) and `__ami__` MISSING, total fuzzy flat at 97.46 and -1 function.
  Reverted for the `__ami__` loss only.
  Spelling the same body `TVec3 r; r = fst; r *= snd;` is worse: boid's gain survives but `__ct__` **and** `__amu__` go MISSING in MarioCollision and the four wire* gains become losses.

## Library closure III, batch 155 (2026-09-18)

- **A one-argument inlined helper level costs exactly 8 bytes of frame**: writing `bankOscToOfs(i)`'s body out at both sites (`unk38[i]->isOsc() ? getOffset() : 1.0f`) landed `overwriteOsc` 0x38 and `playLogicalChannel` 0x50 exactly (JASChannel 6 -> 4). Return-type pricing confirmed on the same unit: a void helper shared by two tails is +0, a value binder +8, a reference/pointer receiver binder on the same member +16; nesting makes a binder per-site (one level over `unkB4[i]` is +8 saturating for two sites, two levels +8 each). Two levers flip the ranking (every binder pair that hit both target frames moved `channel` r31 -> r30), so a missing 16 must come from one construct.
- **An UNUSED helper can be the missing half of two siblings**: calling the UNUSED `JPAGetSVecElement` (0x18c) closed `JPAGetRMtxSTVecElement` and gave `JPAGetRMtxElement` its exact 0x24c body. `JPAConvertFixVecToFloatVec` closed with an uninitialised `TVec3<f32>` local (a 12-byte class local reserves 16 and shifts the caller's temps up).
- `-fp_contract` fuses a sum of squares only from locals: `squared()` on a memory-resident vector keeps three `fmuls` apart, so a contracted `fmadds` pair proves register operands; only a pointer aliasing a local struct keeps a `TVec3` local in memory. `if (x <= e) A; else B;` emits `cror eq, lt, eq; bne` with A as fall-through, the inverted spelling `ble` (+5.4 on `JPAVecToRotaMtx`). m2c's `local_NNN` names encode retail stack offsets and pin layouts (JDRSmJ3DAct's named region is exactly three matrices: the last two concats are the in-place `concat(b)`, 48 bytes of low region left where retail spills concat's twelve results; measured 74.06 vs 74.53, recorded not committed). JASTrack: `writeRegDirect`'s only landing is an identity wrapper (padding in disguise, rejected); `noteOn` is a zero-frame r23/r24 rotation on inner-block locals.

## Header round 26 (2026-09-18): RenderModeObj linked

- **A header inline with no map symbol in any TU is a free carrier**: `JDrama::TDisplay::getRenderMode()` has no symbol anywhere, and the binding lever inside the accessor (`GXRenderModeObj* rmo = &unk10; return *rmo;`) lands all four RenderModeObj frames (0x20/0x28/0x30/0x38; Game pays twice). `return *(&unk10);` is +0, so it is the named binding, never the address-of. RenderModeObj 25.6 -> 100, linked (483).
- **A consumed reference binding at the call site is 4 bytes of low region**, the mirror of batch 142's dead-binding +4 above: `const TVec3<f32>& goal = unk104.getPoint();` slides `calcDist`'s by-value copy 0x24 -> 0x20 in `TSpineEnemy::isReachedToGoal` (100; fishoid 69.7 -> 73.2). `AnimalNerve.cpp` already spelled the same body that way: look for a committed sibling before inventing a lever.
- JGadget `TVector::insert`'s 0x20 is additive over the MSL helpers (`O dummy` in `uninitialized_copy` -0x10, flattening `__copy_backward` -0x8, `dealloc` 0x18 but its dtor is four real instructions), not one object; `if (it == end())` is +8 (an accessor lever works, so it is a shortage). DrawSyncManager: map `end()` is 0x80 (a real method, not an accessor) and the dtor is 0x20 short: a unit round item.
## Research batch 161 (2026-09-18): the JGadget iterator pool ignores the return-type price ladder, and the grouping has no shared cause

Batch 133 left eight functions blocked on the JGadget list/iterator temp pool and batch 142 then found the inline-temp price-by-return-type rule (reference 8, pointer 4, void 0, additive over levels).
This batch re-ran the analysis with that rule on all eight sites at once, measuring slot maps directly out of the compiled objects against the dtk asm (driver `grpek/{score,run}.py`: frame, absolute slot offsets, and group sizes with the dead gaps between them).

**The eight residues, measured.** Retail against ours, as `frame  [group]+gap ...`:

| site | retail | ours |
| --- | --- | --- |
| `TLampTrapIron::loadAfter` / `TLampTrapSpike::loadAfter` | `0x78 [5]+8 [3]` | `0x78 [2]+4 [3]+4 [3]` |
| `TSeal::init` | `0xc0 [1]+4 [1]+56 [2]+8 [2]+8 [1]+52 [2]` | `0xc0 ... [2]+12 [2]+8 [1]+48 [2]` |
| `TMirrorActor::init` | `0xd8 [1]+132 [2]+12 [2]+8 [2]` | `0xd8 [1]+116 [2]+20 [2]+16 [2]` |
| `TRiccoHook::init` | `0xd8 [2]+4 [3]+16 [3]` | `0xd0 [2]+4 [3]+8 [3]` |
| `TEnemyManager::createEnemies` | `0xb0 [5]+8 [4]` | `0xa8 [5]+4 [3]+12 [1]` |
| `SDLModel::entry` | `0xb8 [3]+8 [2]+4 [3]` | `0xb8 [3]+8 [2]+8 [3]` |
| `TPerformList::perform` | `0xe8 [2]+12 [2]+4 [5]+12 [4]` | `0xc0 [4]+8 [2]+4 [3]+12 [4]` |

- **The return-type ladder is inert for this pool.**
  Measured on all eight sites at once, byte-identical slot maps to the stock header: a `void`-returning `TList<T>::iterator::operator++()`, a `void`-returning `TList_pointer<T>::iterator::operator++()`, both at once, `operator*` routed through `operator->`, and a new pointer-returning `operator->` on the derived iterator.
  Probing `TList_pointer<T>::insert` with an extra inlined level whose return type is by-value, pointer or reference leaves the three temps of the `insert` sub-block at exactly the same offsets in all three cases.
  So batch 142's price ladder is a `TVec3`-shaped rule, not a general one; do not re-derive the JGadget grouping from it.
- **Most of the chain is pinned byte-for-byte, so its return types are not free at all.**
  `TList<void*>::begin()`/`end()` (12 B each, ObjHitCheck / enemymanager), `TList_pointer<THitActor*>::end()` (36 B, bossManta), `TList_pointer<THitActor*>::iterator::iterator(TList<void*>::iterator)` (12 B, by-value parameter), `TList_pointer<THitActor*>::insert` (76 B, batch 133), and all of `std-list.cpp` (`operator==` 24 B, `erase` 108 B, `CreateNode_` 116 B, `TList_pointer_void::insert` 136 B) are 100 % today.
  Retail's `loadAfter` loads `TList_pointer_void::insert`'s return slot, so the "void in the base, iterator in the wrapper" split is refuted by the binary, and every `begin()`/`end()` is a by-value return with a hidden r3 slot, so the by-reference variant is refuted too.
  `__pp__`, `__dr__` and JGadget `__ne__` have zero map hits, but so do ours: MWCC only emits the weak copy of an inline it also calls out of line, so that absence carries no evidence.
- **The site spelling is fixed by the frame.** `getChildren().push_back(this)` is the only one of `->push_back`, `->insert(obj)`, `->add(obj)` and `getChildren().push_back` that keeps `loadAfter` at retail's 0x78; the other three land 0x68-0x70. The remaining free spelling on the five `TList_pointer` sites is `TList_pointer<T>::push_back`'s body, and five bodies (`insert(end(), what)`, a named `end()`, a dead named result, an `iterator` return, `this->`-qualified) all miss.
- **The residue is a per-expansion block *stride*, and it is not uniform across the eight.**
  Reading `loadAfter` instruction by instruction, both builds allocate the same eight temps in the same order; retail packs the `end()` block `{0x30,0x34}` and the `insert` block `{0x38,0x3c,0x40}` at stride 8 and we place the second block at 0x3c, i.e. stride 12, leaving a dead word at 0x38 and one at 0x48 where retail has both at 0x44/0x48.
  `TPerformList::perform` has the same stride-12 hole between the `begin()` and `end()` blocks.
  But `TSeal::init` is a flat +4 on one block, `TMirrorActor::init` wants 16 bytes moved from its two upper gaps to the bottom, `TRiccoHook::init` wants 8 *more* dead bytes and `TEnemyManager::createEnemies` wants +8/+12 on two groups: there is no single shift, so **no return-type/level assignment reproduces all eight, and none was found that reproduces even two.**
  A nine-line scratch model of the two-sibling-expansion shape (`D begin(){return D(L::begin());}` over a one-pointer iterator) packs all eight temps contiguously, so the stride is not a property of the derived-iterator idiom by itself.
- **One new lever, refuted by the map's own byte-exact neighbours.**
  Naming the result in `TSingleNodeLinkList::begin()`/`end()` (`iterator r(mTail); return r;`) packs those two blocks at stride 8: `TPerformList::perform` goes 0xc0 -> **0xe8 exactly**, keeps thirteen slots and lands retail's top two groups and both 12-byte gaps (99.33 -> 99.78 tree-wide), and the emitted weak 12-byte `end()` stays byte-identical.
  It is still not committable: it adds one temp to `TSingleLinkList<T,I>::Push_back`'s chain, dropping `TPerformList::push_back(TViewObj*, u32)` 100 -> 92.53 and `push_back(const char*, u32)` 100 -> 94.16, and six `Push_back`/`Insert` respellings (named `end()`, named `Insert` result, dead result) all keep seven slots where retail has six. Reverted.
- Also measured and inert on all eight: `TSingleNodeLinkList::iterator::operator=` deleted or void-returning, the derived `iterator(TSingleNodeLinkList::iterator)` ctor non-`explicit`, and `TList_pointer<T>::end()` spelled with an implicit derived-from-base conversion (the batch-133 lever is about the *emitted copy's* frame only; at an inline expansion it is free).
  Worse: a user copy constructor on `TSingleLinkList<T,I>::iterator` (perform 0xe8 -> 0xa8), `TSingleLinkList::begin()/end()` named (0xf0), `TList<T>::begin()/end()` named (SDLModel +0x20, seal +8, MirrorActor +8), the derived `operator==` comparing `unk0` directly (loses two slots), `TList<T>::push_back` as a named `end()`, a dead result or an `iterator` return (all `SDLModel::entry`-only and all worse).

## Follow-up closure 163 (2026-09-18)

- **Cross-unit header lead**: spelling `Enemy.hpp`'s `setGoalPathMario()` as `THitActor* mario = (THitActor*)gpMarioAddress; setGoalPath(mario);` (a pointer binding, +4) lands seven functions byte-exact tree-wide (`TGesso::behaveToFindMario`, `TNerveHaneHamuKuriUpWait`, `TPakkun::load`, `TNervePakkunShoot`, `TNerveFireWanwanAttack`, `TNerveBombHeiAttack`, `TNervePopoWait`; matched_code 53.91 -> 53.95) and costs two that are then 8 over (`TNerveWalkerTraceMario` 0x60 -> 0x68, `TNerveTelesaFreeze` 0x40 -> 0x48), which therefore carry a lever of their own to remove in the same change. Header round 28 item.
- **A binder's frame price is not uniform across sites**: the same construct was +4 on one function's temp and +8 on its frame, and 8/8/24/16 across a nesting ladder (JASChannel `buf` chain); bracket with two spellings before assuming a step. walkerEnemy `behaveToFindMario`: 8 per binder, 12 per conversion level, so the missing item is worth 4. NpcNerve `GraphWander`: the tracer accessors sit at depth 3, +2 levels make both `bl`s, so retail's chain has four enclosing levels and `getGraphNode` indexed `unk0[i]` directly (rejected: needs two invented forwarders). MarioCollision `damageExec`: the two copies are `operator*`'s by-value parameter copy (depth 3, we expand) and its `return fst` copy; the level cannot go inside `calcDamagePos` (already 0x18 over). `genEventCoin`: two errors netting +8 (one 8-byte pool item too many before the `operator-` copy, 16 missing after).

## Game re-pass 162 (2026-09-18)

- **The price of a dead non-trivial class local in an inlined callee is its exact `sizeof` once >= 8** (12 -> 12, not 16; rounded up to 8 below that): `ShapePacketCallBackFunc` is 0x90 short = exactly 18 expansions of the four UNUSED helpers x 8, an 8-byte local lands 0x178 to the byte and a 12-byte one gives +0xd8. A carrier's expansion count reads off the map by excluding emitted siblings and constant-argument sites. `SMS_InitPacket_Fog` closed: retail reads the PE block above the packet lookup and a named `peBlock` intermediate is the +8 (naming an intermediate also reorders a member chain when the named link is the one retail loads first).
- **A carrier needs an UNUSED function or a callee with no out-of-line copy**: MapMakeData's TU has no UNUSED function and both `setVertex` (0x40) and `initAllCheckData` (0xd0, two `setCheckData` expansions) are frame-exact, so its two 24-byte gaps have no legal carrier; MapObjItem2's `TMushroom1up::control` 12 dead bytes are a named local of its own body. MSModBgm's `getTiming`/`getTimingForce` are legal carriers but no candidate object exists (`getTiming`'s missing `param_2` body, 0x60 vs 0x94, is the way in).
- `MarNameRefGen_BossEnemy`: a TU-local binding level on the `TSleepBossHanachan` factory site puts `TVec3<f>::set<f>` at depth 4 so retail's `bl` appears, and binding the result buys the +8 (98.66 -> 99.99, instruction-exact; two spill slots left). SelectDir `rsetup`: every `push_back` expansion stores five words at stride 0x14 where retail reserves 0x18 (the JGadget stride, accumulating).

## Header round 28 (2026-09-18): +7 exact from one header binding

- **A +4 pointer binding inside a shared header inline is a global knob whose per-caller cost is identical, so its apparent losers were TU-local levers standing in for the header's missing 4**: `TSpineEnemy::setGoalPathMario` as `THitActor* mario = (THitActor*)gpMarioAddress; setGoalPath(mario);` made seven functions exact (`TGesso::behaveToFindMario`, `TNerveHaneHamuKuriUpWait`, `TPakkun::load`, `TNervePakkunShoot`, `TNerveFireWanwanAttack`, `TNerveBombHeiAttack`, `TNervePopoWait`; 53.96 -> 54.03) once telesa's two TU-local binder levels (4 bytes of pool each) and walkerEnemy's `getPosition().y` accessor (worth 4, not 8, when it feeds a single scalar component) were removed. Before rejecting a header change over a regressing function, check whether it carries a removable 4-byte lever.
- **The binding needs a consuming call in the same body to pay**: binding a global inside a header inline that only returns it (`SMS_GetMarioPos` 54.03 -> 53.55, `SMS_CheckMarioFlag`, `SMS_GetMarioGroundPlane`, `SMS_GetMarioX/Y/Z`) is broadly destructive.
## Library re-pass 162 (2026-09-18): the caller-side dead-carrier price is finer than the rules card

Six JSystem closure units (`JAIBasic::initDriver`, `TPolarCamera::perform`, `TFrmGXSet::perform`, `IssueGXCopyDisp`, `JKRExpHeap::allocFromHead`, `J3DModel::entryModelData`) were re-run against header round 26's two new rules.
None closed, but three of the measurements sharpen existing rules.

- **A dead named class local in the *caller* only reserves a slot when its constructor is empty, and its position is pinned to the last-declared slot.**
  `TPolarCamera::perform`'s 104-byte gap is 100.0% with zero markers for `volatile char trash[104]` declared immediately after `local_A4`, and also for `TPosition3f d1; TPosition3f d2; JGeometry::TVec2<f32> d3;` there (48 + 48 + 8).
  Declared *before* the first matrix the same 104 bytes still land 0x1a0 but scramble the three matrices (52 operands), so the dead block is the bottom of the named region, not the top.
  Granularity is 8 with a four-byte pad below the saved registers absorbing the remainder: trash[96] through trash[100] all stop at 0x198, trash[101] reaches 0x1a0.
  Two refinements to the card: a dead `TVec3<f32>` is worth **0** in this caller, not 12, and `JDrama::TRect` is not a zero-instruction carrier at all because its `JUTRect()` initialiser list emits 13 instructions (90.4%).
  The remaining 8 bytes still have no natural owner, which is why nothing was committed.
- **All expansions of one inlined callee share a single temp block, so a large dead low region is never "one pool per expansion".**
  All three `concat` expansions in `TPolarCamera::perform` spill their three float temps to 0xc/0x10/0x14 in *both* builds, which kills the reading that retail gives each `SMatrix34C<f32>::set(12 f32)` expansion its own 48 bytes.
- **An address binding at a call site is +8 whether the local is a pointer or a reference, and it can buy a register cluster.**
  In `J3DModel::entryModelData`, naming the shape packet (`J3DShapePacket* p = &mShapePackets[shape->getIndex()];`, or the `&` form, or with the whole chain inside the binding) removes all five operands of the `addShapePacket` cluster -- retail's `lwzx r4` / `lwz r5, 0x84(r29)` homes appear exactly -- and costs +8 of frame (0xf0 against 0xe8).
  Net markers are unchanged, so retail holds this binding *and* eight fewer bytes elsewhere; the obvious donor, the else branch's `J3DMatPacket* packet`, is an address binding retail demonstrably has too (`add r24, r0, r23`, read twice).
- Inert or refuted, with no new rule: hoisting `content`/`aligned`/`offset` in `JKRExpHeap::allocFromHead` to function scope (byte-identical -- the batch-145 declaration-order knob really is confined to callee-saved locals), folding its `aligned` away (frame 0x30), `size += 3; size &= ~3;` (98.6); hoisting `entryModelData`'s `mat`/`dlSize`/`packet` to function scope (r20/r21 do not move); `render_mode.aa != 0` (98.0) and `(bool)render_mode.aa` (byte-identical) at the `IssueGXSetCopyFilter` call.
- `JAIBasic::initDriver`'s carrier is now known to be *legal* and still unfindable: `initAudioThread` is UNUSED at 0x78 -- the same size as `initDriver`, which is what a pure forwarder gives -- so a dead 8-byte class local in it is allowed, but no two-word non-trivial class is reachable from the TU, and the map refutes an accessor carrier (`getParamAudio*ThreadPriority` are UNUSED **out-of-line** symbols of JAIGlobalParameter.cpp, and every callee takes plain scalars).
  Scanning the map for it turned up an unrelated stub: `bootDSP__8JAIBasicFv` is UNUSED at 0x20 while ours is an empty body.
- `TFrmGXSet::perform`'s frame shortfall is now 0x70, not 0x78, because round 26's `getRenderMode()` binding moved it 0xb8 -> 0xc0; the two-copy reading in its TODO has to be re-derived. Its register residue is exactly one reservation: ours allocates the scratch pool {r4, r6, r7}, retail {r6, r7, r8}, so `cue` stays live past the guard in retail and no guard spelling reaches that.

## Header round 29 (2026-09-18): the consuming-binding shape is exhausted

- **Receiver bindings in shared headers are destructive, argument bindings pay**: binding a global that becomes the callee's `this` (`SMS_LoadParticle` -1, `SMS_EasyEmitParticle` -6, `TNameRefGen::search`/`search2` -62 and DOL broken, `TSpineBase::pushNerve` -58) loses, because a bound receiver binds only as a fresh load; `gpMarioAddress` in `setGoalPathMario` was the only argument-passing global read in `include/`. A consumed *reference* binding on a by-value-returning inline call is the payable form (third confirmation): `switchNextGoalPath` as `const TPathNode& next = unk114.pop(); unkF4 = next;` (+1 exact, `TNerveBPFlyPivot`).
- **When a header binding supplies part of a TU-local lever's value, reduce the lever's expansion count rather than delete it**: `JPAParticleGetStep` is +8 per expansion, the `getEmitterDataBlockInfoPtr` binding +8, so one expansion instead of two restores `checkCreateChildParticle`; `JPADraw::initialize` then sits at retail's 0x190 with one dead 4-byte named local declared first (slot 0x184 reserved, never touched: a dead scalar is worth 0), so it stays parked. `getMaterialID`'s binding is +4 per expansion and the two linked `J3DModelLoader_v26` readers carry no lever (already byte-identical streams, residue purely frame): parked with a positive reason.

## Research batch 171 (2026-09-18): the callee-saved **FPR** allocation order

Research 144 pinned the callee-saved GPR ranking; the FPR one had no rule and was blocking a named residue class.
Measured the same way, in a scratch TU with the game flags (drivers `grpeu/fdrv.py`, `t1`-`t5`: `C::run(f32 p, f32 q, f32 r)` with one `bl sink()` per use so every value is live across a call).

**Callee-saved FPRs are handed out f31 downward: long-lived temporaries of an *inlined callee* first, in *reverse* declaration order inside that callee; then the function's own named `f32` locals in *forward* declaration order; then parameters in *reverse* parameter order.**
Only a **named** local (any initialiser) or a computed callee temporary qualifies; an anonymous literal, member or global read is rematerialised at each use and never takes one.

| variant | ranking, f31 first |
| --- | --- |
| locals declared `a`, `b` / `b`, `a` | a > b / b > a (first-declared highest) |
| the same with `b` used first, or `a` declared uninitialised and assigned after `b` | unchanged (first use and definition order inert) |
| locals `a`, `b`, `c` / `c`, `b`, `a` | a > b > c / c > b > a |
| five locals | f31-f27 in declaration order |
| one local + one parameter | local > parameter |
| parameters `p`, `q`, `r` | r > q > p (reverse parameter order) |
| two locals + two parameters | a > b > q > p |
| named local holding a literal, a member read, a global read, a call return, or a loop accumulator | ranked purely by its declaration position |
| anonymous `1.0f` / `mb` / `0.0f` live across four calls, or a literal stored to five places across a call | **no** callee-saved FPR at all |
| two locals both `= 0.0f` | CSE'd into one register |
| a local not live across any call | no callee-saved FPR |
| inlined helper's temps `u`, `v` / `v`, `u` | v > u / u > v (**reverse** declaration order) |
| inlined helper's temps `u`, `v`, `w` / `w`, `v`, `u` | w > v > u / u > v > w |
| a caller local declared before **or** after the helper call | helper temps outrank it either way |

This **corrects batch 86/90's prose**, which read "the last-declared local gets f31" while its own worked example (`f32 z` before `f32 y` giving f31 = z) said the opposite; first-declared is right.
It also explains batch 153's "literal temps rank above named locals": those were an inlined callee's temps, not literals.
Batch 86's "a member read must be a named local", round 22's "declaration order inside an inlined helper does not transfer" (it does, reversed) and batch 145's "block scope is inert" all hold as measured.

**Volatile FPRs (f0-f13) are allocated as blocks and are not a source-order knob.**
`expf` is the clean instance: every instruction, the whole Horner chain and the 0x28 frame match, and retail simply puts the four long-lived values in f6-f9 with the coefficients in f1-f5 where we do the reverse (f1-f4 long-lived, f5-f9 coefficients) — the same wholesale block trade batch 145 recorded for `checkNextFrameSe`'s GPRs.
Inert there: both declaration orders of the two reciprocal constants, `const`/`static const`/plain (plain is worse), splitting the float declarations, declaring floats before ints, one or two spare float locals, grouping the two constants, dropping the named `finalVal`, reordering the two multiplies, naming the table entry, staging the sum, spelling the constants as literals, and moving `__HI(pow2)` below the chain (68%).
**The one knob that does move a volatile pair is naming the values as locals**, because raw arguments evaluate right to left while named locals evaluate in declaration order: that decides which of the two is loaded first and carries its register with it.
`TPollutionCounterLayer::drawJointObjStamp` closed on exactly that (99.88 -> 100, unit 88.21 -> 94.35 matched_code): `f32 minZ = layer->mMinZ; f32 minX = layer->mMinX;` before the `makeWorldToPollutionMtx` call, **mMinZ first**, which is retail's load order; `minX` first is the stock four markers.

- **`TRideCloud::control` (MapObjCloud, the unit's only non-exact function) is exhausted at four markers.** Fifteen more groupings and operand orders of `getScaling().x * 300.0f * unk160`, reusing `fVar8` as the literal's home (four forms), naming one or both member reads in both orders, `mScaling.x` raw, and putting `mDamageHeight = 50.0f` first (9 markers) are all 4 or worse. Batch 129's "halved on paper" note is optimistic: the named-product form measures the same 4. Keep the stock single expression.
- **`TBathtubBinder::float_` is a group trade, not a rank inside a group.** Retail's dir pair sits in f31/f30 *above* the two `fmadds` products (f29/f27), ours in f28/f27 *below* them (f31/f30), and the swap survives: `f32 dirZ`/`f32 dirX` in place of the `TVec3 dir` (either order, 115 markers and -8 frame), and an inlined four-statement `BathtubDirXZ(dir, rot)` helper in both temp orders (116 markers, +8 frame). What *does* move is the **load order**: declaring or assigning z first reproduces retail's `lfs 0x104` before `lfs 0xe4`, for free. Parked at 96.2.
- **`JASChannel::updateEffectorParam` (14 markers) is also a block trade**: separate `panOut`/`fxmixOut`/`dolbyOut` locals (25), clamping after `unk98` (84), clamping inside the two call argument lists (118) and clamping dolby first (20) all lose. Retail's f5/f6 Clamp01 temps are unreachable from the source.

## 4-byte residue re-pass 172 (2026-09-18): the nested fork is the general +4, and two links

Batch 170's ladder said the +4 rung was "a nested direct-return fork over a global or file-local static".
This pass re-ran the recorded 4-byte residues against it and the rung is broader than that, but also fussier about where it is spelled.

- **A direct-return fork nested inside a binder is +4 over a plain *member* read too, not only over a global**, and it has to be inside the binder -- at the call site it is +0.
  `TMovieSubTitle::setupResource`'s last 4 bytes (`buffer` 0x24 against retail's 0x28, frame already exact since batch 151) close by having the TU-local binder read the member through a new `TMovieSubTitle::getScreen()` fork instead of `p->unk14` directly: +4 on top of the binding's +8, zero instruction change. MovieSubtitle 70.55 -> 100, linked (486).
- **A fork over a global that the header already reaches is +4 only where the site is itself a fork, and it must be used at *every* site or it perturbs the register coalescing.**
  `TMarDirector::preEntry` needed +4 of pool below the `SMSGetRederRect_Game()` return buffer (0xb4 against 0xb8).
  One TU-local `PreEntryRootNameRef()` fork over `JDrama::TNameRefGen::instance`, used by both wrappers in place of the header's `search<T>`/`search2`, is exactly that +4 and closes the function byte-for-byte; MarDirectorPreEntry 0 -> 100, linked (487).
  Routing only the four binder sites through it is frame- and register-neutral, so it costs nothing to reach the fifth site; nesting it under `search2` *alone* lands the 0xb8 buffer but makes the first expansion's root-ref temp coalesce with r28 (the scratch the other four use) where retail coalesces it with r29, `setViewMtx`'s own register.
  Two identical stacked forks collapse (+0), so a redundant wrapper buys nothing.
- **Header round 26's consumed reference binding is +4 out of line but +8 in an inlined expansion of the same body**, which is why it has to be paid for across a whole family at once.
  `TRealoid::clipBoids` closes with `const TVec3<f32>& boidPos = unk150->getBoid(i)->mPosition;` ahead of the named copy (0x48 -> 0x44); the same binding costs `TRealoid::perform` 8, paid back by spelling its loop bound `unk150->getBoidNum()` instead of `getBoidLeader()->getBoidNum()`; that in turn moves `TFishoid::perform`, which takes the same reference binding (+8) plus a new `TRealoid::getBoid(int)` forwarder -- a nested fork over `getBoidLeader()`, the last +4. All three byte-exact, fishoid 73.20 -> 77.27, nothing else in the tree moved.
- **Where the residue is an allocation *order* difference inside a fixed pool, the whole ladder is 0/8/16 and no +4 exists.**
  `TMovieRumble::init` is the clean case: its low region is 0xc..0x30 = 36 bytes on both sides, split 28/`type`/4 by retail and 24/`type`/8 by us, and the frame never moves for +-4 because the named region is 16-aligned. Sixteen init-only constructs were measured and every one is 0, +8 or +16 (list in the unit's TODO); the only two -4 steps both delete a member reload retail keeps, so they are refuted by the binary.
- **`JAIBasic::checkPlayingSeqTrack`'s second dead slot is the `u32* portFlags = sud->mTrackUpdate;` binding**: removing it gives retail's 0x98 exactly at the price of sixteen reloads, so retail holds that pointer in a callee-saved register with no dead pool slot while every caller-side pointer binding we know reserves one. Two bindings in one *comma declaration* do not share a slot the way two bindings in one inlined *level* do.
- `TMarDirector::setupPerformList_console` drops from 17 markers to 6 with a console-only three-level stack (instance fork, `search` fork, existing binder) -- every slot up to 0x54 lands and only the `TList_pointer_void::insert` sub-block is left, 4 bytes high, which is the research-161 stride. Not committed: two fabricated helpers for a function that still does not close.
- Still inert on their own residues, re-measured here: `TFishoid::load`'s TPathNode/`eventId` slot swap (a `const TPathNode&` binding at the call is +8, a dead `u32` copy +0), `TNpcInbetween::execPosInbetween`'s f1/f2 pair (naming the reciprocal, naming the converted timer, and the reversed product are all worse), `TRideCloud::control`'s three-factor product (nine orderings and namings, all 4 markers), `TMapObjTree::initMapObj` and `TMarNameRefGen::getNameRef_BossEnemy` (register/spill-slot permutations with no source knob).

## Library structural pass 170 (2026-09-18): three library links

- **Frame-lever price ladder, measured twice**: a one-parameter pointer-returning binder is +12, a parameterless one +8, a nested direct-return fork over a global +4, a void return +0; sub-8 steps exist. `HardStream::TControl::startFirst`/`startSecond`'s "24-byte class local" was three stacked levels over the two file-local statics (name binder +12, `rootDir` binder +8, `streamFiles` fork +4); JASHardStream linked.
- **A binder's site count is a dial (+8 each): use it at k of n identical sites**, and a TU-local binder beats the same binding in a shared accessor with other callers: `JPADraw::initialize`'s 16 bytes are a TU-local binder over `getEmitterDataBlockInfoPtr()` at two of five sites (header round 27's header version cost `checkCreateChildParticle`); JPADraw linked. `JAIBasic::checkEntriedSeq` closed with one binder over the `&unk0->mSeqTrackInfo[i]` lookup; JAIGFrameSequence linked.
- **`volatile` on a status word converts one CSE'd load into per-site loads: a diagnostic for a CSE residue, never a fix** (`JPABaseField::calcFieldFadeScale` closes with `volatile u16 unk54` but `JPAAirField::affect` CSEs the same member; bitfields refuted). `checkPlayingSeqTrack`'s residue is exactly 4 dead bytes (the real `addi r30, r26, 8` reference costs them). `JPAVecToRotaMtx`: retail's `axis` is memory-resident and scalar replacement is not blocked by an inlined pointer-taking helper (mechanism unknown). Library structural census: 9 functions of 22 units in the 1..16 range; the well is dry.
## FPR re-pass 172 (2026-09-18): an inlined callee's **parameter**, modified in place

Research 171's knobs are all about which value gets which register.
This re-pass found the knob for the other half of the question — which register the *load* lands in — and it is the same lever the rules card already records for subtraction (`f32 x = a - b` subtracts into `b`'s register).

**A parameter of an inlined callee that is modified in place (`amp *= scale`) makes the load land directly in the result's register; copying it into a fresh named local first (`f32 a = amp * scale`) loads into a scratch register and writes the result elsewhere.**
`TCameraShake::setShakeAngleOne_` is the clean instance: retail is `lfs f3, 0x2c(r31); fmuls f3, f3, f31` and the named-local form gives `lfs f1, 0x2c(r31); fmuls f3, f1, f31`, only in the *first* of the three expansions (the other two coalesce either way, because the reciprocal is already CSE'd and the value only needs f1 there).
Batch 129's rejected `f32 a = amp; a *= scale;` looks like the same shape but is not: it is one statement *more* than the stock form and pushes the body out of line at depth 2, while modifying the parameter is two statements *fewer* (no `f32 a`, no `s16 v`), which leaves room to spare.
The integer parameter takes the same treatment (`vel = -vel`), and it is worth 0 frame.

Second finding, on the same function: **dropping the two named locals also drops 8 bytes of frame**, because one inlined callee's named scalars share a single temp block across all its expansions.
`TCameraShake::startShake`/`keepShake` recovered it by reading all six `TCamSaveShake` parameters through `TParamT<T>::get()` instead of the amplitudes through `get()` and the velocities through `.value`: a `const T&` return reserves an inline temporary per expansion, so six get()s are retail's 0x50 where three are 0x48.
That also makes the call sites uniform, which the mixed spelling never was.
Both functions went to 100% and the unit's `matched_code` 32.48 -> 60.05 (only `execShake`, a 56-byte low-region gap, is left).

Negative results from the same re-pass, all measured:

- **`BHSCalcRevisionDistXZByRotateZ` cannot be fixed from its own body.** Naming the two results (`f32 revisedX`/`revisedZ` before the two stores) lands the standalone frame exactly (0x58, and all 9 operand markers go: 94.0 -> 94.3, one instruction-order difference left) **but costs +8 at the inlined site** in `TSphereLink::setDegreeZAndRevisionPosXZ`, which is frame-exact at 0xa8 without it (98.2 -> 98.1, 18 -> 37 markers). Since both sites share one body, retail's extra 8 bytes in the out-of-line copy are not in the body at all — they are the standalone's own dead region, and its 52 bytes are entirely unreferenced in both builds. Also re-measured worse: `sine` before `cosine` (92.9), un-naming both (66.9, it recomputes, though the frame does land).
- **`TBellDolpic::ring`'s two clusters are block trades.** Un-naming the whole `rand()` chain is 30 markers and -8 frame; `0.000030517578f * (f32)r` with `14400.0f * tmp` is byte-identical to the stock spelling. Retail keeps both literals in f1/f0 and chains into the literal's register where we load them into f2/f3 and accumulate into f0 — no operand or naming spelling reaches it.
- **`MSHandle::calcDolby` is a block trade too** (retail merges the four if/else results into f0 and the clamp into f2, ours f3/f0, every instruction and the 0x20 frame exact). Both values are already named locals, which 171 says is the only knob.
- **`JPAGetRMtxTVecElement`'s rotation does not fit 171's ranking.** Retail is x f31, **z f30, y f29**; we are x f31, y f30, z f29. Neither reverse (callee temps) nor forward (own locals) order produces x above a reversed {z, y}, and the sibling `JPAGetRMtxSTVecElement` — same body, but the vector is a reference parameter rather than a local — is byte-exact, so `JPAGetSVecElement`'s body cannot be touched to chase it.
- Re-confirmed as *not* FPR residues, so out of this class: `TPollutionCounterLayer::countTexDegree` (0x58 low-region gap), `JDrama::TPolarCamera::perform` (104 dead bytes, fully diagnosed), `TMapCollisionBase::updateTrans` (24 dead bytes), `CPolarSubCamera::updateDemoCamera_` (36 bytes of pool below the named block), `TMario::surfingEffect` (argument evaluation order plus 0x10 of frame), `TNpcInbetween::execPosInbetween` (a single volatile destination choice between two dying temporaries), `SMS_EmitSinkInPollutionEffect` and `TSmJ3DAct::perform` (both blocked on shared JSystem headers whose tree-wide measurements are already recorded and negative).

## Batch 177 (2026-09-18): execShake and countTexDegree diagnosed

- **The pool is allocated in reverse statement order, so bytes below an inlined callee's local belong to the statements after it**: `countTexDegree` (149 = 149 instructions, 88-byte gap) has 116 below / 8 above the inlined `loadPollutionLayer` texObj against our 32 / 4, while `countObjDegree` with the same inlined body is byte-exact, so the debt is `countTexDegree`'s own later statements. A TU-local pointer-returning binder over `mLayers[t]->getPollutionImage()` is +4 per site here, not the library ladder's +12 (the ladder is per context); a binding inside `getModelStampDrawBuffer` is +8 per expansion but breaks `calcViewMtx`. The three short UNUSED bodies are each one instruction, not a dead local.
- `TCameraShake::execShake`: retail builds the roll axis from `origin`, not `*pos`, and `unitVecTo` must be `static inline` (the map has no room for a local symbol; camerashake's four `>>>` weak duplicates are `setRotate`/`scale`/`dot`/`inv_sqrt`). Residue: one dead 8-byte named local plus 52 dead pool bytes reproduced byte-for-byte with padding, with no legal carrier (the TU's UNUSED callees are not called from here and every inlined callee has an empty frame); ten register markers are retail CSEing a bool flag's constants with neighbouring field stores (`0` in r8 serving `anyActive`, `i` and `mRollAccum = 0`).

## 4-byte residue re-pass II 178 (2026-09-18): `normalize()` is the +4 dial, and a parameterless binder can be +4

Re-pass 172 had left five recorded 4- and 8-byte residues unreached.
This pass closed two of them and pinned a third's remaining half; the three findings are all about *where* a rung's price is charged, not about new rungs.

- **`normalize()` against `setLength(1.0f)` is a per-call-site +4 dial.**
  `JGeometry::TVec3<f32>::normalize()` is `setLength(*this, TUtil<f32>::one())`, so it carries the card's `TUtil<f32>::one()` rung; spelling the unit vector `setLength(1.0f)` drops that level and with it 4 bytes of low region, with a byte-identical instruction stream.
  `TBoidLeader::calcGoalForce` closes on it (99.59 -> 100, frame 0x50 -> retail's 0x48 and every displacement).
  The dial is per site, not per file: the same respelling is **-8** in the sibling `TBoidLeader::calcForces` (0xa0 -> 0x98, still 8 over, 99.6 -> 99.5) and -8 in `TBoidLeader::calcBoids`, which is already 144 bytes short, so both keep `normalize()`.
  Statement-by-statement pool triage on `calcGoalForce` is worth recording as a method: deleting each `+=`/`-=`/`*=` in turn left the frame untouched, so the whole 40-byte low region was the one `normalize()` expansion, which is what made the rung findable at all.
  Scanned and rejected as candidates for the same dial: every other unit with one non-exact function that calls `normalize()` (`CameraJetCoaster` +200, `feetinv` +104, `walker::calcFarthestVertex` +24, `camerashake::execShake` +56, `MathUtil::MsGetRotFromZaxis` +16) is *short*, so the dial points the wrong way there. The inverse -- `setLength(v, 1.0f)` -> `normalize()` to buy +4 -- is the form to try on a short frame.
- **A parameterless binder is +4, not +8, when its body also does the arithmetic the call site would have done.**
  `CLBScreenFPosToSPos` needed 8 bytes of dead pool below its int-to-float conversion pair, with no inlined callee anywhere in the body to carry them (header round 30 had parked it for exactly that reason).
  Two `static inline` binders in `cameralib.hpp`, `CLBGameRenderWidthMax()`/`CLBGameRenderHeightMax()` (`u16 w = SMSGetGameRenderWidth(); return w - 1;`), give +4 each and close it (98.04 -> 100, frame 0x28 -> 0x30; sunmodel matched_code 63.53 -> 71.08, no other unit in the tree moved).
  The `- 1` has to be **inside** the binder: returning the raw size and subtracting at the call site keeps the frame but reorders the `0.5f` literal load against the `1.0f + x` add (89.6), and a plain `u16 width = SMSGetGameRenderWidth();` local in the function body is +0 -- though that local alone is worth 98.04 -> 99.8, because it un-rotates all four volatile FPRs, so measure the naming before measuring the level.
- **The `SMSGetMarDirector()` global fork is the +4 that a `Vec&` base binding costs.**
  `TMapWarp::init`'s every-displacement-4-high residue (batch 136) is closed on displacements: retail's 92-byte low region is 48 (six `>>` continuations) + 16 (loadWarpPointPos's inlined parameters) + 4 (the `Vec& pos = positions[no];` binding, which is *needed* -- without it MWCC folds `+4`/`+8` into the `addi r4` at each call and loses retail's hoisted `addi r20, r5, 8`) + 4 (`SMSGetMarDirector()` instead of `gpMarDirector->`) + 16 (two of the three `getUnk8()` accessor sites, +8 each).
  Also measured there: naming `stream.readString()`'s result rotates seven callee-saved registers for nothing (97.2 -> 98.1 for the un-naming alone), and a continuation count is verifiable by *register* count, not only by frame -- the "3,3,1,2" grouping loses both 8 bytes of pool and one callee-saved register.
  What is left is the known-open callee-saved permutation (retail `this` r31 / stream r25 / one register shared by the array base and `cnt`; ours r24 / r27 / two registers).
- **Two residues re-measured into the block-trade and allocation-order classes, so they are out of the lever budget.**
  `TAmiNoko::isHitValid` is frame-exact and its whole residue is one volatile-FPR block starting two registers higher in retail (f5/f4/f3 against f3/f1/f2), with identical load order and identical `fsubs` destinations; naming matan's discarded result, moving the call, building the vector from the subtractions, `SMS_GetMarioX()/Z()` and a named `const TVec3<f32>& up` are all inert or worse.
  `SampleCtrlMaterial::SampleCtrlMaterial` is the cleanest instance of the pinned-pool class yet: frame 0xc8 and the saved-register block at 0xb0 on both sides, one single referenced slot in the whole 164-byte pool (the 4-byte lookup table a J3D `getColorChan` inline copies to the stack), and it sits at 0x90 retail against our 0x94 -- 4 bytes too many attributed to the statements *after* it, in a pool whose total is already right.
  `TSunModel::perform`'s 72-byte named block is the same size on both sides but internally reversed (retail `mtx` 0x78 below `dir` 0xa8, ours `mtx` 0x98 above `dir` 0x80), and a fabricated `SunModelCalcAnim(TSunModel*)` level around the whole `CUE_CALC_ANIM` body -- the reading its TODO called untested -- is byte-for-byte inert, so `mtx` is not steerable from the caller at all. Its frame half is cheap once the reversal is understood: the `SMSGetCamera()` fork is +4 at each of four read sites there (all four raw gives 0xd8, -16).

## Research batch 184 (2026-09-18): the JGadget stride is not a property of `std-list.hpp`

Delta-debugged the real header against a faithful standalone model (driver under the batch's scratch dir; ~150 compiles, slot maps read off the asm), on `TLampTrapIron::loadAfter` and then all eight blocked sites at once.
The JGadget inline-temp pool splits into two blocks: the `TList_pointer<T>::iterator` objects at the top of the pool (identical in both builds) and the `TList<void*>::iterator` by-value temps below.
The residue on the eight sites is never a missing or extra temp but the position of the dead word inside the lower block: retail packs every expansion's base temps into one contiguous run and parks both dead words at its top; we pad each expansion's sub-block by one word.

- **No structural property of the header controls it.** Private vs public inheritance at either level, replacing the derived iterator's inheritance with containment of a `Base::iterator` member, deleting `operator*`/`operator++`/`operator++(int)`, making `TList<T>::iterator(TNode_*)` public, routing `end()` through `TList_pointer_void::end()` and the batch-133 implicit conversion are all **byte-identical at all eight sites**.
  The one-word iterator size is pinned by the map (`__ct__` of `TList_pointer<THitActor*>::iterator` 12 B, of `TList<void*>::iterator(TNode_*)` 8 B), and the `insert`/`end` signatures by their emitted copies (MapWireManager 0x4c, bossManta 0x24), so only the *bodies* of `begin`/`end`/`insert`/`push_back` are free.
- **In the model the pad is a function of inline nesting depth alone**: one expansion packs, two nested expansions with a discarded by-value class result pad one word per block, a third enclosing level packs again, and naming the outer result packs.
  None of it transfers: a forwarding helper in `push_back` or `insert` (or both), a named-result `push_back`, dead iterator locals before or after the call and `this->`-qualified calls are worse or inert on the real sites.
- **The three free bodies are exhausted.** A 125-cell sweep (5 `end()` x 5 `insert` x 5 `push_back` bodies) over the five `TList_pointer` sites took the summed slot distance from 47 to 22 (`iterator w = where; return iterator(Base::insert(w, what));`, mostly `createEnemies` 21 -> 4 and `TMirrorActor::init` 12 -> 4) at the price of `loadAfter` and `TSeal::init` 3 -> 4, landing nothing.
  `SDLModel::entry` and `TPerformList::perform` never move under any `std-list.hpp` edit: they are not on this header's chain, so "eight sites, one cause" is refuted a second time.
  The remaining lever is outside `std-list.hpp` (the callers' own enclosing levels), and a further header sweep is not worth a batch.

## Header round 31 (2026-09-18): the `operator*` consumption split reaches the number of materialised copies

V2 (`friend TVec3 operator*(const TVec3& fst, f32 snd)` with an internal named local, by-value return) is +12 fuzzy improvements (`calcBoids` +3.3, `damageExec` +2.1, `wireSWait` +3.9, `wireHanging` +2.4, `wireWait` +1.1, `wireRolling` +0.6) against six tenth-of-a-point losses: total fuzzy +0.0046, matched_code flat, zero functions to or off 100, and Tongue's weak `__ami__` (0x34) drops to MISSING.
Retail's two `(a - b) * k` sites in `TYoshiTongue::movement` decode to `bl __ct__`, `bl __ami__`, `bl __ct__`, an inline copy, `bl scale`: **three** objects per site, i.e. V3 (`TVec3 operator*(TVec3 fst, f32 snd) { TVec3 r(fst); r *= snd; return r; }`), which tree-wide costs `TWarpInCallBack::execute` 73.9 -> 42.3, `generate` -14.5, `forceRoll` -9.3 and about 25 more.
The only V2 repair that re-emits `__ami__` is an explicit `TVec3(a - b)` temporary at both sites, inert under the stock header (a pure level substitute, i.e. a fakematch) and costing `movement` 1.43.
Rejected, nothing committed. Do not retry a member/friend by-value pair: the populations differ only by consumption, so overload resolution cannot select between them; the class is closed until a caller-side mechanism is found.

## Research batch 193 (2026-09-18): a fabricated gate hid a constant-folded real one (MSound)

- **`MSound::checkUnkA8` never existed.** Every one of its eight sites passes a constant sound id, and `(id >> 11 & 1) | (id >> 24 & 0xC0)` folds to 0 or 1 for all eleven ids, collapsing the real `MSound::gateCheck(id)` to exactly the `unkA8 & 1` / `unkA8 & 2` test the binary shows. Folding it in is worth +8 of low region per function (saturating, not per expansion) and closed `pauseOn` and `demoModeIn`. `gateCheck` inlines at depth 1 only: at depth 2, inside `MSound::startSoundActor`, it stays a `bl`, which explains every out-of-line `bl gateCheck` in the game and rules the wrapper overloads out at these sites. When a TU's sites are gated by a fabricated one-line flag test whose call sites all pass constants, check whether the TU's real gate function folds to that test before treating the test as a carrier.
- **The missing carrier is sized but unnamed.** A dead 8-byte non-trivial local in the fabricated gate was +8 per expansion and fitted six functions (`startBeeSe`'s four expansions to the byte); the real `gateCheck` cannot hold it (emitted and byte-exact) and `MSBgm::setAllTracksVolume` cannot either (`demoModeOut` goes 8 over). It must live in `MSoundSE.hpp` around `startSoundActor`/`startSoundSystemSE`/`startSeRandPlay`. Refuted at these sites: a named `bool`/`u8` local in the gate, a `u8& getSeStatus()` reference accessor, the `MSoundCheckUnkA8` binder (+0), the 2-arg `MSound::startSoundActor` (turns `gateCheck` into a `bl`).
- A TU-local **direct-return** fork over a raw member read (`MSoundUnk0D(p) { return p->unk0; }`) is +8 in a loop whose body only calls and +16 in a loop that also does a float-to-int conversion; the binder form is +16 in both.
- Data closed (85 -> 100): `@3109` is `0.48f`, not `0.6f` (`talkModeIn` fades the BGM tracks to 0.48), and `talkModeIn`'s category loop is `setCategoryVOLs(0x44, 0.0f)` (the constant folds the `min<u8>` block away). `fadeOutAllSound` 76.7 -> 100: `setCategoryAllVolume`'s list walk carries a counter and breaks after 100 sounds, which also puts the UNUSED body at its map size 0x9c.
- Residues: `startMarioVoice` 0x28, `startBeeSe` 0x18, `pauseOff`/`talkModeIn`/`talkModeOut` 8 (the unnamed carrier); `setParameterSeqSync` 0x30 plus a callee-saved rotation (retail ranks the `TTrack*` parameter above both loop counters); `loadWaveBackword` frame-only with the buffer 12 bytes low; `MSound::MSound` 86.9.

## Library closure 202 (2026-09-18): eight near-link JSystem units, none closed, ladders measured

- **An address binding that costs +8 can be paid for by dropping a neighbouring named pointer local the binding's chain already reloads**: `J3DModel::entryModelData`'s named shape packet (`J3DShapePacket* p = &mShapePackets[shape->getIndex()]`) lands the five `addShapePacket` operands at +8, and dropping the `J3DMaterial* mat` local in the else branch is exactly -8 with no instruction change (99.82 -> 99.92, frame 0xe8 exact, 312/312 instructions). Residue: `dlSize`/`J3DMatPacket*` in retail's r25/r24 against our r20/r21, a zero-frame rotation; reference form, `mMatPackets + i`, `int dlSize`, hoisting to block or function scope are inert. Look for the donor inside the same loop before rejecting a binding as unaffordable.
- **MWCC emits `ptr + index` as `add rD, rIndex, rPtr` and no index spelling reverses it** (casts, reassociation, `&a[..]` vs `a + ..`, named intermediates; `J3DSkinDeform::initMtxIndexArray` 99.96 stays).
- **A TU-local binder scoped to one function prices a frame lever a shared accessor would break elsewhere**: in `JALSystem::append` a binder over `TFlagManager::get()` is +8/+16 alternating per site (0xd0, 0xe0, 0xe8, 0xf8, 0x100, 0x110 for 1..6 sites), over `JALSystem::spFManager` a uniform +8/site, a second nested level +24/site; a direct-return fork inside the binder and a `void` wrapper are identical to the plain binder; a pass-through pointer binder on `&set1`/`&prm` at all twelve arms is +12 each and perturbs the named block. Target 0x130 = six-site binder (0x110) + exactly 4 x 8, matching the four named `JALPrmSet`/`JADPrmS<f32>` constructions in the prologue: a +8 binding level inside those constructors (`JALModSe.hpp`, shared with MSoundBGM, mameGesso, MSSetSound) is the last item, and closing it links the unit.
- Recorded open: `JKRExpHeap::allocFromHead` (the `nor` for `~(align-1)` hoisted into the pre-`stwu` slot; source position inert across five spellings); `J3DDeformer::deform` (frame 0x110 vs 0x118 as two independent 4-byte gaps); `IssueGXCopyDisp` (six-operand scratch permutation inside an inlined body whose out-of-line copy is byte-exact: retire to known-open); `TFrmGXSet::perform` (frame solved by two named `GXRenderModeObj` copies, the blocker is retail keeping `cue` live in r4); JPAEmitterManager ctor (scratch round-robin) and `createEmitterBase` (base temp ranked above the named `linkInfo`, seven named-local-count changes inert); `sendPlayingSeCommand` (low pool 8 larger below the float temps; dropping the `portMask` binding moves the frame -8 at 142 operands). Declaration order and scope stay inert for inner-block callee-saved locals (fourth confirmation).

## JALModSe closure 205 (2026-09-18): the 0x68 hole is positioned but unnamed

- **A contiguous dead block whose neighbours keep their absolute offsets names the expansion region that produced it.** In `JALSystem::append` the six `new`-result temps sit at 0x58..0x6c in both builds, so the missing 0x68 is one block allocated above them, during the four prologue `JALPrmSet`/`JADPrmS<f32>` constructions and before any switch arm; the `JALSeModDataGrp<T>` (6 x 8) and leaf-ctor (12 x 8) families the arithmetic allowed are refuted by position (they would move the arm temps).
- A dead uninitialised non-trivial class local in the prologue is +8 per *expansion*: `JALPrmSet`'s ctor is 3 expansions (0xe0), `JADPrmS<T>`'s 7 (0x100; the standalone `prm` plus two per `JALPrmSet`), and `JALPrmSet` at 16 plus `JADPrmS` at 8 lands 0x130 exactly, 100.0% with zero differing operands (`3 x 16 + 7 x 8 = 0x68`). Not committed: probe structs are padding; the open question is which real C++ in those two constructors reserves it. Refuted: `JADPrmS(const T&, const char*)` (+2 instructions), `JALPrmSet(const f32&, const f32&)` (one opcode), a defaulted `const char* name = nullptr` on `JADPrmS` (exactly +0: a level above the 8-byte out-of-class weak `JADPrm<T>::JADPrm` `bl`).
- Real bug: the `ModType_JALSeModPitFunk` arm passes `&set3` (retail `addi r8, r1, 0xd8`); all four `Pit` arms share the second pair. With it `append` is 371/371 instructions; the unit links the moment the hole is named.

## Closure re-passes 206 and 207 (2026-09-18): the one- and two-function game tiers under today's rules

- **Zero closures in 54 units.** Every residue is a recorded known-open class: the `a = b - c` allocation order (NpcCollision `bind`, wireBinder `bind`, temp exactly 0x18 low), the JGadget stride (PerformList, MirrorActor, seal, SDLModel, riccohook), zero-frame callee-saved rotations (MapObjTree, THPAudioDecode, BossHanachanAnm `setHeadAndBodyAnm`, MapObjPollution `loadAfter` with a 0x28 frame gap), volatile-register coalescing (NpcInbetween, MarioCheckCol `hitNormal`, CameraMultiPlayer's `fmr`), pinned-pool order at exact frames (MovieRumble, CameraDemo, MarioMain, MarNameRefGen_BossEnemy, SampleCtrlNode, ScreenUtil, MarioReceiveMsg), per-expansion temp grouping (beam `calcVertices`: retail groups two `scale` expansions by role, we by expansion), dead low region with no legal carrier (PollutionCount 64 B, MarDirectorLoadResource +24/+72 with every callee a real `bl`). Do not spend another closure pass on these tiers until a research result names one of the classes.
- **Naming a negated variable in only the earlier arm lands the later arm's volatile-FPR pair**: `f32 a = -param_1;` in `matan`'s `param_2 >= 0` arm while the other arm mutates its parameters in place (99.4 -> 99.85); naming it in the later arm, in both, or once at function scope all return to the original assignment; swapping the outer test's polarity costs 4.6.
- New negatives: the `x op= k` compound-assign lever lands a `fmuls` destination but swaps the two int-to-float conversion buffers (NpcInbetween 99.1 -> 97.5); block scope is inert for callee-saved GPR ranking as well as frame (`setHeadAndBodyAnm`, `body` hoisted alone or with `i`); a TU-local inline level at a call site is not codegen-neutral when the callee owns a pool temporary (`MarioMainStartTimer`, -3 instructions); an unnamed `TColor(r, g, b, a)` is +8 frame, `color.set(...)` +2 instructions.
- Research seed (batch 208): retail's named block in `TSunModel::perform` is `mtx` 0x78 / `dir` 0xa8 / temp 0xb4 against ours `dir` 0x80 / temp 0x8c / `mtx` 0x98, so the inlined `moveSun_`'s locals rank above the caller's own `Mtx`; the same question is open in CameraMultiPlayer (batch 151) and in the MapObjPollution/THPAudioDecode rotation pair.

## Research batch 208 (2026-09-18): an inlined callee's class-object locals outrank the caller's own

Measured in a scratch TU with the game flags (a caller with a 48-byte `Mtx` and an inlined member declaring two 12-byte class objects, one `bl` per use).
**Stack objects are grouped by the body they were written in, and the groups stack downward from the top of the named area in expansion order**: the caller's own named locals highest, then the first inlined expansion, then the second, and so on; inside a group addresses ascend with declaration order (later-declared higher), and the compiler's `0x4330` int-to-float temps sit above every group.
Inert, byte-for-byte: the caller's array at function scope declared first or after the call, in an inner block before or after the call; the callee's locals trivial or with a user ctor; the second callee spelled as a member or as a free `static inline` taking the receiver by pointer.
The only knob is **which body the object is declared in**: passing a caller-declared `Mtx` into the callee by pointer restores the caller-first order exactly. So when retail places what looks like a caller's array *below* an inlined callee's locals, that array is a local of a **second** inlined callee.

- `TSunModel::perform` is the worked case (97.48 -> 97.61, 40 markers -> 7, frame 0xe8 -> retail's 0xe0): the `CUE_CALC_ANIM` body becomes `inline void TSunModel::calcAnim_()` holding the `Mtx`, and the three camera subtractions bind `CPolarSubCamera* camera = SMSGetCamera();` once (-8 of low region) while the `scaleAdd` argument stays a fresh `gpCamera->` read, which retail reloads after `MsVECNormalize`. The recorded "`SunModelCalcAnim` level is byte-for-byte inert" was wrong and is withdrawn. Left: a constant 4 bytes at the top of `calcAnim_`'s block (`dir - mtx` 0x34 against 0x30), invariant under a 4- or 8-byte local before or after `mtx`, hoisting `calc()` out, a `J3DModel*` binder and `MTXCopy(mtx, getBaseTRMtx())`; the scratch TU with the same shape shows no such pad. Per-site `gpCamera->` is -4 at each subtraction operand and +0 at the `scaleAdd` argument.
- Re-classified: `TMapObjRevivalPollution::loadAfter` and `AudioDecoderForOnMemory` have no class-object local; both are batch 145's wholesale pool-block/local-block trade (retail ranks the named scalar above the `@ha/@l` address temps, `this` between the groups in `loadAfter`; `loadAfter`'s 40 dead bytes have no legal carrier since `registerPolluteTex` is emitted). `ctrlMultiPlayerCamera_`'s residue is a low-region ordering of a single carrier (`MsSqrtf`'s temp 0x28 retail vs 0x18 ours), which this rule does not reach.

## Research batch 210 (2026-09-19): a zero-frame callee-saved rotation is an inlined callee's `this`-vs-parameter rank; J3DModel linked

- `J3DModel::entryModelData`'s surviving pair (`dlSize` r25 / `J3DMatPacket*` r24 in retail against our r20/r21) is not a declaration-order problem: declaring the packet first, uninitialised, and assigning it after `countDLSize` is byte-for-byte inert (batch 171's FPR finding holds for GPRs). **Making both values arguments of one inlined call lands it**, because the explicit parameter outranks `this` (batch 144). The header already had the callee: `J3DDrawPacket::newDisplayList(u32)` in `J3DPacket.hpp` is retail's `stw 0x30`/`lwz 0x30` pair verbatim, so the else arm is `mMatPackets[i].newDisplayList(pModelData->getMaterialNodePointer(i)->countDLSize())`. **Before fabricating a TU-local level for an ordering, grep the class's own headers for a method with that body**: a fabricated `static inline X(packet, dlSize)` produces the identical stream and the identical +8.
- The +8 was paid by *removing* batch 202's own lever: with the arm going through the header inline, the `J3DShapePacket* shapePacket` binding is frame-neutral and is replaced by a binding of the **array base** (`J3DShapePacket* packets = mShapePackets;` then `&packets[shape->getIndex()]`), which is 0xe8 and also lands a volatile r4/r5 swap at the `addShapePacket` argument: **which operand of an indexed address is the named local decides which volatile register the chain is built in**. Worse, do not retry: folding `shape` into the binding (+8), a named `u16` index (+8), a named `shapePacket` on top of `packets` (+8), a named `J3DMaterial* mat` across the loop (28 markers), dropping `mat` in either NBT loop (+8/+32). The function's whole 0xb0 named region is dead (no `r1` reference in the body): its frame is pure inline-temp accounting.
- `entryModelData` 99.92 -> 100, J3DModel 88.56 -> 100 matched code, **linked (490th object)**.

## Research batch 209 (2026-09-19): a defaulted **class** argument is codegen-neutral and frame-positive

- **A defaulted argument of class type whose object is never read emits nothing and still reserves stack at every call site of the inline that declares it**: `align8(sizeof)` for the object plus the same again as an outgoing copy area once the class is too big for a register (`sizeof <= 4` is +8 per site, 8 is +16, 16 is +32). This refines header round 19's "a defaulted argument is a codegen-neutral level": neutral in instructions, not in frame. A live by-value class argument is never free (a `JADPrmName` wrapper carrying `JADPrmS`'s `const char*` is +24 instructions).
- In `JALSystem::append` one such argument on `JADPrmS<T>`'s ctor (7 sites x 8) plus an 8-byte one on `JALPrmSet`'s (3 sites x 16) is 0x68 exactly: frame 0x130, 100.0%, 371/371, zero differing operands, nothing else in the tree moves. **Not committed**: no clean source names a 4- or 8-byte JAudio debug type, and an empty class invented to hold the slot is a probe struct with extra steps. The unit links the day that type is named from evidence.
- Refuted there: by-value `JADPrmS` parameters on `JALPrmSet` (0x110, +27 instructions); copy-init of the four prologue objects from same-type temporaries (0xe0, +14); forwarding `JALPrmSet`'s defaulted objects to its members instead of defaulting again (0x118, wrong total). Sibling: `MSSetSoundTL`'s ctor is missing the same 0x68 with seventeen `JADPrm` constructions, so "8 bytes per `JADPrmS`" does not generalise; either independent, or both bodies declare one dead object of the same unnamed type.

## Research batch 211 (2026-09-19): the JGadget pool stride is the container receiver's naming at the call site

- **Not a level at all**: whether the receiver of `getChildren().push_back(x)` is a *named local pointer* or an unnamed expression. A named receiver packs every expansion's base-iterator temps into one contiguous run and parks the dead words at the top of the block; an unnamed receiver (a `TNameRefGen::search<T>(...)` result consumed in place, a member read) pads each expansion's sub-block by one word. `TIdxGroupObj* group = JDrama::TNameRefGen::search<TIdxGroupObj>(...); group->getChildren().push_back(this);` takes `TLampTrapIron::loadAfter` and `TLampTrapSpike::loadAfter` from `0x78 [2]+4 [3]+4 [4]` to retail's `0x78 [5]+8 [4]`, both exact.
- The knob is one step out from the list: naming the `TList_pointer<T>&` or `*` packs the pool too but drops the `push_back` block 4 bytes (pointer/reference ladder inert, confirming 161); only naming the *searched object* keeps both. A TU-local helper holding search plus push_back out-lines the chain (0x50); an empty forwarder loses a temp; naming `this` adds a slot.
- **The sign is confirmed in reverse by `TSelectDir::rsetup`**: our seven named receivers pack `[48]` contiguous against retail's four padded `[5]+4` groups, so retail's `root` pushes went through an unnamed expression there (the member `TDirector::unk10` is a `TViewObj*`, not the list type: the open lead is which expression).
- Oversteers by one level where the site already wraps `push_back` (`TIdxGroupObj::add` in `TEnemyManager::createEnemies`: the pool collapses to `[8]`); wrong dial where the pool's shape already matches and the residue is missing dead bytes under it (`TRiccoHook::init` 8 above the pool plus a 4-taller named region; `TMirrorActor::init` a low-region shortfall). `TSeal::init` with the named receiver and `->push_back(this)` gets the first gap right (one slot and 2 markers left) but loses 0.1 fuzzy to its independent r30/r31 rotation: pair it with a 4-byte lever above the pool. `SDLModel::entry` and `TPerformList::perform` not reached.

## Rotation-rule application 212 (2026-09-19): the six-rung callee-saved ladder

- **Full ladder, measured on `TBossHanachanAnm::setHeadAndBodyAnm`**: inlined-callee parameters > caller locals > caller parameters > `this` > pool/base temps > caller inner-block locals. Moving the loop body into an inlined member makes `body` an inlined callee's parameter and it jumps r25 -> r31 with everything below exactly retail's; retail's `body` (r30, one rung under `i`) sits in the *caller locals* bucket, which an inner-block local never reaches and which hoisting to function scope does not reach either (batch 205 reconfirmed). Helper parameter order is inert.
- **An inlined member call costs +8 of frame per parameter** (0x68 -> 0x88 with four), so the rotation rule is priced out wherever the frame is already exact and no named pointer local can be dropped to pay it.
- The rule needs a genuine receiver/argument pair: a dead-range coalescing tie (`TMapObjTree::initMapObj`: the leaf count dies before both format-string bases exist, both assignments legal), a local-vs-stack-address pair (`copyAnmMtx`: retail's first named local is a 4-byte one declared before the matrix, the 0x64/0x68 hole), and a C TU (`AudioDecoderForOnMemory`: a `static inline` helper folds and MWCC regenerates the global-address temp) are outside its reach. `JPAEmitterManager::createEmitterBase`'s two TU-local binding levels are not the cause of its r29/r30 swap (direct calls, frame -0x10, same swap): landing it needs `&unkA4[param_3]` as the explicit parameter of a call whose receiver is `linkInfo`, and no such call exists.
## Research batch 214 (2026-09-19): retail's (4, 12) is reachable but the map forbids its only carrier

Batch 142's driver rebuilt in this batch's scratch (`drv.py`, `x1`-`x7`), reproducing the stock `(16, 0)` and its `(4, 4)` shape byte for byte before anything was changed.
Geometry stays **(below, above)** = pool bytes under and over the live `bl sub` temp, frame 0x40 / temp 0x10 / named 0x28 / 55 instructions being retail.

| spelling (on top of batch 142's `(4, 4)` operator- unless stated) | below | above | instructions |
| --- | --- | --- | --- |
| stock header | 16 | 0 | 55 |
| batch 142's `const TVec3&` `operator-` + `void subV` | 4 | 4 | 55 |
| + defaulted class argument on the **copy constructor** (empty class, or the header's real `TUtil<f32>`) | 4 | **12** | 55 |
| + the same with an 8-byte class | 4 | 20 | 55 |
| + the same with `Vec` | 4 | 28 | 55 |
| + defaulted `int` / `f32` argument on the copy constructor | 4 | 4 | 55 |
| defaulted class argument on `sub` | 12 | 4 | 58 |
| `TVec3 nextPos(mPosition.x, mPosition.y, mPosition.z)` (the real 3-argument constructor) | 4 | 8 | 55 |
| `operator-` as `TVec3 r; r.sub(fst, snd); return r;` (the real two-argument `sub`) | — | — | 51, no `bl` |

- **The exact geometry exists and is worth exactly what batch 142 priced it at.** The copy-constructor row is retail's slot map, and tree-wide it is the predicted **+9 functions to 100** (`TEnemyAttachment::bind`, `TChuuHana::bind` and `attackToMario`, `TAmiNoko::bind`, `TKoopaJrSubmarine::bind`, `TCoasterEnemy::bind`, `TLimitKoopa::bind`, `TLiveActor::bind`, plus `TNerveSmallEnemyJump::execute` where 142 listed `TLeanMirror::loadAfter`) against **90 off 100**.
- **The carrier is refuted by the map, not by taste.** MWCC mangles a defaulted parameter into the signature, and the map fixes the arity of every candidate: `__ct__Q29JGeometry8TVec3<f>FRCQ29JGeometry8TVec3<f>` (linked, weak, MarioCollision.cpp) and `sub__Q29JGeometry8TVec3<f>FRCQ29JGeometry8TVec3<f>` each take one argument, and there is no `subV` in the map at all. Applied, the copy-constructor argument takes the linked `__ct__` weak body from 100% to MISSING. `operator-`, `operator-=` and `operator=` cannot carry one at all — C++ fixes an operator's arity and MWCC rejects the extra parameter outright, so "which of those is expanded at the 130 `bl sub` sites" never becomes a question.
- **Decomposed, the two halves price very differently.** The copy-constructor argument **alone** is +1/-89 (it is the 89, and needs the reshaped `operator-` to pay for any of the nine). The `operator-` reshape **alone** is +0/-2 and otherwise inert tree-wide — its whole cost is dropping the weak `__ami__` copy to MISSING. So batch 142's "(4, 4) half" is nearly free, and the one thing blocking it is a single weak emission.
- **That blocker is a concrete lead, not a wall.** Retail calls `__ami__` out of line **twice inside `TYoshiTongue::movement`** (`build/GMSE01/asm/Player/Tongue.s` lines 582 and 726, the weak body at 0x80268168), so retail's `operator-=` is genuinely inline-refused there independently of `operator-`. Our build only gets the symbol for free because our by-value `operator-` routes through it. Restore those two sites in Tongue.cpp (82.27%) and the `(4, 4)` half becomes committable on its own.
- **Research 211's naming knob does not reach this class.** Eleven caller spellings measured on both the stock and the `(4, 4)` header — `V n = m`, `V n(m)`, declare-then-assign, `n.set(m)`, a named `const V&` or `const V*` operand at either or both ends, a named `this`, a named result — are **byte-identical slot maps** in every case. Only an accessor moves anything, and it moves `below`: a `const V&` `getPosition()` at both ends is `(16, 8)`, a by-value one costs +6 instructions. This confirms and widens batch 142's "the whole caller side is inert".
- **The `above` ladder, for whoever picks this up.** Base 4; the real 3-argument constructor on `nextPos` is +4 (the only legitimate, instruction-neutral rung found, and it is per site, so it costs nothing outside the nine `.cpp`s); a dead binding after the subtraction is +4 (padding); the defaulted class argument is +8 (refuted). Nothing measured lands the last +4 on top of the 3-argument constructor except one dead binding, so the shape is now **one** piece of padding away from retail instead of batch 142's two — still padding, still not committable.

## JGadget application 213 (2026-09-19): MapObjTrap linked; the +4 rung is never alone

- **Two same-typed pointer variables in sibling switch arms, the inner shadowing the outer, cost one callee-saved register**: retail's `addi rD, r3, 0` then `mr rD, r3` on the *same* register is one variable declared then reassigned (`TLampTrapSpike::control` 99.5 -> 100, `stmw r27` for our `stmw r26`).
- **A named pointer local whose only use is one call argument can be +8 of dead frame at zero instruction cost** when its initialiser is an inlined accessor over a real `bl` (`MtxPtr mtx = getModel()->getBaseTRMtx();` ahead of `emitAndBindToMtxPtr`, `TLampTrapIron::receiveMessage` 99.9 -> 100): a cheaper and more honest +8 than a fabricated binder; try it before a TU-local `static inline`. The "pointer local that only feeds a call costs an `mr`" rule is about locals initialised from a plain load. **MapObjTrap linked (491st object).**
- `TNerveRHGraphWander::execute` 99.8 -> 100: pool base 0xa0 vs 0x9c, one dead 4-byte inlined-accessor temp too many; any one of its three accessor reads spelled as the raw member lands it (ambiguity recorded).
- **The +4 low-region rung of a member-read binder is +4 bottom and +4 top (+8 frame), never +4 alone** (`SDLModel::entry`, five configurations: a binder over `mSdlModelData` is +8 bottom/+0 top/+8 frame per site; a direct-return fork nested in it lands retail's groups but overshoots to 0xc0; turning the flag accessor into a fork pays the 8 back but re-opens gap 2, which is 4 only while both flag sites go through a binding level). The residue is one word that must move from the top region to gap 2 at constant frame, and no binder rung does that.
- `TSeal::init`: the named receiver plus `group->push_back(this)` makes the whole slot map byte-exact (43 -> 41 markers) but the independent r30/r31 rotation costs 0.1 fuzzy; named `u8 maxHp`, a named `TMapCollisionManager*`, a split `mScaling.x` and a named spine do not turn it. `TRiccoHook::init`: a binder over `getSaveLoadParam()` or `mSpine` lands the frame but leaves the +4 spill; `THookParams* prm = getSaveLoadParam();` is -8. `TMirrorActor::init` needs +16/+8/+0 under the pool (not a translation); `createEnemies` with `add` removed breaks the body (90.2).

## Closure 216 (2026-09-19): seal's rotation lever and PerformList's header-only grouping

- `TSeal::init` with the named receiver has a byte-exact `push_back` pool (accepted -0.08 fuzzy, 43 -> 41 markers); two residues: the r30/r31 rotation and one 4-byte hole below the inlined `setUpUnk8TRS` Mtx. **The rotation has a measured lever**: one live caller-local pointer in the `createMActor` window (`MActor* actor = createMActor(...); mMActor = actor; actor->offMakeDL();`) puts the `.rodata` base in r31 and the `this`/group chain in r30 as retail does (6 markers, 99.3), but retail reloads `mMActor` (`stw`/`lwz 0x74`) and the local elides it; the same trick on the keeper or the collision manager does nothing, so retail's fourth callee-saved value is a local elsewhere in the body. `mSpine->`/`getMaxHitPoints()` written out each remove 4 under the Mtx *and* 4 at the top: the Mtx hole is an allocation-order difference in a pinned low region.
- `TPerformList::perform`: research 211's receiver knob does not reach this pool (every caller spelling moves only the frame: a named `TSingleLinkList*`/`&` -0x10, a named `this` +8, `++it` -0x10, a `while` byte-identical, a named `end` +2 instructions). The only construct producing retail's grouping (`[4]+12 [5]+4 [2]+12 [2]` at 0xe8) is research 161's named result inside `TSingleNodeLinkList::begin()/end()`, which drops both `TPerformList::push_back` overloads: a header-round item with the lever-removal rule, not a unit item.

## Research batch 215 (2026-09-19): the `ptr + index` add order is the statement form, not the index spelling

- **A compound assignment `p += index` emits `add rD, rPtr, rIndex`; every single-expression address emits `add rD, rIndex, rPtr`.** Measured over ~60 spellings in a scratch TU reproducing `J3DSkinDeform::initMtxIndexArray`'s loop nest. Within one expression the constant decides the *instruction* order: a constant inside the index (`&dl[3+p]`, `dl+(p+3)`) is `add` then `addi 3`; a constant outside (`dl+p+3`, `&(dl+p)[3]`, `&dl[p]+3`, `((Hdr*)(dl+p))->data`, a `return p+n` helper) is `addi 3` then `add` and costs an extra `mr` wherever the product lives in r0. Supersedes closure 202's "no index spelling turns it round": the knob was never in the index.
- `u8* base = dl; base += vtxSize * k; u8* vtx = base + 3;` gives retail's exact pair at 247 instructions and an exact frame, but retail builds the chain in one register (r4/r4) and a two-object chain takes r3 then r4 (2 differing operands against the single-expression form's 1; source kept as the latter). Inert for the register: declaration order and scope of both locals, a TU-local `p += n; return p;` binder, `(base += n) + 3`, the comma operator; `(vtx += n) += 3` spills; any one-object two-step form sinks the `+3` into the load displacements (+2 instructions).
- `J3DDeformer::deform`'s residue is **one** missing word of low inline-temp pool below `pos[3]`, not two gaps: every named slot is 4 low and the `lfd`-aligned u16-to-f32 pair rounds that to the 8-byte frame gap (a first-declared `trash[4]` is +8 and fills it from the wrong end). Candidate: a +4 rung (a direct-return fork over a plain member read nested in a binder) on one of the raw `cluster->`/`vertex->` reads.

## Header round 38 (2026-09-19): the JGadget base pair is priced per half

- `TSingleNodeLinkList::begin()` and `end()` are independent levers: `TPerformList::push_back(TViewObj*, u32)`/`(const char*, u32)` expand only `end()`, so naming `begin()`'s result is free for them while naming `end()`'s adds one iterator temp and a second `bl __ct__(const iterator&)` to `Push_back`'s chain (100 -> 92.5/94.2); `perform` needs both named to reach retail's 0xe8. Eleven derived-layer repairs do not remove the extra temp: a named `end` in `Push_back` (93.7, frame 0x58 right, seven temps), a directly constructed iterator (63.5), derived `end()` named (85.5, perform 0x100), `Insert` by non-const ref (98.1) or const ref (93.5) both at 0x50, `Insert` without the base-ref cast (byte-identical to stock), `Insert` returning `void` (92.8, 0x58 but seven contiguous), `Push_back` calling the base `Insert` directly (68.3), a derived `Insert` on the base iterator (73.1), a named `TSingleLinkListNode**` (95.3), `const` on the named result (inert). Rejected, nothing committed.
- **Correction to closure 216**: research 211's receiver knob does reach `TPerformList::perform`. A named `TSingleLinkList<TPerformLink,0>*` receiver merges our `[2]+4 [3]` into retail's contiguous `[5]+12` in every header configuration; with both base halves named the slot map is `0xd8 [2]+8 [2]+4 [5]+12` against retail's `0xe8 [2]+12 [2]+4 [5]+12`: one word of gap-1 (between the derived and base `operator!=`/`==` by-value parameter pairs) and 0x10 of frame from exact, gap-2 pinned. Base-pair naming steps gap-1 4 -> 8; a named derived `begin()` steps gap-2 instead (0xf0); deleting the derived `operator==` collapses the pool. The named receiver alone costs instructions (97.2): a probe, not a spelling. `singlelinklist.hpp` is included only through `PerformList.hpp` (MarDirector*, MarNameRefGen, PerformList), so its blast radius is small.

## Closure 218 (2026-09-19): deform's word is an accessor temp; rsetup's pad is upstream of the call site

- `J3DDeformer::deform` 99.68 -> 99.76, frame exact: the missing word is one 4-byte inline-accessor temp at the bottom of the dead low pool (the `lfd`-aligned save area rounds +4 to +8). Routing any single raw `cluster->` read through an in-class accessor lands it (`getNrmNum()`, `getPosNum()`, `getClusterVertexNum()`, `getFlags()` each alone: 0x118, 60 -> 18 markers; `getClusterVertex(i)` 37; two reads 40; the angle accessors overshoot to 0x120/0x128). Four members are byte-identical choices each read once tree-wide: the pick is undecidable, recorded. Residue: 18 volatile-register markers in the normal loop plus a swapped emission order of the two sign-index `rlwinm`s.
- `initMtxIndexArray`: the `+3` sink is a **use** property, not a statement form: the same function's outer step `dl += vtxSize*vtxCount; dl += 3;` already compiles to retail's `add rD, rPtr, rIndex; addi rD, rD, 3` because `dl` has non-indexed uses. `vtx += vtxSize*k + 3`, `vtx += 3 + vtxSize*k`, `(3+vtxSize*k)+dl` all canonicalise to index-first; `vtx[3+off]` x3 and `vtx += 3` are +2; `dl+3` before the compound +1. The two-object form has retail's operand order and only the intermediate register wrong (r3 vs r4): a volatile-register knob.
- `TSelectDir::rsetup` 99.6: the 0x38 is three pieces (+28 below the first pool slot, +20 of in-pool pad over the first five expansions only, +8 above the pool where a second temp block alternates 0x10/0x14 groups in retail). Retail's receiver at the `root` sites **is** a named local: every unnamed form (a per-site cast of `unk10`, a TU-local binder returning the cast, `insert()` at four or all sites, `root` reloaded after the store) costs instructions and keeps the pool `[48]`. Research 211's knob is the wrong dial here; the pad's cause is upstream of the call site. `direct()`'s carrier ladder is exhausted in its TODO.

## Closure 219 (2026-09-19): the global-fork rung lands an off-grid frame; binder price tracks object size

- **The global-fork rung (+4 per read) is the lever for a function sitting 4 off an 8-byte binder grid**: `TWalkerEnemy::behaveToFindMario` (99.89 -> 100) with a TU-local `static inline THitActor* WalkerEnemyMario() { return (THitActor*)gpMarioAddress; }` feeding `setGoalPath`; `setGoalPathMario()` and the spelled-out cast bracket retail (0x2c/0x50 vs 0x30/0x48 against 0x34/0x50). Try it wherever those two bracket retail.
- **A binder over a large class member is priced by the bound object's size, not its return type**: on `TDrawSyncManager::mCallbacks` (`JGadget::TVector`, 0x18) a reference binder, a pointer binder, a direct-return fork and a member accessor are all +16 (`setCallback` 99.62, retail = the named-temporary form plus one 8-byte object below it).
- **Ladder application**: `TDrawSyncManager::start`'s five-way rotation is one value moving rungs: MWCC coalesces the inlined `TFifo` ctor's `capacity` parameter with the caller's identically valued `param_2`, demoting it from the inlined-callee-parameters rung to the caller-parameters rung; retail keeps them distinct. A TU-local `MakeFifo(int)` level is worse (28 markers); the ctor with assignments instead of an initialiser list is inert. What keeps the two distinct closes the function.
- `getSpine()` on an inlined `pushAfterCurrent` swaps the nerve/spine volatile registers (r4/r5): not a frame lever at inlined push sites. A named class local inside an inlined callee (`polarXZ`'s `TVec3 v`) is +16 frame but costs a real copy (-5.5). `setSeDistanceVolume`: naming the category index, or an anonymous shift (re-CSEd), inert.
## Research batch 220 (2026-09-19): the volatile-GPR chain is an SSA-value count, and the `+ const` sink is block-local

Two seeds in `J3DCluster`, both reduced to one mechanism: **retail keeps an address chain in a single volatile register where we spend a fresh one per step**, and the lever for that is not a spelling but how many SSA values the statement form creates.

- **The `+ 3` sink in `J3DSkinDeform::initMtxIndexArray` is MWCC's address flattening, and it is *basic-block local*.**
Inside one block, every `p[reg]` use of a pointer whose definition is a visible chain of adds is rewritten to `add rX, base, reg` plus a displacement load, so a separate `addi 3` survives only where the flattener cannot see the definition.
Measured in a scratch TU carrying the loop nest (~40 spellings on top of research 215's ~60): the sink is insensitive to the **number** of uses (1, 2 and 3 indexed uses all sink), to an extra non-address use (`f(vtx)` sinks *and* pays its own `addi`), to a constant-0 index, to `++vtx` x3, `vtx = vtx + 3`, `vtx += sizeof(T)`, a `u8**` or `u8*&` helper, `do { } while (0)`, and a later `vtx - dl` use.
It **is** blocked by retyping (`(Hdr3*)vtx + 1`), by `u32` arithmetic, and by a block boundary.
- **A dead-on-arrival second value always takes the lowest free volatile; the long-lived one takes the first register free across its whole range.**
`u8* vtx = dl; vtx += vtxSize * k;` on its own is `add r4, r25, r0` — retail's operand order *and* retail's register, because the single value is live across the `mr r3` of the upcoming call.
Every form that keeps the `+ 3` alive introduces a second value, and that value dies in one instruction, so it takes r3: the two-object form, the `Hdr3` retype, `u32` arithmetic, an inlined `p += off; return p + 3;` helper, `register`, and both declaration scopes are all r3/r4 (2 markers) — worse than the stock single expression's one reversed `add`.
- **The only construct that reproduces retail exactly is a phi.** `vtx += vtxSize * k; if (c) vtx += 3;` gives `add r4, dl, prod; addi r4, r4, 3` byte for byte: the flattener stops at the block edge and the merge forces both defs into one register. Retail's stream has no branch there, so the function is **unreachable from honest source** and stays at its single marker. Closed.
- **`J3DDeformer::deform`'s 18 markers are the same phenomenon and the stock body is a local optimum.** Retail chains `add r7, r27, r7` -> `lwz r7, 4(r7)` -> `clrlslwi r7` in one register; we spend r7/r9/r6. The `rlwinm 20`/`rlwinm 21` pair is a pure **schedule** swap, not an allocation one: both sides put the y index in r7 and the z index in r6, only the two slots are exchanged. Worse, do not retry: reusing `flag` for the `src` index (20 markers +2 opcode), a named `u16* posFlag` (20/2), dropping the named `clusterKey` in the position loop (17 but 2 opcode diffs — retail's `add; lwz 4` becomes `addi 4; lwzx`, so the named local is right) or in the normal loop (18/2), swapping the declaration order of `flag` and `src` (21, so the stock order is the better one), `sign[(flag >> 15) & 1]` (27/4/5/5), naming the three shift results or the three `sign[]` values (41 each), three `f32` locals in place of the `Vec` (41), `posDstIdx[i]` in the `dst` expression (52), `cluster->mPosDstIdx[i]` in the zeroing loop (91).
- **J3DCluster does not link**: `initMtxIndexArray` is closed as unreachable at 1 marker and `deform` is exhausted at 18, so the unit stays at two non-exact functions.
