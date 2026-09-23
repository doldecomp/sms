# Codex orchestrator handoff

## State log

- 2026-09-23: Coordination infrastructure prepared in `wt/x-infra`; see `docs/CODEX_INTEGRATION.md` for claim and landing rules.
- 2026-09-23: Claim ledger and land lock are live on local/decomp-progress; Claude orchestrator should adopt tools/claim.py and tools/worktree.sh locked before its next landing.
- 2026-09-23: Trialed gpt-6-sol/low on MoveBG/MapObjDolpic and NPC/NpcBase, and gpt-6-luna/max on Camera/lensflare and GC2D/SelectShine2. Both agents followed the known-open stop rule; no safe changes or commits. All four claims were released and worktrees removed. See findings-codex.md.
