# Player and FLUDD units

## Sweep of 2026-09-17

Closed: `checkCollision` (4,972 B; `u32 colType = getCollision(i)->getActorType()` plus `getCollision`/`getColNum`/`getStatus`), `waitMain` (calls `checkCollision`, not `checkReturn`; `getHeldObject()`), `landing` (missing `mStatusArg < 3` guard; `startVoice`), `checkSwimJump`/`fireDowning` (`getIntendedMag()`), `sleeping` (`startVoice` twice).
The FLUDD spray predicate is the out-of-line `TWaterGun::isEmitting()`, not the inline `canSpray()` (which expands to ~30 instructions): `rocketCheck` 50 -> 99.9, `doJumping` 81 -> 99.9, `rocketing` 89 -> 99.9; `MarioDraw.cpp:424,1020` still use `canSpray()` and `MarioMove.cpp:2460` calls `checkCollision()` where its siblings call `checkReturn()` (both unchecked).
Other fixes: `mFootprintPos` is 0x1A8; `barWait`'s lost-holder branch goes to `LAND_SAFE_DOWN`, input bit `0x8000`, height `<= 100.0f`; `startJumpWall` reads the wall normal's `.z`; `TUtil<f32>::one()` reproduces a `fmuls` by 1.0f a written `* 1.0f` folds away; declare the x delta second for retail's f5/f6 pairing in `sqrtf(dx*dx + dz*dz)`.
Open: where `getOnWirePosAngle` inlines (`wireWait`, `wireSWait`, `wireHanging`, `wireRolling`, `hanging`) retail calls `TVec3::operator*=` and the copy constructor and inlines `scale`; we do the opposite at depth 4 (~78-140 instructions each; a `JGVec3.hpp` change plus sweep). `MarioJump.cpp` UNUSED stubs: `askStrongGroundTouch` (0x168), `checkWallJumping` (0x60), `doSlipJumping` (0x180), `doSpinJumping` (0x148), `setJumpingAttackArea` (0x7c); `isSwimWaiting` (0x1c, declared `void`, probably a `bool` predicate). `TMarioCap::TMarioCap` 99.0 suggests `unk30` is indexed.

Scores are as of the batch noted and may be stale.

## `TWaterGun::isEmitting` (batch 50)

Global 252 bytes, between `getCurrentNozzle` and `perform` in `WaterGun.cpp`.
MarioRun, MarioWait and MarioSound call it out of line; the header body wrongly expanded at all three.
Body uses `isDemoMode3`/`isDemoMode4` then `isTalkModeNow`; `isDemoModeNow` adds a boolean merge.
`soundMovement`'s guard must enclose its collision-sound switch.
The fabricated `canSpray` has different callers; don't replace it automatically.
Open frames: `isEmitting` 0x30 vs 0x80, `squating` 0x70 vs 0xA0, `soundMovement` 0x100 vs 0x2D0. WaterGun lacks 24 UNUSED definitions; MarioRun lacks braking.

## `TMario::receiveMessage` (batch 48)

Final sender switch: add 0x08000003/04 to the take/damage group, remove 0x08000010-12, map 0x08000002 with 0x80000001, drop 0x80000002 and door 0x4000002A.
0x0800002A/2C fall through to hinokuri handling; 0x08000014/15 break before door handling.
Do not propagate these into the earlier, separately gated enemy switch.
Mushroom guard rejects only `unk13A == 0 && unk13C < 120`, using `TMushroom1up` fields.
Fruit kick uses `mMotorWall` (0x27F8), not `mMotorTrample`.
Open: frame 0x180 vs 0x220 and wire/conversion slots.

## `TMario::checkCollision` and `hitNormal` (batch 47)

Yoshi guard tests `!isHolding()`.
Both `hitHipDrop` inline sites accept status 2 and 3 (UNUSED helper 112 vs map 116).
Actor IDs 0x08000016-0x08000021 do nothing; 0x08000022/23 call `keepDistance`.
In the 0x10000008 case name the `TSmallEnemy*` receiver before `doKeepDistance`.
`hitNormal` copies Mario's position into the static water-hit actor then adds 80 to that actor's Y; don't reread Mario's Y.
Open: frame 0x1E0 vs 0x238; `hitNormal` 0x18 vs 0x30.

## `TMario::TMario` (batch 46, exact)

See `../tu-reconstruction.md` for the `TEParams` defaults, `DmgHamukuri.prm` and `StageUtil.hpp` table findings.
Auto-demo read rotation and field 0x54E both initialise to 0x400 (shared constant explains r23 reuse); 0x55C is 204.0f.
UNUSED `stageSetting` is an empty four-byte body.

## Jump, run and special (batches 40, 42)

- `mModelFaceAngle = mStatusTimer * -4096` fixes `MarioRun::rotating` and UNUSED `rotateJumping` (348). Keep the positive branch's `extsh`.
- `jumpMain` open: frame 0x60 vs 0x88, `pullJumping` vector slot 0x34 vs 0x60. `checkJumpingThrowStart` restored (92 bytes) without changing the frame.
- `specMain`: `wireWaitToHang`/`wireSWaitToHang` `noHold` locals are `bool` (offsets 0x948/0x9e8). Don't mass-convert other `noHold`s. Open: endpoint-swap slots 0x7c/0x80/0x84 vs 0x68/0x6c/0x70; hoisting, splitting, const or plain `Vec` all fail.

## `TMarDirector::loadParticleMario` (batch 41, exact)

Declared `static`; see `../frame-gaps.md`.
