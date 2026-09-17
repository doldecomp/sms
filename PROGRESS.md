# Progress

Target: North American `GMSE01` rev 0, branch `local/decomp-progress`, forked from upstream `ab00c3c9`.
The decompilation is **not complete**.
Keep this file to the current state only: overwrite the numbers and the in-progress list each batch; do not append a history entry.
Per-batch history through batch 69 is archived in [docs/progress/history.md](docs/progress/history.md); later history is `git log`.

## Current numbers

Measured from `build/GMSE01/report.json` on 2026-09-16 after the six-batch goal run of 2026-09-16/17.

| | Matched code | Source-linked code | Linked units |
| --- | ---: | ---: | ---: |
| Game | 29.75% | 3.60% | 95 / 385 |
| JSystem | 87.11% | 61.61% | 166 / 198 |
| SDK | 95.07% | 92.22% | 142 / 149 |
| All | 42.34% | 17.91% | 403 / 732 |

9,072 of 12,904 functions match. DOL byte-identical. No gameplay test has been performed.

## What recent batches have done

Structural seams are spent: the 308 matching library objects are linked, nerve accessors and class hierarchies recovered, and `egggen`/`CameraTalk` linked by recovering unused locals.
Since then, work has been **whole-TU enemy reconstruction from the map**: scaffold the classes, write every function including nerves and UNUSED helpers, put the file in map order.

| Unit | Unit match | Exact functions |
| --- | ---: | ---: |
| `MoveBG/MapObjWave` | 99.99% | 11 / 12 |
| `Enemy/effectEnemy` | 99.99% | 17 / 18 |
| `Enemy/yunbo` | 99.22% | 27 / 35 |
| `Enemy/amiNoko` | 98.70% | 27 / 37 |
| `Enemy/feetinv` | 95.18% | 24 / 25 |
| `Enemy/seal` | 98.65% | 12 / 18 |
| `Enemy/hanasambo` | 97.51% | 63 / 96 |
| `Enemy/cannon` | 97.26% | 33 / 55 |
| `Enemy/popo` | 96.95% | 30 / 54 |
| `Enemy/tobiPuku` | 94.71% | 90 / 120 |
| `Enemy/igaiga` | 94.61% | 52 / 95 |
| `Enemy/chuuhana` | 93.59% | 33 / 57 |
| `MoveBG/MapObjBall` | 87.60% | 32 / 72 |
| `Enemy/koopajr` | 85.58% | 45 / 81 |
| `Enemy/BathtubBinder` | 81.18% | 5 / 6 |
| `Enemy/hauntLeg` | 52.38% | 20 / 28 |

Most remaining differences in these units are frame gaps and per-call-site inlining; see `docs/catalog/frame-gaps.md` and `docs/catalog/codegen-tells.md`.

## Still open

- **The branch exists only on this disk.** No fork is configured. This is the outstanding risk and needs a remote from the user.
- `Camera/CameraInbetween` matches but will not link: `docs/catalog/linking.md`.
- Units one function from linking (`MSoundBGM`, `MarioAccess`): `docs/catalog/frame-gaps.md`.
- About 200 nerve bodies, and most enemy and MoveBG methods, are unwritten.

## Local artifacts

- Source build manifest: `config/GMSE01/objects.json`. Metadata provenance: `config/GMSE01/README.md`.
- Reports: `build/GMSE01/baseline.json`, `build/GMSE01/report.json`.
- Frame-gap worklist: `docs/progress/GMSE01-frame-gaps.md`. Old per-batch JSON snapshots and closure audits: `docs/progress/`.
- Build artifacts and game data are local only and excluded from Git.
