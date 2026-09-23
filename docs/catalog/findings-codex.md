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
