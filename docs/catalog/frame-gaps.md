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
   - `yunbo`: naming a param fetch used in a comparison (`s32 recoverTimer = ...->getSLRecoverTimer(); if (recoverTimer < spine->getTime())`) added the missing 8 bytes of `TNerveYumboFreeze::execute`; declaring a scalar *before* the vector it is compared against fixed `TNerveYumboAttack::execute`, because an inlined helper's locals lay out in reverse, so the inline site and the UNUSED out-of-line copy want opposite orders (declare for the inline site; the size check still passes).
   - Open: `JGeometry::TQuat4<f32>::rotate` declares ~60 bytes of locals per expansion (`vx/vy/vz`, `w/z/y/x`, two `TQuat4` temporaries); `TYumbo::shotSeeds` inlines it twice and is exactly 0x68 too large with matching instructions. The header already notes the wrong register allocation; trim it from a unit that owns the header.
   - `NpcAnm::sunflowerReviving`: `int kind = unkD0->getCurrentAnmKind()` named inside the flag guard fixed 0x20 vs 0x28.
   - `TMarDirector::loadParticleMario` touches only globals; declaring it `static` removed the implicit `this` slot and fixed 0x20 vs 0x18.
     An unused `this` alone does not prove a method is static; check callers and member-pointer use.
   - Named `SMSGetMarDirector()->getConsole()` result (HelpActor, EventWatcher) and a named `TFlagManager::getInstance()` result (SleepBossHanachan `startFall`) restored frames.
   - `MapCollisionPlane`: computing into a named `int` and returning it gave frame 0x88; returning the float helper directly gave 0x80, naming its int result gave 0x90.

Hypothesis 2 superseded an earlier conclusion (batch 61) that gaps were opaque compiler artifacts to be treated as blocked.
It also explains why adding *initialised* copies to `TEggGenerator::control` (frame 0x38, 55.1%) looked like evidence against the locals theory.

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
