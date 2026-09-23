# Codex orchestrator handoff

## State log

- 2026-09-23: Coordination infrastructure prepared in `wt/x-infra`; see `docs/CODEX_INTEGRATION.md` for claim and landing rules.
- 2026-09-23: Claim ledger and land lock are live on local/decomp-progress; Claude orchestrator should adopt tools/claim.py and tools/worktree.sh locked before its next landing.
- 2026-09-23: Trialed gpt-6-sol/low on MoveBG/MapObjDolpic and NPC/NpcBase, and gpt-6-luna/max on Camera/lensflare and GC2D/SelectShine2. Both agents followed the known-open stop rule; no safe changes or commits. All four claims were released and worktrees removed. See findings-codex.md.
- 2026-09-23: Three-agent repeat: gpt-6-sol/low fixed `Enemy/Kumokun::checkOnMovingWall` (93.36→95.55 fuzzy; operand-order bug, landed as 017508fa). gpt-6-sol/medium audited ten `Map/BathWaterManager` functions and gpt-6-luna/max audited eleven `Animal/BeeHive` functions; neither found another justified edit. All Codex claims released and worktrees removed. See findings-codex.md.
- 2026-09-23: Closure follow-up found no safe edits in Enemy/Amenbo, MarioUtil/MtxUtil, Enemy/rocket, Enemy/mameGesso, Player/MarioParticle, or Enemy/BathtubKiller. The BathtubKiller `setDeadBathtubKillerAnm` expansion is already present at both call sites, refuting the old missing-inline lead. All variants restored, claims released, and worktrees removed. Prefer a shared-cause research batch over another broad audit of documented residues.
