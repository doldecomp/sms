# Batch history, batches 1-69

Archived from `PROGRESS.md` on 2026-09-16. Not loaded by default; read only when a specific old batch matters.
Numbers here are as of each batch and are stale. Current state is in `PROGRESS.md`.

## Target and status

Target: North American English, `GMSE01` revision 0, using the local ISO.
The full decompilation is **not complete**.
The local branch is `local/decomp-progress`.
The upstream starting commit is `ab00c3c9a466152f6e6bc5b9c28aca959d1a8454`.
Before related edits, consult the [shared-fix catalog](../catalog/README.md) and search for other callers.

## Latest checkpoint: batch 69 — nerve bodies, and a real cause for the frame gaps

Game matched code **28.28%**, aggregate **41.18%**, source-linked **17.78%** (399 of 732 files). DOL byte-identical throughout. No gameplay test performed.

### The frame-gap finding, and a correction

Batch 61 concluded that near-exact frame gaps were an opaque compiler artifact and catalogued the class as blocked. **That was wrong for at least some of them.**

A `TParamRT<T>` occupies 0x14 bytes and holds its value at **+0x10**, so a raw field read at, say, 0x364 is really `.get()` on a param declared at 0x354, and the accessor's temporary is the missing stack. In `tobiPuku` three consecutive reads turned out to be exactly this. Declaring the params properly, constructing them with `PARAM_INIT` and calling `.get()` took `TNerveTobiPukuPitiPiti::execute` from a stubborn 8-byte gap to **exact**, and cut `Bound`'s gap from 32 bytes to 8.

Before writing off any frame gap, check every raw field read against the `+0x10` rule. Generated `u8 unkNNN[...]` padding blocks actively hide params.

Two other hypotheses were tested and **disproven**: `TSolidStack::push` taking `const T&` (no frame change, 65 regressions) and holding the nerve in a local before pushing (no frame change, worse score). Both are recorded so they are not retried.

### Nerve bodies

Nine of twelve `tobiPuku` nerves are written. `SwimWander` and `PitiPiti` are exact; `Die`, `Fly`, `Attack`, `Bound` and `PrepareFly` are 99.3-99.9% with **zero structural differences**; `Generate` is 96.2% with only a register swap; `Fall` is 98.3%.
`HitWater`, `ReturnLaunch` and `Land` are still placeholders carrying their map sizes.

Reconstructing a nerve body requires the actor's virtuals declared in vtable order first, since the calls compile to fixed offsets. `docs/MATCHING_CATALOG.md` has the one-liner that dumps a vtable with slot offsets. Read the **whole** dump: a partial read led to `setDeadAnm` being declared non-virtual when it overrides the slot at 0x154.

### Structural seams, now spent

Earlier batches took game matched code from 26.15% to 28.28% mostly through structure rather than bodies: 308 library objects source-linked (2.57% -> 17.61%), 209 nerve accessors across ~30 units, and class hierarchies recovered from destructor vtable chains in another dozen. Those seams are exhausted; what remains is per-function reconstruction.

### Still open

- **The branch exists only on this disk.** 45+ commits, no fork configured. This is the one outstanding risk and needs a remote from the user.
- `Camera/CameraInbetween` remains the last fully matching game object that will not link; its diagnosis is in the catalog.
- ~200 nerve bodies and the bulk of every enemy and MoveBG unit's real methods are unwritten.

## Verified checkpoint: batch 62 — hauntLeg started, fifteen of twenty-eight exact

`src/Enemy/hauntLeg.cpp` and `include/Enemy/HauntLeg.hpp` did not exist. Fifteen of twenty-eight functions match on the first working build.
Game matched code **26.32% -> 26.36%**, aggregate **39.63% -> 39.66%**.

Three classes: `THauntedObject : TTakeActor`, `THauntLeg : TWalkerEnemy` (size 0x1B0) and `THauntLegManager : TSmallEnemyManager`, plus one nerve.
Exact so far: all three destructors bar `THauntedObject`'s, the manager constructor and `load`, `setMActorAndKeeper`, `reset`, `setDeadAnm`, the four animation setters, `getBasNameTable`, `THauntedObject::receiveMessage` and both `@32@` thunks.
`mHolder`/`mHeldObject` at 0x68/0x6C are `TTakeActor` fields, and `THauntedObject::receiveMessage` forwards a kill to its holder.

Near-exact: `__sinit` 99.9% (again 764 bytes from the MSound rogue includes), `attackToMario` 99.8%, `createModelData` 99.2%, `createEnemyInstance` 60.7%, `THauntedObject::~THauntedObject` 81.4%.
Not yet written: the nerve and its destructor, `getTakingMtx`, `isCollidMove`, `calcRootMatrix` (1,032B), `init`, `initSetEnemies` and the `HauntLegCallback` J3D node callback.

Also this batch: `TNerveSealWait::execute` was validated with temporary padding and reaches 100% with zero instruction differences, confirming the near-exact frame gaps are pure reservation rather than wrong code. See `docs/MATCHING_CATALOG.md`.

DOL byte-identical. No gameplay test performed.

## Verified checkpoint: batch 60 — seal decompiled from scratch, eleven of eighteen exact

`src/Enemy/seal.cpp` and `include/Enemy/Seal.hpp` did not exist. Eleven of eighteen functions match, 1,404 of 4,292 bytes.
Game matched code **26.27% -> 26.32%**, aggregate **39.59% -> 39.63%**.

`TSeal : TSpineEnemy` with `TSealManager : TEnemyManager` and three nerves (Sleep, Wait, Die).
The enemy is the orange seal: model `gene_orange_model1.bmd`, collision `gene_orange_col1.col`, scene `/scene/seal`, confirmed by `MSD_SE_EN_ORANGESEAL_WAIT`.
`init` registers the actor into the `"敵グループ"` group list, the same idiom `AnimalBase` uses.
`__sinit_seal_cpp` (764B) matched immediately from the same MSound rogue includes that fixed effectEnemy, confirming that pattern generalises.

Exact so far: both destructors, both constructors, the whole manager (load, createModelData, constructor, destructor), the Sleep nerve's destructor, `@32@__dt` and `__sinit`.
Remaining: `init` 97.4%, `perform` 93.0%, `TNerveSealWait` 89.2%, `TNerveSealSleep` 88.7%, `TNerveSealDie` 87.8%, `calcRootMatrix` 85.2%, and `receiveMessage` at 6.0%, which is still only a call to the base and needs real reconstruction.

DOL byte-identical. No gameplay test performed.

## Verified checkpoint: batch 59 — effectEnemy decompiled, fourteen of eighteen exact

`src/Enemy/effectEnemy.cpp` and `include/Enemy/EffectEnemy.hpp` did not exist. Fourteen of the unit's eighteen functions now match exactly.
Game matched code **26.18% -> 26.22%**, aggregate **39.52% -> 39.55%**.

`TEffectEnemy : TWalkerEnemy` adds one field, and `TEffectEnemyManager : TSmallEnemyManager` one parameter pointer, so nearly the whole 110-entry vtable is inherited.
The enemy is "エフェクト敵", recovered from `@3016` in `.rodata`; its parameters load from `/enemy/moveFireEffect.prm`.
Thirteen functions matched on the first compile once the source order was derived by reversing the map's emission order.

Three fixes came from using existing inlines rather than open-coding, which is what the remaining differences kept pointing at:
`getMaxHitPoints()` replaces an open-coded save-parameter read; `TBGCheckData::isDeathPlane/isPool/isWaterSurface` replace raw `mBGType` comparisons (69.7% -> 84.9%); and `isAirborne()` replaces `checkLiveFlag(LIVE_FLAG_AIRBORNE)` (84.9% -> 91.8%).
The original materialises each predicate as a bool with `li 1`/`li 0`, which only the helper that ends `? 1 : 0` reproduces.

Continued to **seventeen of eighteen exact**, 2,448 of 2,572 bytes. Game matched code **26.22% -> 26.27%**, aggregate **39.55% -> 39.59%**.

