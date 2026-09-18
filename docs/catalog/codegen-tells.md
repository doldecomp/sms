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

**Tiny accessors: steer with one relative level, never a global change.** Retail's call/inline split of `getLatestNerve` (0x1c) and `getMActorAnmData` (0x8) is per site and even per compare within one function (`TKazekun::attackToMario` inlines the first and `bl`s the next two), so no `Spine.hpp`/`ObjModel.hpp` change can be right. Two proven shapes add exactly one level above the accessor and leave the rest of the chain alone: a one-line forwarder on the owning class (`TBossGesso`/`TBossWanwan`/`TBossHanachan::getLatestNerve()`; `TBWPicket::moveRequest` 78 -> 99.8, `BossHanachanParts` data to 100), and an ordinary const/non-const overload pair when the sites hold a non-const receiver (`TMActorKeeper::getMActorAnmData`; fifteen boss nerves up, no call-site edits). Anti-pattern: a wrapper that folds in the next link (`getMActorAnmData()->getUnk2C()`) pushes that link out too and emits a weak symbol the map lacks; the extra symbol is the tell. Convert `this` for a const forwarder with a `static_cast`, not a named `const T*` local (that costs two exact `MapObjSirena` functions).

**Definition order is irrelevant; the budget is a statement count, not a byte count.** Measured in a scratch TU with the game flags, varying definition order, callee size, statement count, wrapper levels, call-site count and declaration form. The `bosstelesa` lead — that `-inline deferred` inlines only a callee defined *later* than its caller — is **refuted**, and it is backwards: order only ever matters with `deferred` *off*, and then it is the *earlier* definition that inlines.

| callee, reached at depth | plain (no `inline`) | `inline` / in-class body |
| --- | --- | --- |
| 1 (direct call from the emitted function) | **14 statements** | **no limit** (measured to 79 statements / 0x1014 bytes) |
| 2 | 9 statements | 9 statements |
| 3 | 6 statements | 6 statements |
| 4 | 2 statements | 2 statements |
| 5 | never | never |

Byte size does not enter into it: at depth 1 a 0x4c body and a 0x278 body both inline at 13 statements and both become calls at 15. The byte thresholds in the depth table above are that table's bodies' statement counts in disguise. What counts as one statement: an expression statement, and an initialised declaration. What is free: an uninitialised declaration, an empty `;`, a bare `{ }`, `return <local>;`, and — the useful one — **splitting an initialised declaration** (`f32 x; x = e;` counts exactly as much as `f32 x = e;`). Statements a callee gains by expanding *its own* inlines do not count either; each nested callee is re-judged against the allowance for the depth it lands at.

Everything else measured had **no** effect: definition order (48 pairs), call-site count (1 vs 2), caller size, `inline` vs `extern inline` vs `static inline` vs in-class, and return by value of a 12-byte struct.

The table was re-measured from scratch with the priced cost model (one plain expression statement per unit, a chain of one-statement forwarders above it, game flags), and it is **confirmed unchanged**: `static` 14 / 9 / 6 / 2 / never, and `inline` and `static inline` differ only at depth 1, where they have no limit (17 statements measured here, 79 earlier).
The `cameragc` reading of 10 at depth 2 was a mis-attribution: `TMario::checkStatusType` is not a budget case at all but the short-circuit rule below — it sits on the right of a `&&` at all five sites, and the ternary body is what expands.
One caveat found while re-measuring: a forwarder that returns `*this` is not an ordinary level. In a chain `s3 -> s2 -> s1 -> self() -> get()` the ROM-flags build calls `self()` at depth 4 (where the allowance is 2 and its body is 1 statement) and still *expands* `get()` at depth 5. Do not count a `*this`-returning accessor such as `TNameRefPtrAryT::getChildren` when working out a depth.

Two consequences:

- **Definition order in a TU is not an inlining lever.** The map's reversed emission order fixes definition order, and that is all it fixes; it never decides what can inline. Do not reorder a TU hoping to move a call/inline decision.
- **A retail inline bigger than its depth allowance was declared `inline`.** That is the whole explanation of `bosstelesa`: `fanfale` (UNUSED 0x1f0) inlines because it was an in-class or `inline` definition reached at depth 1, where the keyword removes the limit entirely; `getSlotResult` (0x8c, emitted) is called because it is a plain out-of-line method over 14 statements. Read it the other way too: a plain method that retail *calls* at depth 1 must have had 15 or more statements, which is a hard lower bound on a reconstruction. `TKukku::updateRotation` is the open case — retail calls it at depth 1, our 12-statement body expands, and four throwaway statements take `TNerveKukkuRecoverGraph::execute` 0.0 -> 67.7% and `TNerveKukkuGraphWander::execute` 42.0 -> 92.9% while leaving `updateRotation` itself at 99.8%. Its frame is already 0x18 over, so the missing statements have to be ones that need no stack slot.

**The `MapObjBall` table below is closed.** The split was never per site: an unnamed `TVec3<f32>(mVelocity).length()` is the level that puts `TUtil<f32>::sqrt` out of line, while `TVec3<f32> v(mVelocity); v.length()` expands it (`TMapObjBall::hold` 48 -> 99.8, `TResetFruit::hold` -> 100, `touchWall`, `touchGround`). `TBigWatermelon::control`/`touchActor` spell out the base bodies verbatim (real duplicated source) while `TResetFruit::control`/`receiveMessage` call them, and a *qualified* call `TResetFruit::touchActor(x)` is how a virtual gets inlined. The depth-1 threshold is exactly 15 statements, measured twice more (`TMapObjBall::control` +3, `calcNearerDirection` +6, both codegen-neutral named locals). Two wrapper levels (`WrapDirection` + one-line `WrapRadian`) keep `TUtil<f32>::mod`/`std::fmodf` a `bl` at depth 4 (`TDirectionCalc::sub` 67 -> 98, `absDirection` 17 -> 99.8); the surviving `(t - l)` with a folded `(r - l)` is the tell for a parameterised helper. `TUtil<f32>::PI()` over the literal keeps a parameter as the multiply's left operand (`d2r` exact). Accessor frame steps are not all 8: `getGroundPlane()->getNormal()` is +32 (`touchGround` 56-byte gap closed exactly with three +8s). The wrong `mBodyRadius * -0.5f` (retail `+0.5f`) had displaced the whole `.sdata2` pool: data 12 -> 83.

**Per-call-site differences (open, historical).** In `MapObjBall` the original inlines differently from us *per call site*:

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

## Rules from `BathWaterManager`

