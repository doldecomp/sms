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
