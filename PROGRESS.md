# Progress

Target: North American `GMSE01` rev 0, branch `local/decomp-progress`, forked from upstream `ab00c3c9`.
The decompilation is **not complete**.
Keep this file to the current state only: overwrite the numbers and the in-progress list each batch; do not append a history entry.
Per-batch history through batch 69 is archived in [docs/progress/history.md](docs/progress/history.md); later history is `git log`.

## Current numbers

Measured from `build/GMSE01/report.json` on 2026-09-16 on 2026-09-17 during the second parallel run.

| | Matched code | Source-linked code | Linked units |
| --- | ---: | ---: | ---: |
| Game | 32.85% | 3.60% | 95 / 385 |
| JSystem | 87.11% | 61.61% | 166 / 198 |
| SDK | 95.07% | 92.22% | 142 / 149 |
| All | 44.79% | 17.91% | 403 / 732 |

9,072 of 12,904 functions match. DOL byte-identical. No gameplay test has been performed.

## What recent batches have done

Structural seams are spent: the 308 matching library objects are linked, nerve accessors and class hierarchies recovered, and `egggen`/`CameraTalk` linked by recovering unused locals.
Since then, work has been **whole-TU enemy reconstruction from the map**: scaffold the classes, write every function including nerves and UNUSED helpers, put the file in map order.

| Unit | Unit match | Exact functions |
| --- | ---: | ---: |
| `MoveBG/MapObjWave` | 99.99% | 11 / 12 |
| `Enemy/effectEnemy` | 99.99% | 17 / 18 |
| `Enemy/tinkoopa` | 97.38% | 29 / 54 |
| `Enemy/elecNokonoko` | 97.02% | 44 / 69 |
| `Enemy/killer` | 97.48% | 39 / 54 |
| `Enemy/limitkoopa` | 87.00% | 38 / 58 |
| `Enemy/Kazekun` | 96.37% | 35 / 43 |
| `Animal/Bird` | 96.73% | 37 / 51 |
| `Animal/BeeHive` | 95.97% | 33 / 49 |
| `Enemy/limitkoopajr` | 94.92% | 19 / 27 |
| `Enemy/TabePuku` | 93.11% | 32 / 48 |
| `Enemy/Kukku` | 87.54% | 27 / 37 |
| `Enemy/bosswanwan` | 95.44% | 48 / 78 |
| `GC2D/SelectShine2` | 94.11% | 8 / 13 |
| `GC2D/Guide` | 90.34% | 6 / 17 |
| `Enemy/BathtubPeach` | 94.13% | 19 / 21 |
| `Enemy/fruitsboat` | 98.40% | 18 / 24 |
| `Enemy/rocket` | 99.16% | 26 / 33 |
| `Enemy/bombhei` | 99.10% | 38 / 48 |
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
| `MoveBG/MapObjMamma` | 90.52% | 66 / 103 |
| `MoveBG/MapObjMonte` | 94.35% | 28 / 54 |
| `MoveBG/MapObjRicco` | 99.42% | 23 / 32 |
| `MoveBG/MapObjPinna` | 99.28% | 50 / 68 |
| `MoveBG/MapObjBianco` | 97.84% | 49 / 74 |
| `MoveBG/MapObjMare` | 96.46% | 38 / 67 |
| `MoveBG/MapObjBall` | 87.60% | 32 / 72 |
| `Enemy/koopajr` | 85.58% | 45 / 81 |
| `Enemy/wireTrap` | 85.92% | 24 / 35 |
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