- A local's float register is handed out in declaration order; its int-to-float conversion temporary in assignment order. When the target's two orders disagree, the original declared the locals as a block and assigned afterwards (`clearEFB_alpha`, `draw_mist`: 99.8 -> 100 each).
- `at()` puts a matrix element in the *second* multiply operand; a raw `mMtx[i][j]` read honours source order (`throwMario` frame exact, `getPos` all nine `fmadds`). `Mtx`/`Mtx44` locals: first declared gets the higher offset.
- `TVec3<f>::set<f>` is a `bl` at depth 3, and a by-value-returning accessor is the level that gets you there (ctor at 2, `set<f>` at 3) and reproduces the word-wise copy of the return temporary (`throwMario` 70 -> 84).
- An unnamed `TVec3<f32>(0, 1, 0)` temporary stays in memory so its 0.0/1.0 products survive; a named local is scalarised and they fold.
- Removing a named local can close a frame gap (`warpTex` for `new JUTTexture(...)` was `loadAfter`'s whole 8 bytes).
- `++x` in the condition avoids the member reload. A fetch invalidated by an intervening store tells you the source put it after that store (RNG seed store before `unk24->getBathtubData()`).
- An UNUSED symbol defined in-class is dead-stripped from our object (`clearEFB` had to move out of class to exist). Four methods the map lists weak must stay out of class or MWCC inlines them into `render` (93.8 -> 51.3): the `TYoshi::onYoshi` family, and the unit's remaining ORDER/BINDING failure.
- GX: `GXSetZMode` compares `GX_LESS` in both renderers; the mesh renderer's polygon colour goes to `GX_TEVREG0`. The drop-pair inner loop is a do-while (ROM behaviour).
- Open header items: `TProjection3<T>::orthographic` takes `(l, r, t, b, n, f)` (height pair later, since arguments evaluate right to left; `prerender` 85 -> 87) and needs `+ n` on `mMtx[0][3]`/`[1][3]`; `TRandom_<T>::get_float(f32)` has no body and should be `return get_float(0.0f, max);` (the level that leaves `get()` a `bl` at depth 5); a named transposed 3x3 multiply for world-to-local on the row-major bathtub matrix (parked as `TBathtubData::getLocalPos`).

## Rules from `MSoundMainSide` and `MarDirectorDirect`

- The const/non-const accessor pair is a re-read lever on plain integer flag words: `checkUnk4CFlag(int) const` with non-const `on/offUnk4CFlag` reproduces the load-per-modify at ten sites (`updateGameMode` 86 -> 91); with the test non-const MWCC CSEs its load into every modify. Routing only the modifies through accessors does nothing.
- Caller size gates `TVector<void*>::begin()`: the ROM `bl`s it once per `calcParamRatioInCube` expansion while the same chain at depth 0 folds to `lwz 0x10`; adding levels does not move it (`TWireTrap::checkHitActors` family; the callers' frames are 60-170 bytes short).
- A weak symbol objdiff reports "extra" can be correct (`std::sqrtf` is UNREFERENCED DUPLICATE for this TU); `static inline` keeps inlined `frsqrte` chains with no symbol where plain `static` emits one. Check the closure before chasing an "extra".
- A two-target jump table with alternating entries is `case 0: case 2: case 4: case 6:` against `default:`.
- A named `int` copy of a `u8` parameter keeps the `clrlwi` result in a callee-saved register across a `bl` and turns later tests into signed `cmpwi`; a two-arm switch kept as a switch shows the unfused `beq +8; b else`.
- An empty `case 0:` grouped with `default:` moves a compare tree's pivot (labels {0,1,2,4} pivot on 2, {1,2,4} on 3): read the pivot backwards to recover the label set (`setMario` 80 -> 99.8 with a named `TWaterGun*` receiver).
- `Map/MapCollisionEntry.hpp` is a `.rodata` prefix for System TUs too (`MarDirectorDirect` data 27 -> 81); `System/StageUtil.hpp` must go where the map lacks its tables.
- `MSStageCubeFade::setBgmVolumeForce` is not static (the caller null-tests `smInstance` in `r3`).
- Open: `TFlagT<u16>`'s copy constructor (weak 0xc from `MarDirectorDirect.o`) is called twice in `decideNextStage` for `TGameSequence::set`'s by-value parameter; MWCC elides the copy in every spelling (`decideNextStage` 63%, and its call/inline split between `updateGameMode` and `changeState`). A real `fmuls` by `1.0f` in `updateGameMode`'s `registFadeout` call that every spelling folds away.

## Rules from `EventWatcher`

- A missing function *permutes* the string pool, it does not merely shift it: two absent builtins reordered the `TSpcStack` trace literals and ~60 functions showed a bare `addi rX, base, imm` diff. Diff the two `.rodata` blobs before chasing operands per function (data 14 -> 100, 15 functions to exact from writing the two).
- A literal used twice wants its own `const char*` local when retail addresses it directly (`lis rX, @NNNN@ha; addi r0, rX, @NNNN@l; mr rN, r0`); `grep 'addi r0, r[0-9]*, "@'` over the asm finds the sites (`evAppear8RedCoinsAndTimer` 92 -> 99.9).
- Return-by-value through a base class selects the converting constructor: `rotateY(Vec&) -> TVec3<f32>` is what leaves `bl TVec3<float>::set(const Vec&)`; the non-template overload only wins when the source really is a `Vec`.
- Storing to a struct member invalidates the cached loads of its siblings: retail reloads `vec.z` and the sin/cos entries after a store but keeps a pre-store `f32 x = vec.x;`.
- Depth budget pair: `TUtil<f32>::sqrt` (4 statements) is a `bl` only at depth 4; `TVec3::dot` (1 statement) drops out at depth 5.
- New frame-gap family, "push an inlined accessor result": every builtin whose pushed value comes from an inlined accessor is 4-8 bytes short in the low region and reads the member one instruction later than we do; those pushing a `bl` result match exactly. The temporaries belong to the accessor side, not `push()`. Six functions; no lever yet.
- `for (int i = 0; i < (int)arg_num; ++i)` gives `cmpw`; mixing `int i` with a `u32` bound gives `cmplw`. `getFromTop(arg_num - i - 1)` folds the `-1` into the accessor's `mSize - 1`.
- `fmadds f, d, d, f` with `f` loaded as `0.0f` is a 3-component `squared()` whose y term folded, not a hand-written 2-D distance.
- Open header items: `length()` spelled `TUtil<f32>::sqrt(dot(*this))` instead of `sqrt(squared())` (retail calls `sqrt` while expanding `dot`; `evIsNearActors` 88.5 -> 97.3, but `boid` and `Kazekun` regress ~1.3 each; `squared()` as `x*x + y*y + z*z` regresses ~25 units); `TSpcTypedInterp<T>::dispatchBuiltin` tests `mNativeCall` in one register then copies to `r12`; `BackgroundMusic.hpp`'s tail looks shifted by one (`evStartMontemanFanfare` plays 0x80010026, `evStartMontemanBGM` 0x8001002f). Ruled out: making `JDrama::TNameRefGen::search<T>` delegate to `search2` breaks a source-linked object and the DOL.

## Refinements from the `bosseel`/`fireWanwan` sweep

- **The depth-1 budget is a cost, not a raw statement count.** Scratch-TU measurement: budget 14, call at 15; each statement costs 1, an `else` costs 1, a `for`/`while` costs 1 for itself plus its init and increment expressions plus one extra for being a loop. Statements gained from a callee's own inlines cost nothing (verified to 14 nested calls); caller size is irrelevant (0-320 statements). `TBossEelHeartCoin::generate` costs 10 and `TBossEel::forceShedTears` 13, which is why both still carry `#pragma dont_inline`: the honest spellings are 5 and 2 short, and padding `forceShedTears` to 15 with an if/else index form matches both callers but drops the function to 91%.
- **A two-`return` body is refused for inlining on the right of a `||`/`&&` chain at every depth** (0-3 wrapper levels measured); the equivalent ternary always expands. `TTakeActor::isTaken` should read `if (mHolder) return TRUE; return FALSE;` (retail `bl`s it from `updateRumble`; measured 0 -> 100 plus two callers up, zero regressions; open header item). This is also the mechanism behind the `isRoll`/`isRolling` entries.
- A 1-statement `BOOL` accessor inlines through four levels and is called at the fifth (direct confirmation of the tiny-body row).
- Argument evaluation order is a source tell: named locals evaluate left to right, inline `.get()` expressions right to left (`updateCollisionFromParam`).
- A member pointer used across intervening member stores is re-read, not cached (`TBEelTears::moveObject`: one named local, retail re-reads `0x15c(this)` after the `mVelocity`/`mPosition` stores); with a single block (`TOilBall::moveObject`) the local is right.
- `MsClamp` tests the maximum first; `TUtil<f32>::clamp` does not; the two `fcmpo`/branch pairs say which was used.
- A named local whose initialiser carries a cast takes a slot above everything declared after it: dropping `const TBossEel* owner = getOwner();` moved the scratch matrix to the top of the region where retail has it.
- The `TVec3` copy-shape rule applies to subtraction: `push.sub(a, b)` stores per component; the three-argument constructor batches six loads before three `fsubs`.
- `TFireWanwan::doAdjustTarget` (UNUSED 0x298) is pasted into both nerves because `TQuat4::rotate` is a 17-statement in-class body: at depth 1 the keyword removes the limit, behind the call it is at depth 2 where 9 is refused.
- Ruled out: `TVec4::normalize()` as the one-argument `setLength(one())` forwarder (two nerves +11 each but seven quaternion sites regress up to 8 points); a `T*&` reference local (`lwzu` becomes `addi`); unreferenced locals inside an *inlined* callee do not grow the caller's frame.
- Header items: `SoundEffects.hpp`'s `MSD_SE_BS_UNG_*` group is wrong (retail `0x2180`, `0x2981`-`0x298d`; header `0x81xx`/`0x89xx`; only `UP = 0x8922` is right; parked as `BOSSEEL_SE_*`); `startAppearBalloon` takes the low byte in both units; retail `bl`s `TVector<void*>::begin()` from `TBossEel::perform` and dereferences at +4 where our chain expands it and reads +0.

## Settled in header round 8

- **A two-`return` body is refused expansion on the right of a short-circuit `&&`; the equivalent single-`return` ternary always expands.** Both compile to the same seven instructions, so the map's weak 0x1c symbol plus a `bl` at depth 1 is the tell, and the fix is the `if (c) return TRUE; return FALSE;` spelling (`TYoshi::onYoshi`; `TMario::onYoshi` 22 -> 100, the `(void)0` hack gone). Depth and statement count are both ruled out for this case; in an if-condition the same body still expands. This qualifies the "no limit at depth 1 for `inline`" row.
- `memset` is never inlined by MWCC 1.2.5, with or without its `.init` declspec. An inlined zero-fill in ROM game code is a hand-written loop: pointer-walk plus countdown (`for (int i = n; i != 0; --i) *p++ = 0;`) gives one 8-store body with `ctr = n/8`; an indexed loop gives eight bodies with `ctr = n/64` (`TGCConsole2::perform` 86.5 -> 87.3).
- Reordering a TU can leave `.rodata`/`.sdata2` untouched (`MapObjBall`'s pools did not move); the gain shows in compiler-generated destructors. A literal-pool mismatch is not evidence of wrong definition order.
- A factory's `new` size is standalone proof of tail members nothing reads (`TKukkuManager` 0x64, `TKazekun` 0x1d4, `TLimitKoopa` 0x1c8; `getNameRef_Enemy` 8,392 B to exact). Nine inline `theNerve()` bodies were exactly `MarNameRefGen_BossEnemy`'s 0x6c `__sinit` gap (`LimitKoopaNerve.hpp` split).
- `TGraphTracer::traceSpline` returns `BOOL`, `TRailNode::mSpeed` is `u16`, `checkMarioVoicePlaying` returns `JAISound*`.
- Ruled out: a non-const `checkLiveFlag` overload (375 sites; 2 up / 32 down as a forwarder, 0 / 17 as a body; `hamukuri::isHitValid`'s residue is a frame gap, not the load). The `cDirtyFileName`/`cDirtyTexName` pair has no natural carrier header (include-closure intersection is empty): it is a rogue carrier like `DummyStrings.hpp`, ordered after `InfectiousStrings`' names in most TUs and before them in `MarNameRefGen_BossEnemy`.

## Settled in header round 7

- A `const` accessor can *cause* a CSE: `MActor::getCurBckAnmPtr` non-const (as the map says) with raw `mMActor` on both halves of a statement shares the member load; routing the receiver through `getMActor()` restores retail's two independent reads (`TChuuHana::setBckAnm` 95.7 -> 99.8). The const-accessor rule is about the re-read and cuts both ways.
- `TKukku::updateRotation` closed: the three statements retail's 15+ budget implied are `v.length()` split into `v.squared()` + `TUtil<f32>::sqrt(sq)` (also -8 of frame) and the two banking factors named per component because the ROM expands `isFalling()` twice (nerves 0 -> 68 and 42 -> 93). Real statements readable off the asm, not throwaways.
- `TPosition3` over `TRotation3` for a **3x3** member puts the empty `SMatrix33R<f>` ctor out of line (the map's weak 4-byte symbol is the tell), companion to the `SMatrix34C`/`BeeHive` case; the ROM's `getPos` loads contiguous rows `0x18/0x1c/0x20` for the sin term, i.e. `at(0,n)/at(1,n)/at(2,n)`.
- A `bl` to a global accessor whose result is immediately overwritten by `mr r3, rN` is a discarded receiver in front of an inherited static member (`SMSGetMSound()->setParamSoundOutputMode(mode)`); it proves `MSound : JAIGlobalParameter`. The same TU inlines the call away elsewhere, so its absence is not evidence against the spelling.
- `RAD_TO_DEG` is exactly `180.0f / M_PI` (`0x42652ee0`); the old `+ 0.000005f` was one ULP off. `ExPane::setCenteredSize` is correct (name fabricated, body right; inlined everywhere so no map symbol).
- `SMS_getNormalStage()` stays `u16`: `u32` gives the map's UNUSED 0x18 but loses three of four call sites; a case where the UNUSED *size* loses to call-site evidence.
- Ruled out: `JUtility::TColor(const GXColor&)` or `set(const GXColor&)` (costs `JSGSetColor` and `TMapObjElasticCode::draw` their exact match; both halves of the copy are load-bearing). MSL C-mode audit: `sinf/cosf/tanf/atan2f/powf` are real globals, `fmod` is never defined, `fabsf/fabs` are already inline for C; `sqrtf` was the only one.
- `turnToCurPathNode` returns `BOOL` (three sites, all up). `MSStageCubeFade`'s statics exist (`BeakDamage` 88 -> 97).

## Rules from Player sweep 2

- Bounded re-dispatch loops: `li rN, 0` in the pre-header plus `addi rN,rN,1; cmpwi rN,K; bgt exit` above the loop test is `if (++passes > K) break;`; two `addi`s around the `cmpwi` mean two increments (`for (i = 0; ...; i++) { if (++i > 4) break; }`, the `for` increment hoisted above the `bgt`).
- `fmadds` fusion needs register operands: `sqrtf(a*a + b*b)` over raw loads gives two `fmuls` + `fadds`; naming the components in `f32` locals fuses the *left* multiply with the right one's `fmuls` as the addend; first declared gets the higher FPR. Swapping terms only changes load order; the accumulate form does not fuse.
- `const T& x = obj->getStruct();` keeps a by-value return's temp out of the named-local region (tell: a 12-byte slot below the lowest named local); `T x = ...` adds a second temp and a word-wise copy.
- A struct-returning virtual is `r3` = hidden return pointer, `r4` = `this` (`lwz r12, 0(r4)` with `addi r3, r1, N` is not an out-parameter call). Decode the vtable before naming the slot.
- `Vec v = { a, b, c };` with non-constant elements emits an all-zero 12-byte `.rodata` template, block-copies it, then stores the components (six such objects in `MarioDraw`).
- Repeated `(u8)(s32)` float-to-byte conversions get hoisted (once, before the blocks); param `.get()`s do not, only their addresses (`addi r3, this, 0x2358` + `lbz`).
- The `-8/+8` for a branching bool predicate vs `return a == b;` reproduces exactly (`isSwimWaiting` 0x1c vs 0x14).
- A `weak` in-class body that retail *calls* at depth 1 exists: `TYoshi::onYoshi()` (0x1c, emitted from `MarioMove.o`, called by `TMario::onYoshi`). The statement-count table's "no limit at depth 1 for `inline`" has at least this exception; open.

## Rules from `MapObjDolpic` and `MapObjRailBlock`

- `__sinit`'s JAL-list order is reverse include order, and a duplicate include inverts it: an early `MSound/MSoundBGM.hpp` put `MSBgm` last where retail has it first (`__sinit` 9.7 -> 100 by removing it; the pair must be the *last two* rogue includes with no earlier copy). Check every nonmatching `__sinit` for an early `MSoundBGM.hpp`.
- `getMActor()` over `mMActor` permutes callee-saved registers with no frame change (`TDemoCannon::initMapObj` 99.2 -> 100): a codegen lever, not a frame one. Likewise a named pointer local is worth zero bytes but can fix allocation (`TGraphTracer* tracer = unk138;` put `this` in r30 and the tracer in r31 as retail).
- `MsAngleDiff` takes the goal first: the bounds `alpha - 180`/`180 + alpha` are built from the *first* argument, so the asm says which is `alpha` (`TMonumentShine::control` 97 -> 100).
- A named fetch is exactly one statement and that decides a call: `TRailMapObj::load` at nine statements inlined into `TWoodBlock::load` (depth 2, allowance nine); a named `TGraphWeb* graph` makes ten and `TWoodBlock::load` went 45 -> 100 with `TRailMapObj::load` byte-identical. Cleanest confirmation of the statement table in the *adding* direction.
- `TPosition3f` for a scratch matrix, second confirmation (`TNormalLift::setGroundCollision` 96 -> 100 with the out-of-line copy still exact).
- `&getPosition()` is a +8 frame lever that hoists the address; `getPosition().x/y/z` is +8/+16 with no instruction change; `unk8` raw over `getUnk8()` is -8. All per site.
- "Distance then step" blocks use one vector: copy the return buffer into a named displacement and subtract `mPosition` in place; the two-vector `sub(a, b)` spelling costs ~15 instructions.
- An unsigned 0x4330 conversion identifies a `u16` field (`TRailNode::mSpeed` at +0x12 is `u16`, not `s16`; open header item), and `TVec3::set(pitch, yaw, roll)` batches the three loads.
- Open header items: `TGraphTracer::traceSpline` returns `BOOL` (bare `mr r30, r3`), `checkMarioVoicePlaying` returns `JAISound*`. `TRailMapObj::initGraphTracer` still needs its pre-existing `#pragma dont_inline`: retail's body had 15+ statements, ours ~7 at 99.9% (removing the pragma drops `load` to 0).

## Rules from `GCConsole2` (GC2D)

- `TFlagManager::getInstance()` vs `smInstance->` is a codegen lever: the accessor materialises the pointer into `r0` and adds `mr r3, r0` at a function's first call (`checkDolpic8` 96 -> 99 with `smInstance`); per function.
- A `static inline` wrapper costs exactly one level, usable on whole functions: `startAppearTank()` (292 B) behind a one-line `updateTankAppear(console)` stays a `bl`. The converse is the systemic cost of decomposing a big function into fabricated helpers: everything inside sits one level deeper than retail.
- `JUTRect r(other)` emits `bl JUTRect::copy`; `r = other` four `lwz`/`stw` pairs; several copies landing on one slot is construct-then-assign.
- `pane->setWhite(JUtility::TColor(r, g, b, a))` is the shape behind `bl JUtility::TColor::set(u8, u8, u8, u8)` (three levels); a `mWhite = 0xRRGGBBAA` store hides the symbol.
- Leading `@NNNN` `.data` objects may be temporaries your own code already emits: three fabricated dead arrays duplicated `@163`/`@154`/`@134` and shifted every later offset by 0x28. Check the object's own symbol table before fabricating.
- A member reread across a call cannot be replaced by a by-value helper parameter (`countShine` reads `unk170` three times because each `changeTexture` invalidates it).
- `u8 done = 1; done = done & pane->update();` reads off as `and`; the `if (!update()) done = false;` form as `clrlwi.` + branch.
- Case-label order decides block order in a compare-tree switch (`case 3, 2, 1, default`); an explicit `case 0:` grouped with `default:` gives `cmpwi r0, 0` rather than `1`.
- Reordering a decomposed `perform` back into the ROM's block order is the single highest-value edit (49 -> 70 in one edit).
- Open header items: `TYoshi::onYoshi()` carries eight `(void)0;` as an anti-inline hack (a fakematch); `GCConsole2` needs the unpadded predicate (no null test on `mYoshi`, `cmpwi` BOOL) and parks a local. `SoundEffects.hpp` lacks `0x405C`.

## Rules from `hamukuri`

- A one-more-level `getManager()` forwarder is a per-class frame lever: `(THamuKuriManager*)TLiveActor::getManager()` instead of `(THamuKuriManager*)mManager` took `makeCapFly` from a 64-byte gap to exact and made three more functions exact in one edit.
- `MsRandF(lo, hi)` is never the original; it is `TMsRange<T>`: literal bounds stored to the stack and reloaded, with `range = mMax - mMin` computed before `rand()`, is a local `TMsRange<f32> r(0, 1); x = r.rand();` (four sites; three functions to exact). `MsRandF(l, r)` contracts to `fmadds` and never materialises the bounds.
- A float flag tested for truthiness, not against `0.0f`: `if (!unk21C)` gives `fcmpu <value>, <0.0>`; `== 0.0f` gives the operands the other way (six sites in `walkBehavior`; per expression).
- An UNUSED bool predicate a caller "pastes" is usually meant to be *called*, and the call is the frame lever: `canGoForSearchActor()`/`isSerialWallDie()` kept every instruction and took `setSearchHamuKuri` (708 B) to exact; `releaseCap()` is a call in two `setDeadAnm`s and pasted in `setCrashAnm` (per site).
- `setGoalPath((THitActor*)gpMarioAddress)` directly, not `setGoalPathMario()`: the wrapper puts the `TPathNode` temporary in the expansion region, the direct call in the named-local region.
- The params getter wrapper is a +8 lever only sometimes (`moveObject` exact, `bind`/`forceRoll`/`generateBody` unmoved); routing every param read through a cached `getSaveLoadParam()` converted three exact functions into +8 ones, so the pakkun rule is per TU.
- Open: ~35 instruction-identical functions with 8/16/24-byte short frames and no identifiable cause; minimal case `THaneHamuKuri::attackToMario` (two statements: `SMSGetMSound()->startSoundActor(...)` plus one virtual call, ROM 16 bytes of temporaries vs our 8; named `MSound*`, an extra wrapper level and accessor reads all moved nothing).
- Open header items: `JGVec3.hpp`'s `operator*(TVec3, f32)` should return `const TVec3&` like its neighbours (fixes `forceRoll` 90 -> 99.7 but regresses ~15 functions in 8 units; per-site story needed); `checkLiveFlag(u32) const` blocks CSE with the non-const `onLiveFlag` (all three `isHitValid`s load `mLiveFlag` once; a non-const overload would fix it).

## Rules from `ShadowUtil` and `ModelWaterManager` (GX)

- Per-component member assignment vs `set(x, y, z)` is a frame lever, not just store order: `set()` on a stack array element reserved three slots per call (`calcDrawVtx` 48 bytes over), and per-component stores also force the member re-reads retail has.
- A double literal in a float expression shows three ways: an `lfd` of an 8-byte `@NNNN`, `fmadd`/`fmul` instead of the `s` forms, and a trailing `frsp`; in the data listing as a 4-byte `.sdata2` constant missing or at 66-75%. `2.0` -> `2.0f` was +28 and +10 points on two matrix builders.
- Six scalar min/max accumulators seeded from `p[0] ± 1.0f`, not `TVec3::setMin/setMax` (which forces memory) (`calcWorldMinMax` 10 -> 100).
- The stretch factor `(1.0f / MsSqrtf(speedSq)) * (1.414f * 0.5f * size)`; `vtx[4]` declared first in the loop body; `TDLTexQuad` slots 0x10/0x14/0x18 are `reset`/`request`/`setEnd`.
- A rotation clamp with an extra `fmr` is `f32 rotY = angle; if (rx > rz) rotY = angle - 90.0f;` (ternary 96.1, `-=` 97.4, this 98.7).
- `JUtility::TColor(GXColor)` takes its argument by value, costing a third dead slot at every `GXSetChanMatColor(..., TColor(c))` site; retail has two. A `const GXColor&` parameter would fix it (global; open).
- `new J3DMtxCalcBasicAnm(...)` in a dead constructor instantiates nine weak J3D symbols byte-identically; a `.sdata2` order (`90.0f` before `0.08f`) can fix source statement order.

## Rules from `bossgesso`

- A missing base-class initialiser shows as a string in the wrong section: omitting `THitActor(name)` never emits the TU's `"胴体"` (`.sdata2`) and adds an extra `"HitActor"` in `.rodata`. Literals of 8 bytes or less land in `.sdata2`, longer in `.rodata`.
- A covariant override of a virtual accessor silently steals its vtable slot and, spelled `return (T*)getSaveParam();`, recurses forever; visible only as a 99% vtable. Use the established `getSaveParam2()` name.
- `startAppearBalloon` takes small message ids (`idx = id == 0x25 ? 3 : id - 3; flag = id == 3 ? 0 : 1 << idx`), not `0xE00NN`.
- `TPosition3f`, not `TMtx34f`, when retail calls the empty `SMatrix34C<f>` ctor for a member (`init` 97 -> 99.9; the map's single `TPosition3<...>` ctor reference corroborates).
- `member = a - b;` keeps `TVec3::sub` out of line where `d = a; d -= b; member = d;` expands it (`bind` 87 -> 99.6).
- `if (a < N || pred()) return;` gives `blt RET` plus the unfused `clrlwi.; beq CONT; b RET`; two `if`s fuse both.
- A named local for an inline's argument defers the arithmetic into the callee: `f32 s = p->mSLSightAngle.get(); inSightAngle(0.5f * s)` puts the `fmuls` at the comparison (four functions 96 -> 99.6+). The params `getSLFoo()` wrapper had no frame effect here, so the `bombhei` ±8 rule is per site.
- The `getLatestNerve()` wrapper vs `mSpine->getLatestNerve()` chooses evaluation order in a nerve comparison: wrapper emits the `theNerve()` guard first, the direct read the spine load first; both occur in one TU (`doAttackShoot` 89 -> 99.6).
- A jump table indexing the raw value with no bias needs `case 0: case 1: case 4: break;` present to be an 8-entry table.
- `setGoalPath(const TPathNode&)` with a `TVec3` argument is one 16-byte `{null, vector}` temporary copied into both path members plus a zero store at the count field.
- `TBossGessoTentacle::isThing()`'s `subi 3; cmplwi 1` merge after peeling `6` is an optimiser artefact (bombhei class); grouping, nested ifs and a switch all fail.
- Open header items: `MSStageCubeFade` needs `static MSStageCubeFade* smInstance;` and `static void setBgmVolumeForce();` (map symbols; `BeakDamage` nerve 87 -> 96 measured); `SMS_GetMarioPos()` (8 B weak) is `bl`-ed inside retail's inlined `doAttackGuard`; `lenFromToeToMario` fuses `x * x` into `y * y` with `fmadds` where a stack-homed local leaves three `fmuls`.

## Rules from `bosspakkun`

- Constant-first nerve identity tests: `&Nerve::theNerve() == spine->getLatestNerve()` is this TU's spelling; the natural order hoists the `mSpine`/`getLatestNerve` loads above the `theNerve()` static-init guard (`setGroundCollision` 56 -> 72 from the swap alone; applied to 14 sites). A named `const TNerveBase*` local flips the `cmplw` operands the other way.
- One body statement can push a constructor out of line: splitting `search<T>(...)->getChildren().push_back(this)` into a named `group` plus the `push_back` made `TBPPolDrop`'s 79-instruction ctor retail's `bl` (`init` 85 -> 99); moving six initialisers into the body did not, so the statement budget counts body statements, not initialiser-list entries.
- A dead inline expansion survives only through a reassignment: `f32 a = MsAngleWrap(helper()); a = helper();` keeps both expansions and the wrap loops; the bare statement pushes the helper out of line; dropping it loses the loops.
- The water-gun actor types `0x1000000D` (nozzle hit actor) and `0x1000001` (droplet actor) are always compared directly (`getActorType() ==`, no materialised bool; `receiveMessage` 77 -> 97).
- `isAirborne()` over `checkLiveFlag(LIVE_FLAG_AIRBORNE)` where the ROM materialises and tests with `cmpwi`.
- A shared `FALSE` exit: `else if (!(a && b && c)) mIsMarioRiding = 0;` lets both chains fall into one `li r3,0` (`checkMarioRiding` exact).
- Recompute a named float before each gate check (`-mVelocity.y` reassigned before the second `gateCheck`) to get the ROM's two `f31` round trips.
- Full-width Shift-JIS parentheses (0x8169/0x816A) in `"シャイン（ボス用）"`: ASCII is two bytes short. Two block-scope `static const TModelDataLoadEntry entry[]` in one function must both be named `entry` (`entry$3613`/`entry$3618`).
- `startTornadoBlur` (0x58) is only the two `ms_bopa_blur1` emitters; including the `swing1` one makes it 0x78: the map size prunes helper contents.
- Open: `TSpineEnemy::turnToCurPathNode` returns `int`/`BOOL` (ROM `cmpwi r3, 0`, ours `clrlwi.`) — needs a sweep; `getLatestNerve` through `mOwner->mSpine` is a `bl` while through `this->mSpine` it inlines (receiver depth); the `set<f>` sites that retail calls pass computed products where Kazekun's inlined ones pass constants (possible discriminator).

## Rules from `MapObjCorona`

- The MSound rogue-include pair has an order per TU: read the JAL-list order off the target `__sinit` (registrations run in reverse declaration order) before choosing `MSSetSound.hpp` before or after `MSoundBGM.hpp` (97.6 -> 100 here; `Talk2D2` needed the same order, `MapObjWave`/enemies the other).
- A TU-local local-yaw helper `s16 getDir(MtxPtr, const TVec3&)` = `getXDir/getZDir/getTrans` + `sub` + `matan(z.rel, x.rel)`, with a three-argument form adding `radial.setLength(rel, 1.0f)` and `tangent.scaleAdd(-radial.dot(offset), offset, radial)` (`getNearGrip` 71 -> 98, `getNextGrip`/`getNextJuncture` 1 -> 98). Naming its two dot products is worth 8 bytes of the *caller's* frame and the `s16` result at the site another 8.
- Statement count, not size, decides a 300-byte helper: with two named dot locals MWCC refused to inline `getDir` (45.8%), without them it expanded (97.8%) with an identical instruction stream.
- A lone `bl TUtil<f32>::inv_sqrt` guard with no stores is `dir.sub(a, b); dir.y = 0.0f; dir.normalize();` on a vector nothing reads: the `y = 0.0f` propagates into `dot` and gives the `lfs 0.0; fmadds` opening; the constructor and `set(x, 0.0f, z)` lose it.
- `1.0f - m.at(1,1) * m.at(1,1)` does not contract; `f32 c = m.at(1,1); 1.0f - c * c` gives `fnmsubs` (extends the `fp_contract` locals rule to an inline accessor's result).
- `setQT` is one level too deep for `setQuat` here: `mtx.setQuat(q); mtx.setTrans(p);` spelled out took `TBathtub::calcRootMatrix` 51 -> 88 (mirror of the BeeHive case; per site).
- Surviving `* 0.0`/`* 1.0` products in a concat prove the right operand is a real local matrix built by `setEularY(a)` + `setTrans(0, 0, 0)`.
- Open: `TBathtub::hipdrop` is instruction-exact with a 16-byte gap, and closing it does not stop retail's two `receiveMessage` callers from inlining it (both lose to that one expansion); `TKoopa::allowsLaunch()`/`effectsTumble()` return `bool` (callers `clrlwi.`).

## Rules from `bosstelesa`

- Three `rand()` results into one vector: per-component `v.x = r.rand(); v.y = ...` stores each result straight to its slot as retail does; the constructor form batches (`TBubble::split` 85 -> 100). A `TMsRange` used in a loop is declared *before* the loop.
- A member typed `TPosition3f`/`TMatrix34` forces the inner matrix ctor out of line (`bl __ct__TMatrix34<SMatrix34C<f>>`) and gives the `new` result its stack home (`createEnemyInstance` 71 -> 100). `TPosition3::translation(x, y, z)` puts `identity33()` at depth 2 where retail calls it here, while `TabePuku` wanted it spelled out: both directions occur, per site.
- `SMatrix34C<f32>` member, not `Mtx`, keeps `gekko_ps_copy12` a `bl` through `set(ConstArrType*)` (`getTakingMtx` 7.5 -> 100).
- `u32& flag = mLiveFlag` before a dead test reproduces retail's materialised `addi rN, obj, 0xf0` (`flashItem` 83 -> 96); it backfires when the test immediately follows (MWCC fuses it into `lwzu`).
- Repeated `x[i] != 0.0f` tests are an unrolled loop (hoisted constant, base reloaded per iteration), not three ifs (`isForceRestart` size-exact only as a loop).
- Dead code is real: `rouletteStart` opens with an unused spin-count loop (90 -> 99.5 written out); two nerves build a Mario-relative vector they never read.
- Three more UNUSED-as-level instances: `fanfale` (496, size-exact) must be called so `getSlotResult` stays a `bl`; `getDrumResult` (44) is what `getSlotResult` calls; `slotStart`/`slotStop` are the `checkPass(53.0f)` body and the `isRollDrum` guard.
- A nerve pushed via `pushNerve` reaches `TNerveBase`'s ctor one level deeper than the same nerve in a comparison (retail `bl`s it). `default: return;` with two ids sharing a case in a fruit switch. `TNerveBubbleLive::theNerve()` sits before `TBubble::appendItem` in the map, so that nerve cannot come from `DEFINE_NERVE`; its halves are spelled out around the helpers.
- Open header items: nine `TMapObjBase` virtuals are declared only (`getRadiusAtY`, `getTakingMtx`, `setModelMtx`, `loadBeforeInit`, `calc`, `draw`, `dead`, `getHitObjNumMax`, `touchWater`); the map has them weak (8/52/56/4/4/4/4/8/8) in `bosstelesa.cpp`, bodies readable at 0x800C6E94 onwards; `MActor::getBckAnmPtr()` is really `getCurBckAnmPtr()` (UNUSED 0x1c); `getObjNumWithActorType` returns `u32`.

## Rules from `CardLoad` and `CardSave` (GC2D)

- **When MWCC emits a jump table** (scratch TU, game flags): at least 6 distinct case blocks *and* a label range of at least 8; otherwise a compare tree. A contiguous trailing group of empty cases (`case 5: case 6: case 7: break;`) folds into the default's range test and gives a tree, while an empty label with a gap below it (`case 7: break;`, labels {0,1,2,3,4,7}) keeps the 8-entry table (`titleDraw` 78 -> 98). `default:` on the empty group and an unsigned switch variable change nothing.
- A `.data` jump table is a structural oracle: its entry count fixes the maximum label and each relocation names a block, so pairing the target's table against ours finds missing cases, wrongly merged groups and block order (`changeScene` 94.8 -> 99.96 in one edit; the `@NNNN` size delta is the first hint).
- GC2D message text comes from the bank, not literals: `SMSGetMessageData(bmg, id)` before `getStringPtr()` in a `strncpy` is the `setMessage(box, size, id)` inline; an extra `.rodata` string shifting later offsets is the tell (36 sites; both pools byte-identical).
- Accessor levers cut both ways: `SMSGetMarDirector()->getGamePad()` over `gpMarDirector->unk18[0]` is +16 (`TCardSave::load` exact); `->mPane->hide()` over `->getPane()->hide()` is -12 (the accessor on the opening `show()`, the raw member on the closing `hide()`). `s8 result = -1;` removes the `extsb` at an `s8` return; a named `s16 timer = unkB4; ... unkB4 = timer + 1;` removes a second `lha`.
- The US title screen has 18 panes (`TITLE_PANE_COUNT`), giving the 9-way `mtctr 2` loop and moving the `u16` counter to 0x27A (`char unk278_[2]` under `VERSION_GMSE01`).
- Dead code really in the ROM: `execMovement_`'s `PROGRESS_UNKA` guard that cannot hold; `changeScene`'s `PROGRESS_UNK26` if/else arms identical; `selectFunction`'s caption loop writing one shared box pair.
- Open header items: `JSUOutputStream` needs a by-value `int write(u8)` (each write of the same byte gets its own slot); `JSURandomInputStream` needs `bool isDrained() const { return getLength() - getPosition() == 0; }` (retail reaches both through *virtual* calls on a local `JSUMemoryInputStream`, i.e. through the base; the fabricated `JSUMemoryInputStream::isNotDrained()` is on the wrong class); `StageUtil.hpp`'s two tables were function-local statics (retail `CardLoad.o` has neither); retail calls `JSUInputStream::JSUInputStream()` out of line inside the inlined `loadBookmark()` (the one MISSING symbol; declaring the ctor does not emit it).

## Rules from `Option` and `ConsoleStr` (GC2D)

- `ary.begin()`/`end()` over the raw `mData`/`mSize` members costs one 4-byte temporary each per inline expansion and no instructions: an accessor-using inline expanded twelve times added exactly 96 bytes of frame and made `adjustView` exact. A gap that is a multiple of 8 x expansion count is this.
- An UNUSED wrapper can exist purely to demote a set of large calls, and the split point matters: `TOptionControl::movementCommon` holds the three unit `update()`s at depth 2 (refused, the ROM's three `bl`s) while `mBackArrow->update()` must stay at depth 1 or its helpers drop out of line (`movementOption` 38 -> 97).
- `ArrayWrapper<const T>` is the wrong instantiation: the map's `begin__...ArrayWrapper<Ul>` says `ArrayWrapper<u32>` with a cast at the call. Check a stray `const` template argument before anything else when a container's `begin()` is MISSING.
- `s16 x = f() - k;` emits two `extsh`; `s16 x = f(); x -= k;` emits one (`subi` then a single `extsh.`). The `u16 + k` counterpart needs one `clrlwi` either way.
- A literal the ROM converts at run time (`li 0xe0` + magic conversion, `subfic r0, r28, 0x1d0`) is a named local declared inside each branch; at function scope it takes a callee-saved register and displaces `this`.
- A dead call is real code and its shape identifies the caller: a discarded inlined `isChangedSetting()` folds away except the compare and `bl typeToFlag`; a discarded `getValue()` leaves a stray `bl ArrayWrapper::begin()`.
- An *empty* `else if (x < 95.0f) { }` produces the bare `fcmpo; blt end` where `else if (x >= 95.0f)` adds a `cror`.
- A member read once and used by two switches is a local held in `r3` across both trees.
- A named `JUTRect` local keeps `JUTRect::JUTRect(int, int, int, int)` a `bl`; as a temporary in `setBounds(JUTRect(...))` it expands to `bl JUTRect::set`. `A + B + C` with contracted products accumulates from the *last* term.
- `TBoundPane`'s own target rect (+0x14/+0x18), not the `J2DPane`'s `mBounds`, is what a materialised bool reads; `getPane()` adds a `lwz`.
- `TBalloonControl` does not exist in GMSE01 and `TOptionSubtitleUnit` (the US subtitle row) does; both were proven by allocation sizes and the map's closure. US layout differences keep turning up in GC2D: check `region-us.md` and the `new` sizes first.
- Open header items: `StageUtil.hpp`'s `scScenarioNameTable` is the Japanese table (the US one skips ids 8/9 and shifts later groups by two; the 70 values are in `ConsoleStr.cpp`'s TODO), and `SMS_getNormalStage()`'s `u16` return adds a `clrlwi` (0x1c vs 0x18); `MSound` may inherit `JAIGlobalParameter` (a dead `bl SMSGetMSound` before `bl setParamSoundOutputMode` is `SMSGetMSound()->setParamSoundOutputMode(mode)` on an inherited static); `math.h`'s `RAD_TO_DEG` fudge (`+ 0.000005f`) is one ULP off the ROM's `57.295776f` and should go; `ExPane.hpp`'s `setCenteredSize` is correct (its "fabricated and incorrect" comment is wrong; `processReady` matched with it).

## Rules from `Talk2D2` and `hx_wiper` (GC2D)

- A ternary over two calls suppresses inlining that an if/else does not: `if (mIsBoard ? openBoardWindow() : openNormalWindow())` leaves both as `bl` with a shared `clrlwi./beq`; the if/else form inlined the 228-byte board path (`perform` 66 -> 82).
- Spelling an UNUSED helper's body out at its call site can be what keeps the *caller* out of line: calling the size-exact `closeTalkWindow()` shrank `checkBoardControler` enough for MWCC to inline it into `perform`; pasting kept it a call (95.8).
- A one-case `switch` is the shape behind `cmpwi` + `beq body; b end` (`switch (gpMarDirector->unk124) { case 2: ... }`, not `== 2` on a `u8`).
- A `u32` member holding a colour, not `JUtility::TColor`: passing it to a by-value `TColor` parameter costs two stack stores where a `TColor` member costs one; `cColorTable` is `u32[6]` (a `TColor[6]` needs dynamic initialisation and shows in `__sinit`).
- `__sinit`'s JAL-list order follows rogue-include order: `MSound/MSSetSound.hpp` before `MSound/MSoundBGM.hpp` or the fifteen registrations rotate by one.
- `arr[a + b + k]` with the literal last folds `k * 4` into the displacement; in the middle it costs an `addi`; `(arr + a + b)[k]` is worse.
- The signed/unsigned int-to-float magic constant is a type oracle: `0.7f * width + 4.0f` with the unsigned magic means the raw `u8 TWidth` field, not `getWidth(int)`.
- Rounded coordinates want named `s16` locals (the nested `move((s16)..., (s16)...)` evaluates right to left).
- `snprintf(box->getStringPtr(), 94, fmt, ...)`, not `setString(fmt, ...)`, for J2D text boxes in this subsystem (`moveTalkWindow` 84 -> 93.5).
- `fcmpo` + `cror eq,gt,eq` + `bne` is `>=`, which m2c drafts as `==`.
- A `.float` of a tiny denormal in a mixed static table is an `s32` read with `lwz` (`drawpath_table` is `{ f32, f32, s32 }`); `$NNN` suffixes on local statics are monotonic in source position, so they prove definition order and reverse emission for a C TU.
- `hx_wiper` (C) has nothing inlined in retail (an empty `Hx_Warning` is still called): measured `-inline noauto` for that object takes it 9 -> 16 exact on the same source. `math.h`'s `sqrtf` body sits behind `#ifdef __cplusplus`, so C TUs get an out-of-line call where retail inlines it (open).

## Rules from `BathtubKiller` and `hauntLeg`

- A discarded call whose body opens with a null test can account for an entire frame gap: a bare `getActiveObjNum();` (its inline opens `if (!unk38) return getObjNum();`) reproduced both the dead `lwz`/`cmplwi` and the whole 32 bytes (`loadAfter` 84.5 -> 90.6).
- Param defaults are the constants stored to `param + 0x10` in the *first* pass of the params constructor; the post-`load()` `.set()` overrides are the second pass and park their literals in `.sdata`. Twenty-five wrong defaults hid a 1,832-byte constructor; a missing 4-byte `.sdata2` constant among a params class's names is a wrong default.
- A POD member goes in the initialiser list when the ROM stores it before a member's constructor call (`: TSmallEnemy(name), unk1CC(nullptr)` before `bl TMatrix34::TMatrix34()`; `createEnemyInstance` 76 -> 100).
- An empty counted loop unrolls unless it counts down: `for (int i = n; i != 0; --i) {}` gives the bare `mtctr`/`bdnz` (`checkHit` 0x18, size-exact).
- `normalize()` over `setLength(1.0f)` when the ROM hoists the 1.0f into a callee-saved register: `normalize()` goes through `TUtil<f32>::one()`, whose bound temporary survives the `dot`/`inv_sqrt` calls (Straight nerve 75 -> 87).
- `v.scale(k); dst.set(v)` stores back into `v` then copies interleaved; `dst.scale(k, v)` multiplies straight into `dst`'s stores. Both occur in one unit.
- Name the length before the parameter fetch (`f32 speed = v.length(); f32 max = p->get();`) to put the inlined `length()` before the virtual `getSaveParam()` (`makeInitialVelocity` 87 -> 96).
- `operator-` into a *named* `TVec3` keeps `TVec3::sub` out of line where `TVec3<f32>(a - b).length()` expands it (Haunt nerve 80 -> 96): which spelling you want depends on whether `sub` or `sqrt` is the out-of-line one.
- Two `li r3, 0` blocks mean two `return false;` statements. `TPosition3::translation(x, y, z)` binds the three floats before the out-of-line `identity33()`; the `TVec3&` overload loads after.
- `theNerve()` expansion is emergent per function from identical source: retail inlines the first nerve comparison in three functions and calls it in two others, emitting `bl TNerveBase<TLiveActor>::TNerveBase()` inside the expansions. Every instruction matches; the artefact is the whole residual (54-79%). A `TSpineBase::isCurrentNerve()` wrapper does not explain it.
- Header layout tells from `hauntLeg`: `getTakingMtx` is virtual (slot 0xa4); an override returning `BOOL` where the base says `bool` loses the slot; `entry$NNNN` non-`const` lands in `.data`.

## Rules from `pakkun`

- A `const` accessor can *cost* a match in a constant multiply: `3.0f * self->getTurnSpeed()` loads the member into the destination register; `3.0f * self->mTurnSpeed` loads the constant first (Hide nerve 99.2 -> 100).
- `TBGCheckData::getNormal()` restores per-use re-reads that raw `mNormal` CSEs away (`behaveToHitWall` 84 -> 100, with `-(1.5f * v.dot(n))` + `+=`: the `fneg` is the tell).
- Two `getSaveParam` families: `bl TSpineEnemy::getSaveParam()` is the non-virtual `getSaveParam2()`; `lwz 0(this); lwz 0x108; blrl` is the virtual `TSmallEnemy::getSaveParams()`. Shadowing `getSaveParams()` in a derived class hides the virtual; a separate `getSaveLoadParam()` for the cached pointer is correct and worth +8 of frame over the raw member.
- An explicit base call can be what an UNUSED helper exists for: `TPakkunSeed::seedSet` is `TEnemyAttachment::set(); mScaling.x = mScaling.y = mScaling.z = unk164;`, exactly 0x3c (a virtual dispatch would be 0x48).
- Chained-assignment direction is readable: `stfs 0x2c, 0x28, 0x24` is `x = y = z = v`.
- A named `MtxPtr` for a hand-built local matrix keeps its address in `r31` across two `MTXConcat`s (+8 of frame); at other sites in the same file retail recomputes `addi r4, r1, N`, so per site.
- `getMActor()->getModel()` inside a loop body reloads the model from `mMActor` where `mMActor->getModel()` CSEs it from the condition (`init` 98 -> 99.9).
- `else if` chain with one trailing `return FALSE` vs early returns is visible as shared vs duplicated `li r3,0`; a null test merged into an `||` guard shares the `li r3,1` exit.
- `TPakkun::onShootCurve` (0xfc) is size-exact yet pasted at all three lob sites (pasted-UNUSED again).
- Open header items: `TSmallEnemy::initAttacker` is `(func,weak)` + `UNREFERENCED DUPLICATE`, i.e. defined in the header, not `smallenemy.cpp`; `TSpineEnemy::getMaxHitPoints()` returns `u8` (proven by a `clrlwi` and a `divwu`) but the header change costs `TEffectEnemy::perform` and `TFireWanwan::moveObject`, so it needs a sweep; the duplicate pakkun nerve stubs in `bosspakkun.cpp` must go when `pakkun.o` links (their `.sbss` match is vacuous).

## Rules from `Koopa`

- Get the shared animation setter exact first: `J3DFrameCtrl* ctrl = getMActor()->getFrameCtrl(t); ctrl->setRate(expr);` (the chained form evaluates `expr` first) plus `table == nullptr ? nullptr : table[i]` took `TKoopa::changeAnm` 76 -> 100, and because it inlines at ~15 sites the unit went 22 -> 84% in one edit.
- A `switch` is the only way to get a signed compare on a `u32` message parameter (`case HIT_MESSAGE_...: return FALSE;` gives `cmpwi`; `==` gives `cmplwi`). A dense three-case set is a switch too (`idx == 3 || 4 || 5` folds to `subi`/`cmplwi`; the switch keeps `cmpwi 6; bge; cmpwi 3; bge`).
- For a dense jump table the table is in case order and says nothing about source order; the code blocks do (`getNeckFocus` blocks 6, 2, 0, 1, 7, 9, 8, 14, 4, 5, 3, 12; reordering alone 78 -> 98).
- `x = <other arm>; if (cond) x = <this arm>;` is how retail hoists a ternary's other value into the result register; both ternary spellings add an `fmr`.
- A dead assignment of the value a variable already holds keeps an empty `if` arm from being inverted (`if (frame <= 200) { focus = 1.0f; break; }` gives the unfused `bne +8; b end`).
- Pasted-UNUSED, fifth and sixth: `TKoopa::breathFlame` (0x1c0) and `resetFlame_` (0x9c) written out inside `setUpHitActors` (37 -> 98).
- An `||` whose second operand is compound is an inlined bool helper and the map size names its shape: `isProvoking` (0x78) only as nested ifs to one shared `li 0`.
- Two bools for a two-step predicate declared together give `li r4,0; addi r5,r4,0`. Read `mSpine` directly in a nerve-identity test; a local flips the `cmplw` operands.
- Nerve destructor size is a depth oracle: 0x5c for a direct `TNerveBase<T>` child, 0x6c through an abstract intermediate. A nerve whose `execute` is *weak* in the map had its body in the class, which forces the nerve set into a header that can see the actor; and inline `theNerve()` bodies in a header another TU includes cost that TU 12 bytes of `.bss` per nerve (broke `__sinit_koopajr_cpp` until the nerve header included the actor header, never the reverse).
- `u8`, not `bool`, for a byte-returning collision query kept in a local, or MWCC adds `neg/subic/subfe`.
- Open header items: `TMatrix34<T>::concat` bodies are wrong (indices transposed, reading past a 3x4); the ROM form is `r[i][j] = a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j]`, `r[i][3] = a[i][3] + (...)` into `SMatrix34C::set(12 floats)`. `LIMITKOOPA_ANM_*` is mostly wrong; `koopa_bastable` names the shared model's slots (DOWN 0, DOWN_WAIT 1, FALL 2, FIRE_END 3, FIRE_LOOP 4, FIRE_START 5, FIRST 6, GETUP 7, HIPDROP 8, STAGGER 9, TURN_L 10, TURN_R 11, WAIT 12, 13, WATERHIT 14), and `TLimitKoopa::getNeckFocus` returns `f32`. `TSpineBase::pushNerve` stays a `bl` inside the ROM's inlined `TKoopa::stagger`.

## Rules from `SelectShine2` and `Guide` (GC2D)

- A member-initialiser list moves an array member's `__construct_array` after every scalar store; assignment in the body constructs the array first. It can also place a scalar store *between* two `JUTRect` member constructions (`TGuide::TGuide` 78 -> 100, `TSelectShineManager` ctor 76 -> 100).
- `return T(args...)` vs a named local plus `return` is a size oracle for a by-value UNUSED accessor (`getPosition` 0xa8 vs the map's 0x8c); the caller keeps one extra 12-byte temporary either way.
- Two unnamed `TVec2` temporaries as inline arguments keep literal products in registers; a named axis local is materialised and read back (the opposite of the `TVec3` rule from `wireTrap::load`).
- A `u16` copy of an `s16` timer parameter, not a cast at the modulo: `u16 t = timer;` removes an `extsh` at every inline site (`linkSelect` 88 -> 94).
- Drop `T& ref = arr[i]` for a small member array: retail computes `this + i * stride` and folds the field offset into each load.
- A clamped value that is both stored and accumulated wants its own local (`resetObjects` 93 -> 95); same for `x / 100` used twice.
- `u8 acc = 1; acc = acc & f();` keeps the raw `and` with `clrlwi.` only at the test; `bool` adds normalisation.
- An explicit bound check before a jump-table switch duplicates the table's own `cmplwi; bgt`; and spell the early exit as a real `else { return; }` to get retail's pair of branches, one dead (`perform` 73 -> 79).
- Pane-tag arithmetic digit by digit: sequential panes add the counter to the low byte (`'ss_1' + i`); two-digit groups are `((i / 10) << 8) + (i % 10 + '00')`; the four-character form needs `('00' << 16)`.
- Open header items: `JGVec2.hpp`'s float copy constructor and `operator=` exist in SMS (retail `lfs`/`stfs` for all three copies in `getAngle`; the header's "SMS didn't have them yet" note is wrong); `StageUtil.hpp` needs `static u32 scNormalStageTable[] = { 0, 1, 2, 3, 4, 0xD, 6, 8, 9, 0xA };` after `scScenarioNameTable` (map: five TUs); `TExPane::setPaneSize/setPaneAlpha/setPaneOffset` are called out of line by the big `TGuide::perform` (caller-size family).

## Rules from `bosswanwan`

- `MsSqrtf` is recognisable by its guard: `frsqrte` with one double-precision refinement, a `stfs`/`lfs` round trip, and a bare `fcmpo; ble` skipping it (no `cror`) is `MsSqrtf(x)` inlined whole, the `x > 0.0f` test included (`shakeCamera` 78 -> 96). A single-precision refinement with `cror eq,lt,eq` is `TUtil<f32>::sqrt`, i.e. `v.length()`.
- Yaw wrap depth is per site: `MsWrap(x, 0.0f, 360.0f)` written out inlines the loops, `MsAngleWrap(x)` keeps `MsWrap<f>` a `bl`; in one TU both occur (`slideToCurPathNode`/`control`/`TBWLeash::perform` direct, `TBWBinder::bind` wrapped). `MsAngleDiff`'s internal `MsWrap` stays a call everywhere.
- Turn clamps want ternaries (`turn = turn > limit ? limit : turn`); `if (diff <= limit)` adds `cror` per arm.
- The three `TVec3` copy shapes decide many diffs: integer `lwz`/`stw` is `a = b` or the copy constructor; `lfs`/`stfs` per component is `a.set(b.x, b.y, b.z)`; interleaved `lfs`/`stfs` is `a.set(b)`. Nine sites in this unit.
- `SMatrix34C<f32>::zero()` fills column by column (`[2][3],[1][3],[0][3]`, then columns 2, 1, 0); a sequential row-major fill is `set()` with twelve arguments. A scratch `SMatrix34C<f32>` plus a named `MtxPtr` from it holds the address in a callee-saved register; a `bl SMatrix34C<f>::SMatrix34C()` at a member's construction means the member is `SMatrix34C<f32>`, not `Mtx`.
- `MsGetVecFromRotY` by value with `JMASSin(DEG2SHORTANGLE(x))` (the `fruitsboat` form) beats the by-reference form; `MsSin`/`MsCos` inside it push the lookups out of line.
- A materialised bool retail lacks means the original compared inline (`getActorType() == 0x80000001` at four sites, 68 -> 96); retail *does* materialise for `0x4000005A`, so that one is `isActorType`. `cmpwi r0, 0` after a materialised predicate means it returns `BOOL`.
- `if (guard) { ...; } return FALSE;` branches to the shared `li r3,0`; `if (!guard) return FALSE;` emits its own copy plus a `b`.
- A transition that writes `mPrevious`/`mCurrent`/`mTime` with no stack push and no null test is `TSpineBase::setNext`; `pushNerve` has both.
- Jump nerves: name the target (`const TVec3&`), the speed and the velocity, and store the velocity *after* `setGoalPath` (four nerves 92-95 -> 97.8-99.8).
- A named `J3DFrameCtrl*` before `setRate(SMSGetAnmFrameRate())` puts `getFrameCtrl` before the rate call.
- A dead computation can be load-bearing: `TBWBinder::bind` subtracts two graph points into an address-taken vector it never reads; the stores survive. `next - actor->mPosition` keeps `TVec3::sub` out of line there.
- Pasted-UNUSED again: `TBossWanwan::takeBath` (0x184) is pasted into the Die nerve (36 -> 96); its standalone copy then expands `changeBck` and `setMtx` (0x27c).
- Open header items: `TSpineBase<T>::getLatestNerve()` is *called* by retail (weak 0x1c from `Bird.o`) at every nerve comparison here while we expand it (the largest remaining loss; also noted under `boss-hanachan`); `TMActorKeeper::getMActorAnmData()` (0x8, weak from `bossgesso.o`) is `bl`-ed at all seven `changeBck` sites; `MsPerpendicFootToLineR` is 73% against retail.

## Rules from `MapObjFence`, `MapObjFlag`, `ModelGate`

- An empty leading case group counts toward the switch pivot but collapses out of the tree: with cases {3,4,5,6} MWCC pivots on label index n/2; adding `case 1: case 2: break;` (same destination as the default) moves the pivot and the low arm collapses into the default's `b` (`controlWall` 98.7 -> 100). Four identical bodies never tail-merge.
- A discarded return value keeps a small inline out of line: `MsWrap(mRotation.y, 0.0f, 360.0f);` as a bare statement (a real ROM bug: the wrap never lands) is a `bl` at four sites where the assigned form expands.
- An UNUSED helper can be size-exact *because* it inlines everywhere: `calcCurrentMtx` (0x10c) inlines into four cases and keeps the 0x7c `MsMtxSetRotY` a `bl` there; its out-of-line copy expands it, which is the 0x10c. `TRailFence::falling` (0x11c) is refused as a call yet size-exact uncalled.
- Spell out `setToNearest`: `mTracer->setTo(mTracer->getGraph()->findNearestNodeIndex(pos, -1))` reloads the member across the call as retail does.
- Constant-first float comparisons: `-180.0f < x` gives `fcmpo f0, f3; bge`; `x > -180.0f` gives `fcmpo f3, f0; ble`.
- `getRotYFromAxisZ(SMS_GetMarioPos())` loads `gpMarioPos` before `this`; `*gpMarioPos` sets `r3` first.
- Hoist a repeated inline argument to the first statement of a loop body (`f32 rate = (f32)z / (f32)mNumZ;` before the angle; folded in, the conversion lands after the inlined `MsWrap`, +11 instructions).
- Bind the array element (`TVec3<f32>& vertex = mVertices[y][z];`), not the array expression; but no `Info*` local for a member array of structs (a local pointer adds an `addi`).
- The rogue-include set is not always all three: `MapObjFlag.cpp` needs `DummyStrings.hpp` and the MSound pair but **not** `InfectiousStrings.hpp` (its `.data` UNUSED list lacks `MtxCalcTypeName`); check that list first.
- `static int total_use_size = 0;` *does* get an `init$NNNN` guard, contrary to `AGENT_MATCHING_TIPS.md`.
- `J3DTevStage::setTevStageInfo` is the shape behind `bl setTevColorOp`, nibble stores to +2/+3, the alpha bit fiddle on +6/+7, `bl setTevAlphaOp`; weak out-of-line `setTevColorOp`/`setTevAlphaOp` mean depth 2.
- Three double-precision Newton steps after `frsqrte` mean `std::sqrtf`; `MsSqrtf` has one; `TUtil<f32>::sqrt` differs again.
- Open: `web->unk0[i].unk0` folds to one `lwzx` in retail (`goOnRail`); `dist.length()` at depth 2 where retail calls `sqrt`.

## Rules from `TabePuku` and `Kukku`

- An actor-type equality test against `0x80000001` is a `switch`, not an `if`: retail hoists the constant and compares with signed `cmpw`; every `if` spelling (literal, `(s32)` cast, `-0x7FFFFFFF`, `| 1`, hoisted `int` local) folds into the `addis`/`cmplwi` trick. `switch (x) { case 0x80000001: ... }` matches (`TTPHitActor::checkHitActors` size-exact; `TTabePuku::control` 82 -> 99.9).
- `TPosition3::translation()` is one level too deep for `identity33()`: spell `identity33(); setTrans(pos);` out to get retail's nine inline stores.
- Naming the `MTXCopy` *destination* as well as `src`/`model` shifts every local by four bytes; name only the source and the model.
- Two more pasted-UNUSED cases: `TTabePuku::setMomentumFromQuat` (0x1d4) and `prepareDrag` (0x17c) are size-exact but pasted into their callers, where retail inlines `MsGetRotFromZaxisY`/`TQuat4::rotate` one level shallower.
- `.sdata2` diagnoses helper forms before any diff: only `+epsilon` present means `epsilonEquals(a, b, TUtil<f32>::epsilon())` (the two-argument overload allocates both signs); `setEulerY(TUtil<f32>::PI())` keeps `0.5f * pi` as a multiply and allocates `3.1415927f`, where a bare literal folds to a `halfPI` retail lacks.
- A dead `.sbss` scalar with a `__sinit` store needs a non-constant initialiser: `TUtil<f32>::PI() / 8.0f` lands in `.sbss`; `0.3926991f`, `3.1415927f / 8.0f` and a `const f32` intermediate all fold into `.sdata`.
- `if (getHitTimer() > 0) mHitTimer--;` reproduces two loads of the field where the raw read CSEs them (`TKukku::control` 92 -> 100), second confirmation of the const-accessor rule.
- Open, now the dominant residual: size-exact helpers (`TKukku::updateRotation` 0x248, `calcMomentum` 0x11c) inline into nerves where retail calls them, at the same depth; naming factors, splitting returns, routing through UNUSED helpers all failed. Also `__ct__Q29JGeometry8TVec4<f>Fv` is weak in `TabePuku.o`, `coasterkiller`, `fireWanwan`, `Bird`: retail calls the empty `TVec4` constructor for `TQuat4::rotate`'s first temporary, which also gives it a stack home; our build scalarises both temporaries (`TNerveTabePukuDrag::execute` 62%).

## Rules from `Bird` and `limitkoopajr`

- `JGPosition3::setQT(quat, trans)` is the level that keeps `TRotation3::setQuat` a `bl`; `setQuat` + `setTrans` written out expand (`TLimitKoopaJr::calcRootMatrix` 39 -> 98). `TQuat4::rotate(v, v)` is the form the ROM inlines; the one-argument forwarder emits a weak copy and a `bl` (`doFlyToCurPathNode` 78.5 -> 95.3).
- Third pasted-UNUSED instance: `TAnimalBird::doWalk` (0x198, size-exact) must be pasted into its caller; behind the call `rotate` sits at depth 2 and becomes a `bl`.
- Nerve singletons need exactly one extra wrapper level to stay `bl`s (`moveObject -> checkChangeToItem -> isChangeToItem -> isChanged -> theNerve`); inlining the condition one level up expands them. The price is a 3-instruction `&&` bool merge the ROM lacks: a genuine open conflict.
- A float predicate materialises `cror; bne; li 0/li 1; clrlwi.` at its inline site only as two statements: `if (a <= b) return false; return true;` (`canRun`: Wait 0 -> 79, Run 83 -> 98).
- Two plain `cmplwi` with no pivot tree is an `if`/`if` chain. `&&`/`||` grouping maps 1:1 onto flag registers: `bind`'s `mWireBinder && (isOnGroundNerve() || latest == PreLanding)` reproduces the r26-r29 ladder exactly, with the helper's own `(a || b) || c` supplying the inner two.
- A `TSpineBase<T>*` local in a nerve predicate keeps the spine in one register across `theNerve()` calls; reading `mSpine` per term reloads it.
- `setVelocity(TVec3<f32>(0, 0, 0))` gives forward stores plus the integer copy the ROM has; `mVelocity.zero()` stores z, y, x directly (`Landing` nerve to 100).
- A per-instance factor used in several arguments gets one CSE'd load *after* the `getSaveParam()` calls; naming it loads before them (`isFindMario` 88 -> 100). A `THitActor*` member dereferenced per component needs `const TVec3& center = actor->mPosition;` or it reloads three times.
- `MtxPtr src = (MtxPtr)mtx;` plus a named `J3DModel* model` before `MTXCopy` reproduces the r30/r31 split.
- Ruled out: `dir.setLength(dir, TUtil<f32>::one())` to push `setLength` out of line (worse); a static helper taking `int&`/`int*` for eagerly materialised timer addresses (folded back, plus a symbol the map lacks).
- A local template instantiation (`MsWrap<f>`, `set<f>`) is emitted next to its **last source-order user**, so `validate-symbol-order.py` can fail on placement with every real symbol in order.
- A wrong `J3DMLF` flag word in a `TModelDataLoadEntry` shows only as a ~93% data object; read it off the `entry$NNNN` `.rodata` dump.
- Open header items: `TVec3::cross()`'s store order (retail stores x and y, reloads `offset.x`, then z); `MsAngleDiff` should read `180.0f + alpha` (constant first).

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
- **Ruled out:** the local `set<f>__Q29JGeometry8TVec3<f>Ffff` is *not* a header-structure clue. The in-class member template of an explicit class specialisation is exactly what MWCC gives internal linkage to (43 of our objects carry it; `Kazekun` matches it byte for byte). Out-of-class forms either fail to compile (`illegal function definition` / `unimplemented C++ feature`) or, on the primary template, give **weak** linkage and never inline, which is further from retail. The units that list it MISSING expand it at every site; the gap is per-call-site, same family as the tables below. Trial table in the header TODO.
- Caller size gates two-instruction accessors, second confirmation: retail's `TMuddyBoat::bind` (frame 0x270) `bl`s the 8-byte `getObjCollisionHeightOffset` three times where our 0x140 `bind` expands it. A MISSING 8-byte accessor in a TU whose caller is frame-short is a symptom of the caller's missing locals, not a wrong spelling.
- A `bl` to a `__<Class>F<args>` symbol whose callers never load `this` (`r3` still holds the previous call's return) proves a static member (`TDirectionCalc::d2r`).
- `TMapCollisionMove::moveMtx` is `MTXCopy` + `move()` (as `moveSRT` is `MsMtxSetTRS` + `move()`); `TMapCollisionBase::setMtx` is the bare copy. Fixed: 32.8 -> 100 in every Enemy TU.
- `TCameraShake::mCamShakeNameSave` (`src/Camera/CamShakeDefine.cpp`) is a complete naming authority for `EnumCamShakeMode`: entry N is mode N's `.prm`, so the remaining `CAM_SHAKE_MODE_UNK*` can all be named from it.
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
- `TMatrix34::concat` is `r[i][j] = a[i][0]*b[0][j] + a[i][1]*b[1][j] + a[i][2]*b[2][j]`, `r[i][3] = a[i][3] + (...)` (fixed; the trailing `fadds` of the stored translation against the accumulated sum is the tell for that grouping). It takes `const T&`, so a bare `MtxPtr` operand cannot use it without a cast (`TKoopa::calcRootMatrix` keeps its written-out terms).
- `JGeometry::TVec2` has no user-declared `operator=`: `std::__copy<TCameraMapTool>` moves its `TVec2` with paired `lwz/lwz/stw/stw`, which is MWCC's aggregate sub-object copy (two `f32` members would give `lfs/stfs`). `getAngle`'s float copies must come from a component-wise `sub()` against a temporary instead.
- Reading a `u8` member through its accessor rather than the field changes which register holds a `divw`'s dividend (`TEffectEnemy::perform` exact only with `getHitPoints()`); `(u8)1` vs `1` on a ternary constant in a `u8`-returning accessor decides whether the narrowing is emitted at the return or at the use. `getMaxHitPoints()` now returns `u8`.
- The `init$NNNN` guard follows the *presence* of an initializer, not its non-triviality: `static int x = 0;` gets one, `static int x;` does not (`TMapObjFlag::init` 99.8 -> 96.0 without the `= 0`).
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

## Literal-pool ordering (batch 56, WaterGun)

`@NNNN` literal ids are assigned in **emission order** (reverse source order under `-inline deferred`) and `.rodata` is laid out in id order. Verified on the whole `WaterGun.cpp` pool: `calcAnimation`'s anim names (`@3741-45`) < the TDeParams ctor's prm path and `PARAM_INIT` names (`@4093-4107`) < `createGunBody`'s model paths (`@4118-19`) < `init`'s joint names (`@4426-39`) < the `TWaterGun` ctor's nozzle names (`@4483-93`) < `TNozzleBase::TNozzleBase`'s param names (`@6089+`). So a function that is instruction-exact but whose `addi rX, rBase, imm` operands are all off by one constant has a pool-order problem, not a code problem: in `__ct__9TWaterGunFP6TMario` the 0xcc offset is 180 bytes of params-ctor strings emitted at the wrong place plus 24 bytes of two unreferenced 12-byte templates (`@2685` zeros, `@2687` three `1.0f`) at file scope whose source is still unknown. The same 24 bytes are the whole residue of the 2 KB `TNozzleBase::TNozzleBase` and of `nozzleBmdData`'s data score. Check the pool before the code when a large function's diff is all `~` on immediates.

Also measured there: a `const` accessor causes a CSE (retail reloads `mActorType` for the second half of `getActorType() == A || getActorType() == B`); `JMASSin`/`JMASCos` on the same angle derive the table index twice in retail (`lhz` once, two `sraw` + two `slwi`), so a single shift on our side means our source shares the subexpression; `fadds` FPR pairing is not steerable by operand order; `mr rD, rS` vs `addi rD, rS, 0` remains unpredicted (eight sites, usually with a callee-saved register swap).

## Batch 58 tells (enemyMario, bgtentacle)

- An inlined accessor returning a constant is the only way an `x * 0.0f` product survives; a literal `0.0f` in the source folds away. Extends the "surviving multiply by literal `1.0f`" rule to the case where the literal spelling cannot work.
- `x = x + k * (y - x)` loads the member into the `fmadds` addend first; `x += k * (y - x)` loads the other operand first.
- Grouped case sets build MWCC's pivot tree, and case block order also fixes the string pool (the pool was permuted, not shifted).
- `operator-` into a named local keeps `TVec3::sub` a `bl`, and the copy's source names the left operand (second confirmation). Per-component vs batched `TVec3` from matrix also covers writes: interleaved `lfs`/`stfs` is `v.x = m[0][3]; ...`, batched is the 3-argument ctor.
- A materialised-bool ladder ending in a shared `li r0, 0` + `clrlwi.` is a predicate that already exists in the class (`TBGTentacle::isAttackable()`).

## Settled in header round 9

- The `&&`/`||`-right two-`return` refusal is a general lever: it explains `TTakeActor::isTaken`, `TYoshi::onYoshi` and `TMario::checkStatusType` (all previously mis-diagnosed as budget cases). When the map has a weak tiny accessor plus a `bl` and the call site is on the right of a short-circuit operator, spell `if (c) return TRUE; return FALSE;` before counting depth.
- A `return *this` forwarder is not an inline level: in `s3 -> s2 -> s1 -> self() -> get()` the build calls `self()` at depth 4 and still expands `get()` at depth 5. Do not count `*this`-returning accessors (`TNameRefPtrAryT::getChildren`) in a depth.
- Exhaustive dispatch tables (`MSBgm::getSceneNo`, byte-exact) pin an id enum's name-to-value map absolutely; the whole `MSD_BGM_*` enum was off by one and now runs 0x01-0x30. Value-preserving relabelling is verifiable: shift values and every user in one commit, require an empty `changes_all` and an unchanged DOL. The SE enum's per-sound 0x800 bit is an independent check on name order (eel group is category 2, 0x180-0x18D).
- `MActorAnmBck::setMotionBlendRatio` census: 61 of 63 ROM sites use the single-read accessor form; `MActor.hpp` is right and `MarioDraw::calcAnim`'s double read is a call-site property.
- Ruled out in headers: `TVec3 tmp(v); mult33(tmp, v);` in the one-arg `mult33` (only cameralib wants the copy; `TMapWire::init` 98.8 -> 95.9); `length()` as `TUtil::sqrt(dot())` (`squared()` is the intermediate; boid loses its weak `dot`); an `MsClamp<f32>` level (cameragc's single `bl` is the caller-size family, with `TVec3::set<f>` and `TUtil::one()` flipping at the same site); `TFlagT` copy ctor out of class (three JDrama ctors by value pin the in-class spelling; `~TFlagT() {}` reproduces the ABI but emits a `TGameSequence` dtor the map lacks); `TVector<void*>::begin()` facades (trial table in `std-vector.hpp`). The depth table is re-measured and unchanged: `static` 14/9/6/2/never, `inline` unlimited at depth 1 (17 measured), identical below.
- `evSetEventStart`/`evSetEventEnd` are UNUSED at 0x174/0x148 with no strings of their own; left empty rather than fabricated.

## Batch 59 tells (bossManta, chuuhana)

- An `else` chain is readable off a lone `b <epilogue>` after a block, invisible in a collapsed diff; grep the `<` markers first.
- Predicates cut both ways per site: retail materialises where we branch -> call the helper (`isAirborne()`, `isBckAnm(6)`); retail branches where we materialise -> compare inline (`getActorType() == 0x400000CF` over `isActorType()`). One TU uses both spellings of one predicate.
- `.sdata2` order is reverse source order, so it is a literal-value oracle: a literal late in the pool belongs to a function near the top of the source, and a slot shared by two functions fixes its value (`-10.0f`, not `-1.0f`).
- A store cannot move across a call: a target store after a `bl` dates the declaration after the call's result was named.
- `a = b = mPosition.y;` gives one load / two stores with the last assignee stored first. `int index = mInstanceIndex;` removes the `extsh.` on an `s16` compared twice. An unnamed `(GXColor){...}` argument temporary sits above the named locals; a named one sits below and adds an init.
- Console accessor levels are +8 per level per site (`gpMarDirector->mConsole` 0x38, `->getConsole()` 0x58, `SMSGetMarDirector()->getConsole()` 0x68 over four sites). `getPosition()` over `mPosition` is +8 as `operator-`'s left operand and zero as `set()`'s argument; naming `getGravityY()`'s result costs an instruction.

## Closure batch 60 tells

- **A weak empty base-ctor symbol names the inline depth.** A 4-byte `blr` ctor in a TU's layout says which level of the base chain hit the depth-4 refusal, so its *name* identifies a member's declared type: `TSimpleEffect::unk48` is `TPosition3f` (chain factory -> TEffectPinnaFunsui -> TSimpleEffect -> TPosition3 -> TRotation3), which closed and linked `MarNameRefGen_Enemy`. objdiff scores a `bl` to the wrong-but-same-shaped symbol as 100% on the caller, so check `-s missing`/`-s extra` before trusting a 100%.
- **MWCC always CSEs two loads of the same member across a short-circuit branch.** Every ROM instance of load/test/reload (29 scanned) has a `bl`, a store to the member, or index arithmetic between the loads; a reference local switches to address-based reloading (`lwzu` + offset 0), which is a different shape. `SMS_IsMarioOnWire`'s second `lwz 0x68` therefore means the null test and the type test are in different inlined helpers with a real call or store between them.
- **Accessor depth controls argument evaluation order, not just the frame.** Accessors for a callee's own members (`getStampInterval()`, `getRevivalStampTex()`) flip a call's argument loads to retail's right-to-left; a named local pair declared in reverse order flips the same pair while keeping the base pointer's register, whereas an accessor on the base object moves it (`TMapObjRevivalPollution::loadAfter`, still open on a four-register rotation).

## Closure batch 63 tells

- **An inlined accessor raises its receiver's register priority.** `TMapObjFloatOnSea::initMapObj`: with the raw member MWCC gave `this` the lowest callee-saved register, a one-step rotation of r27-r31 with relative order preserved; `getUnkF4()` put `this` back in r31 (97.6 -> 100). Loop spelling, index type and declaration order were all inert. A pure rotation of the callee-saved set means a member read that should go through its accessor, not a restructured loop.
- `SMS_RideMoveByGroundActor`: `riding_info->unk0->getRotation().y` on the first rotation read only (the ROM reloads the second raw), plus named `prevYaw`/`yaw`. `SMS_DumpJ3DModel` is an empty UNUSED 4-byte body between `TMultiBtk::update` and `SMS_RideMoveByGroundActor`.
- `calcAndSetMarioData`: name the two distances before storing them; `getPrevPosition()` is load-bearing (-8 raw); a named nozzle local inlines `getCurrentNozzle`, which retail calls.

## Closure batch 61 tells

- Container loop shape (`TStageEnemyInfoTable::getMatchedInfo`): `getChildren().begin()/end()` rather than the inherited `begin()/end()` makes MWCC keep `&pBegin_`/`&pEnd_` in registers (four callee-saved instead of three); the end named in the same for-init declarator list (`T** it = ..., **last = ...`) hoists its load out of the loop, a separate statement loads end first; dropping a named `*it` local was the last 16 bytes of frame.
- The map wants `__dt__Q27JGadget35TVector_pointer<P15TStageEnemyInfo>Fv`: `JGadget::TVector_pointer`'s template argument is the pointer type, not the pointee (header item for NameRefPtrAry.hpp / std-vector.hpp).

## Settled in header round 10

- **Nested guards over a shared fallback are a register lever.** `if (a) { T x = f(); if (x) { ...; return; } } fallback;` and `if (a && f()) {...} else { fallback; }` differ by one `mr`: the nested form gives the guard's test r0 and the named local a second register (two users of one CSE'd load). `TSpcTypedInterp<T>::dispatchBuiltin` 96.3 -> 100; the early-return form duplicates the tail `bl` (79%).
- **A synthesised no-argument ctor is a defaulted argument.** A weak `__ct__XFv` whose body is `li rN, <default>` + tail `bl __ct__XF<args>` forwarding the virtual-base flag is MWCC's entry for `X(T arg = default)` (needed by `new X[n]`); do not write a separate default ctor (`M3UMtxCalcSIAnmBlendQuat`, 0x24 exact). With virtual bases the ctor signature is `(this, vbase-flag, args...)`, first user argument in r5.
- **An implicit base-ctor chain is a depth ruler.** `CardLoad.cpp`'s three `JSUMemoryInputStream stream;` sites cut the chain at different classes (all inline in `changeScene`; `bl JSUIosBase()` via `loadBookmark`; `bl JSUInputStream()` in `perform`'s case 4), so which base ctor is out of line reads the caller's missing inline level: `perform`'s case 4 body sits one level below `perform`. `JSUMemoryInputStream()` is `setBuffer(nullptr, 0)`.
- **A narrowing return can be paid at the call site**: wide return plus explicit `(u16)` at each call is codegen-identical to the narrow return and gives the map's UNUSED size (`SMS_getNormalStage` 0x1c -> 0x18). Retires the "UNUSED size loses to call-site evidence" exception. `scScenarioNameTable`/`scNormalStageTable` are UNUSED `.data` of CardLoad.cpp, not function-local statics.
- **Argument order from evaluation order**: arguments evaluate right to left, so the sibling pair the ROM computes first is the later one; `TProjection3::orthographic` is `(l, r, t, b, n, f)` with `+ n` on `mMtx[0][3]`/`[1][3]`; chained zero stores run high index first (chain written low -> high).
- Literal pool, second confirmation: moving `TWaterGun::TDeParams()` (no-arg ctor, `TParams::load(mPrmPath)` last: 12 bytes, 492 -> 0x1f8) into its map position renumbered the block, `TWaterGun` ctor pool offset 0xcc -> 0x18; the residual 0x18 is the two unreferenced 12-byte templates `@2685`/`@2687`.
- `TVec3::cross()`'s reload is a call-site aliasing property, not a body property: three body shapes measured project-wide, all regress (trial table at the declaration). `MAnmSound` dtor is non-virtual in-class (no vtable slot); retail's `enemyMario.cpp` destroys one somewhere unreconstructed (`kill` UNUSED 0x4c vs our 0x4). `TEnemyMario::consider`'s out-of-line `getPoint`/`sub`/`dot`/`sqrt` mean one unidentified inlined helper between `emWalkGraph` and the distance test (block at depth 2). Transposed `mult33` overloads all regress `throwMario`; `unk385` is `u8` with `s32 getSprayState() const`; `TMario::unkFC` is `s16[4]`; `mHHoverHeight` is the ROM's spelling.

## Settled in header round 11

- **Fork an accessor ladder instead of deepening it.** `getLayers()[i]` (two 0-parameter levels) and `getLayer(i)` (two indexed levels) cost different frame slots at equal depth; `SMSGetPollutionLayer(i)` = `SMSGetPollution()->getLayers()[i]` closed both `watch()` functions (MapObjAirport, MapEventSirena linked) while `getLayer(i)` kept its 25 sites and the linked MapEvent exact. Retires the "level belongs in `getJointModel`" conclusion.
- **A missing vtable store names a missing subclass**: a `lis/addi <Class>::__vtable` + `stw` we lack, with the class in the map but `missing` in objdiff, means the class is absent from the tree and the virtual it overrides was wrongly put on the base (`MAnmSoundMario::startAnimSound` held the category switch; `MAnmSound::startAnimSound` is bare `gateCheck` + `startSoundActorInner`; MAnmSound data 100%).
- **Constructing a derived object inline emits the base's out-of-line destructor** (MWCC synthesises the derived implicit dtor at the site), so an UNUSED base-dtor symbol in a TU that only calls `new` identifies a derived instantiation, not a `delete` (`__dt__9MAnmSoundFv` in enemyMario.o from `new MAnmSoundMario`; ours 0x64 vs 0x60, open).
- **MWCC emits helper instantiations immediately after their user in emission order**, so an UNUSED compiler-generated symbol's map position points at the function that needed it: the map's UNUSED ordering is a caller oracle.
- `TVector_pointer<T>` takes the pointer type (`value_type = T`, `iterator = T*`); all four dtors mangle as the map spells them at UNUSED 0x58. `cDirtyFileName`/`cDirtyTexName` live in `include/Player/MarioDirtyStrings.hpp`, a rogue-include carrier in the DummyStrings idiom (real header unrecoverable; four dead-stripped TUs omit it). Frame arithmetic: locals begin at 0x28 (fixed 32-byte outgoing-parameter area at 0x8); inline-expansion blocks stack upward in call order; slack above the last temporary is allocated space, so a 4-byte lever below a temporary costs 8 of frame. `startTimer(u32)` rejected (nothing measured is (-8 frame, 0 slot)); the sizeof items TKazekun 0x1d4/TLimitKoopa 0x1c8/TKazekunManager 0x60 were already right.

## Closure batch 65 tells

- **The unfoldable `bcc next; b epilogue` pair.** MWCC folds a conditional plus an unconditional branch from the same `if` into one inverted branch (`&&`, `!(A && B)`, explicit `else`, empty then-block, dead stores, `goto` all fold). The pair survives only when the then-block is also the target of another branch, i.e. the test is one term of a longer `||` (`TMapObjWaterFilter::perform`: the water test is the second term of the demo-camera guard's `||`; 1435 such pairs in the dtk asm). An apparently redundant branch pair means the guard is part of a wider `||`, not a standalone `if`.
- Repeated accessor calls inside a condition are a low-region lever: binding `gpCamera->getUnk124()` to a reference costs 36 fewer low bytes than spelling it five times.
- **Accessor levers come in 8-byte steps, never 4** (six forms measured on `warpPosAndAt`); a function needing exactly +4 low has another cause.
- **`new` costs 8 bytes of frame retail does not pay** (`SMS_InitChangeNpcColor`: the new-expression itself, not the local; no explicit `operator new` spelling reproduces it). Mirror image of the "one 8-byte object" family; research item.
- Two int<->bool shapes: `cmpwi/beq/li 1/b/li 0` is a ternary or if/else producing 1/0; `neg/subic/subfe/clrlwi` (or `cntlzw/extrwi/cntlzw/srwi`) is a real int->bool conversion. A retail `cmpwi` normalisation is never evidence for a `BOOL` local.
- `TCameraBck::updateDemo`: retail materialises `checkState()`'s bool straight into `result`'s register (r31 already 1) so only `li r31, 0` survives; ours materialises into r0 and copies (ten spellings rejected in the source). `TMapModel::initUnderpass`: `getModelData()` +8 at both sites, `getChildrenNum()/getChild(i)` +0x10, a material accessor +8 at two or three sites. A trial script must print build failures or it reports stale objects.

## Closure batch 70 tells

- **A by-value `TVec3` operator parameter only materialises its copy for an lvalue argument**: `const TVec3& r = MsGetRotFromZaxis(...); x = r - g();` emits retail's three-word copy before `bl TVec3::sub`; `x = f() - g();` subtracts in the donated temporary (`NPCNeckCallBack` 96 -> 98.3). Decides the open `JGVec3.hpp` question of whether `operator-` takes its left operand by value (header item: a by-value-left signature should produce the shape project-wide).

## Closure batch 71 tells

- **The depth-4 refusal is reachable with two ordinary accessor levels**: `TUtil<f32>::sqrt`/`inv_sqrt` are 3-statement bodies, inlined at depths 1-3 and called at 4; a caller that calls `sqrt` but inlines `inv_sqrt` has the `sqrt` site one or two levels deeper (`TBathtubBinder::float_` 72 -> 96 via the UNUSED `constrain_(TVec3&, f32)` helper, 0x164 exact). Retires the "JGUtil.hpp is a whole-tree lever" claim.
- **An UNUSED symbol can be both emitted and inlined, and `inline` breaks that**: a plain member at exactly the depth-1 budget (14 statements) expands at every site and is still emitted as the dead-stripped copy; `inline` removes the symbol. `v.set(x, v.y, z)` pays one statement for free (two component stores are 15).
- **A reference parameter's conversion temporary is placed at the level that binds it**: `insert(T* const&)` binds at the outer call (slot 0xc); taking `T*` by value pushes the binding into the inner `const T&` and parks it above the other temporaries (0x3c) (`TMarDirector::registerEventWatcher`; header item for `JDRViewObjPtrList.hpp`).
- **A default-argument temporary is built by whichever level declares the default**: `TVector_pointer(const TAllocator<void*>& a = TAllocator<void*>()) : TVector_pointer_void(a) {}` moves the slot one temp block up and closes the `TMarDirector` ctor (measured: unit 100%, zero regressions; header item).
- **Forking a global accessor is a real +8 where the member level is +0** (`SMSGetCamera()->getUnk1EC()` +8, `gpCamera->getUnk1EC()` 0; `TSky::perform`). `MTXCopy(m, model->getBaseTRMtx())` is +16 and `model->setBaseTRMtx(m)` +8 for identical instructions. Accessor levers saturate per receiver: `getStatus()` at one of two compares +0, both +8; `getUnk58()` at one of three sites +0, all three +8.
- The integer 182 degrees->u16 factor is in game code: 75 deg folds to `0x3552 = 75 * 182`, not `DEG2SHORTANGLE`'s 0x3555 (`TNpcCoin::requestAppearCoin`). `TMario::stateMachineUpper`'s spray block is `mWaterGun->isEmitting()` (one more site). `TBGPolDrop::move`: the second `setBckFromIndex(13)` is on `unk54` (real fix). Run `validate-symbol-order` before the first diff on every closure unit: it hands over UNUSED helper signatures and sizes.

## Closure batch 73 tells

- **An inline level above an accessor, not below it, pushes the accessor out of line**: `TBathtubPeach::getSpeed()` over `getParam()` reproduced retail's inlined-then-called pair of param reads (the comparison's fetch inlined, the argument's called: a call in argument position sits one level deeper than as a statement); the same accessor on the params class is worth zero.
- **A missing map symbol dates a free function as `inline`**: `static` does not remove an emitted global for a fully inlined file-scope function, `inline` does with no codegen change (`AnimalNerve::calcDist`). Cross-check the TU's closure duplicates against `-s extra`; they should match one for one.
- **An inlined setter's by-value scalar parameter costs one instruction per site** (`TSpcSlice::setDataFloat` x9 in `linGetSRT`); writing the fields directly or `const f32&` lands the count (the header form regresses the exact `spcFloat`: header decision). The eleven `lin*` builtins are `static`.
- MWCC switch-arm block placement is not source order (second confirmation, `linGetSRT`'s arm 1 before arm 0 with an identical pivot tree).
- A dead non-trivial local of an inlined callee prices at `sizeof` per expansion when several expansions share the pool (spider: 4 sites x 4/8/12/16/20 B = +16/+32/+48/+64/+80); the batch-69 align-8 rounding is for a single expansion. J3DMaterial two-parameter forwarders (`getColorChan(idx)`, `getTevStage(idx)`, `getTevOrder(idx)`) are +4 per expansion over the one-parameter block path — 4-byte steps exist there.
- Leads: `TVec3::distance()` has no map symbol; `TVec3 diff = *this; diff.sub(other); return TUtil<f32>::sqrt(diff.squared());` reproduces all 334 instructions of `TNerveAnimalGraphWander::execute` with no TU helper (header item, untested project-wide). `std::fmodf` needs its real 0x5c body in `math.h` (weak in wireTrap.o; BathtubPeach's whole residue). `TAfterEffect::perform`: pure r30/r31 swap — retail keeps constant 0 in r31 across four byte stores and the `checkFlag(4)` materialisations; look for a named midpoint among the sixteen `rect.` reads.

## Settled in header round 12

- `TVector_pointer(const TAllocator<void*>& a = TAllocator<void*>()) : TVector_pointer_void(a) {}` closed the `TMarDirector` ctor (unit linked). `TViewObjPtrListT::insert` takes `T*` by value: a by-value pointer parameter always materialises its copy, a `T* const&` one only when the conversion has a nonzero offset, so the majority of sites decides (four retail shapes vs one redundant `stw` in `TBossMantaAdditionalCollisionSet`'s ctor, accepted regression 99.75 -> 98.62).
- Promoted and codegen-identical: `TMario::getFludd()`, `SMSGetCamera()`, `MSound::getModBgm()`, `TMapModel::getUnderpassMaterial()`, five `TCameraBck` accessors, twelve boid accessors (`fishoid.hpp` must include `boid.hpp` for the forwarding `getBoidNum`; `TBoidLeader::FLAG_UNK2 = 2` — a parked helper's numeric argument is evidence about the enum, never mapped to the nearest existing name: `FLAG_USE_GRAPH_GOAL = 4` broke the DOL).
- Rejected with trial tables at the declarations: `TEnemyManager::getObj` casting `TObjManager::getObj` (loses five exact functions; `TLiveManager::getObj` is the majority spelling, AnimalManager's `clipEnemies` stays the TU-local exception); `TPosition3::translation` as `setTrans(t.x, t.y, t.z)` (FireWanwan regressions, `associateNPC` unmoved); `TVec3::operator-` returning by value (wins MapWire, loses ~30). `operator-` already takes its left operand by value (batch 70's question is closed). "+8 frame, zero instruction change" levers are only worth applying as the last gap.

## Settled in header round 13

- **`MSound::startSoundActor` result binding: rejected in the six-argument body, landed one level up.** At a site with no low-region slack, each 4-byte value an inlined callee binds pushes the caller's next temporary up by 4, so a byte-exact caller's temporary offset *counts* the inline's bindings: `TMapEventSink::control`'s vector sits at 0x30 = raw 0x28 + two bindings (`this`, `position`), and the result local would make 0x34. Batch 82 put the binding in a **two-argument overload** that carries the four constant arguments (`JAISound* sound = startSoundActor(id, position, 0, nullptr, 0, 4); return sound;`), so only the sites that spell the short form pay for it: 26 functions byte-exact, nothing regressed. Constant arguments bind nothing and `this` binds only when it is a fresh load, so a member wrapper in the `TMario::startSoundActor(u32)` shape is worth +0 and cannot be the carrier; the binding must also stay **live** (returned) or MWCC drops the slot. Full trial table in `frame-gaps.md`, "Closure batch 82".
- **`std::fmodf` has its real 0x5c body** (named `unsigned long long quotient` for the ROM's 0x28 frame); the old `::fmod` forwarder called a symbol absent from the image. The ROM never expands it at any depth (`TDirectionCalc::calcNearerDirection` calls it from depth 1); our build expands at depths 1-2 and calls at 3, so two-level wrap helpers (`WrapDirectionF`/`WrapAngleF`) in wireTrap, MapObjCorona, BathtubPeach, KoopaNerve are a stand-in. Accepted regressions: `calcNearerDirection` 97 -> 70, `TNervePeachEscape::execute` 94.2 -> 93.9 (both nonmatching, mechanism now correct). A one-statement helper can drop a function under the depth-1 budget and inline it into its own callers (`makeRelativeAngle` 95 -> 26): check a body's callers' `bl`s before simplifying it.
- `TSpcSlice::setDataFloat` stays by value (the by-value parameter reserves a 4-byte slot at `spcFloat`'s site; a reference binding does not). `TVec3::distance()` as copy/`sub`/`sqrt` is a per-site split (5 improve, 16 regress; the ROM has both spellings, the copy-and-sub sites want a second helper such as AnimalNerve's `calcDist`).
- **A missing `__dt__` symbol dates a base destructor as in-class and trivial**: `JAIAnimeSound` had an out-of-line declaration and no definition, costing `MAnmSound`'s dtor a register and a `bl` (0x64 -> 0x5c vs map 0x60). The last instruction is pinned: `addic. r0, rThis, <off>` before a second vtable store means a polymorphic subobject at a nonzero offset (`TSpineBase<TLiveActor>` dtor), a bare `beq` an offset-0 one — MAnmSound/JAIAnimeSound has a second base or intermediate class the declaration does not model.

## Closure batch 80 tells

- **A caller's argument setup is evidence about a callee's signature**: every retail call of `THPPlayerInit` does `li r3, 0`; the unused `s32` parameter's home slot is exactly the 8-byte frame gap (THPPlayer linked). Grep `bl <symbol>` in the dtk asm for argument setup before treating a frame gap as an invisible local.
- **`if/else` and the equivalent ternary allocate differently for integers too**: `u32 size; if (c) size = a; else size = b;` decides which register the accumulator keeps (`THPPlayerCalcNeedMemory`).
- **Declaration-then-operation picks the destination register**: `f32 x = a - b;` subtracts into `b`'s register, `f32 x = a; x -= b;` into `a`'s (three confirmations: `TSMSFader::update`, `execNpcObjCollision_`, `setVariableDamageRadius_`).
- **An `fmr` from a scratch FPR into a callee-saved one means two source variables**: retail keeps the unclamped and the clamped/assigned value in separate locals (`overlap`/`dVar8`, `base`/`fVar6`, `diffY`/`absY`, all real behaviour fixes in NpcCollision — the degenerate-overlap branch stores the *signed* Y difference, and the push is Mario-minus-me).
- **`if (A || !B) return nullptr;` shares one exit block**; two separate early returns leave an extra `li r3, 0; b` pair (`TMovieSubTitle::getCurEntry`'s missing subtitle-option guard `getFlag(0x90001)` took `perform`/`setCurMessage` 73/82 -> 100). Mirror of the unfoldable-branch-pair rule.
- **A named local can be a cheaper register lever than an accessor**: `TMapObjBase* obj = unk140[unk13C];` at the first of five tail calls fixed a 3-cycle rotation at zero frame cost where `getObj(unk13C)` costs +8 (`TJointCoin::makeObj`; MapObjEx linked). Adding unused inline accessors to a class is not codegen-neutral (five unused ones turned +8 into +48). The cast-inside-vs-outside choice is per function (`TNPCManager::clipEnemies` inside, `perform` outside). A predicate helper materialising a `bool` cannot reproduce a flat `||` chain whose terms re-read a global after a `bl` (`fabricatedInline3` right in cameragc, wrong in NpcManager; `SetViewFrustumClipCheckPerspective(fovy, aspect)` were swapped there).
- `TMapObjNail::receiveMessage` is now exact via a `getDownCount()` accessor, so batch 74's `startSoundActor` table is stale by one. Open placement residues (seed for a low-region research batch): `setupResource`'s 12-byte object below `buffer`, `TSMSFader::load`'s three-temporary order, `clipEnemies`' saturated 8, `execNpcObjCollision_`'s 32, `bind()`'s `operator-` temporary at 0x10 vs 0x28. `THPPlayerSkip` (UNUSED 0x1f0, no disassembly) left undefined.

## Research batch 82: the sound sites' fourth byte

- The second inlined level is a **two-argument overload** `JAISound* MSound::startSoundActor(u32 id, const Vec* position) { JAISound* sound = startSoundActor(id, position, 0, nullptr, 0, 4); return sound; }` in `MSound.hpp`; the six-argument body returns `JAISound*` directly (free: zero functions moved). A site pays the extra 4 bytes only by spelling the short form, so the choice is **per site**: 26 functions to exact (DebuTelesa linked; `TEffectEnemy::setDeadAnm`'s fabricated local deleted; `emitOneTimeSandPillar_`, `TMario::stayWall`, hamukuri/telesa/pakkun/gorogoro/bombhei/tamanoko/tobipuku/moepuku sites), while sites already exact with the long form keep it (`TMapObjNail::receiveMessage` in the linked MapObjEx — the short form there broke the DOL). Rule: a constant argument binds nothing, `this` binds only when it is a fresh load, and a binding must stay **live** (bound and returned) or MWCC drops the slot; a discarded binding is +0.
- There is no per-class `startSoundActor`-shaped helper anywhere in the map (`TMario::startSoundActor(u32)` and `TMapObjBase::startSound(u16)` are emitted, so cannot be carriers); the group split is neither class nor arguments.
- Open: functions with several sound sites in mutually exclusive branches overshoot with the short form and need the binding one level deeper (`TBellDolpic::control`, `TRoulette::switchStop`, `TDptMonteFence::touchPlayer`, `TMapObjBase::startSound`, `TFireWanwan::receiveMessage`, `TNerveKazekunHitWater::execute`, `TTelesa::changeByJuice`); `TMareGate::control` passes a real handle and needs a three-argument sibling. **WoodBarrel is 100/100 but breaks the DOL when linked** (CameraInbetween-style; diagnose with a re-link map before retrying).

## Closure batch 83 tells

- **"x reloaded, y/z in callee-saved FPRs" is a pair of named `f32` locals** (solved, research batch 86). MWCC gives a callee-saved FPR only to a **named scalar local of the function's own body**; an aggregate member read (`v.y`) is reloaded at every use, and a local of an *inlined callee* is reloaded too, because it is re-initialised inside the expansion. So retail named exactly the two components it preserves and left `.x` as a member read. Declaration order picks the register: the **last**-declared local gets f31, so `f32 z` before `f32 y` gives f30=y/f31=z and the reverse gives f31=y/f30=z. A named local used this way costs **no frame** (`updateTrans` 0x48 -> 0x58 is the two FPR saves alone). Measured in a scratch TU: the promotion needs the reads to be per-component at the use site -- passing the vector to an inlined callee by `const&` (`it->updateTrans(delta)`) suppresses it entirely, naming the components *inside* that callee does nothing, `const` on the vector does nothing, and naming all three promotes all three. Closed `CPolarSubCamera::updateDemoCamera_` (93.2 -> 99.7, frame exact), `TBGTentacle::calcAttackGuideAnm` (94.8 -> 98.9) and `TMapCollisionBase::updateTrans` (84.9 -> 97.8, residue 24 bytes of frame). In `updateTrans` the loop body has to be spelled in the caller, which duplicates `TBGCheckData::updateTrans`; that helper is `UNUSED 0xdc` in the map and our copy is exactly 220 bytes, so retail carried the same dead duplicate.
- **Rotate-about-a-point idiom** (`updateDemoCamera_` 58 -> 93): offset as a three-argument `TVec3` ctor over component differences (`sub(a, b)` interleaves the stores), only the x component named (retail's early `fneg`), `JMASSin`/`JMASCos` spelled out at every use, write-back as `center + offset` (by-value left operand = the three-word copy before `bl TVec3::add`).
- **An inlined setter's reference parameter costs a full slot per site, so the object must be shared**: one function-scope `TVec3 scale` plus `setGlobalScale(scale)` at both sites gives retail's 0x30 (`TPollutionAction::fire` closed); per-site temporaries give 0x48, direct field writes 0x18.
- **Copy-and-subtract distance sites want a TU-local `inline` helper** holding both `TVec3 diff = a - b;` and `diff.length()`: `distance()` expands the subtraction, the call-site spelling expands `sqrt`, the helper gives both `bl`s (`TEMario::perform` 72 -> 98). Confirms the batch-73 prediction.
- **Forking one member's receiver expression is an anti-CSE lever**: `mMActor->getModel()` in a loop bound with `getMActor()->getModel()` in the body reproduces retail's per-iteration `lwz r3, 4(r3)` (`TEMario::init` 97.8 -> 99.9). A named index local caches `no * 0x14` across virtual calls where `unk4[checkData->getData()]` recomputes it (`TMapWarp::watchToWarp` 66 -> 99.8).
- **UNUSED sizes are an inlining-decision oracle**: one statement added to `setCheckData` changed `updateCheckData`'s UNUSED size 0xf0 -> 0x6c and broke the exact `update()`; restoring two UNUSED free functions that inline to identical code moved nothing in their caller. `volatile char trash[N]` never changes the schedule. `decomp-diff` `missing` rows named `gap_NN_*` are dtk fillers.
- Open: `JGadget::TVector<T>::insert` is 32 bytes of frame too big (0x120 vs 0x100 on `TDrawSyncTokenRange`, r29/r30 swapped; std-vector.hpp, blocks DrawSyncManager); `setCheckData`'s per-component vs batched vertex copies differ between the out-of-line copy and its two expansions (a callee of `setVertex` holds 40 bytes we do not reserve); `Emario.hpp` needs `execKill` (UNUSED 0x54) and `checkCollision` (UNUSED 0x220) declared.

## Settled in header round 14

- Three-argument `startSoundActor(u32, const Vec*, JAISoundHandle*)` binding and returning its result closed `TMareGate::control`. **The two-argument overload costs +8 per expansion, not +4**, so functions with several sound sites in exclusive branches (`TBellDolpic::control` needs +0x10 over 4 sites, `TDptMonteFence::touchPlayer`/`TRoulette::switchStop`/`TMapObjBase::startSound`/`TTelesa::changeByJuice` +8 over 2) need a carrier that saturates per function; nothing measured does (per-class wrapper +0, caller `MSound*` local wrong — retail re-reads `gpMSound` before each `gateCheck`; the short form at one of two sites lands it but is not credible). `TFireWanwan::receiveMessage` and `TNerveKazekunHitWater::execute` are already right.
- Promoted, codegen-identical: `TWaterGun::getCurrentNozzleIndex()`, `TBossHanachan::getChangeParams/getHead/getSpine/getMarchSpeed`, seven `getSL*` unwrappers.
- Rejected with trials at the declarations: every `JUtility::TColor` spelling for the 8-vs-4-byte conversion temporary (explicit copy ctor breaks nine JSystem units; the temporary is the `get()`-plus-conversion chain in both builds and only its width differs); `(TPollutionLayer*)mJointModels[i]` in `getLayer(i) const` (does not move `cleanedAll`, costs `getPollutionType`/`isPolluted`). **An accessor's level count is not interchangeable with which function declares the level.**

## Settled in header round 15

- **An inlined free function's by-pointer parameter binds 8 bytes; the same logic as a member accessor does not** — a parked `static inline f(T*)` helper and a promoted member are not interchangeable even at identical instruction output (`TMirrorActor::checkIsInMirror`: the member predicate over `getUnk18()` is 0x60, retail's 0x68 is the free function's binding; `getUnk18()` promoted, the predicate stays parked). The binding only happens when the helper returns a value; a reference-returning one folds (+0).
- **A frame excess that appears only where a helper is inlined, equal to 8 bytes per copy/fill expansion inside it, is a depth surcharge, not a wrong body**: `TVector<T>::insert` 0x120 -> retail's 0x100 exactly when `InsertRaw`'s body is spelled in place, but `InsertRaw` is a real UNUSED 0x38c function. Dropping `uninitialized_copy`'s `O dummy = d_first;` or flattening `__copy_backward<T>` unlinks std-vector (an UNUSED symbol's *existence* constrains an MSL body; `__as__15TStageEventInfo` disappearing proves `__copy_backward` assigns rather than copy-constructs). Left open.
- `TDisplay::getRenderMode()`: the RenderModeObj frames (GCLogo 0x20, Movie 0x28, Title 0x30, Game 0x38 — Game pays twice) close only with an unnameable temporary (`GXRenderModeObj& rmo = unk10; return rmo;` or a dead 4-byte non-trivial local); six other spellings are +0. `JDrama::TFrmGXSet::perform`'s 0x78 gap is exactly two `GXRenderModeObj` copies (0x3c each, packed) — retail copied the render mode twice, recorded not applied.
- JGadget `push_back` temp pool: retail has four more temporaries at the bottom and one fewer in the middle than ours — a different expansion structure, not one missing object; five iterator/insert spellings measured (trial table in std-list.hpp). `iterator copy = *this; operator++();` and `iterator copy(*this); ++(*this);` are codegen-identical.
- `TEMario::checkCollision` (UNUSED 0x220) is dead code, not an inline (`perform` spells the loop itself); `execKill` (0x54) has no caller; emario symbol order PASS. **A vtable's size bounds the inheritance graph**: `__vt__9MAnmSound` is exactly `__vt__13JAIAnimeSound`'s 0x10, so MAnmSound has no second polymorphic base and no new virtual; the destructor's extra instruction (0x5c vs 0x60) is a fresh null test with no candidate left in the map.

## Closure batch 89 tells

- **A `getChildren()` level is a real +8 frame lever per call site** (`unk14->getChildren().reserve(n)` closed `TCubeManagerBase`'s ctor, `getChildren().size()` its `load`; adding it to `push_back` too overshoots). Not a level for *depth* (the `return *this` rule) but it binds.
- **An accessor level on a call's argument flips receiver/argument evaluation order**: `getBool(getEventFlag())` loads the argument first, `getBool(unk2C)` the receiver first — per site in one class (`MapEventDolpic::load` vs `watch`), and +4, not +8. Pollution counter-layer rungs: `gpPollution->offLayer(i)` +0, `gpPollution->getCounterLayer().offLayer(i)` +8/site, `SMSGetPollution()->offLayer(i)` +8 total for two sites.
- Chaining a discarded read into the vector's chain (`stream >> v.x >> v.y >> v.z >> unused;`) instead of its own statement is one continuation = 8 bytes (`RiccoMammaGate::load`). `f32 x = a; x += b;` vs `f32 x = a + b;` picks the `fadds` operand order too (`rising`). `unk48.x = unk48.y = unk48.z = 0.0f;` in a ctor body gives descending stores an initialiser list cannot.
- **A caller-side inline level can be the only way to reproduce a depth-3/4 refusal**: one extra level above `TModelDataKeeper::createAndKeepData` (parked `ObjModelKeepModelData`) flipped `loadModelData`+`registerDataAndJoinNewNode` to called and took `createMActor` 61 -> 100% of instructions; retail's `createMActor` duplicates `createMActorFromNthData`'s body. A refusal at 4 statements is depth evidence, not statement-count evidence.
- **A fabricated virtual declaration with no map symbol silently mis-slots a vtable** (`TRiccoHookManager::perform` replaced the inherited `TEnemyManager::perform` word while every function scored 100%): sweep vtables below 100% whose only differing word names a method the map lacks. The MSound rogue-include pair (`MSSetSound.hpp` + `MSoundBGM.hpp` after `InfectiousStrings.hpp`) also supplies `__sinit` and the fifteen 12-byte `.bss` objects (riccohook data 36 -> 100).
- Real fixes: `TLampTrapSpike` ctor's three missing initialisers; `TMapEventDolpicSink::watch`'s missing `MSD_SE_SY_CLEAR_SIGN_BIG`; `TRiccoHook::perform`'s missing else-branch with two crane sounds. `TNerveRHGraphWander::execute`'s distance is the copy-and-subtract shape (parked `riccoHookDistance`; third unit needing it — header item for a shared helper). `getScaling()` is +8 in `touchWater` and +1 instruction in `control` of the same class. Open: `SMS_IsInOtherFastCube` 0x20 vs 0x28 with no referenced slot; `MapEventDolpic::watch` and both MapObjTrap `loadAfter`s are "+4 low / -4 named"; `TLampTrapSpike::control` callee-saved permutation with r27 saved but skipped.

## Second-pass closure batch 90 tells

- **A TU-local `inline` distance helper takes `const f32&` scalars, not by value**: by value swaps the `fsubs` operand registers at a site whose other operand is a fresh load; named `f32` locals passed by `const&` keep their callee-saved FPRs (`updateMantaEscape` 84 -> 100 with `distanceTo(const TVec3&, const f32& x, const f32& y, const f32& z)`; the map has no UNUSED symbol, dating it file-scope `inline`).
- **The batch-86 named scalars must be declared at the statement that first reads them**: at the top of the body they load early; with their initialiser at a later use they get a scratch register plus an `fmr` (`TBGTentacleMtxCalc::calc` 98 -> 99.3 with zero instruction differences; its cross operand is the previous joint's Z column `[i][2]`, the fallback its Y column `[i][1]`).
- **`MsClamp` in place vs into a new variable**: `x = MsClamp(x, l, r)` binds a scratch FPR; only `if (x > r) x = r; else if (x < l) x = l;` clamps in the variable's own callee-saved register (+8 frame trade-off).
- An accessor lever applies per block (`getUnk70()` at one of two identical comparisons is exact, at both not — `updateGateDemoCamera_` closed); a named `const char* name = actor->getName()` used by two calls is +4/+8 (`startGateDemoCamera` closed).
- **An extra inline level is a register lever independent of its frame cost**: a TU-local `sqDistance` fixed `ctrlMultiPlayerCamera_`'s inner-loop `fsubs` pair (its 28 dead low bytes are a `TVec3` local of that helper: a local of an inlined callee gets a stack home where the same statements in the caller's body are scalar-replaced); any level fixes `execPosInbetween`'s FPR pair, so when a leaf's frame is already minimal a level cannot be the answer. `removePlayer` must stay `inline` (retail's expansion carries a redundant `found == false &&` term).
- Carrier test, second confirmation: min/max helpers cannot carry `setCheckData`'s 40 bytes (a dead local there grows the exact emitted `TBGCheckData::setVertex`). `updateTrans` 99.71: fetch `mCheckDatas` before hoisting `delta.y/z`; residue two 12-byte objects (one dead `TVec3` after `delta`, one parameter-area). Open header item reconfirmed: `TVec3::cross()` store order (x, y, reload, z) for `setAttackTarget`.

## Settled in header round 16

- **An `inline`-for-linkage member always expands into its one caller (no budget at depth 1); the fix is an in-class wrapper above it, not a pragma**: `ctrlTowerCamera_(f32)` holding the tower branch puts `calcTowerCenterPos_` at depth 2 where its 9-statement budget is exceeded, so it is a `bl` with weak linkage and the `$localstatic0$` mangling (CameraNormal data 93 -> 100). A function-local static's mangling identifies the weak function, and its caller is then one inlined level short (proved twice).
- **A member accessor's binding is not cheaper than a free function's**: a named-and-returned result inside an inlined member wrapper is +8 per expansion (TBellDolpic), dropping the name +0; no bound local saturates per function — the multi-site sound carrier must be a +4-per-site global-accessor level. A free function's by-pointer parameter binds nothing when the pointer is only the receiver of the single returned member read (`getWaterHitCounter`, member and free identical).
- Promoted: `TMario::getM3UModel()` (kept at the MarioDraw/MarioCap sites that improved — `setAnimation` 94.8 -> 96.0, `calcAnim`, `initModel`, `addUpper`; reverted where an exact function was lost), `mHPMax` with `PARAM_INIT`, `SMSGetCameraOption()`, `SMSGetFlagManager()` (a fork over the in-class `getInstance()` is codegen-identical — batch 95; the +4 measured in SunGlass came from a named result). Three named squares vs a summed expression is per site; no map support for a shared squared-distance helper (`TVec3::squared(const TVec3&)` with named squares costs `TShine::calc`).

## Closure batches 91 and 94 tells

- **Lever pairs**: two +0 accessor levels can be +8 together (`getFludd()` + a named `getIntendedMag()` in `waitingCommonEvents`; `getActorNum()` + `getModelLoaderFlags()` in `createMActor`). Measure pairs before declaring a lever inert. **Mutually exclusive `||` branches do not share the inline temp pool**: a carrier in a callee expanded in three short-circuited branches is +8 each (`SMS_IsInOtherFastCube`, whose last 8 bytes are one two-word object of its own body, unnamed).
- **"+4 low" must go on the earliest inline expansion in the function** (`RiccoMammaGate::watch`: `getJoint()` before `SMS_ShowJoint` closes it; the same level on the next call breaks it). A `getUnk8()`-style accessor at both the guard and the call CSEs to one load (+8); at the guard only +0.
- **A weak `__dt__` with no `~T()` in the class is the implicit destructor**; writing it out makes it global (BINDING failure, position unchanged — seal, TBGPolDrop). Particle owner keys can be `(u8*)owner + 1`. A TU-local `min`/`max` spelled `b > a ? a : b` permutes every FPR pair downstream — use `<algorithm>`. A lone `~` on `lfs f0, 0(r3)` vs `4(r3)` is a field bug (`mGridExtentX` vs `Y`). Counter-example to batch 63: accessor levels on `this` do not always reorder the callee-saved set (`TSeal::init`, seven variants).
- **The named-scalar rule extends to integer vertex loops**: name exactly the reused components of an indexed struct member and leave the once-used one raw (`drawNear` 86 -> 100, `drawFar` 91 -> 100; naming all three regresses). Three named `f32` scalars beat a `TVec3` for a scale triple and their declaration order picks f27/f26/f25 (`TMapObjGrassGroup::load`, x, z, y).
- **A dead 4-byte local after a `char buf[0x100]` path buffer is a three-TU idiom** (`TLensFlare`/`TLensGlow` ctors, `TSunModel::load`); only an unwritten declaration reserves the slot. One inline level above a two-return predicate flips it out of line even at depth 1 -> 2 (`isWaterSurface`, parked `EnemyAttachmentIsWaterSurface`). `self` is a valid virtual-call receiver to check (`sendMessage` calls `this->kill()`; two more in enemyAttachment).
- **An out-of-line SDK function can be re-derived as a FIFO writer**: `FifoSetFog`/`FifoSetFogRangeAdj`/`FifoSetTevColorS10`/`FifoSetTevKColor`/`FifoSetChanMatColor` keep the register encodings, drop `SET_REG_FIELD` masking and `gx->bpSent`, write `GXWGFifo.u8 = 0x61; GXWGFifo.u32 = reg`, and interleave computation with writes (up-front computation cost 30 points). A `.data` jump table in the missing list dates an unwritten switch and its entry count is the case count (`ShapePacketCallBackFunc`, 11; `SMS_InitPacket_CallDL` recovered from the UNUSED list). `static` vs `static inline` on a TU-local helper is decided by the map (PacketUtil's four helpers are UNUSED, so `inline` is wrong there — the opposite of `calcDist`).
- **A vtable at 75% with 5 slots is a declaration-order swap, usually with crossed return types** (`PollutionCount`: `getCounterNo` -> `int`, `getTokenNo` -> `u16`, evidenced by a stray `clrlslwi`/`clrlwi` at the callers). `SetMActorAnmFrame(MActor*, f32, bool, bool)` is 0x84 with one function-scope `J3DFrameCtrl*`. `System/DummyStrings.hpp`'s prefix also fixes pointer relocations (`const char*` globals at 50%).
- Real bugs: seal's `receiveMessage` has no base tail call; MarioWait's two masks (`0x4000`, `0x8000`); `addJellyFishParts` wrote `&unk0[5][1]` for `&unk0[0][11]`; enemyAttachment's `behaveToHost()` vs `kill()` and `checkGround` into `&mGroundPlane`. Open: `TLensFlare::perform` (three inline levels below, needs `inline` fabricated members), `TNPCParts` ctor (flat 24-element loop unrolled by eight), `ShapePacketCallBackFunc` (8 bytes per inlined expansion, depth surcharge), `getGridArea` 112 low bytes.

## Structural batch 97 tells

- **The depth at which a library helper stops inlining is set by how many levels the call site sits below the function, and one fabricated level moves several helpers at once**: on `TLensFlare::perform`, a `CLBCalcNearNinePos` fovy/aspect wrapper at depth 3 leaves `TVec3::set(const Vec&)` (3 statements) out of line at depth 4 and `JMASSin`/`JMASCos` at depth 5 while 1-statement `fakeTan` still expands (two thin `static inline` levels in the .cpp; +3 exact functions; the `lookat` argument is `gpCamera->unk148`, a real bug). An out-of-line `TVec3::set(const Vec&)` is evidence the source member is typed `Vec`.
- **One statement decides call-versus-expand at depth 1, and a bare `int i;` declaration does not count toward the budget while an initialised one does** (`TCameraMultiPlayer::removePlayer` now a plain method at the map's exact 0x88 with `removeMultiPlayer` still exact: `if (found == true) mPlayerCount -= 1; return found;`). `if (found == true)` emits `clrlwi; cmplwi 1; bne`, `if (found)` emits `clrlwi.; beq` — not interchangeable.
- tobiPuku's four UNUSED helpers landed size-exact by trimming against the map one statement at a time; each is a block a nerve pastes in (`canBound` 0x2c ends with the `unk1AE = 1` store). hinokuri2's per-site `((THino2Params*)getSaveParam())->` is net-positive (Burst +2.5) but costs PrePol 0.8 (the wrapper's level folded `addi 0x170` + `lwz 0x10` into `lwz 0x180`). MapWireManager's `dont_inline` trade-off is quantified both ways (one exact function beats two UNUSED sizes).

## Settled in header round 17

- **A trailing `li r3, 1` in a callback body is a return type, and a sibling typedef names it** (`J3DCallBackPacket::CallbackT` returns `BOOL`, pinned by `J3DNodeCallBack`; callers discard the result so the change is free). **One index register serving two members merges them into one array** (`TNpcModelData::unk0[2]`; brace elision keeps the data bytes).
- **A read and a write of the same member can want different spellings**: `removeCheckListData`'s store goes through an inlined `setEntryStart(int, u16)` (address lands in r3), the reads stay raw (a getter is +8 and no help). **Wrong parameter order can hide behind a compensating swap in the callee**: `makeWorldToPollutionMtx` is `(scale, min_x, min_z)` — the load order at the inline sites exposes it; `calcViewMtx` closed with `getMinX/getMinZ` + `getModelStampDrawBuffer(i)` + `getCounterNum()` on the loop bound (lever pairs, fourth confirmation; a "+12 low and -4 named" need is usually one +12 low, the pad above the named block being alignment). An accessor level on a loop bound is a cheap register lever (`getJointObjStampTaskNum()`, 30 diffs -> 4).
- **The map's out-of-line copy is the body-shape oracle for a header inline**: two `blr`s = two-return form, one exit = ternary (`TBGCheckData::isIllegalData` 0x1c). Counter-example to round 9: the two-return shape alone does not produce the `&&`/`||`-right refusal — it still inlines at depth 2 behind `isLegal()`; the rule needs the extra level as well. `isLegal()` forwarding to `isIllegalData()` costs `TMario::checkGroundPlane` (rejected). `SMS_IsWaterSurface` promoted to `MapData.hpp` (codegen-identical; Yoshi's two refusals unreproduced; `>>> UNREFERENCED DUPLICATE` rows enumerate which TUs compiled a copy — a per-site refusal census, and a weak duplicate with no surviving `bl` is a dead-stripped refusal). `TModelDataKeeper::keepModelData` promoted. `scShineTable*` layout is byte-identical in the one includer where nothing is stripped (Guide.o) — do not regroup. `tools/check-weak-defined.py` needs `--nm build/binutils/powerpc-eabi-nm`.

## Closure batch 101 tells

- **`x / 2.0f` and `x * 0.5f` are different codegen**: division by a constant power of two folds to `fmuls` with the dividend as the left operand; a literal multiply always puts the constant left (`TLeanBlock::initMapObj`; MapObjBlock linked, its `TIceBlock::calc` scratch buffer is `Mtx44`).
- **`JDrama::TNameRefGen::search<T>` is fabricated; the real entry point is `search2` with the cast at the call site**: the template's `static_cast` shows as `addi rD, rS, 0` where retail has `mr`, and costs 8 bytes of frame (`makeShineAppearWith*` x3 closed). 306 `search<T>` sites across ~40 units — a sweep item.
- **`SMS_GetLightPerspectiveForEffectMtx` writes `mtx[3][*]`, so every caller's buffer must be `Mtx44` (64 bytes)**: eight other callers (`bosseel`, `namekuri`, `bosstelesa`, `telesa`, `Shimmer`, `MapStaticObject`, `NpcParts`, BathWaterManager's `TProjection3f`) declare a 48-byte `Mtx` — a real stack overflow and exactly their 16-byte frame gap (Shimmer carries a fabricated `Vec lightPos` to pay 12 of it). Sweep item.
- Two +4-low levers stack (`SMSGetMarDirector()` fork plus one `const TVec3&` accessor read for the first `setScale` only, `TTelesaBlock::perform`); lever pairs, fourth confirmation (`getMapObjData()` + `getInitialScaling()`). **The dead-scalar C-style declaration block is a three-unit idiom** (`TAreaCylinder::load`, `TMapWireManager::load`, `TMario::canSleep`) and its position is load-bearing: declared last it lands below the address-taken locals. **A parked accessor can need a strict subset of its sites** (`getGamePad()` at four of five reads in `squating`). `NpcWalkTurn`: one inline level between `isCanWalk` and the unnamed vector's ctor (a squared-XZ helper inside `isCanWalk`) reaches the map's local `TVec3::set<f>` by `bl` (symbol order PASS). `TMapWireManager::getPosInWire` allocates retail's eight 12-byte slots 16 lower with two permuted: a term-order problem.

## Research batch 104: why retail calls a weak header body where we expand it

The "weak symbol emitted *and* `bl`-ed at a site we expand" family splits in two, and the larger half is not a new mechanism.

**The whole `JGeometry::TVec3<f>` family is the depth table, and each retail `bl` is a depth measurement.**
Every one of the 22 `TVec3<f>` member symbols in the map is weak, emitted once and called, and the members are in-class after all: `div__Q29JGeometry8TVec3<f>Ff` (weak, 0x30, `boid.cpp`) *expands* `scale` inside its own body, which no never-inline reading can produce.
Measured against the real `JGVec3.hpp` with the game flags, with one-statement forwarders as the levels:

| member | statements | expands through | first `bl` |
| --- | --- | --- | --- |
| `scale(f)`, `scale(f, const&)`, `sub`, `add`, `set(const&)`, `set(f,f,f)`, `negate` | 3 | depth 3 | **depth 4** |
| `dot`, `operator=` | 1 | depth 4 | **depth 5** |

That is exactly the published allowance (2 at depth 4, never at depth 5), so a retail `bl` to a three-statement vector member says the site sits **three inline levels** above it and a one-statement member says four — the same reading batch 97 got from `TVec3::set(const Vec&)` at depth 4 under `TLensFlare::perform`.
Census for planning: `set<f>(f,f,f)` 184 retail call sites, `sub` 130, `scale(f)` 64, `dot` 49, `add` 40, `scale(f, const&)` 40, `set<f>(const&)` 23, `setMin`/`setMax` 10 each, `setLength(const&, f)` 9.
`TTamaNoko::landEffect`'s four sites and `TRope::moveHead` are depth-4 sites; a `bl scale` bracketed by a copy in and a copy out is a by-value hand-off through those levels.

**Do not make the vector members never-inline to get those `bl`s.** Trial, reverted: moving `scale`'s body out of the class as a primary-template member (see the next rule) gives `landEffect` 57.2 -> 82.8 and `TIgaiga::setMeltAnm` 84.4 -> 96.1 and costs **217 functions**, including byte-exact `JPAParticle::calcVelocity`, `TBeeHive::controlSound`, three `CubeMapTool` loads and `div` itself (100 -> 19.6); total fuzzy 97.27 -> 96.99.

**New rule — a member of a class *template* defined outside the class body without `inline` is never inlined, at any depth, in any caller.**
MWCC 1.2.5 instantiates one weak out-of-line copy per TU and emits a `bl` at every site.
This is the only source shape that produces "weak plus a `bl` everywhere", and it is invisible in the map, so it has to be inferred from the call sites.

| callee | site | linkage of the callee |
| --- | --- | --- |
| class template, body in class | expands | none emitted |
| class template, body out of class, `inline` | expands | none emitted |
| class template, body out of class, no `inline` | **`bl`** | **weak** |
| explicit specialisation (`template <> struct T<f32>`), body out of class, no `inline` | expands | global |
| explicit specialisation, body out of class, `inline` or `template <>`-qualified | expands | weak / none |
| non-template class, body out of class in the same TU | expands | global |

Corollaries: a weak symbol called at **depth 1 from a plain emitted function** cannot be reached by any call-site or depth change, only by moving the definition out of the class body of a class template; and an explicit specialisation can never be spelled to produce it, so `TVec3<f32>` would have to become the primary template (`template <class T> class TVec3 : public Vec`, s16 kept as a specialisation after a forward declaration) for the shape to be available at all.
In-class **member** templates are a third category: their instantiations are `local`, not weak, and stay inlinable per site — which is what the map shows for `set<f>`/`__ct<f>` and why `JGVec3.hpp` keeps them in class.

**Ruled out for the two remaining depth-1 refusals** (`TTelesaSlot::TTelesaSlot(const char*)`, weak 0x98, `bl` straight after `__nw__FUl` in `getNameRef_MapObj`; `std::sqrtf`, weak 0x64, `bl` with the squared sum computed in `MSoundSE::startSoundActorWithInfo` itself).
Each was probed in a scratch TU with the game flags and each **inlines**, so none of these is the lever: callee statement count for an in-class constructor (1 to 30 statements, no limit at depth 1); a base-class initialiser, two base classes, a vtable and a virtual destructor, an array-of-class member needing `__construct_array`, and a defaulted `const char*` argument (all together, still expanded); caller size (64 expansions, 1732 inlined instructions) and distinct-callee count (256 different in-class constructors in one `new` ladder, all expanded); callee body content (`u64`/`s64` conversions that emit their own runtime `bl`, a `%` runtime call, a `static` local, a `while`/`do` loop, an `__frsqrte` chain, a `volatile` local, a call to an external function); definition order and `-inline deferred` order (definition after the call site, and a callee calling an inline defined later); float-register pressure at the site (up to 18 live `f32` locals across it); and a by-value or by-reference inlined parameter as the receiver.

## Sweep batch 105: search2 and Mtx44

- **`search2` vs `search<T>` is codegen-identical in 95 of 115 measured groups**; the template's `static_cast` bites only where the cast's temporary changes register allocation (21 groups won, one lost: `TBossWanwan::init`). 13 functions to exact (eleven EventWatcher builtins including the three wood-box ones that were exactly 4 bytes short, two NpcEvent, `TLightWithDBSetManager::loadAfter`). Receiver forms (`instance->search<T>()`) are inert. **Do not delete `search<T>`**: 256 sites remain, ten inside exact functions. An identity cast to `JDrama::TNameRef*` is still load-bearing at `MarDirectorSetupObjects.cpp:229`, which fits a looser retail return type (`void*`) — follow-up in `JDRNameRefGen.hpp` (touches linked units).
- **`Mtx44` for every `SMS_GetLightPerspectiveForEffectMtx` caller**: `TNameKuri::perform` closed, `TBEelTears::perform` 99.98, `bosstelesa`/`MapStaticObject`/`telesa`/`NpcParts` frames +16 toward the ROM (real 16-byte stack overflows removed); Shimmer's fabricated `Vec lightPos` replaced by the same bytes (byte-identical, still linked); `TProjection3f` already holds a 4x4.
- Driver notes: a measurement must rebuild before it reads the `.o` (a stale object silently discarded four real keeps), and receiver expressions must be swallowed with the call when rewriting.

## Depth-level sweep 106: JGVec3.hpp's operators supply the levels

Census: 531 retail `bl`s to the seven `TVec3<f>` members over 332 (function, member) pairs; 170 of the 230 candidate functions already emit the `bl`; 17 of 20 tried functions kept (koopajr/limitkoopa `bind` 68/75 -> 99.9, `TLeanMirror::loadAfter` 75 -> 99.9, `TCannon::moveObject` 90 -> 99.9, `TFlyEnemy::fly` 90 -> 99.7, `TBossHanachan::execWalk` 96 -> 99.6, chuuhana `bind` 92 -> 100, tamaNoko `landEffect` 57 -> 95, ...; `isCanWalk` 0xc0 and `updateAttachPos` 0x1c0 size-exact as side effects).

| retail shape | source spelling | depth reached |
| --- | --- | --- |
| `bl sub` | `a = b - c` or `TVec3 d = b - c` | `operator=`/copy-ctor 1, `operator-` 2, `operator-=` 3, sub 4 |
| `bl add` | `a = b + c`, and `a = a + b` instead of `a += b` | add 4 |
| `bl scale(f)` | the product inside an *inlined* call's argument (`setGlobalScale(mScaling * 0.8f)`), or `TVec3 v; v = m * k;` | scale 4 |
| `bl dot` + `bl sqrt` + `bl sub` | a file-scope `inline` length level `H(a - b) { return v.length(); }` | sub 5, dot 5, sqrt 4 |
| `bl dot` + `bl scale(f, const&)` + `bl inv_sqrt` | one thin level above `v.normalize()` | dot 5, scale 4, inv_sqrt 4 |
| `bl set<f>(f,f,f)` | a squared-XZ helper called from an inlined `isCanWalk` | ctor 3, set 4 |

- **An inlined call's argument costs one level; an out-of-line call's argument costs none** (evaluated in the caller). Anything whose value is *stored* (assignment, named local, by-value parameter/return) accepts a level because the store is the level; `MsGetRotFromZaxis(a - b)` with no copy-out is refused, the same expression assigned to a named local lands. The copies bracketing a `bl` name the spelling: before it, `operator-`/`operator*`'s by-value left operand or a by-value parameter; after it, a named local or by-value return.
- Per-site is a spelling difference: `TTPHitActor::bind` calls `sub` for `moved = pos - mPosition` while `updateTerrainCollsion` expands the identical idiom; Tongue's `movement` has `a = a + b` at exactly two of five tip advances. Refused: `TNerveFlyEnemyChaseFly` (three levels above `dst.set(...)`, retail also `bl`s `MsCos` there — depth 5 for one-statement callees). Open research lead: Koopa and limitkoopa `perform` each have five identical `emitAndBindToMtxPtr` + `setGlobalScale(flameScale)` blocks and retail expands `set<f>(const&)` in the first and `bl`s it in the other four at identical depth — a per-caller budget or repeat-callee heuristic. Header item: `TPathNode(THitActor*)` zeroes `unk4` through a `TVec3(0,0,0)` temporary (BeeHive's four nerves each show one more retail `set<f>` `bl`). The batch-104 note that tamaNoko's levels belong in a header is overturned (in-TU `setGlobalScale(mScaling * 0.8f)` closes it).

## Settled in header round 18

- **The dead-low-region carrier can be a shared header inline with no map symbol**: `MSCreateRandPlayVec`/`MSRegisterRandPlayTrans` in `MSoundSE.hpp`, each holding one uninitialised (sized, unnamed) `TVec3`, closed all four Animal `loadAfter`s (`TAnimalBase`/`TAnimalBirdManager` each needed one further +0 accessor as a lever pair); AnimalManager linked (449). The parameter shape picks the per-expansion price as the batch-69 table predicts; a copy-constructed `TVec3` cannot be it (it emits the copy).
- **A lever added to one shared inlined helper can be paid for by removing the equivalent lever from a sibling overload the same callers use** (compensating pair): the named `timeArray` in `endTimer` is neutral for the linked `TLiveManager::perform`/`TObjManager::perform` when the four-argument overload loses its own. `instance()`-style accessors can be load-bearing for the caller's register numbering, not just its frame. `startTimer(u32)` still needs a pure (-8, 0).
- **A source local outranks a compiler-generated global-address temporary only when it is live before that temporary is materialised**: `readBuffer.frameNumber = frame = 0;` reproduces `AudioDecoderForOnMemory`'s whole ranking (89 -> 97.7); declaration order ranks source locals among themselves only. `TMapObjRevivalPollution::loadAfter` is a swap of two groups with internal order preserved, and the liveness rule does not transfer.
- Promoted, codegen-identical: `JAIBasic::getData()`, `TMario::getGamePad()`, `SMSGetMap()`, `TBaseNPC::getUnk1A0()` (+8 at the angle2 site, `execTurnToFirstState` still 8 short). `check-weak-defined`: NOBODY is down to ShadowUtil's two `$localstatic` guards.

## Closure batch 108 tells

- **A wrapper whose body is `return cond ? f(x) : nullptr;` binds 8 bytes per expansion; the `if/else` with two `return`s binds nothing, and naming the result binds 4 more** (`TMapObjManager::loadMatTable`, 14 expansions: 0xe0 -> 0x150 -> 0x188, instruction count unchanged). The `MSound::startSoundActor` carrier rule generalised to an ordinary TU helper: a pure +8/site lever with zero instruction change. `TMapObjManager::load` closed with it plus a director fork; `newAndRegisterObjByEventID` closed (shine buffer is `char[256]`).
- **A defaulted constructor argument is a real inline level** (`TTelesaBlock(const char* name = "テレサブロック")` puts the 3-statement `TVec3::set<f>` at depth 4 = `bl` and binds the `this` the ROM spills; measured, header item). **A chain of assignments to one value keeps one register; separate statements pay an `fmr` per group** (`TMatrix34::identity()`'s nine zero stores are one chain written in reverse store order; header item). `TMerryPole::unk138` is `TPosition3f` (the ROM `bl`s the empty `SMatrix34C` ctor at depth 5). Together they close `newUniqueObjByName` and `TMapObjFlag`'s ctor (MapObjManager code 42 -> 97; diff saved in the batch-108 scratchpad).
- **`void TSpcInterp::push(int v) { push(TSpcSlice(v)); }` as a level** puts `TSpcStack<TSpcSlice>::push` at depth 4 for builtins reaching it through a static helper, producing the map's weak `push__21TSpcStack<9TSpcSlice>FRC9TSpcSlice` (`evIsNpcSinkBottom` 72 -> 100, NpcEvent symbol order PASS; measured, header item). `search2` binding its result before returning is +8/+16 per expansion (parked `ECTSearch`; `initECDisp` overshoots — needs a whole-tree measurement).
- **A group of UNUSED accessors all short by the same instruction count share a missing prefix** (LightUtil's three draw-buffer getters: the `index > unk1C` clamp). **Two `MTXMultVec` results written to the same `r1` displacement are one source variable** (`setLight`'s fabricated second `Vec` removed; frames exact). **A one-ULP float literal is a real data bug** (`0.91346145f`, `.sdata2` singletons at 75%). `reviveOneSunflower` 81 -> 99.9 (named `const TVec3*`, director fork before the nine-argument `fireStartDemoCamera`, `search2`, named `TFlagT<u16> demoFlag(0)`; `CheckNerve4Npc_` pushes a plain `int`). Counter-example: adding a sparse switch's missing labels as an empty group does not flip a compare tree to a jump table (MWCC folds a case equal to the default; `decideMarioPosIdx`). The EventWatcher live-actor trio is the inverted "last 8 bytes" (8 unreferenced bytes above the top local): the level is one below `search2`.

## Settled in header round 19

- **A defaulted constructor argument and a one-line forwarding overload are both real inline levels** — cheap, non-local levers that change no instruction in the callee but shift every expansion's depth by one: `TTelesaBlock(const char* name = "テレサブロック")` flipped `set<f>` to the map's `bl` (`newUniqueObjByName` 97 -> 100, MapObjManager 68 -> 96.5 code, MapObjBlock still linked); `TSpcInterp::push(int v) { push(TSpcSlice(v)); }` emitted the weak `TSpcStack<TSpcSlice>::push` (`evIsNpcSinkBottom` 72 -> 100, NpcEvent symbol order PASS; EventWatcher's builtins reach `push` without the static helper and do not move). `TMerryPole::unk138` is `TPosition3f`.
- **An assignment chain is safe to spell globally where a per-site level is not**: `TMatrix34::identity()` as one nine-deep chain changed register pressure inside one body with zero regressions tree-wide (`TMapObjFlag::TMapObjFlag` closed).
- **The out-of-line UNUSED copy's map size is a second, independent depth probe**: for `TBGCheckData::isIllegalData` the pair (`execMapCollision_` stays 0xbc *and* `moveHead` gains the `bl`) pins exactly two missing levels between `TSphereLink::execMapCollision_` and the predicate (one changes nothing, three overshoot) — which is why the `isLegal()` trial supplied only half. The second level is unidentified; measurement recorded in `MapData.hpp`.
- **A level that closes one caller family and breaks another is per-call-site, and the counter-example is in the diff, not the totals**: `TPathNode(THitActor*)` initialising `unk4(0,0,0)` closes BeeHive's nerves (94 -> 99) but forces the `bl` at every other TPathNode site where retail expands (`TNerveWalkerTraceMario` 100 -> 94, ~15 units); `search2` binding its result wins four MarDirectorInitECT functions and loses 18 exact ones. Both rejected with trials at the declarations.

## Closure batch 114 tells

- **A result variable the ROM initialises before a `bl` must be declared before that call in source** — MWCC sinks the `li 0` otherwise and loses a callee-saved register (`CheckNerve4Npc_`: `evCheckCurNerve4Npc` 93 -> 99.7). One TU-local level flips the two-return `TSpineBase::getLatestNerve` out of line even from a static helper (`evCheckLatestNerve4Npc` 87 -> 99.7; shared carrier: a `getLatestNerve()` forwarder on `TLiveActor`).
- **A parameter register the prologue clobbers with the `.rodata` base dates a member function as `static`** (`TMarDirector::initECDisp`; MWCC mangles static and non-static members identically — measured zero-regression, header item). **An inlined bool-returning member call whose receiver is a fresh stack load binds 4 bytes; on a register receiver it binds nothing** (`makeObjAppear` vs its two exact siblings; the topmost local's offset is a byte count of everything below it, so such functions are bisectable by deleting blocks — still 4 over).
- `TItem::calc` 72 -> 99.9 (missing `isActorType(0x20000022) || isActorType(0x2000002A)` branch through the vtable), `TCoinRed::TCoinRed` closed (descending-store chain in the ctor body — the ROM uses it in place of initialiser lists across the MoveBG factory), `TNozzleBox::touchPlayer` 91 -> 99.9 (balloon ids `0x5A`/`0x59`, raw low bytes). `TItemNozzle::calcRootMatrix` is exactly one 16-byte dead local (bracketed by 48/12/16-byte probes; unnamed). The "push an inlined accessor result" family has a single signature (ROM stores the slice's `mType` first, then evaluates the pushed expression) and no lever found. `getNameRef_MapObj`'s remaining residues are enumerated in its TODO (nine ctor-body initialisations in other units' headers; everything from 0x2580 on cascades from the `TTelesaSlot` refusal).

## Settled in header round 20 (getters)

- **A group of unused in-class getters can be codegen-neutral even though a single one need not be**: a 12-getter `Mario.hpp` set and a 37-getter mixed set measured zero movement tree-wide, as did pruning them — protocol: add the set, measure once, sweep, prune, re-measure (~40x fewer builds than one at a time). +26 exact functions this round. **A base getter that subclasses duplicate privately is free to hoist**: `TLiveActor::getSpine() const` is byte-identical to four private copies (for a pointer returned by value, const-ness is codegen-irrelevant) and is the strongest own-class lever measured (15 of 43 groups, 14 to exact). Kept `TMario::getForwardVel/getVel/getInput/getStatusTimer/getSlopeAngle/getStatusState/getWallPlane` (7 to exact); `getFaceAngle/getModelFaceAngle/getFloorPosition/getSlideVelX/Z` paid nowhere and were removed. Const-blocked pairs are only three in the whole population (`getHolder() const` + `getLodAnm() const` closed `TBaseNPC::isNeedNeckStraight`).
- **A level above a body is not interchangeable with a binding inside it**: `TRailMapObj::checkRailFlag` binding its own result costs `TRailMapObj::control` while the parked level above it closes `TRideCloud::control`; a *new* binding member (`getGraph`/`getCurrentNode`) equals the parked free function, and a binding member accessor equals the parked helper when the helper's only binding is its return (`TalkCursor::getMActor`, `MSound::getBgm` promoted). **A binding put into an existing shared accessor is paid by every caller** (`MActor::getModel()` binding breaks the DOL; `TMapObjBase::getMapCollisionManager()` costs three functions). **A level can be spent below a function instead of above it**: `TTimeRec::endTimer()` forwarding to a shared private `appendTime(u32)` reproduces the caller-side level, stays neutral for the linked callers and moved `performShared` — splitting a duplicated body into a shared helper is the credible way to buy one level. `TMarDirector::initECDisp` is `static` (accepted). `TLiveActor::getLatestNerve()` forwarder promoted (two sweep keeps). Adding `Spine.hpp` to `LiveActor.hpp` and `Graph.hpp` to `MapObjRailBlock.hpp` was BSS-neutral (no statics).
- Factory-inlined constructors, three readings: a *missing* store means the initialiser must be dropped (`TAmiKing::mEffectPos`), a ctor with no definition and no map symbol belongs in the class (`TSandBlock`), a whole-branch class mismatch shows only as a differing `__vtable` word (`new THideObj`, not `THideObjBase`). **Short string literals live in `.sdata2`**: `addi rN, r31, off` against a ROM `li rN, @NNNN@sda21` means the string is the wrong one (`TMapObjTree`'s default `"木"` overridden by an explicit argument), not a section difference. `getNameRef_MapObj` 97.35 -> 98.89; everything left is the `TTelesaSlot` cascade. Census: 475 near-exact functions remain, 676 (class, member) pairs without a getter (`missing.txt` etc. in the batch-117 scratchpad; `TCardLoad`'s `unk*` and `JPADraw::mDrawCtx`/`MActor::mAnmByType` were added and pruned as unpaid).

## Closure batch 122 tells

- **A literal `* 0.03125f` and `/ 32.0f` are different codegen inside an `fmadds`** (the division keeps the dividend as the multiply's left operand; the literal puts the constant left; the `.sdata2` entry is identical either way) — `TMario::doSwimming` closed; MarioSwim linked with a named `dashMax` (+8) and a lever pair (named params `.get()` + own `getPosition()`); collapsing three `getStatus()` sites into one named local is -8 (how to price an accessor already present).
- **Naming an inlined helper's receiver fetch can be a pure callee-saved fix with no frame change** (`J3DModel* model = getModel();` before `setup(model, ...)` restored `TBaseNPC::init`'s r28-r30). **A negative placement-new alignment is a real source property** (`new (-0x20) char[N]`, tail allocation; `li r4, -0x20`; mixed signs in one function — `TMarDirector::loadResource` 71 -> 99.95 with its eight-case map switch written in source order 33, 5, 58, 56|57, 9, 52, 4, 60: **MWCC emits switch arms in source order**, while the pivot tree is value-sorted, so the two give the case set and its order independently). `loadParticle` 99.99 (restored `staticLoadParticle` and the pre-allocated hanachan buffer).
- **The named-scalar rule applies to volatile FPRs too**: in a call-free tail block retail kept `oldUp.y`/`oldUp.z` in f4/f5 (`TCameraShake::execShake` 94 -> 96). **A retail 16-byte record copy into a struct member is the tell for a whole-object local** (`TWalker::bind` builds a `TPathNode` local and assigns it; retail dead-strips our default ctor's four zero stores — open). Declaration order of two `f32`s in a loop body is readable off which gets the higher volatile FPR (`radius` before `d2`, `calcBoids`). Two more members of the by-value-return population (boid's three, walker's `bind`): a `<`-only run of six `stw`/`lwz` between two operator `bl`s and a frame too *big* when named locals stand in for the missing return slots. `setIndividualDifference_`: retail loads `initInfo->unk34[j][i]` twice with two different address associations (no CSE). Header item: `TLiveActor::checkLiveFlag2` returns a 32-bit type (retail `cmpwi r4, 0`, not `clrlwi.`). Tooling: the fuzzy percentage and the marker count can disagree in direction when the instruction count moves — use markers for "right source", fuzzy for "lands".

## Closure batch 123 (Sonnet calibration) tells

- A dead trailing `u8[1..4]` in a caller's own body is absorbed for free; 5+ bytes overshoots by 16, not 8 (caller-scope refinement of "the last 8 bytes"). `bVar1 += uVar5;` reuses the mutated variable's register where a named `threshold = bVar1 + uVar5;` does not (declaration-then-operation for integers; `startSoundSetDyna`). **A struct's last element wants a named pointer, not repeated subscripts**: `T* p = &arr[n-1];` gives retail's direct address where `arr[n-1].x/.y/.z` computes one-past-end-then-back (`TMapObjFlag::draw` 94 -> 98.9, 122 structural markers -> 0). A `TPathNode`-typed field assigned through its setter (`setFleeTarget(THitActor*)`) gives a fixed-base struct copy where direct `=` through the converting ctor gives a pointer-advancing `stwu` copy (`TFishoid::load` 96.6 -> 99.8). `FifoSetFog`'s `fog3` is `(c_hex >> 12) | (type << 21)`. `TMushroom1up::control`: a named `f32 deg = 5.0f * t` shared by `JMASin`/`JMASCos` plus `mPosition = pos` (raw struct copy) 86.7 -> 99.8. Open: `TJumpBase::control`'s missing `unk13C = 0; unk138 = 2;` pair; `TSpineEnemy::isReachedToGoal` wants the copy-and-sub distance spelling in Enemy.hpp.

## Settled in header round 21

- **A member accessor cannot reach the free function's rung**: the parked `MarioMainGetFludd(TMario*)` is +16 = 8 (by-pointer parameter binding) + 8 (binding a value that came through an accessor); a member has no parameter to bind and caps at +8, and making the existing `getFludd()` bind breaks the DOL. Conversely a member accessor and a by-pointer free helper are interchangeable when the helper's only binding is its return (`TSmallEnemy::getJuiceBlock()`, `TMario::getYoshi()`, `SMSGetMarioOriginal()`, the binding `SMSGetMapObjManager()` — all promoted codegen-identically; sweeps of their other sites found one keep, `TBossHanachanManager::loadAfter`).
- `MsMtxSetRotX/Y/Z` already live in MathUtil.hpp; the header's `JMASSin(DEG2SHORTANGLE(x))` and a `JMASin(x)` spelling are codegen-equivalent (bgpoldrop stays linked); `TBossGesso::calcRootMatrix` closed with `MsMtxSetRotX(local_50, 90.0f)`. **A dead trivial 48-byte `Mtx` in an inlined callee is worth exactly zero** (MWCC drops an unused POD array); `J3DFrameCtrl::update` is a real `bl` twice in `TLensGlow::perform`, so it carries no pool — refuted. `TVec3(T,T,T)` forwarding through a helper is per site (eight units gain the `bl`, seven exact ones lose it: -28 exact); an initialiser list calls the ctor directly so a deriving class's header has no room for a level. `search<T>` binding its result: -60 exact, same family as `search2` (single-`search` `init`/`load`/`loadAfter` bodies and twenty EventWatcher builtins) — closed as a header idea. A 27-getter set across 20 headers was neutral (third confirmation) and yielded one keep (`TNozzleBase::getUnk372()` closed `movement`): this tier of the census is near exhausted (1 keep in 26 groups vs round 20's 38%).
- Tooling: a sweep driver must rebuild before snapshotting its baseline (a `restore()` without rebuild leaves `report.json` describing the last trial and produces phantom gains); a header trial that breaks the DOL still yields its report (`ninja build/GMSE01/report.json`); the write filter must cover compound assignment and `[i] =`.

## Closure batch 126 tells

- **`JUTNameTab::getIndex` narrowing is per call site, decided by whether the index crosses a call**: an `s32` local with `(u16)` at the use where a `bl` sits between (retail keeps the raw value callee-saved), a `u16` local at the assignment where the receiver is already in a register (four NpcEffect functions to exact). **Two loops over one range with only one touching the object are a single loop with the guard inside** — MWCC unswitches and dead-strips the unused `getObj()` load (`TEnemyManager::perform` closed; real bug: the cleanup writes `mHitFlags |= HIT_FLAG_NO_COLLISION`, not `onLiveFlag(DEAD)`). `TSharedMActorSet::calcAnm()` is the map's 0x64; `copyFromShared` closed by declaring the concat scratch matrix before the saved view matrix.
- **`MsRandI`'s argument order is readable off the registers**: the last-declared bound is loaded first into the lower callee-saved register and `subf r0, rLow, rHigh` names `l` (NpcNerve; real bug: `TNerveNPCGraphWait` reset the wander timer instead of the wait timer). A `const T&` binding to an accessor that already returns a reference elides the copy and loses retail's three `stw`. **A jump table whose grouping matches tells you how many instructions the function is out by** (addend delta / 4) and brackets the region (CameraChange `@3715` 4 bytes, ModelGate `@3054` 4, SelectMenu `@3640` three blocks). The unfoldable `bcc; b` pair is a property of the guard's source grouping, not the body (merging two early returns with `||` produced it; ten spellings of `popThing`'s guard never did). **One body, two inlining decisions**: a TU-local level that puts `MsClamp` out of line in `changeCamModeSub_`'s expansion also puts it out of line in `setUpToLButtonCamera_`'s own emitted copy (100 -> 79) — the level must go above the enclosing function. A trivially dead sibling channel (`u8 midA`) is a credible +8. A fabricated `volatile` on a dead local array is not load-bearing (SelectMenu; its `stages[]` is the Etc shine-conv table, the name box reads `stagename.bmg`). **A dtk false relocation can be in `.text`**: `MSD_BGM_SCENARIO_SELECT = 0x80010024` equals `TBeeHive::doWait()`'s address, so `startOpenWindow` can never score 100 — the whole `MSD_BGM_*` range (0x8001xxxx) needs a `block_relocations` sweep. `TAfterEffect`'s radial-blur direction is one `TVec3` member at 0x5C (`ScreenUtil.hpp`, header item; read off `TModelGate::screenBlur`'s two-copy signature). NpcNerve's `getCurGraphIndex` (weak 0x8) `bl` needs four levels (an unreconstructed `TBaseNPC` predicate above `hasOnlyOneNext`, plus the argument position).
