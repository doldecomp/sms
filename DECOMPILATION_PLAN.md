# Decompilation plan

Rewritten 2026-09-16. The milestone-by-milestone plan through batch 39 is archived in `docs/progress/plan-history.md`.

## Where to look

| Question | File |
| --- | --- |
| How to build, diff, verify and commit | `CLAUDE.md` |
| What to work on next | `STRATEGY.md` |
| Current numbers and open risks | `PROGRESS.md` |
| A specific matching problem | `docs/catalog/README.md`, then one topic file |
| General MWCC codegen | `docs/AGENT_MATCHING_TIPS.md` |
| Scaffolding a new TU | `docs/PROGRAM_STRUCTURE_REVVING.md` |

## Input

- `Super Mario Sunshine (2002)(Nintendo)(US).iso` in the repository root, 1,459,978,240 bytes, SHA-256 `67cec1634e641227a4cd51e6a0b277730cb9a1adaa867530c9e66de45373e51d`. Excluded via `.git/info/exclude`.
- Main DOL SHA-1 `a6782903ef79d4196c8489ecb1b57decb5b3728f`, the byte-identical target for every batch.
- Linker map `orig/GMSE01/files/marioUS.MAP`.
- Setup and metadata provenance: `config/GMSE01/README.md`.

## Constraints

- All libraries (MSL, MetroTRK, THPPlayer, Dolphin SDK, JSystem) are in scope since 2026-09-15.
- Provenance is the one hard rule: only the binary, the disc's map and this repository. Never leaked Nintendo sources.
- No fakematches or committed stack padding; leave a TODO instead.

## Done means

- Every translation unit reconstructed, matching and linked from source, with the rebuilt DOL at the SHA-1 above.
- A matching DOL assembled partly from original objects is not completion.
- Gameplay smoke check (boot, menus, English text, loading, movement, camera, representative levels) once an emulator is available. Until then, record that no gameplay test was performed.
