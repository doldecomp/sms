# GMSE01 matching strategy

Written 2026-09-15 after batch 38 (`562ed4e0`), from `build/GMSE01/report.json`.
This replaces the small-file completion queue as the primary work order.
Project rules in `CLAUDE.md` / `AGENTS.md` still apply in full.

## Why change

After the first run (38 batches), game code is **25.03% matched** (712,856 / 2,847,660 bytes), up from 24.11% at the branch start.
That is +26,396 bytes (+0.93 percentage points) for the whole run.
Recent file-completion batches often gain only 10–300 matched bytes each.

The Priority 1 small-file queue (96 files / 90,016 unmatched bytes) is worth only about 3 points of game code in total.
Much more code is in functions that are already almost exact:

| Target | Bytes | Game code if all matched | Whole binary if all matched |
| --- | ---: | ---: | ---: |
| First run, for reference | 26 KB | +0.93 pts | +0.73 pts |
| Top 10 list below | 61 KB | 27.16% | 40.29% |
| Entire small-file queue | 90 KB | 28.19% | 41.11% |
| All game functions at 99%+ similarity (1,137) | 618 KB | 46.72% | 55.75% |
| All game functions at 98%+ similarity (1,267) | 732 KB | 50.74% | 58.93% |
| Game functions at 0% (not started, 2,652) | 769 KB | — | — |

The 98%+ figures are a ceiling, not a forecast: some of those functions have difficult stack-frame gaps.
Even matching half of them would be worth 11–13 points, several times the entire small-file queue.

## Strategy

1. **Target near-matching functions, not whole files.**
   Work from a list of nonmatching game functions at 98%+ similarity, ordered by size.
   Refresh the list from `report.json` after every batch (see the script below).
   Finish a file and link it from source only when that is nearly free.
2. **Group functions by shared cause.**
   Many 99% functions likely share one root problem: a wrong type, a missing inline or a wrong layout in a shared header.
   The `TMario` functions are the first such group; NPC and boss functions are likely others.
   When a fix works, check whether the same mismatch pattern appears in neighboring functions of the same class or header.
   Batch 18's sound-layout fix, which made 15 functions exact at once, is the model.
3. **Time-box each function to about 20–30 minutes.**
   If it is still stuck, leave a `// TODO:` describing the remaining difference (frame gap, suspected missing inline, register swap) and move on.
   Never add `volatile`/padding arrays to force a frame size.
4. **Cut documentation overhead.**
   Each batch gets a few lines in `PROGRESS.md`, plus a `docs/MATCHING_CATALOG.md` entry only when the pattern is reusable.
   Stop writing per-batch closure-audit Markdown and JSON snapshot files.
5. **Keep verification, but reuse baselines.**
   A batch touching only one `.cpp` file can reuse the previous baseline.
   Always capture a fresh `ninja baseline` before any header change, and run `ninja changes_all` after every batch.
   Keep the SHA-1 / DOL byte checks and `tools/validate-symbol-order.py` for changed units.
6. **Defer from-scratch decompilation.**
   Untouched units (`bosstelesa`, `bosspakkun`, `Koopa`, `bosswanwan`, …) are real work but pay back more slowly.
   Start them once the 98%+ group is mostly exhausted.

## Top 10 targets

These 10 functions total 60,568 bytes, about 2.1 points of game code.
They are ordered by how easy the remaining mismatch looks, not by size.
In `tools/decomp-diff.py` output, LEFT is the original and RIGHT is ours; offsets are as printed by the tool.
Functions 1, 3, 7, 8 and 9 are all `TMario`: if a fix belongs in a shared Mario header, try it against all five and check other users with `ninja changes_all`.

View a diff with:

```
python tools/decomp-diff.py -u <unit> -d "<function>" --no-collapse
```

### 1. `TMario::specMain()`

Unit `mario/Player/MarioSpecial`, 3,532 bytes, 99.857%.
Two instruction opcodes differ; six stack operands also differ in the wire-endpoint swap.
At 0x948 and 0x9e8 the original tests `r29` as a byte (`clrlwi. r0,r29,24`); ours compares a full int (`cmpwi r29,0`).
The flag variable in `r29` is probably a `bool`/`u8` in the original.
The stack frame already matches, but the swap temporary uses 0x7c/0x80/0x84 instead of 0x68/0x6c/0x70.
Batch 40 corrects the two flag types; the stack operands remain unresolved.
A rounded similarity of 100.0% is not an exact match.

### 2. `TMarDirector::loadParticleMario()` — matched in batch 41

Unit `mario/System/MarDirectorLoadResource`, 4,708 bytes, 99.991%.
Every instruction matches; only the stack frame differs.
At the initial snapshot, the frame was 0x18 in the original and 0x20 in ours (8 bytes too big).
Resolved by declaring the global-only member static: removing the implicit instance parameter fixes the frame and all 4,708 bytes match.
Direct particle-helper expansion did not change the frame.

### 3. `TMario::jumpMain()`

Unit `mario/Player/MarioJump`, 4,224 bytes, 99.890%.
Batch 42 removes the extra `extsh` at 0xaf8 before `sth r0,0x9a(r31)` by spelling the angle as `mStatusTimer * -4096`.
The same verified fix completes MarioRun::rotating; jumpMain remains 99.98485% with stack differences.
The frame is 0x88 in the original and 0x60 in ours (0x28 too small), suggesting a missing inline or temporaries.

### 4. `TBaseNPC::npcWetting()`

