# BossHanachan units

Covers `BossHanachanMain`, `Parts`, `Anm`, `Sub`, `Effect`, `Nerve`, `Save`.
Scores are as of the batch noted and may be stale; check `decomp-diff.py` before relying on them.
Shared rules that came from here (bool normalisation, vector temporaries, trig order) are in `../codegen-tells.md`.

## Layout

- Owner: eight body pointers 0x150, head 0x170, weak-body index 0x174, collision-adjusted position 0x17C, sphere-chain `TSphereLink*` 0x178, previous linear velocity 0x188, sand-pillar `MActor*` 0x19C with position 0x1A0-0x1A8, death-sound position 0x1AC, param pointers 0x1BC/0x1C0.
- `moveObject` stores 0x188's components from 0x94 individually; `execSlip` copies with integer loads. Keep component stores.
- Body: scalar 0x120 (signed terrain/sand roll target), previous position 0x124, older 0x130, previous roll 0x13C, older roll 0x140, centrifugal force 0x144, previous force 0x148, `MtxPtr mLegMtx[2]` 0x14C/0x150.
- Parts 0x110: `TNpcInbetween*`.
- Manager: size 0x64, common params 0x54, three change-param pointers 0x58.
- Params (`BossHanachanSave.cpp`, linked): common 29 members size 0x24C, changeable 34 members size 0x2B0; names from rodata base 0x803820A0.
- `TSpherePoint` stride 0x2C: previous position 0, current 0xC, velocity 0x18, segment length 0x24, roll 0x28. `TSphereLink` 0x1C: u16 count 0, points 4, velocity multiplier 8, vertical increment 0xC, radius 0x10, roll-to-displacement 0x14, head yaw 0x18. Field names are inferred.
- Anim tables: head/body 18 entries each; head BCK/BTP/BTK at 0x190/0x1D8/0x220 from rodata base 0x80381E08; body BCK at 0x80381F50; weak body selects indices 11/14 for kinds 2/3.

## Behaviour notes

- `CalcMtxPtrFromJointName`: `u16 index = names->getIndex(name)` (UNUSED 76 bytes).
- Animation: `MActor::getBckAnm()` is the current animation (0x24); `getBckOldMotionBlendAnmPtr()` is the previous blend animation via an out-of-line getter. `copyFrameFromOldAnmToNewAnm_` uses `getBckAnm()`. `TNpcInbetween::execMotionBlend` and `TYoshi::thinkAnimation` correctly use the previous-animation getter.
- `isCurBckAlreadyEnd`: `MActor::isCurAnmAlreadyEnd` is similar but lacks the outer null-MActor guard; don't substitute.
- Hit handlers share `isThing()`, the overturned test (exact compares to -179/179 with ternary bool), and trample reaction via `isReactToTrampleOrHipDrop_()`/`restartBck_()`. Body hip drops affect kinds 2, 3, 5, 13, 16, 17; the head has its own response.
- `getLatestNerve()` is called out of line in both hit handlers and damage fog; ours inlines. Unresolved; don't change the shared getter globally.
- Dispatcher `considerSetAnm_` (1,844 bytes): states 0/1 share completion cases 5, 6, 13, 16, 17; state 1 also checks `isMarioOn_` and blending. State 2 checks completion before its switch (9/12 do nothing) with chains 7->8->9 and 10->11->12. State 3 distinguishes a positive countdown reaching zero from one already <= 0.
  Blending: initialise true and set false when both helpers return false (99.68%); `bool b = a() || b()` leaves the second out of line.
