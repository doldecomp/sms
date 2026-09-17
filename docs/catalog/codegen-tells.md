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
Moving one case ID gives the wrong comparison tree; moving the whole original interval restores tree and registers (TMario `checkCollision`, `receiveMessage`).

**Loops.**
- An index loop over a fixed-size member array unrolls (`getUnusedSeed` 344 bytes vs map 48); a pointer loop does not, and a separate `T** end = &mSeeds[16];` before the loop makes MWCC compute the end before the begin and keep `this` in the lower register (`TYumbo::perform` 93.1 -> 100).
- `fcmpo` + bare `bge` means the source tested the other way: `if (x < limit) { ...; } return false;`, whereas `if (x >= limit) return false;` gives `cror eq,gt,eq` + `bne`.
- Timer loops over a reversed order: ascending `i = 0..7` with `mBodies[7 - i]` times `i` matches; counting down leaves runtime arithmetic after unrolling (boss get-up timers).
- `fabs(mBodies[i]->mRotation.z)` compared directly, then assigned to the member, fully unrolls; a local `angle` breaks it (`getBodyMaxRotateZ`).
- An `s32` counter with a signed bound keeps the initial branch before sixteen unrolled stores (Strategy constructor).

## Inlining

**Depth limit.** With `-O4,p -inline auto,deferred`, a small inline expands through four wrapper levels and is called at the fifth; a body the size of `std::fmodf` (about 20 instructions, two runtime calls) expands through three and becomes a weak out-of-line copy at the fourth.
Declaration form and caller size make no difference (eighty `normalize()` expansions in one function all inline).
That is why `TUtil<f32>::inv_sqrt` (under `normalize` -> `setLength`) is always called while `sqrt` under `length()` expands.

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
- `JGeometry::TRotation3::getXDir/getYDir/getZDir` use `dst.set(at(0,0), at(1,0), at(2,0))`, which batches the loads; the ROM interleaves `lfs`/`stfs`, so the original bodies were per-component (`popo` and `rocket` `calcRootMatrix` work around it locally; open shared fix in `JGRotation3.hpp`).
- **Holding a matrix in a local costs a register** and blocks folding `+0x30` into the inlined base body's load offsets (`TMoePuku::calcRootMatrix` 86.5% vs 99.1% re-fetching).
- `mRotation = p->getRotation()` costs 8 bytes of frame when a call follows (`TTobiPuku::initAttacker`); `p->mRotation` matches both sites.
- Keep a computed matrix in a local rather than re-reading the member it was stored to.
- `s16` narrowing on jump animation locals adds sign extensions; don't narrow without evidence.
- A single initialised `JUtility::TColor` shared by two conversions shares their slot (GCConsole2).
- `J2DPicture::setWhite` reproduces eight-byte spacing between temporary colours (CardSave).
- `CLBAbs<s16>` narrows after negation and mishandles -32768; keep an int.
- `u16 index = names->getIndex(name); model->getAnmMtx(index)` narrows the local, not `getIndex` (boss joints).
