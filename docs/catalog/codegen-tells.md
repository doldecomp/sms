# Codegen tells: reading source shape from the asm

Rules measured in this clone, each tied to a function where it was verified.
General MWCC guidance is in `docs/AGENT_MATCHING_TIPS.md`; this file does not repeat it.

## Booleans and predicates

**Return shapes.** From 30 near-identical 48-byte accessors in `tobiPuku`:

| Source | Assembly tail |
| --- | --- |
| `BOOL f() { return field == N; }` | `subfic`/`cntlzw`/`extrwi`, no branch |
| `BOOL f() { return helper(); }`, helper returns `bool` | bool materialised, then `clrlwi r3,r0,24` |
| `BOOL f() { return helper() ? TRUE : FALSE; }` | bool materialised, `clrlwi.` **and a second branch** to `li r3,1` / `li r3,0` |
| `bool f() { return helper() ? true : false; }` | branches straight to `li r3,1; blr` / `li r3,0; blr` |

`TSmallEnemy::isBckAnm(int)` is the helper those accessors call; writing `mCurrentBckAnm == N` inline never matches.
Get one accessor exact, then apply the shape to the rest.

**Materialised bool = inlined bool-returning helper.**
`li r0,1` / `b` / `li r0,0` / `clrlwi. r0,r0,24` / branch means the condition came from an inlined function returning `bool`, not from the call site.
Check the TU's UNUSED list (`isRoll__9TTobiPukuFv` in `tobiPuku`; no inline spelling of the same condition reproduced it).
It reads both ways:
- Materialised in the target, direct branch in ours: call the predicate. Verified with `TBGCheckData::isDeathPlane/isPool/isWaterSurface`, `TLiveActor::isAirborne` (over plain `checkLiveFlag`), `THitActor::isActorType`.
- Direct branch in the target, materialised in ours: the original compared inline. `TSeal::receiveMessage` tests `mActorType == 0x01000001` directly while `TSeal::perform` uses `isActorType`; boss `perform`'s three collision loops also compare directly. Check each site.

The helper body shape matters, and the map size checks it:
- `isRoll` as three early returns is 0x150 (map 0x140) and inlines at 89.7%; as one `a || b || c` it is 0xd4 and drops the caller to 57%.
- `TChuuHana::isRolling` as `return a == b;` is branchless 0x84 and folds to a bare compare when inlined; as `if (a == b) return true; return false;` it is the map's 0x8c and materialises at every call site.

**Normalisation groups.**
- `(checkHitFlag(0x80000000) ? true : false)` restores a group a direct condition lacks (boss head/body collision transitions).
- `(a && b) == false` preserves groups that `!` removes (`isAllBckAlreadyEnd`); the same trick regresses the parts dispatcher.
- `ground && (ground->isIllegalData() == true ? false : true)` reproduces both conversions (boss `bind`, sphere chain).
- `(completed || looped) || frame + 0.1f >= end` assigned to a `bool` local matches; `BOOL` adds normalisation.
- Explicit `if`/`else` assigning a bool reproduced TMario's mushroom guard; direct assignment hoists a zero, a ternary adds a merge.