- `isMarioOn_` returns `bool` via `SMS_IsMarioTouchGround4cm()`, `SMS_GetMarioGroundPlane()`, `getActor()` (UNUSED 100 bytes).
- Timers: damage/death `delay * CLBAbs(mWeakBodyIndex - i)`, head uses `+ 1`; snort from head then bodies with multipliers 1-8; get-up reversed (see loops in `codegen-tells.md`).
- `setTumbleBckRate_`: one float for the signed rotation difference, `frames = (1.0f / unk198) * (frames >= 0.0f ? frames : -frames)`, nested `2 * (40 * SMSGetAnmFrameRate())`. Helper is 180 vs map 176.
- `setHeadAndBodyAnm`: BCK uses an integer remainder, BTP/BTK a second conversion to a shared float. Keep both conversions.
- Sphere chain: vertical increment added to each position then velocity; first point pinned to head; followers projected to segment length; final pass damps velocity; near-zero displacement becomes up vector, else `PSVECNormalize`.
- `BHSCalcCentrifugalForce`: squared XZ, 0.001 threshold, `TVec3::sub(first, second)`; angle difference int for sign test, narrowed to s16 before magnitude as int.
- Water receiver uses `gpMarDirector->isThing()` (modes 1, 2, 4), accepts only the spray message.
- Effects: foot-step frames 14/34, snort 21/36/55, special snort 134; sand pillar particle 0x7E and BCK 0x25/BTK 2/BRK 2; dust checks sphere point `i`, not `i + 1`.
- Nerves: define GraphWander, Tumble, Down, GetUp, Damage, Snort, Dead with `DEFINE_NERVE`; `spine->pushAfterCurrent(&Next::theNerve())`, return TRUE even if the push fails. Tumble uses `startAppearBalloon(7, true)` via `SMSGetMarDirector()->getConsole()`. Snort checks timer 200 and flag 0x20000, BGM 0x80010029, tempo (0,1) at health 2, (1,1) at health 1.
- `perform` (6,108 bytes): head anchor converts yaw twice with a param load between (unlike `init`, which reuses one converted yaw). Terrain probe rotates a 200-unit side vector, probes 500 above, named `f64 absoluteLeft/Right = fabs(...)`. Sand response within `CLBSquared(50.0f)`, zero roll within 15 degrees of front/back, else 70 x `SMS_GetSandRiseUpRatio`. First body-yaw wrap uses `MsAngleWrap(MsGetRotFromZaxisY(axis))`; `TSphereLink::setDegreeZAndRevisionPosXZ` correctly inlines `MsWrap`.
- `throwMario_`: signed short-angle difference, absolute value as int, scaled 1/32768, subtracted from one; separate normalised-difference and `1 - ratio` assignments.
- `CalcRevisionPosByRotateZ` uses `fabsf`; `execHeadCalcAnim_`/`execBodyCalcAnim_` then match map sizes 280/368.
- Foot init: joint indices in `int[2]` narrowed to u16; fetch foot params directly at `initHitActor`; param pointer before model/name-table locals; `mMActor->getModel()`.

## Open and rejected

- `perform`: `getBodyMaxRotateZ` inlines twice where the original calls; history loop unrolls eight iterations instead of four-twice; horizontal distance lacks the fused op. Direct `mBodies[i]`, reference local, unsigned index and do/while all failed.
- `moveHead` (~85.7%): does **not** emit weak `isIllegalData` (inlined). `isLegal`/`checkFlag` chaining, explicit returns, equality-to-false and split velocity locals all failed.
- `isCanWalk` inline sites call `TVec3::set<float>` in the original; ours inlines it. Assignment/copy-init adds an integer copy (216 bytes); `.set(temp)` fuses; const-ref binding doesn't help. Don't disable setter inlining globally (the 100-unit check needs it inline).
- `BHSCalcRevisionDistXZByRotateZ`: sine/cosine inside the loop, scaled vector, split offsets, second rotated vector all regressed.
- Parts: `getMActor()` at each access or held in a local, split completion OR, `(isMotionBlending() || isForcedBlendRatio()) == false` all regressed.
- `setHeadAndBodyAnm`: removing the shared texture float (74.1%), moving its init before BCK (~91.9%) rejected.
- Effects: one shared `int i` across loops, explicit `(s16)` on the water counter did nothing.
- Tumble nerve frame 0x38 vs 0x40: named console/director locals don't help.
- Hit predicate UNUSED size 208 vs 196; `isCanWalk` sizes fixed at 192.