Unit `mario/NPC/NpcAnm`, 3,176 bytes, 99.806%.
No structural differences, only the frame and register numbering.
The frame is 0x160 in the original and 0x178 in ours (0x18 too big).
Batch 43 leaves these differences as a source TODO and moves on to BossEel.
The neighboring sunflowerReviving helper now matches all 204 bytes after naming its animation local; its callers do not regress.

### 5. `TBossEel::init(TLiveManager*)`

Unit `mario/Enemy/bosseel`, 2,816 bytes, 99.447%.
At 0x4dd4, around `SDLModelData` and `operator new(0x80)`, ours copies `r22` into `r23` earlier than the original.
That points to a temporary or variable that should not exist, or an assignment in the wrong order.
The frame is 0x310 in the original and 0x300 in ours (0x10 too small).
Batch 44 also finds and corrects tooth-array indices, eye/heart loader flags and collision setUpTrans; similarity reaches 99.50852%.
A named skin-deformer local and shared resource pointer improve argument setup/register use; the remaining frame/register/copy differences are a source TODO.

### 6. `TGCConsole2::load(JSUMemoryInputStream&)`

Unit `mario/GC2D/GCConsole2`, 3,776 bytes, 99.615%.
Batch 45 resolves the original 0xec04 store/update sequence by correcting the blend-call receiver to unk2A0, while hiding unk2AC.
The original health-pane stride is two pointers, and a US-only class-layout extension at 0x3AE/0x3B0 shifts later fields by four bytes.
That layout correction plus constructor initialization matches 12 functions / 5,936 bytes; load improves to 99.8411%.
Reusing one color temporary reduces the frame from 0x148 to 0x120 versus target 0x118.
The resource constructor still lacks a pointer copy at 0xe6d4; frame/color/buffer offsets remain TODOs.

### 7. `TMario::TMario()` — matched in batch 46

Unit `mario/Player/MarioInit`, 20,080 bytes, 99.232%.
The frame matches.
Almost all 769 operand differences are register numbering caused by one scheduling difference at 0x57ac.
There the original loads `li r21,0x12c` before `addi r3,r31,0x30d8`; the value is then stored with `sth ...,0x3168(r31)`.
This is around the `TBaseParam` initializations near offsets 0x3158–0x3168.
Some string-literal labels also differ: check the unit's string order first, since those differences may be data rather than code.
Batch 46 corrects TEParams defaults (down type 1, motor 25, minimum speed 16, invincibility 300), auto-demo rotation 0x400, initial fields 0x54E/0x55C, and the Hamukuri filename.
Removing StageUtil static tables from this unit restores the virtual-table offsets; all 20,080 constructor bytes now match.

### 8. `TMario::checkCollision()`

Unit `mario/Player/MarioCheckCol`, 4,972 bytes, 99.340%.
At 0xbe0 and 0xc74 the original checks a field against both 2 and 3; ours drops one comparison.
The original also loads the vtable earlier for the virtual call at 0xe10.
The frame is 0x238 in the original and 0x1e0 in ours (0x58 too small), so inlines are probably missing.

### 9. `TMario::receiveMessage(THitActor*, unsigned long)`

Unit `mario/Player/MarioReceiveMsg`, 8,900 bytes, 99.213%.
The comparison tree for the message `switch` around 0x16a0–0x1790 differs.
The compared constants are off (0x16 vs 0x1f, 5 vs 9, and so on), so the set of case labels is wrong: some are missing, extra, or grouped differently.
The frame is 0x220 in the original and 0x180 in ours (0xa0 too small), so there are probably missing inline helpers with locals.

### 10. `TSelectDir::rsetup()`

Unit `mario/GC2D/SelectDir`, 4,384 bytes, 99.600%.
Instructions match, but about 790 offsets differ.
Most are string-pool offsets (`addi r3,r31,0x20` vs `0`), so strings are missing or out of order in this unit.
The stack offsets differ too: the frame is 0x648 in the original and 0x610 in ours (0x38 too small).
Fix the strings first, then look at the frame.

## Refreshing the target list

Run after each batch's build to get the current largest near-matching game functions:

```
python3 - <<'EOF'
import json
r = json.load(open("build/GMSE01/report.json"))
fns = []
for u in r["units"]:
    if "game" not in u.get("metadata", {}).get("progress_categories", []):
        continue
    # THPPlayer is categorized as game but is restricted by AGENTS.md.
    if u.get("metadata", {}).get("source_path", "").startswith("src/THPPlayer/"):
        continue
    for f in u.get("functions", []) or []:
        fz = f.get("fuzzy_match_percent", 0) or 0
        if 98 <= fz < 100:
            name = f.get("metadata", {}).get("demangled_name", f["name"])
            fns.append((int(f["size"]), fz, u["name"], name))
for size, fz, unit, name in sorted(fns, key=lambda x: -x[0])[:40]:
    print(f"{size:6d} {fz:7.3f} {unit} :: {name}")
EOF
```

## Before the next run (human decisions)

- **Back up the branch.**
  All commits on `local/decomp-progress` are local only; push them to a fork.
- **Review shared-code judgment calls** before more work builds on them.
  Examples: the water-hit field union (batch 14), link-order archiving for duplicate globals (batch 37), US-only code paths in shared code, and moving a boss hit-actor declaration into the shared header (batch 16).
  Also spot-check matched code for fakematches.
- **Decide whether this US-version work is meant to go upstream.**
  Upstream `doldecomp/sms` `main` only has `GMSJ01` and `GMSP01` configs.
  If upstreaming is the goal, supply a Japanese disc image so header changes can be checked against the version upstream builds.
