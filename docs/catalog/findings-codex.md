# Codex findings

Record binary-backed findings from Codex unit work here before promoting reusable rules into the topic catalog.

## 2026-09-23 two-model trial

- `MoveBG/MapObjDolpic`: `TMonumentShine::hitByWater` remains 91.9% (396 B). Frame is exact; the remaining reload and register differences are already documented in its TODO. No edit.
- `NPC/NpcBase`: `TBaseNPC::perform` remains 94.7% (2024 B). Two sight-vector spellings scored 94.5% and 93.9%, below the baseline. The 0xa0 frame deficit and call-order gap are documented structural residue. No edit.
- `Camera/lensflare`: `TLensFlare::perform` remains 94.8% (1512 B); target frame 0x2b8 versus source 0x208. The lerp and vector-copy residue is already documented in frame-gaps.md. No edit.
- `GC2D/SelectShine2`: `TSelectShineManager::perform` remains 94.0% (1180 B); target frame 0x220 versus source 0x1b8. The main `getAngle` residue is documented in the source. The apparent `cCenter` relocation mismatch is an object-symbol alias at the same offset. No edit.
- Model comparison: gpt-6-sol at low effort and gpt-6-luna at max effort each audited two units, each found no safe semantic change. This trial does not separate model ability; target selection reached already researched residues.

## 2026-09-23 three-agent repeat

- `Enemy/Kumokun`: Retail `checkOnMovingWall` saves `param_3` from r6 in r30, loads the plane normal through offset 0x198, then uses `fmadds` at 0x472c to compute `normal.x * 100.0f + param_3.x` (and likewise for y/z). Source had the scale and addend operands reversed. Reusing the normal vector with `normal.scaleAdd(100.0f, normal, param_3)` improved the function from 93.36% to 95.55%, with zero `changes_all` regressions, unchanged DOL SHA-1, and symbol order PASS with nine pre-existing UNUSED-size warnings. Landed as 017508fa.
- `Map/BathWaterManager`: gpt-6-sol/medium audited all ten open functions. Remaining gaps were documented frame, register, inlining, and instruction scheduling differences; no justified variant or edit.
- `Animal/BeeHive`: gpt-6-luna/max audited all eleven sub-99% functions plus mapped data. Nerve branch order, water-message vector direction, event ID order, member use and data values matched the retail evidence. Remaining differences were documented `TPathNode`/quaternion inline behavior, frame/register allocation, or inert structural probes; no justified variant or edit.
- This repeat found one semantic fix with low-effort Sol and none with medium-effort Sol or max-effort Luna. Units differed, so this measures these assignments rather than model capability in isolation.

## 2026-09-23 closure follow-up

- `Enemy/BathtubKiller`: The map lists `setDeadBathtubKillerAnm` UNUSED at 0xb8 and `breakBathtubKiller`/`explodeBathtubKiller` UNUSED at 0x14c each. The current object emits the set-dead body within both methods; the nerve callers are 99.9% and 100%. Spelling the set-dead statements directly made the methods smaller (0xcc/0xf0 versus 0xd4/0xf8) without improving the nerves. Retail has no addresses for these UNUSED bodies, so the extra map bytes remain unexplained. The old missing-expansion lead is refuted.
- `Enemy/mameGesso`: Three new reference-lifetime variants for `TNerveMameGessoThrown::execute` worsened its 99.8% baseline, even when a variant reached the retail 0x70 frame. Its 0x248 object size matches the map; the remaining slot and FPR allocation is unresolved.
- `Player/MarioParticle`: Six TU-local copy/chain/reference variants did not reproduce the six vector slots in `TWarpInCallBack::execute`; the best preserved the 80.2% baseline. The shared `JGVec3.hpp` reference-return form previously raised the function to 92.47% but regresses the tree and loses weak out-of-line bodies, so it remains parked.
- `Enemy/Amenbo`, `MarioUtil/MtxUtil`, and `Enemy/rocket`: Unit audits and scored variants found only documented stack, register, quaternion, or inline residues. No source changes were retained.

## 2026-09-23 operand scan follow-up

- `Enemy/bgtentacle`: Two switch cases had `unk40` and `unk44` feeding the wrong sine terms. Swapping the source inputs reduced mismatch markers from 160 to 158. Landed as 33707b4e.
- `Enemy/bosswanwan`: Retail clears `mPullVelocity` in z/y/x order. Explicit field clears reduced `TBossWanwan::control` mismatches from 33 to 31. The shared inverted `li 0/1` residue remains. Landed as e74b5986.
- `Camera/cameragc`: `calcPosAndAt_` read `mSLAimAngleYChaseMin` at 0x194 where retail reads `mSLHoldAngleXChase` at 0x16c. Correcting the member removed one mismatch. Landed as cfa6b3f5.
- `Enemy/Koopa`: The apparent 0x68/0x74 access in `TKoopaFlame::attack_` belongs to a documented inline-depth mismatch. Related variants gave no defensible improvement.
- `MoveBG/MapObjMare`: The 0x174/0x170 store difference in `TMuddyBoat::bind` is instruction alignment within the third `touchWall` expansion; both builds write `mEffectPos.x/y/z`.
- `Enemy/limitkoopa`: `setUpHitActors` reads the same fields in both builds; the 0xc/0x2c difference is load order. Its five open functions all have deep-search TODOs.
- `GC2D/SelectShine2`: The swapped f30/f31 constants in `perform` accompany a different but equivalent up-vector construction and angle arithmetic. No edit.
- `MoveBG/MapObjInit`: `TMapObjBase::initMActor` is the unit's only sub-95% function; its unresolved `param_2` use and inert probes are already documented. No edit.
- `MSound/MSound`: Three constructor declaration/size variants were inert or worse than 86.9%; the remaining functions have known-open TODOs. No edit.
- `Enemy/hamukuri`: All nine open functions are 99.67–99.98% fuzzy. Three previously unexplored functions differ only by a known `operator-` temporary stack-slot class; the other six have deep-search TODOs. No edit.