**Type tells.**
- `cmpwi r0,0` after a materialised bool: the value went through an `int`/`BOOL` local. `clrlwi.`: `bool`.
- `clrlwi.` on a byte flag register (`clrlwi. r0,r29,24`): the local was `bool` (MarioSpecial `noHold`).
- A caller masking a getter result with `clrlwi.` means the getter returns `u8`/`bool`; comparing the register directly means `int` (`TBathtub::getNumKillerLaunchable/Burstable`).
- A caller's result handling resolves a callee's return type when callee streams are indistinguishable (boss `setAnm_` is `bool`, proven by `clrlwi.` at the owner's call).
- `cmpw` vs `cmplw` on a counter: `s32` vs `u32`.
- `divw` with an `xoris` bias: signed integer division converted to float afterwards, not a float divide.
- A redundant `clrlwi` after `lbz`: the value passed through a `u8`.

## Control flow

**`||` vs separate `if`s.** For `if (a || b) return;` the first operand branches directly and the last is unfused:

```
    <test a>
    bne  RET
    <test b>
    beq  CONT
    b    RET
CONT:
```

Two separate `if (...) return;` fuse both (`bne RET` twice).
`TTobiPukuLaunchPad::perform` wanted separate ifs, `TTobiPuku::kill` wanted `||`.
If you see the redundant `beq +8; b end`, join the conditions.

**`else if` vs a following `if`.** `if (a) { if (b) {...} } if (c)` and `... else if (c)` differ only in the `beq` target after `b`'s test.
The diff shows a single `~ beq {target}`; compare branch targets, not just opcodes (`TNerveChuuHanaStick` needed `else if`).

**Switches.** Decode case *destinations* and branch intervals, not just compared constants.
The comparison tree's pivot tells you the case set: with cases {0,1,2}+default MWCC pivots on 1 and adds a `cmpwi r0,3`; with {0,1,2,3}+default it pivots on 2 and folds the high side into one compare. A `default` that skips the code after the switch is `case 3: default: goto <after>;` (`fruitsboat`, three switches); a `const char*` local with `default: anm = nullptr` leaves a `li r0,0` and a null test.
Moving one case ID gives the wrong comparison tree; moving the whole original interval restores tree and registers (TMario `checkCollision`, `receiveMessage`).

**Loops.**
- An index loop over a fixed-size member array unrolls (`getUnusedSeed` 344 bytes vs map 48); a pointer loop does not, and a separate `T** end = &mSeeds[16];` before the loop makes MWCC compute the end before the begin and keep `this` in the lower register (`TYumbo::perform` 93.1 -> 100).
- Inside an `&&` chain, `fcmpo` + bare `bge`/`ble` wants `!(a < b)` / `!(a > b)`; `a >= b` and `a <= b` always add `cror` (`setGroundCollision` 94.8 -> exact).
- A surviving multiply by literal `1.0f` proves an inline boundary: a TU-local `MsGetVecFromRotY(rot, len)` called with 1.0 and 300.0 reproduces both sites (`fruitsboat`), but it must call `JMASSin(DEG2SHORTANGLE(x))`, not `JMASin(x)`, whose extra wrapper level pushes the table lookup out of line in the bigger caller.
- `&mMemberVec` instead of the `operator Vec*()` conversion fixes argument-setup order (`MTXRotAxisRad(roll, &mRollAxis, ...)` set `r3` before `r4`).
- `fcmpo` + bare `bge` means the source tested the other way: `if (x < limit) { ...; } return false;`, whereas `if (x >= limit) return false;` gives `cror eq,gt,eq` + `bne`.
- Timer loops over a reversed order: ascending `i = 0..7` with `mBodies[7 - i]` times `i` matches; counting down leaves runtime arithmetic after unrolling (boss get-up timers).
- `fabs(mBodies[i]->mRotation.z)` compared directly, then assigned to the member, fully unrolls; a local `angle` breaks it (`getBodyMaxRotateZ`).
- An `s32` counter with a signed bound keeps the initial branch before sixteen unrolled stores (Strategy constructor).

## Inlining

**Depth limit: the per-expansion size allowance shrinks with depth.** Measured in a scratch TU with the game flags, varying only callee size and wrapper levels:

| body size | depth 2 | depth 3 | depth 4 |
| --- | --- | --- | --- |
| 0x5c / 0x6c | inline | inline | **call** |
| 0x74 / 0x84 | inline | **call** | call |
| 0x8c | **call** | call | call |

Very small bodies expand through four levels and are called at the fifth. Declaration form (`inline`, `extern inline`, in-class, statement count) and caller size make no difference (a 0x7bf8 caller still inlines a 0x5c body at depth 1; eighty `normalize()` expansions in one function all inline).
That is why `TUtil<f32>::inv_sqrt` (under `normalize` -> `setLength`) is always called while `sqrt` under `length()` expands, and why `std::fmodf`/`TUtil<f32>::mod` (0x5c) are calls in the ROM: their sites reach them at depth 4, ours at depth 2 (`nerve -> faceTo -> fmodf`), so two inline wrappers are missing above them. Every ROM site computes `l + std::fmodf((r - l) + (t - l), r - l)`, a wrap-into-`[l, r)` helper distinct from the loop-based `MsWrap<f>`; recovering it is `.cpp` work in `koopajr`, `Koopa`, `BathtubPeach`, `MapObjCorona`, `wireTrap`. The header keeps the `::fmod` wrapper. This likely also explains the per-call-site tables from `MapObjBall` and `amiNoko` below.

**Depth is measured from the innermost expression, not the statement.** An inline call nested inside another inline's argument sits one level deeper: `q.setEulerY(-(k * MsGetRotFromZaxisY(dir)))` pushes `MsGetRotFromZaxisY` past its limit and emits a weak out-of-line copy the retail object lacks; hoisting it into `f32 yawRad = ...; q.setEulerY(yawRad);` restores the expansion. One-line forwarders count too: `q.rotate(dir)` forwards to `rotate(dir, dir)`, so call the two-argument form directly (`TYumbo::shotSeeds` 66 -> 95.7). Symptom: a weak symbol in your object for a header inline the map shows nowhere in the TU. `MsGetRotFromZaxisY` expands exactly one level, so a caller cannot reach it through any wrapper (`TYumbo::lookatMario`, UNUSED and size-exact, is not what the nerves call; they spell its body out).

**Per-callee budget counts statements.** `TChuuHana::rolling` (UNUSED 0x120) is inlined in the original.
Same-size reconstructions with one more statement are refused: a dead `d.y = 0.0f;` flips it to a call, an empty `;` does not, and replacing two statements with `d.sub(v)` flips it back despite more code.
When a size-matched UNUSED function is called instead of inlined, look for a cheaper spelling of its body.

**Per-call-site differences (open).** In `MapObjBall` the original inlines differently from us *per call site*:

| Callee | Original | Ours |
| --- | --- | --- |
| `TUtil<f32>::sqrt` in `calcCurrentMtx`, `TBigWatermelon::touchActor` | inlined | inlined |
| same `sqrt` via `length()` in `hold`, `touchGround`, `touchWall` | **called** (weak from `boid.cpp`) | inlined |
| `TMapObjBall::control` from `TResetFruit::control` LIVING/HOLDING | **called** | inlined |
| `TResetFruit::touchActor` from `control`'s loop and `receiveMessage` | inlined | **called** |

The map lists exactly which `JGeometry::TVec3<f>` members exist as weak symbols (`add`, `sub`, `dot`, `div`, `negate`, `scale`, `scaleAdd`, `setLength`, `setMax`, `setMin`, `set(const Vec&)`, three operators, constructors; `TUtil<f>` `sqrt`, `inv_sqrt`, `mod`, `one`) and never `length`, `squared`, `normalize`, `isZero` or `set(x,y,z)`.
Our build emits weak `TVec3::sub` in 35 objects; the original in one (`Animal/BeeHive.o`), yet calls it out of line from `tobiPuku`, `chuuhana` and others.
Moving those definitions out of the class body with `inline` changed nothing.
Do not force these with `#pragma dont_inline` or pasted bodies; leave the functions at their current scores.

**`fmodf` / `mod`.** The binary calls `std::fmodf` and `TUtil<f32>::mod` out of line everywhere (`koopajr`, `MapObjCorona`, `wireTrap`), with the same 0x5c body: return `x` if `|y| > |x|`, else `x - y * (f32)(s64)(u64)(x / y)`.
`TUtil<f32>::mod` carries that body in `JGUtil.hpp`; `std::fmodf` keeps the `::fmod` wrapper because the body there inlines everywhere.

**Big candidates have a statement budget that shrinks with depth too.** `TKazekun::getAroundQuat` (0x3d8) expands at depth 1 but not at depth 2 until its statement count drops; one `setRotate(axis, angle)` in place of a named half-angle plus `scale`/`cos` flipped it (Attack nerve 68.1 -> 98.8). It has to sit behind the UNUSED `doAttack` to be at depth 2 at all, where its leaves (`TVec3::set<f>`, `TVec4::set<f>`, `scale`, `dot`, `TUtil::sqrt`) become the ROM's out-of-line calls; spelling the nerve out inlines everything (630 instructions vs 352). Statement count also decides whether the *caller* inlines: `flyAroundMario` inlines into the Turn nerve at 13 statements and not at 14 (split `TVec3 vel(0, 0, 1)` into declaration plus `set`).

**Which quaternion overload:** the second source term becomes the standalone `fmuls`, so `mul(other)` gives `x * o.w` and `mul(quat, other)` gives `w * o.x`. `TVec3::setLength(f32)` (the one-argument forwarder) is the level that puts `dot`/`inv_sqrt`/`scale` out of line where `setLength(v, len)` keeps them inlined. A stray `li r3, 0` mid-function is a `bool` return value (`doAttackPose`).

**Trivial constructors and one-line wrappers buy a level.** Declaring a scratch matrix as `TPosition3<TMatrix34<SMatrix34C<f32>>>` rather than `TRotation3<...>` makes MWCC call the 4-byte empty `SMatrix34C<f>::SMatrix34C()` and `TRotation3::identity33()` out of line, exactly as the ROM does; a weak 4-byte constructor for the innermost matrix class in the map is the tell (`BeeHive` `receiveMessage` 88.5 -> 92.4). `setSQ` (0x100, 21 statements) inlines at depth 1 however it is spelled; through a one-line `setSQT(scale, quat, trans)` it becomes the ROM's call. And an UNUSED one-liner can exist to *be* the call: routing `TBee::receiveMessage` through `TBee::behaveToEat` (just `mBeeHive->receiveMessageFromChild(this)`) kept the callee out of line, 0 -> 99.8; same for `setShakePower` and `SMS_EmitWaterHitParticleAndSound`.

**Mutating operators are depth spacers.** `TVec3<f32>::scale(f32)` is called out of line throughout `wireTrap`; `v.scale(k)` expands it, `v *= k` routes through the one-line `operator*=` forwarder and adds the level that keeps `scale` a `bl` (three nerves 66/81/37 -> 99.0/99.5/98.4). Generalises the forwarder rule to the mutating `TVec3` operators.

**Ternaries cut the statement budget without changing codegen.** `f32 r; if (t > 0) r = 1 + f(); else r = 1;` and `f32 r = t > 0 ? 1 + f() : 1;` emit the same instructions, but the ternary made an UNUSED helper 48 bytes smaller and flipped it from called to inlined (`doSearchMove` 440 vs 528; its nerve 37.5 -> 98.4).

**One statement flips the decision both ways.** `TFruitsBoat::setBckTrack` (0x118, called in the ROM) was being inlined into `load`; naming one param fetch (`f32 speed; speed = ...->getSLBckMoveSpeed();`) pushed it over the budget and took `load` 29.9 -> 100.

**Paying the statement budget with wrappers.** A size-exact UNUSED body that is *called* instead of inlined can be fixed without changing its codegen: `TRocket::checkTrigger` (0x148) was refused at ~15 statements; replacing two `if (gpMSound->gateCheck(id)) MSoundSE::startSoundActor(...)` pairs with `gpMSound->startSoundActor(...)` took it under the limit with identical instructions (`TNerveRocketPossessedNozzle::execute` 55.7 -> 99.3).

**Specific helpers.**
- `TLiveActor::getModel()` is out of line; `getMActor()->getModel()` is the inline path. A stray `bl TLiveActor::getModel()` means the wrong one — but boss head/body constructors really do call the out-of-line one.
- `TSpineBase::pushAfterCurrent` is a plain push; `pushNerve` also writes `mPrevious`. The wrong one cost three nerves about ten points each.
- `getLatestNerve()` is exactly `mCurrent ? mCurrent : mPrevious`.
- `MsMtxSetXYZRPH` has an `f32` degree overload; use it instead of `(s16)(182.04445f * rot)`.
- `MsWrap<f>` is a local copy in nine TUs; a unit missing exactly that symbol is not missing a real function. `MsAngleWrap(...)` adds the inline boundary that keeps it called.
- `MsSqrtf` (one double-precision refinement) is not `JGeometry` sqrt nor MSL's three-refinement sqrt.
- Default arguments vs explicit arguments change inlining depth (`TPollutionTest` default name).
- Use `group->getChildren().push_back(actor)`; the fabricated `group->add(actor)` changes iterator inlining.

## Floating point

**`fp_contract` only fuses products of locals** (probed in a scratch TU with game flags):

| Expression | Result |
| --- | --- |
| `m.x * m.x + m.z * m.z` (members via `this`, pointer, reference, struct, `Vec`) | `fmuls`, `fmuls`, `fadds` — never fused |
| `f32 x = m.x; x * x + m.z * m.z` | `fmuls` for the member, `fmadds` for the local |
| `m.x += 0.2f * (v.x - m.x)` | `fmadds` |
| `d.x = 0.2f * (v.x - m.x); m.x += d.x;` with local `TVec3 d` | `fmuls` then `fadds` |

A target `fmadds` whose product operand was loaded from memory does not happen: that operand was a local.
Kept-apart `fmuls`/`fadds` for a register value means it went through a local struct member.
Hence `isZero()`/`squared()` on a member are unfused while `squared(const TVec3&)` is fused (`TChuuHana::rolling`).

**Other float rules.**
- `mModelFaceAngle = mStatusTimer * -4096` matches; `-(mStatusTimer * 4096)` adds an `extsh` (MarioJump, MarioRun).
- `0.5f * x` and `x * 0.5f` both give `fmuls x, 0.5`, constant second only when written constant-first.
- Literal `0.0f * sin` is folded away; building a vector and reading its components keeps the zero products. Quaternions from `setEulerY/Z/X` with two-argument `mul` also keep them (`koopajr`).
- Declaring the cosine local before sine reproduces sine/cosine table register order (`CalcRevisionPosByRotateZ`, `execSlip`).
- `atan2f(v.x, v.z)` loads `x` first; if the original loads `z` first, copy both to locals with `z` declared first.
- `fabsf` into a float local; global `fabs` adds an `frsp`.
- `remaining = end - frame` named before loading the rate matches; passing `end - frame` as an argument spills registers (`changeTumbleAnmRate_`).
- `powf(dx,2) + powf(dy,2) + powf(dz,2)` with global functions matches `MSound::getDistPowFromCamera`; `std::powf` wrappers delay the sum.
- Constructor yaw truncates `yaw * (65536.0f / 360.0f)`; BHS helpers use rounding `CLBDegToShortAngle`. Do not swap them.
- `JGeometry::min(a, b)` is `a >= b ? b : a`; `x = min(limit, x)` gives `fcmpo limit, x; cror eq,gt,eq` with `x` loaded before the param fetch (`moveSwing` 64 -> 88.7).

## Rules from `feetinv` (leg IK)

- **Refinement-free sqrt:** `volatile f32 f = x * __frsqrte(x);` reproduces `frsqrte`/`fmul`/`frsp`/`stfs`/`lfs` with no Newton step. It is a distinct idiom, not a broken `TUtil<f32>::sqrt`. Register allocation says whether it sat behind an inline: `frsqrte f1` next to `lfs f0, <const>` means `k * helper(x)` with the helper an inline function (MWCC allocates the multiply left to right, so the literal takes `f0`); written in place the chain takes `f0` (`enemy.cpp` sites). `THitActor::calcEntryRadius` uses a TU-static inline `HitActorFastSqrt`.
- **Degree-angle helpers:** acos is `1.0f -> 0.0f`, `-1.0f -> 180.0f`, else `90.0f - matan(f, c) * (360.0f / 65536.0f)`, with the product in a named local or MWCC fuses it into `fnmsubs`; asin is `1.0f -> 90.0f`, `-1.0f -> -90.0f`, else `matan(f, c) * (360.0f / 65536.0f)`. `TSpineEnemy::calcTurnSpeedToReach` spells the acos form by hand.
- **`TVec3` from matrix elements is per-component:** `v.x = m[0][3]; v.y = ...` interleaves `lfs`/`stfs`; the three-argument constructor and `set(x, y, z)` batch all three loads first. With memory arguments the constructor is always the batched form.
- **Name `JMASin`/`JMACos` results before storing into a local `Mtx`:** `jmaSinTable` is an `f32*`, so each matrix store invalidates the cached table pointer and MWCC re-reads it per use.
- **Nested-call argument order:** in `MsAtan2(a.dot(b), MsVECMag2(c))` the second argument's call is evaluated first; naming the dot product in a local forces it before the inner call.

## Rules from `BathtubPeach`

- `JDrama::TNameRefGen::search<T>()`'s internal `static_cast` makes the assigned local a propagated temporary, so later basic blocks re-read it out of `r3`; `(T*)search2(...)` gives the local a register home and reproduces the ROM's reads through `r31`/`r28` (two sites; supports the doubt in `JDRNameRefGen.hpp` that `search` was a template).
- Ternary direction: `cond; bne <load>; b <join>` with the zero hoisted above is `x == nullptr ? nullptr : x[i]`; `x ? x[i] : nullptr` gives a plain `beq` and folds the false arm.
- A weak `TVec2`/`TVec3` member listed in the map for one TU is a depth measurement: `nerve -> goTo -> setLength -> setLength -> squared -> dot` puts `dot` at level five and out of line, while the same `dot` at level three in the same function inlines. `TVec2` needs the two `setLength` forwarders `TVec3` has (open header fix in `JGVec2.hpp`).
- `MsWrap<f>__Ffff`'s retail body is the loop version (0x48, `Animal/boid.o`); the recurring `l + std::fmodf((r - l) + (t - l), r - l)` in `koopajr`, `MapObjCorona`, `BathtubPeach` is a different, always-inlined helper. `std::fmodf` itself must be a declaration with the 0x5c body in a `.cpp` (open fix in `math.h`); the current inline wrapper expands to `bl fmod` + `frsp` + a double load at every site.

## Rules from `bombhei`

- Range merging in the `TBGCheckData` predicates is context-dependent within one TU: the same inlined `isWaterSurface()` peels `0x100` and merges `0x101..0x105` in one nerve, peels `0x100`/`0x101` in `forceKill`, and `isPool()`'s `0x104/0x105` are left unmerged there. A merge-shape mismatch on these predicates is an optimiser artefact, not evidence of a different helper.
- `lha` + `clrlwi` on a Mario angle means `JMASSin(SMS_GetMarioAngleY())`; `*gpMarioAngleY` or a named `s16` folds to one `lhz` and CSEs the table index.
- Matrix-bound particles: `lwz 0x4; lwz 0x58; addi 0x30` is `getMActor()->getModel()->getAnmMtx(1)` (joint = offset / 0x30); `lwz 0x4; addi 0x20` is `getBaseTRMtx()`.
- `MActor::checkCurAnmFromIndex(int, int)` returns `int` in the ROM (`cmpwi`), but the header and `MActor.cpp` say `bool`; changing both is an open shared fix.

## Rules from `MapObjWave`

- `x / 2.0f` and `x * 0.5f` both emit `fmuls x, 0.5`, but the division loads the member into the *first* operand register; only `mAreaSize / 2.0f` matched.
- `a * sinf(A) + b * sinf(B)` as one expression evaluates the second term first and contracts the last product into `fmadds` even with a memory operand (so the `fp_contract` locals rule is not absolute); two named products plus `return heightX + heightZ;` matched.
- An inline boundary blocks contraction, and the map size proves it: `getMoveTexPos0` as `mTexPos0 + v * mTexScale0` fuses to 0x10; as `mTexPos0 + getStaticTexPos0(v)` it is the map's 0x14 and the call site keeps separate `fmuls`/`fadds`.
- Two `b` to the same label after an inlined `return` means the sibling branch was spelled out: `if (isSea()) return getWaveHeight(...); else return groundY;` keeps the redundant `b`; dropping `else` collapses it.
- `int` vs `u8` return on an inlined getter can move the caller's frame without moving an instruction (`getAlpha` returns `int`; that made `draw` exact).
- Frame levers measured here: `gpMarDirector->mMap` 0x20, `gpMarDirector->getCurrentMap()` or `SMSGetMarDirector()->mMap` 0x28, `SMSGetMarDirector()->getCurrentMap()` 0x38, and naming that result *reduces* it (0x30), the opposite of the console case; `getChildren()[i]` vs `(*list)[i]` is +8 with identical instructions. Measure per site.

## Rules from `amiNoko`

- **Float clamp shapes:** `fcmpo a,b; cror eq,lt,eq; bne` is `if (a <= b)`; a bare `fcmpo; ble X; b Y` is a ternary written with `>` whose other value was hoisted into the result register. A per-component converge only matches as `cur < t ? (cur + s > t ? t : cur + s) : (cur - s > t ? cur - s : t)`; `<=` in the second arm costs two instructions per component (`calcDirection`, `creepToCurPathNode`).
- **One named local for a repeated inline argument:** two `setGlobalScale(TVec3(2, 2, 2))` temporaries cost 0x18 of frame, not 0xC; hoisting one `TVec3 scale(2, 2, 2)` for both calls made `emitEffects` exact.
- **Out-param reassignment inside the guard:** `mGroundPlane = plane; if (mGroundPlane) { plane = mGroundPlane; ... }` lets MWCC forward the register; reassigning before the test reloads.
- **Declare loop accumulators after the preceding call:** declared before `isTouchedWallsAndMoveXZ`, `nearest`/`nearestIdx` lived across it in `r29` with an early `lfs f31`; declared after, they land in `r6`/`r7` like the original.
- **Open:** `TNerveAmiNokoWalkOnFence::execute` *calls* `TUtil<f32>::sqrt` for `toGoal.length() < 1.5f` while the same callee inside the inlined `creepToCurPathNode` is expanded later in the same function. Contradicts the depth model; naming the result and swapping sites did not help. Same class of problem as the `MapObjBall` table.

## Rules from `MapObjPinna` and `MapObjRicco`

- An UNUSED helper can supply the *shallower* level: `TShellCup::perform` reaches `MsMtxSetRotX` (0x7c) as a `bl` through a per-shell `calcJointMtx()`, which puts the 0x7c body at depth 2 where it is refused (42 -> 90.7); the same helper expands at depth 1 in `TPinnaShell::control`.
- Name both `jmaSinTable` lookups before a matrix fill, and keep `DEG2SHORTANGLE` at the use site: a shared `s16 a` local is worth exactly 8 bytes of frame (`MsMtxSetRotX` 6 -> 100).
- `MsRandF()` wherever `rand()` feeds arithmetic: the inline boundary at the random value blocks contraction (`mObjSpeedXZ * (MsRandF() - 0.5f)` keeps `fmuls`/`fsubs`; the expanded `3.0517578e-05f * (f32)rand()` contracts to `fmsubs`).
- Empty `case` labels are visible in the comparison tree (`case STATE_DOWN: break;` costs a `cmpwi; beq default` retail has); case *order* is block address order, not numeric (`TCraneUpDown::control` 1, 0, 3, 2).
- Which class owns a field is decided by where the derived vptr store lands: `TViking`'s constructor writes its vtable between 0x138-0x148 and 0x14c-0x158, so the last four fields are `TViking`'s, not `THorizontalViking`'s (84 -> 100).
- `TVec3<f32>(a - b).length()` is the speed-for-sound idiom (`TPinnaCoaster::control` 73 -> 98.7; same as `TMario::soundTorocco`): by-value `operator-` gives the integer copy plus per-component `fsubs`, the copy constructor the second aggregate, and the extra level puts `sqrt` out of line.
- Destructors weak in the map are in-class even when the vtable forces them into the object; constructors with no map symbol are in-class too.
- Open: a four-term `isState` `||` chain that retail normalises once more after the last term (`TAmiKing::moveObject`); `TLiveActor::getMActor` (8 bytes) emitted as a real symbol from `MapObjRicco.o` for one site (`TFruitLauncher::fireObj`), needing a wrapper level we cannot find.

## Rules from `MapObjMare` and `MapObjBianco`

- `dest += dir * 100.0f` keeps `TVec3::scale` out of line: the friend `operator*(TVec3 fst, f32)` takes and returns by value, costing two 12-byte copies and putting `scale` at depth 3 (`TMapObjPuncher::touchPlayer` 80 -> 99.6). `v *= k` alone leaves it at depth 2, where it still expands.
- `MsAngleWrap` vs `MsWrap` cuts both ways: at some depths `MsAngleWrap` pushes `MsWrap` out of line and emits a weak symbol the TU lacks; writing `MsWrap(mRotation.y, 0.0f, 360.0f)` directly took `TMuddyBoat::control` 93 -> 99.9.
- A 12-element `Mtx` fill is twelve element assignments, never an aggregate initialiser (which compiles to a `lwz`/`stw` copy from `.rodata`); a hand-built rotation must name its `sin`/`cos` in locals or the table lookup is recomputed per element (`TBellWatermill::control` 81 -> 96).
- GX constants that read wrong from a decompiler: source factor 1 is `GX_BL_ONE` (not `GX_BL_SRCALPHA`), alpha op 1 is `GX_AOP_OR`. `GXSetChanMatColor(GX_COLOR0A0, JUtility::TColor(c))` gives the conversion temporary plus the by-value copy the ROM has.
- `cmpwi N; beq caseN; bge end; b end` (two branches to one label, no second compare) is a switch with an empty `case 1: break;` folded into the default.
- Two stacked bool materialisations are `TMapCollisionBase::isSetUp()` (`if (checkFlag(...)) return false; return true;`), not `!checkFlag(...)`.
- Statics are declared per class, interleaved with that class's methods: `.sdata`/`.sbss` order follows the reverse class order of `.text`, each class's statics at the head of its own block.
- Open: `TCogwheel::initMapObj` keeps literal `0.0f * sin`/`0.0f * cos` for the z of an unrotated `(sRadius, 0, 0)` offset; every direct spelling folds them and `rotateVecByAxisY` is never called there. `TLeafBoat::bind` calls `TVec3::sub` where `TMuddyBoat::bind`'s identical spelling inlines it.

## Rules from `tinkoopa` and `elecNokonoko`

- A deeply inlined param read must skip the params class's wrapper: at depth 3 `p->getSLFoo()` pushes `TParamT<T>::get()` out of line and emits a stray `bl`; `p->mSLFoo.get()` is one level shallower (`TNerveTinKoopaBreak::execute` 96.0 -> 99.2). This is a codegen lever, not only a frame one.
- Sequential `cmpwi` on a dense small case set is an `if`/`else if` chain, not a `switch` (which builds a pivot tree): five sites in `tinkoopa`, `startBreaking` 82 -> 99.8. An empty trailing `else if (x == N) { }` keeps the ROM's dead compare at some sites.
- Each accessor level on an array of pointers costs a 4-byte temporary: `mLaunchSchedule->getOrder(i++)` gave 0xa8 vs 0x28; the raw `mOrders[i++]` 99.6; naming the element 100, which also pushed `makeLaunchSchedule` over the inline budget so `init` stopped absorbing it.
- A materialised bool means the predicate is the `if`/`return` inline: `bool hasCarapace() { if (mHasCarapace == 0) return true; return false; }` took two functions to exact; `return a == b;` folds to a bare compare. One site (`sendAttackMsgToMario`) wants the bare compare.
- `setGoalPath(actor)` vs `setGoalPath(position)`: `TPathNode(THitActor*)` stores the actor, then the zero vector, then conditionally the actor's position; `TPathNode(const TVec3&)` stores a null (`Collect` nerve 84.9 -> 95.9).
- A discarded call whose body starts with a null test leaves the loads behind: `mKillerManager->getActiveObjNum();` as a statement is the ROM's `lwz; lwz; cmplwi 0` with no branch.
- `operator-` is the lever that puts `TVec3::sub` out of line (three levels: `operator-` -> `operator-=` -> `sub`) where `.sub()` or `-=` expand; it also moved `TUtil<f32>::sqrt` to a call in `TElecCarapace::shoot`.
- Open shape: the ROM copies a goal point into a local, subtracts `mPosition` in place **with stores**, and still calls `TUtil<f32>::sqrt`; `distance()` gets the call without the stores, `d -= p; d.length()` the stores without the call (three `elecNokonoko` sites).
- A dead function must not introduce string literals: giving an UNUSED debug printer `OSReport` calls added 100 bytes of `.rodata` and shifted every later string. The map also says whether `static const TModelDataLoadEntry entry[]` (`.rodata`) or unqualified `static` (`.data`) was used per TU.
- The rogue-include block is not always last: `tinkoopa` needs `InfectiousStrings.hpp` and the MSound pair **before** `Map/MapCollisionEntry.hpp`, or the two `setUpTrans` literals land ahead of `MtxCalcTypeName`.
- Open: `TTinKoopa::hitParts` (99% alone) inlines into both `receiveMessage`s, three statements under the budget; `TTinKoopa::emitTinKoopaEffects` (769 instructions) is exact and 120 bytes short of frame.

## Rules from `MapObjMamma` and `MapObjMonte`

- `TMatrix34<T>::identity()`'s chained assignments give a fixed 12-store order (`[2][3],[1][3],[0][3]; [1][2],[0][2]; [2][1],[0][1]; [2][0],[1][0]; [2][2],[1][1],[0][0]`): that order means a `TMatrix34<SMatrix34C<f32>>` local plus `identity()`, not `PSMTXIdentity`.
- A row term `m[i][3] + (m[i][0]*x + m[i][1]*y + m[i][2]*z)` with no `bl` is `TMatrix34::mult(v, out)`; `MTXMultVec` always stays a call (`TFluffManager::load` 65 -> 100).
- Reaching `gekko_ps_copy12` through `SMatrix34C<f32>::set(ConstArrType*)` keeps the `bl`; calling it directly expands the twelve `psq_l`/`psq_st` pairs (`calcDefaultMtx` 54 -> 99.6).
- A materialised bool before a state compare is `TMapObjBase::isState(N)`, not `mState == N` (three functions to 100).
- `stfsu` + two `stfs` on a member position is `mPosition.set(x, y, z)` with constants first (`2.0f + mPosition.x`); three independent `stfs` is per-component. For a *stack* vector the opposite: interleaved compute/store means per-component.
- `operator=` vs `.set()` on a member vector decides between a stack temp (`lwz`/`stw` triple) and passing the member's address (`drawRopeBetweenBoards` 61 -> 97.7).
- Open: `TLeanMirror::calcCurrentMtx`'s best-evidenced body compiles to 0x84 against the map's 0x100, so `controlGoTarget` stays under the budget and inlines where retail calls it.

## Rules from `killer` and `limitkoopa`

- Chained assignment reproduces reverse-order field stores (right to left): `mEyesColor.r = mEyesColor.g = mEyesColor.b = 0;` stores b, g, r.
- `JUTNameTab::getIndex` results belong in `s32` locals: the ROM masks with `clrlwi` at each use, not at the assignment (`setMActorAndKeeper` 78.9 -> 100).
- Vector copy from a global has three shapes: `v = *gpMarioPos` (integer words), `v.set(*gpMarioPos)` (interleaved `lfs`/`stfs`), `TVec3<f32> v(p.x, p.y, p.z)` (batched loads, and a `y` already in a register is reused).
- A `new` result wants a named local when it is the callee's *second* argument (`setSkinDeform(deform, flag)` sets `r4` first as the ROM does).
- A three-term `||` guard merges the early returns into one `li r3,1; b`; `(a || b)` plus a separate `if` gives two blocks; three `if`s give three.
- Turn clamps: `turn = diff > limit ? limit : diff;` in the positive arm and `diff = diff > -limit ? diff : -limit; turn = diff;` in the negative arm give the two-branch shape.
- Statement budget does not generalise to large single-call-site callees: `TLimitKoopa::startHipDrop` (0x208, called in the ROM) stayed inlined through every lever that worked on `setBckTrack`.
- Another pasted-UNUSED case: `TFlyEnemy::flyMove` (0x2bc) is not called from the chase nerve; pasting it took the nerve 49.4 -> 91.0. `MsSin`/`MsCos`/`TVec3::set<f>` still expand where the ROM calls them (same class as `MapObjBall`; see the local `set<f>` lead above).

## Rules from `BeeHive`

- A `const` accessor restores a second member load: a named `u32 flags = obj->mFlags;` does not reproduce the ROM's re-read of `mFlags` in the `on/offFlag` after a test; only reading through a const-qualified inline does (`appearBee` 85.6 -> 100). `TRealoidActor` needs `bool checkFlag(int) const` (open fix in `fishoid.hpp`).
- `JGeometry::TUtil<f32>::clamp` emits `fcmpo; bge L; b STORE` twice; a nested ternary matches only when the low bound is a literal already in the result register, and costs `fmr` + `b` when it is an `fneg`. Use `clamp()`.
- `epsilonEquals` overloads: one `lfs` of the epsilon plus `fneg` is the three-argument form; two loads of the literal is the fabricated two-argument form.
- In-place accumulate: `v.scale(s); mDst.set(v.x, v.y, v.z)` multiplies into the accumulators' registers and lands the frame where `mDst.set(a * s, b * s, c * s)` allocates fresh ones (`controlSound` 99.3 -> 100); `mDst.set(v)` loses 16 bytes of frame.
- `mCoins + n - 1` is `add` then `subi 4`; `&mCoins[n - 1]` is `subi`, `slwi`, `add`.
- `a += b * c` contracts to `fmadds` even with memory operands; naming the product keeps `fmuls`/`fadds`.
- `JGQuat4.hpp`'s `mul(const TQuat4&)` had its `_y` signs swapped against the ROM (fixed; `calcRootMatrix` 71.5 -> 75.5 with the two-argument form). `TPosition3::setSQT` is the level that keeps `setSQ` a `bl`. Still open there: `setRotate(from, to, amount)` is an f28-f31 permutation decided inside `cross()`'s batched `set`; `slerp` is a validated 16-byte frame gap (146 instructions exact); `getQuat` is one `fadds` operand order; `TRotation3::setSQ`'s `qt.y, qt.z, qt.x, qt.w` load order survives ten spellings (removing its locals makes MWCC inline it, which retail does not).
- A `lwz`/`addi`/`blr` body with `this` in `r3` and no hidden return buffer (`TWireBinder::getDir` 0x8, `TWireTrap::getWireDir` 0xc) proves a `const T&` return; fixing it made `calcMomentum` size-exact and took three nerves to 98-99%.
- An unnamed `TVec3<f32>(...)` temporary gets a stack home; a named local is scalarised (`TWireTrap::load` 87.4 vs 80.4).
- Once a function is big enough MWCC stops inlining even two-instruction accessors: retail's `TWireTrap::checkHitActors` (frame 0x1c0) `bl`s `getWireBinder`/`getDir`; ours (0x98) expands them.
- One name, two bodies in one file: `fireWanwan` calls the guarded global `SMS_CalcToDirMatrix` from `TFireWanwanTailHit::perform` and expands an unguarded cross/setLength frame in `TFireWanwanTailNode::perform` (a fabricated `static inline CalcToDirMatrixNoGuard`); writing the statements out in place binds the `up` temporary a level shallower and loses 34 points.
- **Lead:** `set<f>__Q29JGeometry8TVec3<f>Ffff` is a **local** (internal-linkage) symbol in 20+ retail objects and retail calls it. That is what a member template gets when its definition is not inline, which would explain the per-call-site tables below. MWCC 1.2.5 refuses the out-of-class definition (`illegal function definition`, since `TVec3<f32>` is an explicit specialisation), so reproducing it needs a different structure for `JGVec3.hpp`. Worth its own batch.
- Ruled out: `TPathNode(THitActor*) : unk4(0, 0, 0)` reproduces the out-of-line `TVec3::set<f>(0,0,0)` and improves four BeeHive nerves, but regresses ~15 other units. Per call site; do not push into `PathNode.hpp`.

## Load and store order

- **Param fetches:** `TParamT::get()` returns a reference, so `f(p->a.get(), p->b.get())` defers both loads to the call. If the original loads each value as soon as its getter returns, copy to locals first (`TKoopaJrSubmarine::init` 87.8 -> 94.4).
- **But** `new T(args...)` evaluates `operator new` first, so do not hoist ctor-argument param fetches above the `new`.
- m2c hoists pure arithmetic above calls; place fetches where the `lwz`/`lfs` actually sit (`igaiga` `behaveToWater` 72 -> 99.8).
- `MsRandF() < param.get()` binds the parameter address before `rand()` and reads after, matching the original.
- `MTXCopy(mtx, getModel()->getBaseTRMtx())` evaluates `getModel()` first; if the original calls the source getter first, hold it in a local.
- **Hand-built `Mtx`:** the three `[i][3]` stores come *first*, then the rest row-major (`popo` joint callbacks 82.8 -> 99.8).
- **Member re-read after `theNerve()`:** `if (mPopo->isRollJump()) return mPopo->receiveMessage(...)` loads `mPopo` twice because `__register_global_object` is a call. Keep the member access; a local removes the second load.
- Name a `TSmallEnemy*` receiver before a virtual call to get vtable-load-before-this-copy order (TMario `checkCollision`).

## Vectors and locals

- `.squared()` directly on a horizontal-vector temporary keeps separate multiplies; a named vector gets scalarised and fused (`isCanWalk`, boss 100-unit check).
- Construct an opposite vector from `(-v.x, -v.y, -v.z)`; copy-then-`negate()` forces integer copies and stack (boss `perform`).
- Copy position to a local goal, add, then `setGoalPath(goal)`; `setGoalPath(mPosition + dir)` adds an out-of-line add.
- A named displacement local restores six copy instructions (`bind`).
- A plain `Vec` copy followed by scalar sums keeps Y/Z in float registers where `TVec3` blocks it (Spider).
- Default-construct then assign fields separately; the by-value constructor emits integer copies (Beam partition).
- Normalising cross-products in place saves a float register (EffectUtil).
- **A `TVec3` array defeats scalar replacement where three separate locals do not.** Three named column vectors get promoted to registers and their `squared()` products fuse; the same three as `TVec3<f32> dir[3]` with constant indices get a stack home, are read back with `lfs`, and stay unfused (`TRocket::calcRootMatrix` 85.2 -> 96.7). Contiguous 12-byte slots in the target are the tell. `mtx.getXDir(v)` does not force the memory home, and an unrolled loop fails when the body has `length()`.
- `JGeometry::TRotation3::getXDir/getYDir/getZDir` batch their loads with `set(x, y, z)`, and that **is** the real body: `JPABaseEmitter::calcEmitterGlobalParams` batches the three `lfs` before the stores and drops 98.5 -> 93.0 with per-component bodies. `popo` and `rocket` `calcRootMatrix` read the columns out themselves.
- A float literal in **`.sdata`** rather than `.sdata2` was materialised to bind to a `const T&` parameter; that is how `std::min`/`std::max` were identified in `fruitsboat`. This project's `std::min`/`max` take `const T&`, **return `T`** (a `const T&` return makes MWCC select the address and spill the argument) and compare `a > b` (gives `fcmpo a, b` + bare `ble`).
- Particle scale vectors are unnamed temporaries: `SMS_EasyEmitParticle(id, &mPosition, nullptr, TVec3<f32>(1, 1, 1))` matches; a named `scale` local lands below the other locals (`TWireTrap::kill`).
- `TSpineBase::initWith` already clears the vertebrae; a separate `mSpine->reset()` before it adds three instructions.
- `calcRootMatrix` levers (`wireTrap`): the up vector as an unnamed `TVec3<f32>(0, 1, 0)`; in-place `quat.mul(quat, spin)` instead of a third quaternion (-16); a named `J3DModel* model = getModel()` before `MTXCopy` (-8); declare `mtx`, then the quaternions, then the vectors.
- Open (same family as `MapObjBall`): `behaveHitWireTrap` (UNUSED 0x258) is expanded twice in retail's `checkHitActors`; MWCC inlines at most one expansion for us. `TVec3<f>::set<f>(f, f, f)` is emitted **local** at 0x10 in `wireTrap.cpp` and `Kazekun.cpp`; our constructor expands it and scalarises the vector.
- **Holding a matrix in a local costs a register** and blocks folding `+0x30` into the inlined base body's load offsets (`TMoePuku::calcRootMatrix` 86.5% vs 99.1% re-fetching).
- `mRotation = p->getRotation()` costs 8 bytes of frame when a call follows (`TTobiPuku::initAttacker`); `p->mRotation` matches both sites.
- Keep a computed matrix in a local rather than re-reading the member it was stored to.
- `s16` narrowing on jump animation locals adds sign extensions; don't narrow without evidence.
- A single initialised `JUtility::TColor` shared by two conversions shares their slot (GCConsole2).
- `J2DPicture::setWhite` reproduces eight-byte spacing between temporary colours (CardSave).
- `CLBAbs<s16>` narrows after negation and mishandles -32768; keep an int.
- `u16 index = names->getIndex(name); model->getAnmMtx(index)` narrows the local, not `getIndex` (boss joints).
