# Codex findings

Record binary-backed findings from Codex unit work here before promoting reusable rules into the topic catalog.

## 2026-09-23 two-model trial

- `MoveBG/MapObjDolpic`: `TMonumentShine::hitByWater` remains 91.9% (396 B). Frame is exact; the remaining reload and register differences are already documented in its TODO. No edit.
- `NPC/NpcBase`: `TBaseNPC::perform` remains 94.7% (2024 B). Two sight-vector spellings scored 94.5% and 93.9%, below the baseline. The 0xa0 frame deficit and call-order gap are documented structural residue. No edit.
- `Camera/lensflare`: `TLensFlare::perform` remains 94.8% (1512 B); target frame 0x2b8 versus source 0x208. The lerp and vector-copy residue is already documented in frame-gaps.md. No edit.
- `GC2D/SelectShine2`: `TSelectShineManager::perform` remains 94.0% (1180 B); target frame 0x220 versus source 0x1b8. The main `getAngle` residue is documented in the source. The apparent `cCenter` relocation mismatch is an object-symbol alias at the same offset. No edit.
- Model comparison: gpt-6-sol at low effort and gpt-6-luna at max effort each audited two units, each found no safe semantic change. This trial does not separate model ability; target selection reached already researched residues.