- `__sinit_effectEnemy_cpp` (764B, the unit's largest function) matches once `MSound/MSSetSound.hpp` and `MSound/MSoundBGM.hpp` are added as rogue includes; its whole body is JAL sound-list registration driven by include set, not by code.
- `perform` 86.5% -> exact. The particle scale is **integer** division converted afterwards, `(f32)((s32)mHitPoints / (s32)(u8)getMaxHitPoints())`, not a float divide: the original emits `divw` plus the `xoris` signed bias, and the `(u8)` cast reproduces a `clrlwi` that the u32 return alone does not.
- `forceKill` 91.8% -> exact by inverting the guard so both paths fall into a **single** `kill()` call, which is the shape the original's shared tail has.

Only `setDeadAnm` remains at 99.8%: every instruction matches and the frame is 0x18 against 0x20, the familiar 8-byte reserved-inline-local gap.
The unit still cannot be source-linked until that closes. DOL byte-identical. No gameplay test performed.

## Verified checkpoint: batch 58 — BathtubBinder reconstructed, five of six exact

`src/Enemy/BathtubBinder.cpp` and its header did not exist. Five of the unit's six functions now match exactly (540 of 1,688 bytes): the constructor, destructor, `bind`, `init` and the out-of-line `TVec3<f32>::set<f>`.
Game matched code **26.157291% -> 26.18%**, aggregate **39.50% -> 39.52%**.

The class is a `TBinder` that floats an actor on the bath water. `init` resolves two Shift-JIS named objects, `@2160` = "バスタブ" and `@2161` = "バスタブの水", recovered from `.rodata`.
`init` returns **`bool`, not `BOOL`**: with `BOOL` the compiler adds a `clrlwi r3,r0,24` byte widening the original does not have. That one change took it from 98.6% to exact.
`bind` tests `mBathtub->unk29A` directly, matching how `GCConsole2` and `BathWaterManager` already read that flag.

`float_` (1,148B) remains at 65.7%. It samples the water ahead of and behind the actor, clamps each point inside the tub circle, then blends height and pitch.
The clamp is expanded three times with no symbol in the map, so it is a header inline; it is written as `clampToTub` and the shared `TBathtubData::getThing()` already had the exact expression the assembly uses.
Its frame is 0x148 against 0x178, 0x30 short, and the original caches the two matrix elements `rot[0][2]` and `rot[2][2]` in f30/f31 across the whole function where we reload them. The missing stack is consistent with each inlined clamp keeping both the returned temporary and a named copy of the centre.

The unit cannot be source-linked until `float_` matches. DOL remains byte-identical. No gameplay test performed.

## Verified checkpoint: batch 57 — enemyinterp decompiled from scratch and linked

`src/Enemy/enemyinterp.cpp` did not exist. It is now written, fully matching and source-linked, so **game matched code moves for the first time this run: 26.15378% -> 26.157291%**, and game files reach 91 (3.428571% -> 3.471861%).

The whole TU contributes only three symbols to the binary, all weak, and all now exact: `SpcTrace(const char*, ...)` (80B), `TSpcTypedBinary<TLiveActor>::~TSpcTypedBinary()` (100B) and that template's 16-byte vtable.
`SpcTrace` is a varargs debug stub with an empty body; its 0x50 is entirely MWCC's PPC EABI varargs register-save prologue, which is why an empty function is 80 bytes.
Nothing in the file emits those symbols directly. Declaring `TEinBinary : TSpcTypedBinary<TLiveActor>` is what instantiates the template's weak destructor and vtable, and the `ein*` builtins' `SpcTrace` calls are what force its out-of-line copy, since MWCC cannot inline a varargs function.

Source order was derived by reversing the map's `.text` emission order, and `validate-symbol-order` reports presence, order and linkage all correct on the first attempt.

Seven `ein*` builtins and `TEinBinary::initUserBuiltin` are UNUSED with no assembly to compare against, so their bodies are reconstructed from their names alone and are marked `// TODO: incorrect size`.
`TEinBinary::~TEinBinary` already matches the map's 0x74 exactly; the other six are between 8 and 224 bytes short.
They are deadstripped, so the link is unaffected.

Verified: `cmp` identical, SHA-1 `a6782903ef79d4196c8489ecb1b57decb5b3728f`. No gameplay test performed.

## Verified checkpoint: batch 56 — PollutionEvent linked after four batches deferred

`Map/PollutionEvent.cpp` links byte-identically at last, deferred since batch 2 as "emits a destructor before loadAfter".
Game files 89 -> 90, **3.386243% -> 3.428571% source-linked**.

Cause: the four event classes were declared with no base, no virtual functions and no destructor, but the map gives each a vtable of 0xa0-0xa4 and a virtual destructor.
Without them the compiler flushed its weak queue early, emitting `TPollutionTest::__dt` and `perform` *before* `loadAfter`; the original emits them after.
Declaring the four destructors restores the original emission order, and placing them to match the map (ModelStampMario, ModelStamp and Maze first in source; AreaObj last, since emission reverses source order) makes `validate-symbol-order` report **order matches the map**.

Still missing from that unit: the four `@32@` destructor thunks and `TVec3<f>::set`, all UNUSED.
The thunks imply multiple inheritance with a second base at offset 0x20, which is unreconstructed; they are deadstripped so the link is unaffected.

`Camera/CameraInbetween.cpp` remains the last fully matching unlinked object. It fails differently, shifting the DOL header at byte 204, so a section size changes rather than code content.

Game **26.15378% matched / 3.428571% source-linked**; `cmp` and SHA-1 verified. No gameplay test performed.

## Verified checkpoint: batch 55 — THPPlayer linked; frame gap measured

The three fully matching THPPlayer objects link cleanly now that the library restriction is lifted: game files 86 -> 89, **3.254321% -> 3.386243% source-linked**, DOL still byte-identical.

The other two unlinked fully-matching game objects still fail, as they have since batch 2, and they fail differently.
`Camera/CameraInbetween.cpp` shifts the DOL header at byte 204, so a section size changes.
`Map/PollutionEvent.cpp` shifts text at byte 1,700,992; its unit is missing nine UNUSED symbols, the virtual destructors of four event classes plus their `@32@` thunks and a `TVec3<f>::set` inline, and ten UNUSED stubs are 4 bytes against map sizes up to 388.

Measured the real blocker for the near-exact backlog: across the 70 largest game functions at 99.9%+, **60 have frames that are too small**, 4 exact, 6 too large, 6 leaf.
The remaining 343 near-exact functions (217,272 bytes) are therefore gated on missing inlined helpers that reserved locals, not on wrong instructions.
Details and the diagnostic rule are in `docs/MATCHING_CATALOG.md`.

Game code **26.15378% matched / 3.386243% source-linked**; all 399 linked files verified with `cmp` and SHA-1. No gameplay test performed.

## Verified checkpoint: batch 54 — all matching library objects linked

Reconstructing the deadstripped `__dec2num` restores `ansi_fp.c`'s `.sdata2` pool order, so the last held-back library object links.
**Every library object that matches in code and data is now source-linked: 394 files, 17.639089%.** SDK reaches 92.22% linked; JSystem holds at 61.61%; the DOL stays byte-identical.

The fix is a literal-ordering constraint, not a code change. The compiler allocates this TU's float literals in first-use order across the whole file, and `-inline deferred` emits `__dec2num` first.
Its literal sequence must be exactly 0.0, 1.0, then the int-to-double magic `0x4330000080000000`, with no 10.0 or 0.1 literal, which is why digit accumulation uses integer arithmetic and `digit_values` rather than multiplying by 10.0.
The pool then matches the map: @268=0.0, @270=1.0, @272=magic, @362=0.1, @363=10.0.

**Caveat:** the reconstructed body is 0x100 (256 bytes) against the map's 0x2a4 (676), so it is not the original implementation and is marked `// TODO: incorrect size`.
The real function is 420 bytes larger and presumably handles the empty significand, the 'I'/'N' texts `__num2dec` emits, and exponent overflow.
Because the function is deadstripped, only its literal usage reaches the binary, so the link is sound while the body remains wrong.

Verified: `cmp` against `orig/GMSE01/sys/main.dol` identical, SHA-1 `a6782903ef79d4196c8489ecb1b57decb5b3728f`, matched code unchanged at 1,423,420 bytes.
Game code untouched at **26.15378% matched / 3.254321% source-linked**. No gameplay test performed.

Remaining unlinked: 32 JSystem and 7 SDK objects that do not yet fully match, plus 299 game files. These need decompilation, not promotion.

## Verified checkpoint: batch 53 — library objects source-linked

The user lifted the library prohibition, so the 308 JSystem/MSL/SDK objects that already matched in code and data were promoted through `config/GMSE01/objects.json`.
307 of them link cleanly: source-linked code rises from **2.571545% to 17.609865%** (+541,944 bytes, 86 -> 393 files) with the DOL still byte-identical.
JSystem goes 0% -> 61.61% linked (166/198 files) and SDK 0% -> 91.84% (141/149).

One object is held back: `PowerPC_EABI_Support/Msl/MSL_C/MSL_Common_Embedded/ansi_fp.c`.
Linking it changes exactly 23 bytes: four inside `__num2dec` (0x80338cbf, 0x80338cdf, 0x80338ce3, 0x80338cff) and the 8-byte `.sdata2` literal `@272` at 0x80417560, plus its neighbour.
Its per-object code and data both report 100%, so objdiff's comparison does not see whatever differs; suspect the `.sdata2` literal pool contents or ordering.
Diagnosed: the map records a deadstripped `__dec2num` (UNUSED, 0x2a4) and its literal `@269` in this TU.
With that function missing, our `.sdata2` pool orders the int-to-double magic constant last instead of third, so four `@sda21` displacements resolve differently after linking.
The code itself is correct; objdiff reports 100% because it compares symbolic references, not resolved offsets.
Reconstructing `__dec2num` against its 0x2a4 size is the fix and the next target.

Verification: `cmp` against `orig/GMSE01/sys/main.dol` is identical, SHA-1 `a6782903ef79d4196c8489ecb1b57decb5b3728f`, matched code unchanged at 1,423,420 bytes with no function regressions.
Game code is untouched at **26.15378% matched / 3.254321% source-linked**. No gameplay test performed.

## Verified checkpoint: batch 52 — US CardLoad class layout restored

The US title animation carries five additional panes: `unk1D4` is 18 entries, not 13, shifting `unk208` through `unk22E` by 0x14 and removing the Japanese three-byte tail padding.
Guarded with `VERSION_GMSE01`. The constructor becomes exact (**+548 code bytes / one function**) and 16 further CardLoad functions improve with no regressions.
`waitForChoice` reaches **98.36285%** (from 98.30238%); its `setCenteredSize` duration is 40, not 20, with 1.5x start sizes, confirmed by matching `li r4, 0x28` at four call sites.
Only seven structural differences remain, one scheduling cluster at 0x7de8-0x7e54 driven by the 0x40 frame gap (0x368 vs 0x3a8); left as a source TODO.
Baseline at `4ea41229`; build, changes_all, unit-data checks and DOL byte/SHA-1 checks pass. Map output unchanged: one missing function, three UNUSED-size warnings, all pre-existing.
Game **26.15378% matched / 3.254321% source-linked**. No gameplay test performed.

## Verified checkpoint: batch 51 — shared menu animation fields corrected

`updateCenteredSize` must read the offset interpolator at 0x14/0x18, as shown by CardSave and CardLoad callers. The shared correction improves three functions with no regressions.
`waitForChoice` reaches **98.96596%** (from 98.9611%); using the existing color setter restores eight-byte color-temp spacing and reduces its frame from 0x3B0 to 0x3F8 vs original 0x428. Frame, register and particle-position scheduling differences remain TODOs; no new exact code or source-linked files.
Fresh baseline at `4ea41229`; header rebuild, changes_all, all 12,904 function comparisons, unit-data checks and DOL byte/SHA-1 checks pass. No gameplay test performed.
CardSave/CardLoad/ExPane map output is unchanged: CardSave retains two missing functions/one UNUSED-size warning, CardLoad one missing function/three size warnings, and ExPane passes. Game remains **26.13458% matched / 3.254321% source-linked**.
Queue: 1,254 functions / 704,856 bytes; next is `TCardLoad::waitForChoice` (3,704 bytes), with a possible shared US layout difference to investigate.

## Verified checkpoint: batch 50 — shared FLUDD emission call restored

Move `isEmitting` from the header to its map-recorded WaterGun definition and restore its demo/dialogue guard. This makes `considerRotateStart` exact: **+144 code bytes / one function**.
`soundMovement` improves **98.25304% → 99.994804%**, including a corrected damage-sound guard; `squating` reaches **99.91304%**, and the restored helper reaches **99.92063%**. Their remaining frame/stack differences are source TODOs.
Fresh baseline at `27ef0a79`; header rebuild, changes_all, all 12,904 function comparisons, unit-data checks and DOL byte/SHA-1 checks pass without regressions. No gameplay test performed.
Map checks introduce no new errors: WaterGun's missing count drops from 25 to 24 UNUSED functions; MarioRun still lacks UNUSED braking. Existing UNUSED-size warnings remain (MarioRun 15, MarioSound/MarioWait one each); linked-symbol order/linkage are preserved.
Game **26.13458% matched / 3.254321% source-linked**, still 86 linked files. Queue: 1,254 functions / 704,856 bytes; next is `TCardSave::waitForChoice` (6,580 bytes).

## Verified checkpoint: batch 49 — SelectDir string data restored

`rsetup()` improves **99.600365% → 99.63048%** after restoring the original 32-byte string-pool prefix through DummyStrings.hpp and spelling out the stage-display constructor arguments.
All SelectDir data now matches: **+688 data bytes**. Frame 0x610 vs 0x648, list/constructor stack slots and camera-vector setup remain a source TODO; no new exact functions or source-linked files.
Baseline at `ed1fe590`; full build, changes_all, all 12,904 function comparisons, unit-data checks and DOL byte/SHA-1 checks pass without regressions. Map presence/order/linkage and the UNUSED size pass; no gameplay test performed.
Game remains **26.129522% matched / 3.254321% source-linked**. The strategy's ten initial targets have each been reviewed; next is the largest unreviewed near-match, `TMario::soundMovement()` (6,924 bytes). Queue: 1,252 functions / 703,776 bytes.

## Verified checkpoint: batch 48 — receiveMessage behavior corrected

`receiveMessage()` improves **99.212585% → 99.981125%**: restore the sender-type case groups and two fallthrough boundaries, the 1-up pickup guard, and fruit-kick rumble parameter.
All 2,225 instruction shapes now match; frame 0x180 vs 0x220 and wire/conversion stack slots remain a source TODO. No new exact code or source-linked files.
Baseline at `118d0546`; full build, changes_all, all 12,904 function comparisons, unit-data checks and DOL byte/SHA-1 checks pass without regressions. Map presence/order/linkage and all four UNUSED sizes pass; no gameplay test performed.
Game remains **26.129522% matched / 3.254321% source-linked**. Queue: 1,252 authorized functions / 703,776 bytes; next reviewed target is `TSelectDir::rsetup()`.

## Verified checkpoint: batch 47 — collision behavior and dispatch corrected

`checkCollision()` improves **99.3395% → 99.87611%**: fix the inverted Yoshi holding guard, restore hip-drop state 3, and correct fourteen enemy-ID cases. A named small-enemy pointer restores virtual-call scheduling; only stack operands remain (frame 0x1E0 vs 0x238).
`hitNormal()` improves **97.371216% → 99.643936%** by retaining the water-hit actor pointer and adjusting its copied position; remaining frame/register differences are TODOs.
Baseline at `46691bc4`; full build, changes_all, all 12,904 function comparisons, unit-data checks and DOL byte/SHA-1 checks pass without regressions. Map presence/order/linkage pass with four existing UNUSED-size warnings; hitHipDrop grows from 104 to 112 bytes vs map 116.
No new exact code or source-linked files: game **26.129522% matched / 3.254321% source-linked**. No gameplay test performed.
Refreshed queue: 1,252 authorized functions / 703,776 bytes; next reviewed target is `TMario::receiveMessage(THitActor*, unsigned long)`.

## Verified checkpoint: batch 46 — Mario constructor matched

`TMario::TMario()` now matches all **20,080 bytes / 5,020 instructions** after correcting shared damage defaults, two other initial values, the auto-demo rotation default, and the Hamukuri parameter filename.
Remove StageUtil's unused static tables from this unit by declaring its sole required function directly; this restores the virtual-table offsets. Restore the four-byte UNUSED `stageSetting` body recorded by the map.
Game **26.129522% matched / 3.254321% source-linked**; aggregate **39.47912% / 2.571545%**; **+7,048 matched data bytes**, still 86 source-linked game files.
Baseline at `42041014`; full build, changes_all, all 12,904 function comparisons, unit-data checks and DOL byte/SHA-1 checks pass without regressions. Map presence/order/linkage and all 21 UNUSED sizes pass; no gameplay test performed.
Refreshed queue: 1,251 authorized functions / 703,248 bytes. MarioInit retains nonmatching routines and is not promoted; next reviewed target is `TMario::checkCollision()`.

## Verified checkpoint: batch 45 — US console layout and constructor

Restored the US-only 16-bit field at 0x3AE and byte at 0x3B0, correcting all later member offsets; restored missing constructor initialization. **+5,936 exact code bytes / 12 functions**, including the entire constructor.
Load improves **99.61546% → 99.8411%**: fix health-pane pair indexing, apply blend changes to the original pane, and reuse one color temporary. Its frame is now 0x120 vs 0x118; texture-construction and stack differences remain TODOs.
Game **25.424381% matched / 3.254321% source-linked**; aggregate **38.92192% / 2.571545%**; still **86 source-linked game files / 92,672 code bytes**.
Fresh baseline at `9e60782a`; full header rebuild, changes_all, all 12,904 function comparisons and unit-data checks show zero regressions; DOL byte/SHA-1 checks pass. No gameplay test performed.
Map results are identical to the pre-change source: five missing symbols, weak-order warnings and four UNUSED-size warnings. The file remains incomplete; US timer/flag behavior still needs reconstruction.
Refreshed queue: 1,252 authorized functions / 723,328 bytes; next reviewed target is `TMario::TMario()`.

## Verified checkpoint: batch 44 — BossEel initialization corrections

`TBossEel::init()` improves **99.44744% → 99.50852%**: restore distinct tooth-model array entries, correct both eye/heart loader flags, and call collision setUpTrans at the original virtual slot.
A named skin-deformer local restores argument setup order; sharing the resource pointer improves the later loader registers. Frame, eye/skin registers and the extra heart-model copy remain a source TODO.
No new exact functions or source-linked files; game **25.215933% matched / 3.254321% source-linked**, aggregate **38.757206% / 2.571545%**.
Baseline at `05ee5955`; full build, changes_all, all 12,904 function comparisons, unit-data checks and DOL byte/SHA-1 checks pass without regressions. No gameplay test performed.
Map comparison against the original source confirms the existing missing weak TVector::begin, weak-order warning and eight UNUSED-size warnings; no new map errors. BossEel remains incomplete.
Refreshed queue: 1,263 authorized functions / 727,976 bytes; next reviewed target is `TGCConsole2::load(JSUMemoryInputStream&)`.

## Verified checkpoint: batch 43 — NPC revival helper matched

Following the npcWetting target exposed a neighboring helper: `sunflowerReviving()` now matches all **204 bytes**, with a named animation local restoring its original stack frame.
NpcWetting retains its frame/register differences; unsuccessful trials are reverted and a source TODO records the remaining regions.
Game **25.215933% matched / 3.254321% source-linked**; aggregate **38.757206% / 2.571545%**; still **86 source-linked game files / 92,672 code bytes**.
Baseline at `bbd9ab73`; build, changes_all, all 12,904 function comparisons, unit-data checks and DOL byte/SHA-1 checks pass without regressions. No gameplay test performed.
NpcAnm map presence/order/linkage pass with four existing UNUSED stub-size warnings; the file remains incomplete.
Refreshed queue: 1,263 authorized functions / 727,976 bytes; next reviewed target is `TBossEel::init(TLiveManager*)`.

## Verified checkpoint: batch 42 — shared Mario spin-angle correction

A shared negative-multiplier spelling removes the extra sign extension in rotateJumping/jumpMain and makes `TMario::rotating()` exact: **+296 code bytes / one runtime function**.
Restored `checkJumpingThrowStart` from jumpMain's existing inline body: its 92-byte UNUSED size and definition order now match the map.
JumpMain improves to 99.98485%, with only frame/stack operands remaining; animation-local narrowing regressed and was reverted, and the remaining blocker is a source TODO.
Game **25.208769% matched / 3.254321% source-linked**; aggregate **38.751545% / 2.571545%**; still **86 source-linked game files / 92,672 code bytes**.
Baseline at `b1d5cd9c`; full build, changes_all, all 12,904 function comparisons, per-unit data checks and DOL byte/SHA-1 checks pass without regressions; no gameplay test performed.
Map checks still fail on existing missing UNUSED helpers: three in MarioJump and braking in MarioRun; both now pass symbol order/linkage, with four and fifteen size warnings respectively. Neither file is promoted.
Refreshed queue: 1,264 authorized functions / 728,180 bytes; next reviewed target is `TBaseNPC::npcWetting()`.

## Verified checkpoint: batch 41 — Mario particle loader matched

`TMarDirector::loadParticleMario()` now matches exactly: **+4,708 code bytes / one function**.
Declaring the global-only loader static removes the spurious implicit-instance stack slot; its caller also improves (99.133224% → 99.2977%).
Direct expansion of the fabricated particle helper did not affect the frame and was reverted; other particle callers and the helper remain unchanged.
Game **25.198374% matched / 3.254321% source-linked**; aggregate **38.743332% / 2.571545%**; still **86 source-linked game files / 92,672 code bytes**.
Fresh baseline at `1981f998`; full header-dependent build, changes_all, all 12,904 function comparisons and per-unit data checks pass with zero regressions; changed-unit map validation passes without warnings.
DOL byte comparison and expected SHA-1 pass; no gameplay test performed. The loader's file retains three nonmatching routines and is not promoted.
Refreshed near-matching queue: 1,265 authorized functions / 728,476 bytes; next reviewed target is `TMario::jumpMain()`.

## Verified checkpoint: batch 40 — apply near-matching strategy

Adopted [STRATEGY.md](../../STRATEGY.md): prioritize large 98%+ functions, group shared causes, time-box stalls, and keep short batch notes.
Corrected the two wire-transition flag types: specMain improves from 99.85731% to 99.9932%; its six swap-stack operands remain nonmatching (TODO recorded).
Preserved verified spider collision corrections (84.298% → 99.590256%) and beam geometry corrections (88.64047% → 95.671425%); rejected trials are reverted and frame blockers noted in source.
No new exact functions or source-linked files: game **25.033045% matched / 3.254321% source-linked**; aggregate **38.612686% / 2.571545%**, still **86 source-linked game files / 92,672 code bytes**.
Baseline at `f1c2fc62`; full build, changes_all, all 12,904 function comparisons and per-unit data checks show zero regressions; map checks pass with existing UNUSED-size warnings (12 MarioSpecial, one beam).
DOL byte comparison and SHA-1 `a6782903ef79d4196c8489ecb1b57decb5b3728f` pass; no gameplay test performed.
Refreshed `build/GMSE01-near-matching.txt`: 1,266 authorized functions / 733,184 bytes; next reviewed target is loadParticleMario, with TMario callers grouped when shared evidence applies.
A local Git bundle is saved outside the repository; no fork remote is configured, so an off-machine backup remains pending.

## Verified checkpoint: batch 39 — closure trials, no file completed

Reread the plan and retain its whole-file completion order.
Corrected two reversed random-timer bounds in AnimalNerve and improved NpcCallback's flag test and Mario-height retention.
Both files remain nonmatching; no new exact or source-linked bytes are claimed.
Unsuccessful distance/timer and pollution-accessor trials were reverted and recorded.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **25.033045%** | **3.254321%** |
| Aggregate | **38.612686%** | **2.571545%** |

Source linking remains **86 game files / 92,672 code bytes**.
Baseline at `562ed4e0`; full build, changes_all, all 12,904 function comparisons, changed-unit map checks, DOL byte comparison and expected SHA-1 pass with zero regressions.
NpcCallback retains its existing weak-symbol ordering warning; no gameplay test was performed.
The regenerated queue remains 96 authorized small game files / 90,016 unmatched bytes.
See the [batch 39 audit](GMSE01-closure-audit-batch39.md) and [measurements](GMSE01-batch39.json).

## Verified checkpoint: batch 38 — MapCollisionPlane completed

Completed and source-linked **MapCollisionPlane.cpp**, adding **1,348 code bytes** and **220 data bytes**.
A separate integer cell-index conversion recovers the original conversion reuse and stack layout.
MapObjPlane keeps its fractional-coordinate conversion; all its function scores remain unchanged.
One runtime function / 308 code bytes becomes exact.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **25.033045%** | **3.254321%** |
| Aggregate | **38.612686%** | **2.571545%** |

Source linking is now **86 game files / 92,672 code bytes**.
Baseline at `d4900785`; full build, changes_all, all 12,904 function comparisons, map checks, DOL byte comparison and expected SHA-1 pass with zero regressions.
All code/data sections match; no gameplay test was performed.
NpcInbetween helper trials did not complete that file and were reverted.
The regenerated queue contains 96 authorized small game files / 90,016 unmatched bytes.
See the [batch 38 audit](GMSE01-closure-audit-batch38.md) and [measurements](GMSE01-batch38.json).

## Verified checkpoint: batch 37 — map factory completed and source-linked

Completed and source-linked **MarNameRefGen_Map.cpp**, adding **824 code bytes** and **488 data bytes**.
Restored its three missing factory branches, pollution-test constructor context and header-generated strings.
The original map records duplicate sun-path globals; GMSE01 now archives the ordered link inputs so MWLD resolves them with the original layout.
One runtime function / 824 bytes becomes exact, and 448 additional data bytes match.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **25.02223%** | **3.206984%** |
| Aggregate | **38.604145%** | **2.5341396%** |

Source linking is now **85 game files / 91,324 code bytes**.
Baseline at `b8c96698`; full build, changes_all, all 12,904 function comparisons, factory map checks, DOL byte comparison and expected SHA-1 pass with zero regressions.
The archive uses the same source/extracted objects; it does not change coverage accounting.
PollutionEvent's existing map gaps remain deferred; no gameplay test was performed.
The regenerated queue contains 97 authorized small game files / 90,324 unmatched bytes.
See the [batch 37 audit](GMSE01-closure-audit-batch37.md) and [measurements](GMSE01-batch37.json).

## Verified checkpoint: batch 36 — SplashManager and CameraMode completed

Completed and source-linked **SplashManager.cpp** and **CameraMode.cpp**, adding **3,904 code bytes**.
Naming alpha before constructing SplashManager's call-site color recovers its stack slots and scheduling.
A current-mode camera predicate restores the original current/previous-mode inlining and jump-table layout.
Two runtime functions / 536 code bytes become exact, and 1,432 additional data bytes match.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.993294%** | **3.1780477%** |
| Aggregate | **38.581276%** | **2.5112743%** |

Source linking is now **84 game files / 90,500 code bytes**.
Baseline at `8f7a03e5`; full build, changes_all, all 12,904 function comparisons, map checks, DOL byte comparison and expected SHA-1 pass with zero regressions.
Both files match every code/data section; no gameplay test was performed.
The regenerated queue contains 98 authorized small game files / 91,148 unmatched bytes.
See the [batch 36 audit](GMSE01-closure-audit-batch36.md) and [measurements](GMSE01-batch36.json).

## Verified checkpoint: batch 35 — deferred files linked after metadata correction

Completed source linking for **MarDirectorCreateObjects.cpp** and **TargetArrow.cpp**, adding **444 code bytes**.
Resolved the imported map's string-boundary discrepancy at Application's two English disc-error messages and corrected downstream regional metadata.
Verified 1,273 shifted strings, all 17 affected Application strings and 6,502 unchanged strings.
Removed 35 obsolete gap labels whose bytes now belong to correctly attributed objects.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.97447%** | **3.040953%** |
| Aggregate | **38.566406%** | **2.4029427%** |

Source linking is now **82 game files / 86,596 code bytes**.
No source implementation changed; the **14,824-byte matched-data increase** reflects corrected comparisons.
Baseline at `92740f19`; full builds before/after promotion, changes_all, all 12,904 function comparisons, DOL byte comparison and expected SHA-1 pass.
Function presence/similarity is unchanged, no unit loses matched data, and both promoted files pass code/data/map checks.
No gameplay test was performed.
The small-file queue remains 100 authorized game files / 91,684 unmatched bytes; only CameraInbetween and PollutionEvent remain in the deferred exact-code queue.
See the [batch 35 audit](GMSE01-closure-audit-batch35.md) and [measurements](GMSE01-batch35.json).

## Verified checkpoint: batch 34 — shared timer fix completes two managers

Completed and source-linked **livemanager.cpp** and **objmanager.cpp**, adding **2,912 source-linked code bytes**.
One shared timer-helper reconstruction matches both perform routines; restoring objmanager's mapped initObjArray helper and stream extraction completes its loader.
Three runtime functions / 656 code bytes become exact, and the recovered UNUSED helper has its exact 60-byte map size.
SnapTimeObj improves after removing its volatile color workaround, but retains a four-byte stack-slot difference.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.97447%** | **3.025361%** |
| Aggregate | **38.566406%** | **2.3906221%** |

Source linking is now **80 game files / 86,152 code bytes**.
Baseline at `7e3e3455`; full build, changes_all, all 12,904 function comparisons, DOL byte comparison and expected SHA-1 pass with zero regressions.
Both completed files pass all code/data and map checks; SnapTimeObj also passes its map check.
TimeRec retains three pre-existing UNUSED stub-size warnings in an already source-linked file; these remain completion-audit debt.
No gameplay test was performed.
The regenerated queue contains 100 authorized small game files / 91,684 unmatched bytes.
See the [batch 34 audit](GMSE01-closure-audit-batch34.md) and [measurements](GMSE01-batch34.json).

## Verified checkpoint: batch 33 — two whole files completed

Completed and source-linked **DemoBossHanachanBase.cpp** and **SleepBossHanachan.cpp** from the small-file queue.
This adds **21 exactly matching functions/helpers / 2,440 code bytes / 1,496 data bytes**.
Recorded the successful accessor/local-variable patterns and their MapEventSirena exception in the shared-fix catalog.
MapModel, MarDirectorPreEntry and Sirena trials did not complete those files and were reverted.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.951435%** | **2.9231017%** |
| Aggregate | **38.548203%** | **2.3098176%** |

Source linking is now **78 game files / 83,240 code bytes**.
Baseline at `f7a677b8`; full build, changes_all, all 12,904 function comparisons, DOL byte comparison and expected SHA-1 pass.
Zero regressions; both completed files pass all code/data and map checks.
No gameplay test was performed.
The regenerated queue contains 102 authorized small game files / 92,340 unmatched bytes.
See the [batch 33 audit](GMSE01-closure-audit-batch33.md) and [measurements](GMSE01-batch33.json).
Continue the plan's file-completion order, consulting recorded blockers before retries.

## Verified checkpoint: batch 32 — completion queue and shared bounds check

Reread the plan and reaffirmed its file-completion order.
One shared bounds-check change improves sunmgr, lensglow and lensflare; sunmodel requires a direct-member exception.
**No file completed, no newly exact function, and no new source-linked bytes.**
Recorded CameraSecureView, sunmgr-load and egggen trials; revisited CameraInbetween and the deferred string-boundary evidence.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.86575%** | **2.8374174%** |
| Aggregate | **38.48049%** | **2.24211%** |

Source linking remains **76 game files / 80,800 code bytes**.
Baseline at `08c838ba`; full build, changes_all, all 12,904 function comparisons, DOL byte comparison and expected SHA-1 pass.
Zero similarity regressions; sunmgr and lensglow pass map checks.
sunmodel and lensflare retain map failures reproduced with the baseline sources; no gameplay test.
The regenerated queue remains 104 authorized small game files / 94,780 unmatched bytes.
See the [batch 32 audit](GMSE01-closure-audit-batch32.md) and [measurements](GMSE01-batch32.json).
Continue with file-closing hypotheses in the plan's order, consulting recorded blockers before retries.

## Verified checkpoint: batch 31 — MapCollisionEntry close to completion

Matched **three MapCollisionEntry functions / 608 code bytes** and restored its missing 116-byte UNUSED base constructor definition.
The file now passes every map check and has one nonmatching runtime function: its 168-byte initializer retains an eight-byte stack-frame difference.
**No new file was linked from source.**

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.86575%** | **2.8374174%** |
| Aggregate | **38.48049%** | **2.24211%** |

Source linking remains **76 game files / 80,800 code bytes**.
Baseline at `915877d3`; full build, changes_all, all 12,904 function checks, DOL byte comparison and expected SHA-1 pass.
Zero regressions across all header consumers; no gameplay test.
The regenerated small-file queue contains 104 authorized game files / 94,780 unmatched bytes.
See the [batch 31 audit](GMSE01-closure-audit-batch31.md) and [measurements](GMSE01-batch31.json) for retained changes, rejected trials and next files.

## Verified checkpoint: batch 30 — small-file closure audit

Reread the plan and continued its file-completion queue.
MessageLoader's parser now has the original instruction sequence and registers, improving from 99.64336% to 99.94405%; stack offsets and its constructor still differ.
**No new file was completed or linked from source in this checkpoint.**
Recorded blockers and rejected trials for MapXlu, PollutionPos, PerformList, BossHanachanNerve and MovieRumble to avoid repeating work.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.844398%** | **2.8374174%** |
| Aggregate | **38.463623%** | **2.24211%** |

Source linking remains **76 game files / 80,800 code bytes**.
Baseline at `f3694700`; full build, changes_all, all 12,904 function checks, DOL byte comparison and expected SHA-1 pass.
Zero regressions; MessageLoader passes its map check including both UNUSED sizes.
No gameplay test was performed.
The regenerated small-file queue remains 104 authorized game files / 95,388 unmatched bytes.
See the [batch 30 audit](GMSE01-closure-audit-batch30.md) and [measurements](GMSE01-batch30.json).
Continue Priority 1; require full-file verification before any source-link promotion.

## Verified checkpoint: batch 29 — HelpActor file completion

Completed and source-linked **GC2D/HelpActor.cpp**, adding **1,272 source-linked code bytes**.
The US help ID offset and named console locals restore its final two functions.
Also recovered AnimalManager's missing 128-byte UNUSED helper and matched CameraTalk's talk setup and Strategy's constructor.
These three files retain other differences and remain unlinked.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.844398%** | **2.8374174%** |
| Aggregate | **38.463623%** | **2.24211%** |

Source linking is now **76 game files / 80,800 code bytes**.
Baseline at `ad9a47f2`; full build, changes_all, all 12,904 function checks, DOL byte comparison and expected SHA-1 pass.
Zero regressions; four newly exact runtime functions / 916 code bytes.
All four changed source files pass map checks; no gameplay test was performed.
The refreshed small-file completion queue contains 104 authorized game files / 95,388 unmatched bytes.
See the [batch 29 audit](GMSE01-closure-audit-batch29.md) and [measurements](GMSE01-batch29.json) before related trials.

## Verified checkpoint: batch 28 — US selector data and Shimmer definitions

Restored ProgSelect's five original US English strings and text dimensions: **312 newly matched data bytes**, with all 328 unit data bytes now exact.
Removed its pre-existing padding array; the remaining draw-function differences are stack offsets.
Its similarity decreases from 99.85859% to 99.79798% because the artificial frame padding is gone.
No exact code or source-linked coverage was lost.

Recovered Shimmer's two UNUSED near/far bodies with correct 24/20-byte map sizes, corrected three rendering calls, and restored relative matrix-local order.
Both changed files pass their map checks without warnings.
**No new source-linked file is claimed.**

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.812231%** | **2.7927492%** |
| Aggregate | **38.438206%** | **2.2068136%** |

Source linking remains **75 game objects / 79,528 code bytes**.
Baseline at `6bb39882`; full build, changes_all, all 12,904 function checks, DOL byte comparison and expected SHA-1 pass.
All previously exact functions remain exact; one explained nonmatching-function score regression, zero unexplained regressions.
No gameplay test was performed.
See the [batch 28 audit](GMSE01-closure-audit-batch28.md) and [measurements](GMSE01-batch28.json) before further trials.
Continue the Priority 1 file-completion queue.

## Verified checkpoint: batch 27 — PollutionObj file completion

Completed and source-linked **`Map/PollutionObj.cpp`**, adding **2,092 source-linked code bytes**.
Its final 564-byte function now matches exactly after restoring the original local-variable and inline-call structure.
Removed its pre-existing no-op expression; no stack padding was added.
EffectUtil improves to 98.7541% but remains deferred with a missing UNUSED definition.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.812231%** | **2.7927492%** |
| Aggregate | **38.438206%** | **2.2068136%** |

Source linking is now **75 game objects / 79,528 code bytes**.
Baseline at `68fb613c`; full build, changes_all, comparison of all 12,904 functions, DOL byte comparison and expected SHA-1 pass.
Zero regressions; PollutionObj passes all map checks including both UNUSED sizes.
No gameplay test was performed.
The refreshed completion queue has 105 authorized small files / 96,304 unmatched bytes.
Continue file completion in the plan's order; consult the [batch 27 audit](GMSE01-closure-audit-batch27.md) and [measurements](GMSE01-batch27.json).

## Verified checkpoint: batch 26 — restore missing file definitions

Recovered both missing NPC color helpers from repeated call sites.
Their UNUSED map sizes are **48 and 80 bytes**, and the file now passes symbol presence/order/linkage checks without warnings.
The runtime caller still has an eight-byte frame difference.
Water-filter `perform` improves from **78.38614% to 98.68317%** through existing camera helpers, correct pointer reuse, and relative matrix-local order.
A frame gap and one branch remain unresolved.
**No new source-linked file or exact runtime code is claimed.**

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | **24.792425%** | **2.7192852%** |
| Aggregate | **38.422554%** | **2.148763%** |

Source linking remains **74 game objects / 77,436 code bytes**.
Baseline at `4d0e4561`; full build, changes_all, comparison of all 12,904 functions, DOL byte comparison and expected SHA-1 pass.
Zero regressions; both changed files pass map checks without warnings.
No gameplay test was performed.
See the [batch 26 closure audit](GMSE01-closure-audit-batch26.md) for retained evidence, rejected trials and next candidates, and [measurements](GMSE01-batch26.json).

## Verified checkpoint: batch 25 — file-closure audit

Reread the revised plan and audited the Priority 1 completion queue.
**No additional file reached source-link completion**; coverage remains **74 game objects / 77,436 code bytes**.
Restored the airport clear-sign sound through the existing wrapper, improving its 208-byte `watch` function from 79.88461% to 99.96154%.
Its camera flag stack location and two UNUSED pool definitions still prevent a completion claim.
No new exact code bytes or functions are claimed.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | 706,004 / 2,847,660 (**24.792425%**) | 77,436 / 2,847,660 (**2.7192852%**) |
| Aggregate | 1,384,652 / 3,603,748 (**38.422554%**) | 77,436 / 3,603,748 (**2.148763%**) |

Baseline at `6ed21f59`; full build, `ninja changes_all`, comparison of all 12,904 functions, DOL byte comparison and expected SHA-1 pass.
Zero function regressions.
Airport map presence/order/linkage pass with two pre-existing UNUSED size warnings.
No gameplay test was performed.

The [closure audit](GMSE01-closure-audit-batch25.md) records missing definitions, frame/register issues, shared investigation groups and rejected trials.
The [completion queue](GMSE01-completion-queue.json) now excludes protected THPPlayer objects: **106 authorized small game candidates / 96,868 unmatched bytes**.
Continue the smallest remaining file closures; next uninspected candidates include `M3DUtil/M3UModel` and `Map/MapCollisionPlane`.
See [batch 25 measurements](GMSE01-batch25.json).

## Verified checkpoint: batch 24 — Priority 1 file completion

Completed source-link promotion of **`GC2D/MessageUtil.cpp`**, closing its batch 2 deferral.
The inherited stream availability API avoids two unwanted emitted getters; both message functions still match all **968 code bytes**.
Removed the parser's two pre-existing stack-padding arrays using the stream position accessor, a signed 32-bit block tag, and a separate 16-bit entry-size variable.
There are now **74 source-linked game objects / 77,436 code bytes**.

| Coverage | Matched code | Source-linked code |
| --- | ---: | ---: |
| Game code | 706,004 / 2,847,660 (**24.792425%**) | 77,436 / 2,847,660 (**2.7192852%**) |
| Aggregate | 1,384,652 / 3,603,748 (**38.422554%**) | 77,436 / 3,603,748 (**2.148763%**) |

Aggregate exact code and function counts are unchanged: this checkpoint moves an already matching file into the source link.

### Validation and next completion targets

Saved baseline at `5d1b2705`; full build, `ninja changes_all`, all-function presence/score comparison, direct executable byte comparison, and expected SHA-1 checks pass.
Zero function regressions; the message unit's symbol-map check passes without warnings.
No gameplay test was performed.

Follow the user's revised priorities in [DECOMPILATION_PLAN.md](../../DECOMPILATION_PLAN.md): close nearly complete game objects first, then finish the existing boss objects, then resume larger reconstructions.
The next complete-code candidates are camera interpolation, pollution events, director object creation, and target arrow.
Record unresolved evidence and move down the completion queue when a candidate stalls.
The [refreshed completion queue](GMSE01-completion-queue.json) contains 108 unlinked game files with 1..2,000 unmatched code bytes, totaling 98,036 unmatched bytes; these are candidates pending full validation.
Library promotion and edits remain outside the authorized queue.
See [batch 24 measurements](GMSE01-batch24.json) and the [matching catalog](../catalog/README.md).

## Verified checkpoint: batch 23

Applied an evidence-supported horizontal vector calculation to the boss and NPC walking predicates and the boss's path-transition check.
Boss walking improves from **76.784% to 95.624%**, and NPC walking improves from **89.435486% to 92.30645%**.
Corrected the NPC's original 10-unit horizontal threshold and absolute yaw difference, and restored an integer-word direction copy.
Both UNUSED walking predicates now have their original 192-byte map size.
The existing `MsAngleWrap` helper restores one original call in boss `perform` and emits **72 exactly matching code bytes / one function**.
The main update improves to **78.874916%**.

Aggregate exact code is **1,384,652 / 3,603,748 bytes (38.422554%)**, with **8,222 / 12,904 functions** matching.
Matched data increased by 56 bytes.
Source linking remains **73 objects / 76,468 code bytes (2.121902%)**.
Both changed objects remain linked from the original binary.

### Validation and remaining work

Used the saved `ninja baseline` at `211b726c`, rebuilt, and ran `ninja changes_all`.
The comparison of all reported functions, including missing-function detection, found zero regressions.
Full build, direct DOL byte comparison, and expected SHA-1 checks pass for the mixed source/original-object executable.
Both changed units still fail the map check for one missing emitted `TVec3::set<float>` helper; their function order, linkage, and all UNUSED sizes pass.
No gameplay test was performed.

Next work: recover the shared walking setter-call context, then address the boss main update's remaining maximum-roll calls, three signed angle-wrap calls, history-loop unrolling, and arithmetic differences.
The sphere-link angle-wrap site is a verified exception to the main-update wrapper correction.
Keep the current best-evidenced history loop; trials that merely raised similarity while adding non-native pointer reloads were reverted.
See [batch 23 measurements](GMSE01-batch23.json) and the [matching catalog](../catalog/README.md) for successful patterns, exceptions, and rejected trials.
Validation logs and full instruction diffs are saved under `build/GMSE01-*-batch23.*`.

## Verified checkpoint: batch 22

Reconstructed the boss's full 6,108-byte main update/render routine from its original instructions and m2c draft.
It covers body motion, terrain and sand response, collision handling, animation blending, targeting, shadows, and rendering, and currently has **78.2685% instruction similarity**.
All strong functions in `BossHanachanMain.cpp` now have reconstructed definitions.
The boss destructor and adjustment thunk match exactly, adding **116 exact code bytes and two functions**.
Its full virtual table also matches; aggregate matched data increased by **1,144 bytes**.

Aggregate exact code is **1,384,580 / 3,603,748 bytes (38.420555%)**, with **8,221 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The boss main object remains linked from the original binary.

### Validation and remaining work

Captured `ninja baseline` at `8176629d`, rebuilt, and ran `ninja changes_all`.
The comparison of all reported functions, including missing-function detection, found zero regressions.
Full build, byte comparison, and expected SHA-1 checks pass for the mixed source/original-object executable.
The main symbol-map check has two missing emitted helpers: `MsWrap<float>` and `TVec3::set<float>`.
All strong functions have correct order and linkage; the UNUSED walking predicate retains its size warning.
No gameplay test was performed.

Next work: resolve the main routine's helper inlining, loop unrolling, and floating-point expression differences, then finish matching the remaining boss units and verify source linking.
The two maximum-roll calls and four angle-wrap calls currently inline, while the original keeps calls; the slipping routine correctly needs the maximum-roll helper inline.
These differences need caller-specific investigation before shared changes.
See [batch 22 measurements](GMSE01-batch22.json) and the [matching catalog](../catalog/README.md) for verified patterns and exceptions.
The current instruction diff is `build/GMSE01-boss-perform-diff-batch22.txt`; validation logs are `build/GMSE01-*-batch22.*`.

## Verified checkpoint: batch 21

Reconstructed boss initialization, Mario throwing, the local rotation-position helper, and both UNUSED animation helpers.
The three newly reconstructed linked routines cover **2,456 original code bytes**: initialization is **99.07692%**, throwing **96.03125%**, and rotation-position adjustment **95.57692%**.
The animation helpers reproduce their original map sizes of 280 and 368 bytes and inline into initialization with the expected calls.
Corrected the body's position and roll history declarations and both existing callers together; the body constructor remains exactly matching.
A shared trig lookup ordering correction also improved slipping to **99.89655%**.

This batch adds no new exact functions or source-linked objects.
Aggregate exact code remains **1,384,464 / 3,603,748 bytes (38.417336%)**, with **8,219 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.

### Validation and remaining work

Captured `ninja baseline` at `ded6891c`, rebuilt all affected consumers, and ran `ninja changes_all`.
The all-function comparison, including missing-function detection, found zero regressions.
Full build, byte comparison, and expected SHA-1 checks pass for the mixed source/original-object executable.
The parts symbol-map check passes with its prior UNUSED size warning.
The main map check now has five missing symbols: `perform`, emitted `MsWrap<float>` and `TVec3::set<float>`, and the boss destructor/thunk.
Existing strong definitions have correct order and linkage; its weak-order and walking-predicate size warnings remain.
No gameplay test was performed.

Next work: reconstruct the 6,108-byte `perform` routine, using the corrected position/roll histories and the recovered animation helpers.
Remaining instruction differences include temporary stack locations, parameter loads, throwing-ratio evaluation, and the rotation helper's final arithmetic scheduling.
The initializer's resource string offsets also depend on strings in the missing main routine.
See [batch 21 measurements](GMSE01-batch21.json) and the [shared-fix catalog](../catalog/README.md).
The original full draft remains `build/GMSE01/BossHanachanMain-batch20.c`; new validation logs are `build/GMSE01-*-batch21.*`.

## Verified checkpoint: batch 20

Reconstructed 21 linked functions and the UNUSED walking predicate in the previously empty `BossHanachanMain.cpp`.
The new code covers manager/model setup, walking, slipping, damage transitions, collision binding, recovery-path selection, body-roll checks, and head movement.
Recovered the manager class and three stored position/velocity vectors from their original reads and writes.
Fourteen functions match exactly, adding **2,364 exact code bytes** and **220 matched data bytes**.
Slipping is **99.63793%**, collision binding is **99.55385%**, and damage handling is **96.304344%**.

Aggregate exact code is **1,384,464 / 3,603,748 bytes (38.417336%)**, with **8,219 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The boss main object remains linked from the original binary.

### Validation and remaining work

Captured `ninja baseline` at `3afc3252`, rebuilt all affected header consumers, and ran `ninja changes_all`.
The comparison of all reported functions, including missing-function detection, found zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

The main unit's map check still fails with ten missing symbols: initialization, Mario throwing, main update/render handling, two UNUSED animation helpers, the local rotation-position helper, two compiler-emitted helpers, and the boss destructor/thunk.
All currently present strong functions have the correct order and linkage.
The manager destructor has a weak-order warning, and UNUSED `isCanWalk` is 164 bytes versus the map's 192.
Walking remains 76.784% because vector construction, squared-length evaluation, and inline context differ.
Stack/register differences remain in other reconstructed routines; no artificial padding or middleware edits were introduced.

Next work: reconstruct `CalcRevisionPosByRotateZ`, `init`, `throwMario_`, and the two animation helpers, then the 6,108-byte `perform` routine.
The [shared-fix catalog](../catalog/README.md) records the reusable vector, loop, and boolean patterns and the rejected trials.
See [batch 20 measurements](GMSE01-batch20.json).
The map inventory, full draft, instruction overview, and validation logs are under `build/GMSE01-boss-main-map-inventory-batch20.txt`, `build/GMSE01/BossHanachanMain-batch20.c`, and `build/GMSE01-*-batch20.*`.

## Verified checkpoint: batch 19

Reconstructed the missing US `MSound::getDistPowFromCamera(const Vec&)` using the original three `powf` calls and evaluation order.
All **136 bytes match exactly**.
Corrected sequence callback commands 40 and 123–126 to fall through to the base audio callback, and case 110 to return 0xFFFF for scene 8 in episodes 6 or 1.
The complete US switch routing now matches the original; the callback improved from 95.40247% to **98.75232%**.
Other regions retain their existing behavior through version guards.

This batch adds **136 exact code bytes and one matching function**.
Aggregate exact code is **1,382,100 / 3,603,748 bytes (38.351738%)**, with **8,205 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.

### Validation and remaining work

Captured `ninja baseline` at `19b7f21f`, rebuilt header consumers, ran `ninja changes_all`, and compared all reported functions including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
The sound unit's symbol-map check now passes: all 67 mapped functions, including nine UNUSED entries, are present.
It retains compiler-controlled weak-order warnings and seven existing UNUSED size warnings, including stubbed routines; the unit is not complete or source-linked.
No gameplay test was performed.

Remaining callback differences concern register allocation, a child-track pointer move, and a 0x58 versus 0x88 stack frame.
Direct child-array access and splitting the local declaration/assignment did not resolve the mismatch and were reverted.
The camera-distance helper needs global `powf`; the existing `std::powf` wrappers delayed summation and added spills.
The neighboring animation-distance helper has evidence for different scheduling and was left unchanged.
The [shared-fix catalog](../catalog/README.md) records these patterns and exceptions.
Next work: continue the missing boss main-unit reconstruction, with the sound callback's inline context and UNUSED routines retained in the backlog.
See [batch 19 measurements](GMSE01-batch19.json).
Draft and validation logs are under `build/GMSE01/MSound-distance-batch19.c` and `build/GMSE01-*-batch19.*`.

## Verified checkpoint: batch 18

Corrected the shared US `MSound` layout using the original constructor, water-filter routine, timer routine, and camera/boss callers.
The game-side class now declares the water-filter override byte at 0x98 and timer sequence parameter at 0x9A.
This places the tempo controller at 0x9C, crossfade controller at 0xA0, and later sound fields at their observed US offsets.
Restored the filter override check and its constructor initialization order, and updated all seven timer writes plus the sequence callback read together.
Other regions retain their previous layout and accesses through version guards.

**Forty-one functions improved across seven units; fifteen now match exactly, adding 2,060 exact code bytes.**
The exact gains include camera/player sound setup, stage sound entry, sound gating, reset, water filtering, and both inner-camera demo setters.
Aggregate exact code is **1,381,964 / 3,603,748 bytes (38.347965%)**, with **8,204 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.

### Validation and remaining work

Captured `ninja baseline` at `399978d6`, rebuilt affected consumers, ran `ninja changes_all`, and compared all reported functions including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
The boss nerve unit passes its symbol-map check.
The sound unit retains its baseline map failure: missing `getDistPowFromCamera`, weak-symbol ordering warnings, and seven UNUSED size warnings.
Its complete map-check output is unchanged from the baseline.
No source-link promotion or gameplay test was performed.

The constructor still lacks the previous-voice-ID word initialization at 0x94; properly recovering that field requires resolving ownership of the existing `JAIBasic` tail, which is now in scope after the 2026-09-15 autonomy change.
`playTimer` is 99.881355% and boss Snort is 99.9186%; their field offsets now agree with the original, leaving stack-layout differences.
The sound callback also has remaining US behavior differences in cases 40 and 110, recorded in the catalog for the next focused pass.
Next work: reconstruct the missing game-side sound distance helper and callback differences, then continue the boss main-unit reconstruction and remaining nerve stack differences.
See [batch 18 measurements](GMSE01-batch18.json) and the [shared-fix catalog](../catalog/README.md).
Validation logs are `build/GMSE01-*-batch18.*`.

## Verified checkpoint: batch 17

Reconstructed all seven boss behavior states in the previously empty `BossHanachanNerve.cpp`: graph wandering, tumbling, down, get-up, damage, snort, and death.
The existing nerve macro supplies the singleton accessors; all 22 mapped functions, including destructors and initialization, are present in the correct order and linkage.
Twenty functions match exactly, along with all 444 bytes of data sections.

This batch adds **3,156 exact code bytes, twenty matching functions, and 444 matched data bytes**.
Aggregate exact code is **1,379,904 / 3,603,748 bytes (38.290802%)**, with **8,189 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The nerve object remains original-linked pending its two nonmatching functions.

### Validation and remaining work

Captured `ninja baseline` at `e3d8ae6b`, ran the full build and `ninja changes_all`, and compared every reported function with missing-function detection: zero regressions.
The nerve unit passes symbol presence, order, and linkage checks with no map warnings.
Expected SHA-1 and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

`TNerveBossHanachanTumble::execute` is 99.901405%, differing only in its 0x38 versus 0x40 stack frame.
`TNerveBossHanachanSnort::execute` is 99.89535%, with a 0x28 versus 0x40 frame and a shared sound-layout discrepancy.
The original snort routine and `MSound` constructor agree that the tempo controller is at 0x9C; the current header's `MSModBgm* unk98` is at 0x98.
The constructor also writes a byte at 0x98 and a word at 0x94, which must be considered during a dedicated shared-layout audit.
No raw-offset cast, arbitrary padding, or protected middleware change was introduced to hide that discrepancy.

The [shared-fix catalog](../catalog/README.md) records the transition rules, exact helper pattern, sound evidence, and reverted stack-layout trials.
Next work: audit the US sound layout and its game-code consumers, resolve the two nerve differences, and continue the boss main-unit reconstruction.
See [batch 17 measurements](GMSE01-batch17.json).
Draft and trial source are under `build/GMSE01/BossHanachanNerve-batch17*`; validation logs are `build/GMSE01-*-batch17.*`.

## Verified checkpoint: batch 16

Reconstructed the previously empty `BossHanachanEffect.cpp`: particle loading, state-dependent particle emission, sand-pillar animation and sound, and camera shake/rumble.
All four routines and compiler-generated initialization are present in the correct map order and linkage.
Particle loading (820 bytes) and static initialization (764 bytes) match exactly, along with all 844 bytes of the unit's data sections.

Recovered the body's two-element leg-matrix array and the owner's sand-pillar model pointer and position vector from original field accesses.
Moved the foot-hit actor declaration into the shared boss header because both parts and effects use its joint matrix.
The two body-constructor matrix lookups were updated together and remain exact.

This batch adds **1,584 exact code bytes, two matching functions, and 844 matched data bytes**.
Aggregate exact code is **1,376,748 / 3,603,748 bytes (38.203228%)**, with **8,169 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The effects object remains original-linked pending three nonmatching routines.

### Validation and remaining work

Used the saved `ninja baseline` at `d78586d8`, rebuilt affected header consumers, ran `ninja changes_all`, and compared all reported functions with missing-function detection: zero regressions.
Effects and parts pass symbol presence, order, and linkage checks.
Parts retains its previously recorded UNUSED hit-predicate size warning (208 versus 196 bytes).
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

`emitOneTimeSandPillar_` is 99.93507% (only an eight-byte stack-size difference).
`emitCamShake_` is 99.70303% (stack layout and two loop-counter registers).
`emitParticle_` is 98.038315% (stack/register allocation and a water-counter sign-extension comparison).
The existing `MsSqrtf` and `MsRandF` helpers reproduce the original arithmetic and probability-load order; direct random arithmetic did not.
Remaining differences and reverted trials are recorded in the [shared-fix catalog](../catalog/README.md).
Next work: resolve effects inline contexts where evidence supports them, then continue boss main/nerve reconstruction and the pending sub-unit collision helper.
See [batch 16 measurements](GMSE01-batch16.json).
The draft and trial backups are under `build/GMSE01/BossHanachanEffect-batch16*`; build, regression, and map logs are `build/GMSE01-*-batch16.*`.

## Verified checkpoint: batch 15

Reconstructed the previously empty `BossHanachanSub.cpp`: sphere-chain initialization and movement, wall/ground collision, rotation-dependent position corrections, centrifugal force, and water-hit handling.
Recovered `TSpherePoint` (0x2C bytes) and `TSphereLink` (0x1C bytes), with declarations in `BossHanachanSub.hpp`.
Corrected the owner's offset-0x178 field to a sphere-link pointer using its original call sites.
All seven retained strong functions and the map's UNUSED collision helper have bodies.

Five linked functions match exactly: the water-actor destructor (132 bytes), its thunk (8), the sphere-point constructor (4), water-message handling (112), and counter reset (12).
All 232 bytes of the unit's data sections match.
This batch adds **268 exact code bytes, five matching functions, and 232 matched data bytes**.
Aggregate exact code is **1,375,164 / 3,603,748 bytes (38.15927%)**, with **8,167 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The sub-unit remains original-linked pending its remaining differences.

### Validation and remaining work

Captured `ninja baseline` at `6e2992e9`, rebuilt affected header consumers, ran `ninja changes_all`, and compared all reported functions with missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

**The sub-unit's symbol-map check still fails one presence check:** `TBGCheckData::isIllegalData()` is defined in the existing header but is inlined where the original calls and emits it.
Function order, linkage, and the UNUSED collision helper's 188-byte size pass.
Do not promote this object while that emission issue or its five nonmatching routines remain.
Those routines are `setDegreeZAndRevisionPosXZ` (98.19259%), `moveHead` (85.69796%), the sphere-link constructor (99.53333%), `BHSCalcRevisionDistXZByRotateZ` (94.02857%), and `BHSCalcCentrifugalForce` (99.023254%).

The catalog records recovered field semantics, angle rounding versus truncation, signed angle-difference behavior, matching helper patterns, and reverted trials.
Next work: resolve the sub-unit's collision/vector inline context and rotation-helper stack layouts, then continue the boss main/nerve reconstruction and pending source-link investigations.
See [batch 15 measurements](GMSE01-batch15.json) and the [shared-fix catalog](../catalog/README.md).
The `m2c` draft is `build/GMSE01/BossHanachanSub-batch15.c`; build, regression, and map logs are `build/GMSE01-*-batch15.log`.

## Verified checkpoint: batch 14

Reconstructed the last two TODO routines in `BossHanachanParts.cpp`: head/body map-collision setup and the two foot collision actors.
The code recovers the original actor names, joint names, collision dimensions, actor-group registration, collision flags, matrix bindings, and initial positions.
`initMapCollisionAndHitActor_` is **99.76471%** (544 bytes); only stack offsets differ.
`initFootHitActor_` is **99.55238%** (420 bytes), with stack offsets and two register assignments still different.
The foot destructor (156 bytes) and its pointer-adjustment thunk (8 bytes) now match exactly.
All mapped functions in the parts unit, including UNUSED functions, are present for the first time.

Audited the water-hit payload across the boss and particle users.
The binary uses offset 0x68 as a signed 16-bit countdown in boss receivers and a 32-bit particle index in senders.
The shared class now represents those views with a union, and all existing particle-index uses were renamed together.
The named constructor initializes the receiver counter; the default constructor preserves the static sender's original lack of a payload store.
This is an evidence-based representation of the binary's accesses, not proof of the original header's exact spelling.

This batch adds **164 exact code bytes, two matching functions, and 800 matched data bytes**.
Aggregate exact code is **1,374,896 / 3,603,748 bytes (38.151836%)**, with **8,162 / 12,904 functions** matching.
Matched data is 300,595 bytes.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The parts unit remains original-linked until all remaining instruction/data differences are resolved.

### Validation and remaining work

Captured `ninja baseline` at `918bf10e`, rebuilt the shared header's consumers, ran `ninja changes_all`, and compared all reported functions, including missing-function detection: zero regressions.
Existing particle-manager and enemy function scores are unchanged.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

The parts unit passes map presence, order, and linkage checks; its only map warning is the existing 208-versus-196-byte UNUSED hit predicate.
The new strings and joint-name array also complete the unit's `.rodata`, `.sdata`, and `.sdata2` section matches.
The `.data` section still differs.

Next work: reconstruct `BossHanachanSub.cpp` using the audited counter field, and resolve the parts/owner animation stack and register differences before source-link promotion.
The sub-unit map inventory and both initialization drafts were inspected; the draft files remain in `build/GMSE01/BossHanachan{Foot,Collision}-batch14.c`.
See [batch 14 measurements](GMSE01-batch14.json) and the [shared-fix catalog](../catalog/README.md).
Build, change, and map logs are saved under `build/GMSE01-*-batch14.log`.

## Verified checkpoint: batch 13

Matched `isAllBckAlreadyEnd` (184 bytes) and `setTumbleAnm` (420 bytes) exactly.
The completion check compares each compound condition explicitly with `false`, preserving the original boolean normalization without the extra stack slots of named boolean locals.
The tumble helper reuses one float and expresses absolute value directly with a ternary; both inline sites now match the original 0x60-byte stack frame and instruction sequence.

This adds **604 exact code bytes and two matching functions**.
Aggregate exact code is **1,374,732 / 3,603,748 bytes (38.147285%)**, with **8,160 / 12,904 functions** matching.
Matched data remains 299,795 bytes.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
Eight of the animation unit's ten linked routines now match; the unit remains original-linked pending the other two.

### Validation and remaining work

Used the saved `ninja baseline` at `75133a1f`, ran `ninja changes_all`, and compared every reported function, including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.
The animation unit passes map presence, order, and linkage checks.
The UNUSED tumble helper is 180 bytes versus the map's 176; its two retained inline sites are exact, but the standalone size remains unresolved.

The analogous boolean-expression change in the parts dispatcher regressed inlining and was reverted.
Frame-conversion trials in `setHeadAndBodyAnm` did not establish a match and were reverted too.
The shared-fix catalog records these exceptions to prevent repeating them or applying the successful forms blindly elsewhere.

Remaining owner routines are `changeAnmRateAndFrameUpdate_` (99.89781%, stack/register differences) and `setHeadAndBodyAnm` (91.75%, conversion scheduling/register differences).
Resolve these before testing source-link promotion of the animation unit.
The existing collision/foot initialization and water-hit field discrepancy remain outstanding in the parts/sub units.
See [batch 13 measurements](GMSE01-batch13.json) and the [shared-fix catalog](../catalog/README.md).
Logs and trial backups are saved under `build/GMSE01` and `build/GMSE01-*-batch13.log`.

## Verified checkpoint: batch 12

Reconstructed `BossHanachanAnm.cpp`: all ten linked routines and all four UNUSED helpers from the map.
The routines coordinate walk/run blending, frame preservation, directional tumble rates, animation completion, and staggered head/body transitions.
The recovered parameter fields are reused throughout.

Six routines now match exactly: `considerSetAnm`, `isFinishedGetUp`, and the four `setAnmTimerWhen*` methods.
They add **1,108 exact code bytes** and six matching functions.
All 32 bytes of the unit's mapped constants match.
Four routines remain nonmatching: `changeAnmRateAndFrameUpdate_` (99.89781%), `isAllBckAlreadyEnd` (99.891304%), `setTumbleAnm` (99.85714%), and `setHeadAndBodyAnm` (91.75%).
Their remaining differences are documented in the shared-fix catalog.

The owner's `setHeadAndBodyAnm` caller tests the low byte of the virtual setter's result, providing new evidence that `TBossHanachanPartsBase::setAnm_` returns `bool`.
Corrected the base declaration and both head/body overrides together, including their result locals.
Existing parts function match scores did not regress.

Aggregate exact code is **1,374,128 / 3,603,748 bytes (38.130524%)**, with **8,158 / 12,904 functions** matching.
Matched data is 299,795 bytes.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The animation and parts units remain linked from the original executable until their remaining differences are resolved.

### Validation and remaining work

Captured `ninja baseline` at `50f5cffc`, rebuilt affected consumers, ran `ninja changes_all`, and compared all functions, including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass for the mixed source/original-object executable.
No gameplay test was performed.

The animation unit passes map presence, order, and linkage checks.
Its three forwarding/blend helpers have the recorded 100-, 112-, and 116-byte sizes; `setTumbleBckRate_` is still 172 rather than 176 bytes.
The parameter and sound objects pass all map checks.
The parts unit retains the missing foot destructor/thunk and its hit-predicate UNUSED-size warning.

Next work: resolve the owner animation routines' remaining stack/register differences and the tumble helper's size, then test source-linking the whole animation unit.
The frame-update routine's short stack suggests missing inline context; the map-visible wrappers are present, but their current direct blend-field accesses may omit a nested helper boundary.
Do not add artificial stack padding.
Collision/foot initialization and the water-hit field discrepancy remain outstanding in the parts/sub units.
See [batch 12 measurements](GMSE01-batch12.json) and the [shared-fix catalog](../catalog/README.md).
Logs and the full `m2c` draft are saved under `build/GMSE01` and `build/GMSE01-*-batch12*.log`.

## Verified checkpoint: batch 11

Reconstructed all six branches of the boss-part animation dispatcher, including Mario standing on a body segment, animation completion, directional get-up sequences, and delayed damage/snort/death transitions.
`considerSetAnm_` improved from a stub (0.21691974%) to **99.6833%**, reproducing the original 461-instruction length (1,844 bytes).
Remaining differences are the stack frame (0xD0 versus 0x110), register allocation, and one constant-load/move choice in the blending check.
The existing animation-completion and motion-blending helpers are reused.

Reconstructed `isMarioOn_` from the original dispatcher inline site and corrected its result to `bool`.
Its inline instructions match and its emitted UNUSED body now has the mapped 100-byte size.
Added the missing enum values; descriptive animation-state names remain provisional.

This batch adds no fully exact linked functions or source-linked objects.
Aggregate exact code remains **1,373,020 / 3,603,748 bytes (38.099777%)**, with **8,152 / 12,904 functions** matching.
There are still **73 source-linked objects**, covering **76,468 code bytes (2.121902%)**.
The unfinished parts object remains linked from the original executable.

### Validation and remaining work

Captured `ninja baseline` at `90cba87f`, rebuilt affected consumers, ran `ninja changes_all`, and compared all functions, including missing-function detection: zero regressions.
The full build, expected SHA-1, and direct byte comparison pass.
These executable checks cover the mixed source/original-object build; the dispatcher is not yet used by that executable.
The parameter and sound objects pass all map checks.
The parts object's order and linkage pass; the foot destructor/thunk are still missing, and the hit predicate has the sole remaining UNUSED-size warning (208 versus 196 bytes).
No gameplay test was performed.

Shared accessor/local-variable trials did not resolve the stack differences and were reverted.
Their outcomes and the successful blending-expression form are recorded in the [shared-fix catalog](../catalog/README.md).
Next work: reconstruct collision/foot initialization after resolving the water-hit field discrepancy, then continue exact matching and the owner's animation routines.
The owner animation unit's complete map inventory is at `marioUS.MAP:59489`; it remains empty and has linked and UNUSED wrapper functions suitable for grouped reconstruction.
See [batch 11 measurements](GMSE01-batch11.json).
Logs and the dispatcher draft are saved under `build/GMSE01` and `build/GMSE01-*-batch11*.log`.

## Verified checkpoint: batch 10

Completed `BossHanachanSave.cpp` and linked it from source after verifying the full executable.
Both parameter constructors match exactly: 1,608 bytes for the changeable parameters and 1,408 bytes for common parameters.
Recovered all 63 parameter names, defaults, types, and field offsets from native strings, constructor stores, and parameter virtual-table references.
Reused the existing `System/DummyStrings.hpp` for the leading shared literals; this fixed the remaining differences in both constructors together.

Completed the previously missing base-part constructor logic using the recovered parameters and existing model/motion helpers.
It now reproduces the original instruction sequence apart from an eight-byte stack-frame difference (99.90598%).
Correcting the shared joint-matrix helper to hold a `u16` index made both head/body constructors exact (192 and 260 bytes) and restored the helper's mapped 76-byte size.
The existing `CLBPalFrame<short>` template is now emitted and matches all 92 bytes.

This batch adds **3,560 exact code bytes and five matching functions/helpers**.
Aggregate exact code is **1,373,020 / 3,603,748 bytes (38.099777%)**, with **8,152 / 12,904 functions** matching.
Matched data is 299,763 bytes.
There are now **73 source-linked objects**, covering **76,468 code bytes (2.121902%)** and 51,024 data bytes.
The unfinished parts object remains linked from the original executable.

### Validation and remaining work

Captured `ninja baseline` at `58f3e2d0`, ran `ninja changes_all`, and compared all functions, including missing-function detection: zero regressions.
The complete executable passes its expected SHA-1 and byte comparison after linking the parameter object from source.
The parameter and sound objects pass map presence, order, and linkage checks.
The parts object passes order and linkage but still lacks the emitted foot destructor/thunk; its two remaining UNUSED-size warnings concern the hit predicate and `isMarioOn_`.
No gameplay test was performed.

Remaining parts work includes collision/foot initialization, the animation-state dispatcher, `isMarioOn_`, and exact matching of the partial routines.
The water-hit counter/particle-index layout discrepancy recorded in batch 9 must be resolved before adding a shared water-hit constructor.
The base constructor now contains model and blend setup, but the parts unit is not yet a runnable replacement as a whole.
See [batch 10 measurements](GMSE01-batch10.json) and the [shared-fix catalog](../catalog/README.md).
Logs and drafts are saved under `build/GMSE01` and `build/GMSE01-*-batch10*.log`.

## Verified checkpoint: batch 9

Reconstructed the boss head/body animation setters and hit-message handlers, plus tumble-rate adjustment, circular-shadow submission, and damage fog.
The shared hit-reaction predicate is reconstructed once and used by both message handlers.
Added the main boss's mapped declarations and field layout needed by these parts, including its eight body pointers, head pointer, weak-body index, and parameter pointers.
Owner fields whose meanings are not established retain provisional names.

The tumble-rate function now matches all **188 bytes** exactly.
The other six routines remain nonmatching: shadow submission 99.54%, head animation setter 97.746475%, body animation setter 91.818184%, head hit handler 96.02941%, body hit handler 96.78832%, and damage fog 93.683334%.
Their remaining differences include state-lookup inlining, registers, and stack layout.
Both hit handlers use the shared overturned-state test; the body handler also follows the original six-case hip-drop eligibility table.

Corrected `TWaterHitActor::onWaterHitCounter` to an out-of-line declaration, consistent with its global symbol in `BossHanachanSub.cpp` and the head caller's original call instruction.
The former inline wrote a 32-bit value, while the original function stores a 16-bit counter.
The underlying field declaration still needs a coordinated audit with particle-manager consumers; it was not globally narrowed.
See the [shared-fix catalog](../catalog/README.md) for the evidence and pending work.

Aggregate exact code is **1,369,460 / 3,603,748 bytes (38.00099%)**, with **8,147 / 12,904 functions** matching.
Matched data remains 298,283 bytes.
There are still **72 source-linked objects**, covering **73,452 code bytes (2.0382113%)**.
The parts object remains incomplete and is not source-linked.

### Validation and remaining work

Used the saved batch 9 baseline at `f7eb2cf2`; rebuilt all affected units and ran `ninja changes_all`.
Compared every reported function against the baseline, including missing-function detection: zero regressions.
The full build, expected SHA-1, and byte comparison against the original US executable pass.
These executable checks validate the current mixed source/original-object build, not the unfinished parts implementations.
No gameplay test was performed.

The parts object's ordering and linkage pass; its map-presence check still fails on the foot destructor/thunk and `CLBPalFrame<short>`.
Three UNUSED-size warnings remain: the hit predicate (208 versus 196 bytes), `isMarioOn_` (still a stub), and the joint-matrix helper.
The sound object's complete map check passes.
Remaining parts work includes the constructor, collision/foot initialization, animation-state dispatcher, `isMarioOn_`, and exact matching of the partial routines.
The parameter-constructor draft is saved locally for the next layout reconstruction.

See [batch 9 measurements](GMSE01-batch9.json).
Build and change logs are `build/GMSE01-build-batch9.log` and `build/GMSE01-changes-batch9.log`.

## Verified checkpoint: batch 8

Started reconstructing `BossHanachanParts.cpp`, with complete mapped base/head/body method declarations and explicit TODO bodies for unfinished behavior.
Identified the shared motion controller as `TNpcInbetween` from its constructor layout and calls in `BossHanachanMain`.
Reconstructed ground-actor lookup, moving collision translation, getting-up rotation, frame transfer, animation-end checks, and three UNUSED animation helpers.
The first three routines match exactly; frame transfer and animation-end checks remain different only in stack offsets.
The three UNUSED helpers reproduce their original 48-, 12-, and 16-byte sizes.

The derived virtual tables exposed a missing pure virtual animation setter in the base class.
Its null slot was incorrectly called padding in batch 7.
The corrected base table now matches all 256 bytes, and the previously source-linked sound object still reproduces the executable exactly.

This batch adds **1,496 exactly matching bytes and nine functions/helpers**, including three destructors, two adjustment thunks, and static initialization.
Aggregate exact code is now **1,369,272 / 3,603,748 bytes (37.995777%)**, with **8,146 / 12,904 functions** matching.
Matched data is 298,283 bytes.
There are still **72 source-linked objects**; the unfinished parts object is not promoted.

### Validation and remaining work

Captured the batch 8 baseline at `122eab40` before edits; ran `ninja changes_all` and compared every reported function with that baseline.
There are zero function regressions.
The full build, SHA-1 check, and byte comparison against the original US executable pass.
No gameplay test was performed.
The parts object's ordering and linkage pass map checks, but its overall map check fails because the foot destructor/thunk and `CLBPalFrame<short>` are not emitted yet.
There are three UNUSED-size warnings, including two TODO bodies and the joint-matrix helper.
The sound object's map check passes.

The main boss class and parameter declarations still need a full-map reconstruction before completing the parts constructor, collision setup, damage effects, and animation/message behavior that depends on them.
The partially reconstructed constructors are not runnable replacements for the originals yet.
Do not promote this object based on its matching helper functions.

Added a durable [shared-fix catalog](../catalog/README.md) with successful cases, exceptions, search terms, and unresolved cross-file work.
For example, the boss frame-transfer routine needs the current animation accessor, while the already-exact NPC blend routine needs the previous-animation accessor; a mass replacement would be incorrect.
See [batch 8 measurements](GMSE01-batch8.json).
Drafts and validation logs remain under `build/GMSE01` and `build/GMSE01-*-batch8.log`.

## Verified checkpoint: batch 7

Reconstructed `Enemy/BossHanachanSound.cpp` from the original executable's 38-entry animation sound table.
The table getter and destructor adjustment thunk now match exactly, and the entire object is linked from source.
Added every mapped `TBossHanachanPartsBase` method declaration, including UNUSED methods, with inheritance checked against its constructor and virtual table.
Constructor and animation-setting instructions establish the current/previous animation fields; collision initialization identifies the water hit actor, moving collision, and joint matrix pointers.
The animation enums remain incomplete, and the motion-controller pointer remains opaque.

There are now **72 source-linked objects**, covering **73,452 / 3,603,748 code bytes (2.0382113%)**.
This batch adds **20 exactly matched code bytes, two matching functions/helpers, and 1,440 source-linked data bytes**.
Aggregate exact code is **1,367,776 bytes (37.95426%)**, with **8,137 matching functions**.
Aggregate matched data increased by 1,032 bytes to 297,687 bytes.

### Validation

Captured `ninja baseline` at `6718c96a`, rebuilt, and ran `ninja changes_all`.
Every reported function was compared against the baseline: zero regressions.
The sound object's map presence, ordering, and linkage checks pass.
The final source-linked executable passes both the expected SHA-1 and a full byte comparison against the US original.
No gameplay test was performed.

At this checkpoint, four bytes after the reconstructed 252-byte table were interpreted as alignment padding.
Batch 8 disproved that interpretation: the final slot belongs to a pure virtual animation setter.
The batch 7 executable matched because padding happened to produce the same null bytes; the class declaration was incomplete.

### Remaining work

`BossHanachanParts.cpp` is still empty and nonmatching.
Its base-class declarations now support reconstructing animation selection and collision behavior, but the head/body subclasses must also be fully scaffolded from the map before implementing that unit.
The new sound-table object does not imply that the boss itself is decompiled.
See [batch 7 measurements](GMSE01-batch7.json).
Build, change, and map-check logs remain under `build/GMSE01-*-batch7.log` and `build/GMSE01/BossHanachanSound-symbol-order-batch7.log`.

## Verified checkpoint: batch 6

Corrected the US crash-reporting filename to `/marioUS.MAP` and its read-only data placement in `System/marerr.cpp`.
Both functions and both data symbols now match in the detailed object comparison, and the object is linked from source.
There are now **71 source-linked objects**, covering **73,432 / 3,603,748 code bytes (2.0376563%)**.
This adds 284 source-linked code bytes and 40 matched data bytes.
The aggregate report already counted the two functions as matched before the data/relocation correction, so aggregate exact code remains **1,367,756 bytes (37.95371%)**, with **8,135 matching functions**.

### Validation

Captured `ninja baseline` at `57f560d3` and ran `ninja changes_all` after the final changes.
Comparing every reported function found zero regressions.
The crash-reporting object's map presence, ordering, and linkage checks pass.
The full build, expected SHA-1, and byte comparison against the original US executable pass.
This remains a mixed source/extracted-object build; no gameplay test was performed.

### Data-layout investigation

The native light-manager name starts at `0x803A8DC0`, eight bytes before its address in the imported map.
Several preceding movie strings are also eight bytes before their mapped addresses.
A limited object-boundary correction made the light-manager object compare exactly, but left the legacy linker running for over five minutes with an empty output ELF.
That experiment was stopped and reverted, and the restored configuration passed the executable checks.
Audit the surrounding string table and relocations together before attempting another source-link promotion of `MarDirectorCreateObjects.cpp`.
Matching trials in `MarioAccess.cpp` and `MapObjPollution.cpp` were also reverted; the egg-generator control routine still differs in stack layout.

See [batch 6 measurements](GMSE01-batch6.json).
Build and change logs remain under `build/GMSE01-*-batch6.log`.

## Verified checkpoint: batch 5

Reconstructed the missing bathtub grip family in `MoveBG/MapObjCorona.cpp` and its header.
This includes the grip, collision-part base, hard parts, fragile parts, and all 26 bathtub parameters.
Class inheritance and fields follow constructor stores and virtual tables; descriptive field names are inferred from their uses.
All four new grip-family virtual tables match exactly.
Corrected `TBathtub::getNumGripsDead` from `u8` to `int`, eliminating the wrong narrowing operation in the Bullet Bill item-drop caller.

This batch adds **1,376 exactly matched bytes and 20 matching functions/helpers**.
These include the grip counter, animation update, collision removal, message forwarding, matrix getters, four destructors, four adjustment thunks, and an emitted existing nozzle getter.
Exactly matched code is now **1,367,756 / 3,603,748 bytes (37.95371%)**, with **8,135 / 12,904 functions** matching.
Matched data increased by 1,476 bytes to 296,615 bytes.
There are still **70 source-linked objects**; neither bathtub unit is complete or newly source-linked.

### Validation

Captured `ninja baseline` at `218d1312`, rebuilt the changed units, and ran `ninja changes_all`.
Every reported function was compared against the baseline: **zero regressions**.
The full executable build, expected SHA-1, and byte comparison against the local US executable all pass.
This checks the mixed source/extracted-object build, not completion of the new decompilation bodies.
No gameplay test was performed.

The bathtub file's function ordering and linkage now pass map validation.
The overall map check still fails because two expected template symbols are not emitted: `SMatrix33R<float>`'s constructor and `TVec3<float>::set<float>`.
There are also 17 UNUSED-size warnings.
The Bullet Bill unit's map check passes with its existing warnings.
No library implementation was changed.

### Remaining work and next checkpoint

| Routine | Comparison | Remaining issue |
| --- | ---: | --- |
| Grip control | 99.951965% | Stack frame differs by 16 bytes |
| Bullet Bill item drops | 99.926384% | Temporary vectors are four bytes low on the stack |
| Bathtub parameter constructor | 99.84395% | String offsets depend on unfinished TU data |
| Grip-part matrix lookup | 99.72222% | Stack frame differs by 16 bytes |
| Grip constructor | 99.36323% | Stack layout, model-flag registers, and string offsets |
| Bathtub constructor | 86.833336% | Missing nested matrix constructor call |
| Tumble eligibility | 82.83871% | Local-coordinate helper and template inlining |
| Grip messages | 78.30846% | Stubbed quake, hipdrop, and demo callees; parameter loads |
| Nearest-grip query | 70.982605% | Vector temporaries and `std::fmodf` inlining |

These are partial comparison scores, not exact matches.
The standalone reset, collision removal/setup, part constructors, and `startBreak` helpers have the original UNUSED sizes.
`startCrack`, `isCracking`, and `trample` still differ in size.
Other bathtub behavior and several direction/demo helpers remain TODO stubs.

The original bathtub constructor calls `SMatrix33R<float>` at the shared data matrix at `0x188`.
The current `TBathtubData` declaration uses a column-major matrix.
A trial correction exposed transposed-access assumptions in existing water routines and was reverted; update that type and its consumers together after checking each caller's assembly.
`allowsTumble` and `TBathWaterManager::throwMario` share the same original local-coordinate conversion sequence, which is a useful next reconstruction target.
The quake and hipdrop drafts reference `TKoopa`, whose class scaffolding is also missing; scan its full map before introducing declarations.

See [batch 5 measurements](GMSE01-batch5.json).
The m2c drafts, baseline/build/change logs, and both map validation logs remain under `build/GMSE01` or `build/GMSE01-*-batch5.log`.

## Verified checkpoint: batch 4

Reconstructed five more previously stubbed routines in `Enemy/BathtubKiller.cpp`, together with its mushroom-spawning and break helpers.
Recovered manager fields at `0x60`, `0x64`, `0x68`, and `0x69` from the load and item-drop instructions.
Their descriptive names are inferred from the lives comparisons and mushroom-spawning behavior.
The compiled `generateMushroom` helper now has the original map's 100-byte size.

| Routine | Previous comparison | Current comparison |
| --- | ---: | ---: |
| Model and material setup | 1.09% | 88.07% |
| Item drops | 0.61% | 98.94% |
| Water reaction | 1.43% | 54.46% |
| Break state | 2.75% | 99.94% |
| Manager load-after | 1.41% | 84.49% |

These percentages describe partial instruction comparisons, not completed functions.
Exactly matched code remains 1,366,380 bytes (37.915524%); none of these five routines is newly exact.
Matched data increased by 808 bytes to 295,139 bytes.
The build still links 70 source objects, and `BathtubKiller.cpp` remains nonmatching.

### Verification and remaining issues

Captured a fresh `ninja baseline` at `1ccbf133`, rebuilt, and ran `ninja changes_all`.
Comparing every reported function found zero regressions.
Symbol validation passes with weak-symbol ordering warnings and 16 remaining UNUSED size mismatches.
The final executable passed SHA-1 verification and a full byte comparison against the local US executable.
No gameplay test was performed.

The item-drop routine still differs in temporary-vector stack offsets and the declared return type of `TBathtub::getNumGripsDead`.
Its caller compares a signed integer in the original, while the existing declaration returns `u8` and adds a narrowing operation.
The grip class currently has only a forward declaration, so reconstructing the grip-count body requires further class work.
The break state has the same four-byte temporary-vector offset discrepancy as the explosion state.
The water reaction still needs its original inlined state-check boundaries recovered.
Manager loading retains unexplained null comparisons in the original, and model setup needs further scheduling and register-allocation work.

See [batch 4 measurements](GMSE01-batch4.json).
The current logs are `build/GMSE01-build-batch4.log`, `build/GMSE01-changes-batch4.log`, and `build/GMSE01/BathtubKiller-symbol-order-batch4.log`.
The m2c drafts remain under `build/GMSE01/BathtubKiller-*-m2c.c`.

## Verified checkpoint: batch 3

Reconstructed the bathtub Bullet Bill initialization, reset, matrix update, gravity query, active-count loop, personality parameters, and explosion state from the local executable.
The original symbols already existed as stubs.
The five personality fields are now grouped in the existing `TBathtubKillerPersonality` class, whose three parameter-copy methods reproduce the inlined instructions and the map's 44-byte UNUSED sizes.
Field names describe their corresponding parameters; they are inferred names.

Five game functions now match exactly: `init`, `reset`, `calcRootMatrix`, `getGravityY`, and `countActiveKillers`.
The reconstructed explosion state also emits the previously missing 92-byte integer-vector helper with an exact match.
This batch adds 632 exactly matched bytes and six matched functions, bringing the total to 1,366,380 of 3,603,748 bytes (37.915524%) and 8,115 of 12,904 functions.
Cumulative gains over the initial regional baseline are 1,272 bytes and ten functions.

The reset helper is at 99.944954%, and the explosion state is at 99.95%; both still differ in stack layout.
The manager's load routine is at 81.03704%, with unexplained null comparisons and stack space still to reconstruct.
Other behavior in this unit remains stubbed, so `BathtubKiller.cpp` remains nonmatching and is not promoted into the source-link manifest.
There are still 70 source-linked objects.

### Validation

- Captured `ninja baseline` at `21fd1969` before changes, then rebuilt and ran `ninja changes_all`.
- Compared every reported function against that baseline: zero regressions.
- Checked individual instruction differences and validated map symbols, ordering, and linkage.
  The map check passes with warnings for weak-symbol order and 17 remaining UNUSED size differences.
- Rebuilt the executable and passed both the expected SHA-1 and a full byte comparison.

See [batch 3 measurements](GMSE01-batch3.json).
Drafts from m2c, the build log, the regression log, and the map validation log are preserved under `build/GMSE01` or as `build/GMSE01-*-batch3.log`.
The original disc image remains excluded from Git.
No gameplay test has been performed; the matching executable still uses extracted objects for unfinished units.

### US audio layout dependency

Several game-side audio routines access fields four bytes later in the US binary than in the current declarations.
Examples include `MSMainProc::toInnerCameraDemo` (target byte `0xCE`, current `0xCA`) and `MSound`'s camera array (target `0xB0`, current `0xAC`).
The constructor also writes additional fields at `0x94` and `0x98` and uses `0x9A` for a field currently at `0x94`.
This indicates a regional layout change around the end of the `JAIBasic` base class.
This was blocked pending supervised library review; as of 2026-09-15 it is in scope and is a good first target for the lifted library restriction.
No library source or speculative padding was added.

## Verified checkpoint: batch 2

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

See [batch 2 measurements](GMSE01-batch2.json).
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

Exact machine-readable measures are in [the initial snapshot](GMSE01-initial.json) and [the batch 1 snapshot](GMSE01-batch1.json).
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
- Metadata provenance and reproducible commands: [regional README](../../config/GMSE01/README.md).
- Work plan: [DECOMPILATION_PLAN.md](../../DECOMPILATION_PLAN.md).
- Initial game-code inventory: `docs/progress/GMSE01-backlog.json`.
- Full comparison reports: `build/GMSE01/baseline.json` and `report.json`.
- Build and regression logs: `build/GMSE01-build.log` and `build/GMSE01-changes-batch1.log`.
- Incremental promotion results and logs: `build/GMSE01/promotion-results.json` and `promotion-logs/`.
- `m2c`: commit `e07f7e1c1d1d52f33ab3f7fedbe74b634404b943`, installed in `build/venv`.

Build artifacts and original game data are kept locally and excluded from Git.
Source, configuration, progress snapshots, and notes are versioned locally.

## Next work

Follow [STRATEGY.md](../../STRATEGY.md) and the refreshed near-matching list.
Start the next time-box with `TCardLoad::waitForChoice()`; consult source TODOs before revisiting GCConsole2::load, BossEel, npcWetting, jumpMain, specMain, spider or beam.
Keep the small-file queue and old audits as supporting references; defer from-scratch work until the near-matching group is mostly exhausted.