## 2026-09-23 targeted operand follow-up

- `Enemy/fireWanwan`: `TFireWanwan::bind` loaded `mAirFric` at 0x370 in the grounded flying branch; retail loads `mFloorFric` at 0x384. Corrected member, reducing one mismatch. Landed as f2e59f60.
- `Player/Tongue`: `TYoshiTongue::movement` called virtual `ensureTakeSituation` (slot 0xa8) where retail calls `checkTaking` (slot 0xbc). Corrected call, reducing one mismatch. Landed as 51fbcfee.
- `System/MarDirectorSetupObjects`: The final heap virtual call in `setupObjects` was `freeTail` (slot 0x18); retail calls `getTotalFreeSize` (slot 0x28). Corrected call. Landed as 2d090efc.
- `MarioUtil/PacketUtil`: The apparent 0xe1/0xe0 discrepancy in `ShapePacketCallBackFunc` is diff alignment. Both binaries emit 0xe0 for the first TEV register and 0xe1 for the second.
- `Enemy/chuuhana`: The 0x204/0x20c loads in the Roll nerve read the same x/z members in both builds; inlined `rolling` changes their schedule.
- `Player/MarioMove`: `checkCurrentPlane` retains documented structural residue. `checkController` stores at `getCurrentNozzle()+0x714` in retail, beyond the declared turbo nozzle size; no justified source member change was found.
- `Enemy/pakkun`: `TPakkunSeed::rebirth` tests the same water-surface enum values in both builds; the 0x101/0x102 subtraction and 4/3 bounds are equivalent range groupings.
- `MoveBG/MapObjMamma`: `TLeanMirror::controlShake` computes the same `getBaseTRMtx` address; retail performs +0 then +0x20, while the compiler folds them into +0x20.
- `GC2D/ConsoleStr`: The 0x34/0x38 versus 0/4 loads in `perform` use equivalent pointer bases for `unk34`.
- `Enemy/bossgesso`: `perform` has the correct source `mMActor` and destination `unk178` for its matrix copy; three named-local variants did not change load order. A comment-only agent commit was discarded.
- `System/MarDirectorInitECT`: Both builds store image width and height in the correct projection fields; only load/conversion order differs.

## 2026-09-23 refreshed operand scan

- `Animal/AnimalBase`: Retail applies the frame-rate factor once to max march speed and twice to march acceleration in `execWalk`; the source had the factors reversed. Correcting both operands improved the function from 89.89% to 90.25%. Landed as 73d9cc33.
- `System/CardManager`: `cmdLoop`'s 0x1ffc versus 0 immediate reflects a documented `setCheckSum`/`CalcCheckSum` inline split, not a bad buffer length. No edit.
- `MarioUtil/ShadowUtil`: The old scan's load-offset clue was stale at current HEAD. `drawShadowVolume` is 99.9% with a documented stack slot shift; `calcVtx` retains a known frame deficit. No edit.
- `Enemy/hanasambo`: `SamboHeadRollCallback` reads the same matrix columns in both builds; the 0x10/0 load difference is scheduling. No edit.
- `Map/MapWire`: Naming x in `getPointPosDefault` improved `getPointPosOnWire` but regressed `move`; naming x/z changed the frame. Source restored.
- `Player/MarioDraw`: The paired waist pitch/roll loads follow right-to-left argument evaluation. Swapping source arguments would rotate the wrong axes; `calcBaseMtx` retains documented frame residue. No edit.
- `Camera/CameraBGCheck`: `calcInHouseNo_` has a documented deep search of its far-vector pointer and loop layout. No justified change from the isolated store offset.

## 2026-09-23 operand scan continuation

- `GC2D/CardLoad`: Retail loads 1.5f for both dimensions of the middle choice pane in `TCardLoad::waitForChoiceBM`; source used 0.5f. Correcting both factors removed one operand mismatch marker (`~253` to `~252`). Frame remains 0x468 retail versus 0x3f8 source, so the function and unit remain nonexact. Landed as 1d645406.
- `Player/MarioAutodemo`: Retail multiplies normalized x/z movement by 50.0f before 2.0f in `TMario::readBillboard`; source had the float operations reversed. Correcting the order improved 99.8% (`~15` markers) to 99.9% (`~13` markers) with the same 520-byte body. Its 0x70 versus 0x30 frame gap remains. Landed as bd64a8fa.
- `Enemy/conductor`: `TConductor::isBossDefeated` dispatches every map except 3 to the hinokuri arm in retail. The source only spells cases 2 and 3; its TODO already records failed switch/default variants. The unit was Claude-claimed when found, so Codex made no edit.
- `Enemy/bosswanwan`, `Enemy/feetinv`, `Enemy/beam`, `Enemy/Kumokun::initAttachPlane`, and `Player/WaterGun`: Follow-up operand candidates were explained by equivalent Boolean order, commutative matrix sums, constant-label aliases, instruction scheduling, or diff alignment. No edits.
- Across 93 sub-95% game functions and 358 functions at 95–99.5% fuzzy, a virtual-call slot scan found no further aligned slot error. Branch/call scans of 69 sub-95% functions found no other binary-backed semantic correction.
- A wider near-match pass found no differing paired virtual-slot or field offsets among 562 functions at 99.5–99.99% fuzzy. Call/literal review of unclaimed small functions and 127 mid-size call/compare rows found helper scheduling, register renumbering, or previously documented equivalences only. The smallest 4–8-function, data-exact game units also retain multiple documented frame/inline blockers, with no defensible closure target from that screen.
