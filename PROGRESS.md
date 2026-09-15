# Local decompilation progress

## Target and status

Target: North American English, `GMSE01` revision 0, using the local ISO.
The full decompilation is **not complete**.
The local branch is `local/decomp-progress`.
The upstream starting commit is `ab00c3c9a466152f6e6bc5b9c28aca959d1a8454`.

## Latest checkpoint: batch 2

The build now links 70 verified game objects from source.
Exactly matched code is 1,365,748 of 3,603,748 bytes (37.897987%), with 8,109 of 12,904 functions matching.
The two batches together added 640 exactly matching bytes and four matching functions to the initial regional baseline.
The complete decompilation remains unfinished.

### Changes and verification

- `TNpcThrow::throwMario` now matches all 324 bytes after removing an unnecessary intermediate yaw variable.
  Its source object was added to the verified link manifest.
- `TTalkCursor::associateNPC` now uses the existing matrix translation operation, reproducing the original load/store sequence.
  Its comparison improved from 93.62857% to 99.6%; the stack layout still differs, so the unit remains nonmatching.
- `THitActor::calcEntryRadius` and `initHitActor` now have inferred `void` return types, removing the invented height-squared return value.
  No callers consume their result.
  The radius routine improved from 97.6129% to 98.258064%, and the affected `TEMario::init` improved from 93.97248% to 97.76147%.
  These routines remain nonmatching where indicated by the comparison report.

A fresh baseline was captured at commit `1ccb911a` before these source changes.
All changed source units passed symbol presence, order, and linkage checks.
A full rebuild of the affected callers and `ninja changes_all` found zero function regressions.

The entire `build/GMSE01` output directory was then moved aside, and Ninja successfully rebuilt from the stored source/configuration and extracted input.
The clean rebuild passed the expected SHA-1 and a full byte comparison with the input executable.
A deliberately modified copy of the input was rejected by the configured hash check.
No original game input was changed during this test.

See [batch 2 measurements](docs/progress/GMSE01-batch2.json).
The clean-build log is `build/GMSE01-clean-build.log`; the current regression log is `build/GMSE01-changes-batch2.log`.
The prior build, intermediate experiments, and reports remain in `build/GMSE01-before-clean-v1`.

### Deferred source-link investigations

Further testing confirmed the three deferred objects still break the complete binary match.
`CameraInbetween` changes constant layout and reduces `.sdata2` by eight meaningful bytes; shared weak-symbol selection also needs investigation.
`MessageUtil` emits stream getters before `SMSMakeTextBuffer`, shifting its address.
`PollutionEvent` emits a destructor before `loadAfter`, shifting the latter's address.
The diagnostic logs are preserved in `build/GMSE01-before-clean-v1/diagnose-*.log`.
These are pending layout/source-emission tasks, not verified source-link improvements.

## Verified checkpoint: regional setup and batch 1

The new regional configuration rebuilds `mario.dol` byte-for-byte identically to the executable extracted from the supplied disc.
Both the expected SHA-1 and a direct `cmp` passed.
That checkpoint linked 69 verified game objects from source and retained extracted objects for the remaining units.

| Measure | Initial baseline | Batch 1 |
| --- | ---: | ---: |
| Matched code bytes | 1,365,108 | 1,365,424 |
| Matched functions | 8,105 | 8,108 |
| Overall matched code | 37.88023% | 37.888996% |
| Verified game objects linked from source | 0 | 69 |

Exact machine-readable measures are in [the initial snapshot](docs/progress/GMSE01-initial.json) and [the batch 1 snapshot](docs/progress/GMSE01-batch1.json).
Category totals changed when previously unclassified US-only metadata was assigned to the correct libraries; this is not a source regression.

### Source improvements

- `TNpcBalloon::updateBalloon`: use the director accessor and a named console pointer to reproduce the original inline call and stack layout; 108 bytes now match.
- `TProcessMeter::TProcessMeter`: preserve the US-specific field value `0x1C2`, while retaining the existing value for other versions; 64 bytes now match.
- `TTimeRec::flip`: use a reference to the current timing entry so the loop reproduces the original register allocation; 144 bytes now match.

The batch gained 316 exactly matching code bytes and three matching functions.
All three changed units also passed incremental source-link tests against the complete executable.

### Checks performed

- Repository-pinned toolchain installed and compiler execution verified.
- Disc region, revision, filesystem, and extracted executable inspected.
- Regional map layout checked against the executable.
- `ninja baseline` captured before source changes.
- Per-function `tools/decomp-diff.py` comparisons reached 100% for the three changed functions.
- Symbol presence, order, and linkage checks passed for all three changed source units.
- `ninja changes_all` and a function-by-function baseline comparison found zero function regressions.
- Full `ninja` build passed the expected executable SHA-1.
- `cmp orig/GMSE01/sys/main.dol build/GMSE01/mario.dol` passed.
- No emulator gameplay test has been performed.

### Known limitations

The symbol validator reported existing incomplete UNUSED functions: `TProcessMeter::draw`, and `TTimeRec::suppleGXTime`, `drawSyncCallbackSt`, and `end`.
Those functions are absent from the retail executable, and their current stubs do not have the sizes recorded in the linker map.
The warnings remain unresolved and must not be treated as reconstructed code.

Three otherwise matching game objects changed the executable when linked from source and remain deferred: `Camera/CameraInbetween.cpp`, `GC2D/MessageUtil.cpp`, and `Map/PollutionEvent.cpp`.
Their per-object code/data comparison alone was insufficient to establish a full link match.
`Enemy/enemyinterp.cpp` is now recognized in configuration but still has no source file.
The repository's restrictions on autonomous runtime/middleware reconstruction remain applicable.
Japanese and PAL binary regression checks are unavailable because their inputs have not been supplied.

## Local artifacts and tools

- Source build manifest: `config/GMSE01/objects.json`.
- Metadata provenance and reproducible commands: [regional README](config/GMSE01/README.md).
- Work plan: [DECOMPILATION_PLAN.md](DECOMPILATION_PLAN.md).
- Initial game-code inventory: `docs/progress/GMSE01-backlog.json`.
- Full comparison reports: `build/GMSE01/baseline.json` and `report.json`.
- Build and regression logs: `build/GMSE01-build.log` and `build/GMSE01-changes-batch1.log`.
- Incremental promotion results and logs: `build/GMSE01/promotion-results.json` and `promotion-logs/`.
- `m2c`: `/home/netflix/m2c`, commit `e07f7e1c1d1d52f33ab3f7fedbe74b634404b943`, installed in `build/venv`.

Build artifacts and original game data are kept locally and excluded from Git.
Source, configuration, progress snapshots, and notes are versioned locally.

## Next work

Continue with small game-code mismatches, capture a fresh baseline for the next batch, and investigate the three deferred source-link candidates.
The largest unfinished game units in the initial inventory include the HUD, save/load screens, boss logic, and event scripting.
