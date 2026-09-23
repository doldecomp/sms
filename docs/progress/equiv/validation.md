# Validation of `tools/equiv-check.py`

Runs of 2026-09-23 on the build at `ab0676a0` (the objects and `report.json` the verdicts in `equiv.tsv` come from).

## (a) Exact functions must come out EQUIVALENT

`python3 tools/equiv-check.py --exact-sample 200 --seed 7`: 198 EQUIVALENT, 2 UNSUPPORTED, 0 DIFFERENT.

- `OSReport` is variadic and takes the address of its caller's frame (`va_list`); the checker refuses that by design.
- `TRKHandleRequestEvent` has an epilogue the frame recogniser does not accept (LR restore).

An earlier run of the same sample found 9 multiple-inheritance thunks (`@32@__dt__7TAmenboFv`: adjust `this`, tail-jump) UNSUPPORTED; tail calls are now modelled as a call whose target reads our registers.

## (b) Deliberate mutations must come out DIFFERENT

`python3 tools/equiv-check.py --selftest 40 --seed 3 -v` takes exact functions of 64+ bytes and mutates our side only (the parsed instruction stream of a scratch copy, never the object or the source).
Instructions carrying a relocation are never mutated.

| mutation | expected | result |
| --- | --- | --- |
| swap the operands of subf/fsubs/fsub/divw/fdivs | DIFFERENT | 40/40 DIFFERENT |
| immediate +1 on addi/cmpwi/cmplwi/mulli (not r1-relative) | DIFFERENT | 39 DIFFERENT, 1 EQUIVALENT |
| displacement +4 on a non-stack lwz/stw/lfs/stfs (member offset) | DIFFERENT | 40/40 DIFFERENT |
| flip the sense of a conditional branch | DIFFERENT | 40/40 DIFFERENT |
| positive control: swap r30 and r31 everywhere | EQUIVALENT | 40/40 EQUIVALENT |
| positive control: swap two adjacent independent ALU instructions | EQUIVALENT | 40/40 EQUIVALENT |

The EQUIVALENT constant mutation is correct: it turns `addi r3,r31,0` into `addi r3,r31,1` in `TAnimalManagerBase::__ct__`, changing only the returned `this`.
No retail caller reads r3 after that constructor and its address is never taken, so the return-use analysis proves the value unobservable.

## (c) Hand spot-check of 10 EQUIVALENT verdicts

Six L1 and four L2 verdicts drawn at random from `equiv.tsv`, checked on the retail/ours diff (and on the full listings for the two marked *).

| function | level | what differs | verdict by hand |
| --- | --- | --- | --- |
| `loadAfter__23TMapObjRevivalPollutionFv` | L1 | roles of r28-r31 permuted, otherwise identical | equivalent |
| `sub__14TDirectionCalcFf` | L1 | FPR permutation; float pool entries `@3467/@3657` vs `@1620/@2035` equal by bytes | equivalent |
| `__ct__11TPinnaShellFv` | L1 | `this` spilled around the base constructor at 12(r1) vs 8(r1) | equivalent |
| `setCallback__16TDrawSyncManagerFUlUsUsP17TDrawSyncCallback` | L1 | 8-byte stack struct (sth/sth/stw, reread as words) at 40 vs 36 | equivalent |
| `initSetEnemies__15TPoiHanaManagerFv` | L1 | frame 224 vs 64, same saved registers at other offsets | equivalent |
| `perform__14TEffectObjBaseFUlPQ26JDrama9TGraphics` * | L1 | retail saves r31 but never uses it (frame 32 vs 8); bodies identical | equivalent |
| `emitAndSRT__11TMapObjBaseFlUcPCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>RCQ29JGeometry8TVec3<f>` | L2 | frame 80 vs 72; fctiwz scratch slots moved and reordered | equivalent |
| `isChangedSetting__14TOptionControlCFv` * | L2 | `mr r29,r31` vs `li r29,1`, r31 was set to 1 two instructions earlier | equivalent |
| `SandCastleCallBack__FUlUl` | L2 | address-taken Vec at 48 vs 16 (frame 64 vs 32) passed to `TTargetArrow::setPos` (12-byte extent); its three stores move with it | equivalent |
| `control__11TKoopaFlameFv` | L2 | FPR reallocation, one extra `fmr`, a load moved past arithmetic only | equivalent |

All ten agree with the tool.
