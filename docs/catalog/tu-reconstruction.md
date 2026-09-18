# Reconstructing a TU: layout, declarations and order

Do these before writing bodies in a from-scratch unit.
`docs/PROGRAM_STRUCTURE_REVVING.md` covers the general procedure; this file is what this clone learned on top of it.

## Virtual order from the vtable

Virtual calls compile to fixed offsets, so nerve bodies cannot match until the class declares its virtuals in vtable order.
Dump every slot:

```
awk '/\.obj "?__vt__9TTobiPuku"?/,/endobj/' build/GMSE01/asm/Enemy/tobiPuku.s \
  | awk 'NR>=2 && /4byte/{printf "0x%03x %s\n",(NR-2)*4,$2}'
```

That shows which methods are virtual, which override base slots, which are new, and the header order.
Read the **whole** dump: `TTobiPuku`'s base `TWalkerEnemy` table ends at 0x1b4 and eighteen new virtuals follow; a partial read once led to `setDeadAnm` being declared non-virtual when it overrides slot 0x154.
Reordering the header cost zero regressions and turned `TNerveTobiPukuSwimWander::execute` from unmatchable into exact.

**Null tail slots are pure virtuals, not padding.**
`TBossHanachanPartsBase`'s 256-byte table ends in a null at 0xFC; both derived tables put `setAnm_` there, so the base declares it pure virtual.
Padding preserved the executable bytes but hid the interface. Inspect derived tables before calling a null padding.

## Names and defaults from `.rodata`

`PARAM_INIT` stringifies the member name, so real param names sit in `.rodata` next to the `.prm` path, and defaults are the constants `load()` stores into each `TParamRT`.
Dump the strings from `build/GMSE01/asm/<path>.s` and read the stores; do not guess (five fabricated `tobiPuku` names were all wrong, three of five defaults too).
Integer params are `TParamRT<s32>` (mangles `TParamT<l>`); `TParamRT<int>` gives `TParamT<i>` and shows as a vtable-store operand mismatch.
Filenames matter too: `/Mario/DmgHamukuri.prm`, not `Hamakuri`, or the whole string pool shifts.
Wrong defaults cause register differences far from the store (TMario constructor: `TEParams` down type 1, motor 25, minimum speed 16.0f, invincibility 300).

## Literal numbering and dead-code data

- Aggregate-initialiser templates are emitted at parse time, dead code included: a `.rodata` `@NNNN` object can belong to a function the linker stripped, so a missing prefix in the section blob may only be recoverable by reconstructing (or stubbing) a dead body. `.rodata` is one blob per TU under `-str reuse,readonly`, so it must match even for UNUSED functions; `.data` entries are individually strippable and the map marks them UNUSED. Proved by `ShadowUtil`'s two `f32[4]` corner tables (`TModelShadow::draw`) and the unreferenced `J3DJoint.hpp` literals present in both objects.
- `@NNNN`/`$NNNN` numbers come from one counter read at two times: named local statics, aggregate templates and function-local classes take theirs at **parse** (monotonic in source line); string literals at **deferred codegen** (higher than every parse-time number). A function-local class's `$NNNN` is the one thing a smaller include set makes unmatchable: `ShadowUtil`'s six local classes are off by a constant 1277 with byte-identical bodies (4,116 bytes reported missing/extra on the name alone). Not fixable inside the unit.
- `.sdata2` interleaves each short `PARAM_INIT` name with the literal for its default, so a lone unexplained constant between two names names its parameter (`TWaterEmitInfo::mSize` = 17.0f).
- dtk can infer a false relocation inside a data blob (`tmp_data` scores 99.9% forever because four bytes at 0xc10 look like an address); check before hunting a data mismatch that does not exist.
- `System/StageUtil.hpp` emits its four static tables (400 bytes of `.data`) into every includer; declare the one function you need instead (second confirmation).

## Animation indices from the `.bas` table

A unit's `_bastable` names only some animation slots, but the model's `.bck` files are indexed in alphabetical order, so filling the gaps alphabetically recovers every name and every `setBckAnm`/`isBckAnm` constant (`amiNoko`: six named slots gave all sixteen, including the `end/loop/start` triples three apart that a flag selects between).

## Owner keys of `this + 1`, `this + 2`

An effect owner argument of `this + sizeof(T)` or `this + 2 * sizeof(T)` is the original's `this + 1` / `this + 2`, giving each looping emitter a distinct key; it also confirms the class size (`TAmiNoko::emitEffects`, 0x214).

## Factories are a complete oracle for class shape

Per `MarNameRefGen_*` branch: `li r3, N; bl __nw__` gives `sizeof`; `bl __ct__X` names the class whose constructor is **out of line**; a following `lis/addi __vt__Y; stw 0(obj)` (plus `addi r0, r3, 0x24; stw r0, 0x20(obj)` for a second vptr) proves Y's constructor was in-class and names Y; a derived constructor absent from the whole map confirms it. Intermediate vptr stores survive, so an abstract middle class is visible, and a member allocation stored *between* the base vptr store and the derived one proves the `new` lives in the base constructor's body. Script it: join the string pool to the `addi r4, r31, off` / `li r4, @NNNN@sda21` keys and the ladder falls out (22 in-class constructors recovered in `_Enemy`, 28 in `_NPC`; the measured 14-header patch made `_NPC` fully matching and linked).
A missing brace in a long `if` ladder is diagnosed from the data section: `.rodata` simply ends at the last reachable branch's string while `.text` still scores 90%. Compare section sizes before reading a diff.
Marker subclasses: a weak `__vt__X` + `__dt__XFv` in one TU, byte-identical to the base's vtable apart from the destructor slot, is a marker subclass with an in-class constructor and implicit destructor; declaring it in the `.cpp` reproduces both symbols (four for four).
`MarNameRefGen_BossEnemy` needs a third string-pool block: TU-local `static const char cDirtyFileName[]`/`cDirtyTexName[]` (0x34) between `DummyStrings` and `InfectiousStrings`; the map has that pair `(object,local)` in every Player TU plus a handful of others, so it comes from a still-unidentified shared Player header.
Sizes the factories prove wrong in current headers: `TKazekunManager` 0x64 (not 0x60), `TKazekun` 0x1d4 (not 0x1b4), `TLimitKoopa` 0x1c8 (not 0x1bc). `LimitKoopa.hpp`'s nine inline `theNerve()` bodies cost every includer 12 bytes of `.bss` each (`MarNameRefGen_BossEnemy`'s `__sinit` is 0x6c off); split them into a `LimitKoopaNerve.hpp`.

## MoveBG: names, sizes and depth from the factory

- For classes whose constructor is inlined everywhere (no `__ct__` in the map), `MarNameRefGen_MapObj.cpp`'s string pool pairs the lookup name with the Japanese default in adjacent `@NNNN` objects, and `li r3, 0xNNN; bl __nw__` at each site gives `sizeof` (every name and size in `MapObjMamma`/`MapObjMonte`, including copy-pasted defaults and classes that add no fields).
- Destructor size is an inheritance-depth oracle: a weak dtor for a direct `TMapObjBase` child is 0x84, a grandchild 0x9c (two extra vtable stores), and at three levels MWCC emits `bl __dt__11TMapObjBaseFv` while staying 0x9c (`TSandLeaf` sits under `TMapObjBase`, not `TSandLeafBase`).
- Switch case order is read off the jump table's `.rel` entries, not the compared constants (`TSandBombBase::control` 7.3 -> 80.3 from reordering alone).
- A weak 8-byte `getRadiusAtY` (`lfs f1, <const>; blr`) is an in-class `return 20.0f;`; defining it out of line drops the class's whole vtable from the object.
- `-inline deferred` emission is strict reverse **definition** order, UNUSED helpers included; they do not float to the end.
- objdiff can score a whole `.data` section 0 when our object also emits dead-stripped vtables the extracted target lacks (`MapObjMamma`: ours 0x15a8, the map's size; target `.o` 0x1360). Check the per-vtable diffs before believing a data percentage.

## Override return types

`TEnemyManager::createEnemyInstance()` returns `TSpineEnemy*`; an override declared with a different return type (`TLiveActor*`) is not covariant and gets a **new vtable slot** instead of overriding. A *covariant* return (`TBee*` for the base's `TRealoidActor*`) also gets a second slot with the same pointer. Declare the base return type; check any class whose vtable is not exact for this (`BeeHive` data 60.4 -> 100).

The map size is a shape oracle for dead helpers: `TBeeHive::doFall` at 0x24 is four instructions more than `mSwingAngle += mSwingSpeed;`, exactly a trailing `return !checkLiveFlag(...)`.

The MSound rogue-include set also supplies the 15 zero-filled 12-byte `.bss` objects enemy TUs carry, and the 12-byte `@NNNN` `.bss` objects paired with `instance$`/`init$` in `.sbss` are `__register_global_object` chain nodes, one per `theNerve()` singleton.

## String-pool prefixes

If every string offset in a unit is off by a constant, a shared header supplies the prefix.
- `System/DummyStrings.hpp`: 12 zero bytes plus the 20-byte Shift-JIS memory-error string (UNUSED `SMS_NO_MEMORY_MESSAGE`). Fixed `SelectDir` (+688 data bytes) and both boss save-param constructors (32 bytes before the first param name).
- `Map/MapCollisionManager.hpp` (via `MapCollisionEntry.hpp`'s `setUpTrans`) contributes a 12-byte zero vector and a 12-byte one vector to `.rodata` right after the `InfectiousStrings` block. A unit whose every string offset is 0x18 low is missing exactly this include (present in 24 Enemy TUs; `BathtubPeach` `@2852`/`@2854`, `fruitsboat` `@2333`/`@2335`).
- `M3DUtil/InfectiousStrings.hpp` has a 0xE0-byte prefix; it must precede `SunModel` in `MarNameRefGen_Map`.
- A header can also add unwanted data: `StageUtil.hpp` emits static shine/scenario tables even when only `SMS_isMultiPlayerMap` is used, shifting TMario's vtable by 0x168. Declare the one function directly instead.
- Trailing zero bytes in `.sdata2` can come from linker alignment; no dummy definition needed.

## `__sinit` in enemy units

`__sinit_effectEnemy_cpp`, `__sinit_seal_cpp`, the hauntLeg and BossHanachanNerve `__sinit`s are all 764 bytes and match with no code: add `MSound/MSSetSound.hpp` and `MSound/MSoundBGM.hpp` as rogue includes next to `M3DUtil/InfectiousStrings.hpp`.
The body is JAL sound-list registration from the include set, typically the largest function in the unit. Try it first.

## Nerve shapes from the map

- `nerve$localstatic0$theNerve__X` / `init$localstatic1$theNerve__X` (instead of `instance$NNNN` / `init$NNNN`) means `theNerve()` was an **inline** function written out in the header with a local static named `nerve`, not `DEFINE_NERVE` (all ten `limitkoopa` nerves).
- A nerve vtable of `{0, 0, dtor, 0}` with no `execute` is an abstract intermediate nerve; the derived nerves' destructors walking its vtable identify the children (`TNerveLimitKoopaWait`/`Tumble` under `TNerveLimitKoopaTurn`).
- Wrong global definition order can show as a **data-layout** mismatch rather than an order failure: in map order (not reversed) MWCC shared one `lis` between two vtables and four destructors stalled at 83.9%.
- `TParamRT<T>::set(const T&)` parks its literal in `.sdata`: standalone 4-byte `@NNNN` `.sdata` objects next to a params class are post-`load()` overrides, not statics.

## Reordering a TU mechanically

`validate-symbol-order.py` says the order is wrong but not what to write.
Take the `.fn` lines from `build/GMSE01/asm/<path>.s` in order (demangled name in the comment above each), reverse for `-inline deferred`, split the `.cpp` into top-level chunks keyed by `Class::method` or `DEFINE_NERVE` argument, and emit them in that order.
Assert the key sets match before writing.
Reordering does not fix `.rodata` offsets that depend on objects the original emits and we do not.

## Layout evidence

- **Zero stores prove extent and initialisation order, not vector boundaries.** Boss body history looked like three vectors from 0x120 but is scalar 0x120, previous position 0x124, older position 0x130, previous roll 0x13C, older roll 0x140; copy sites proved it.
- **Two access widths on one offset can be a union.** `TWaterHitActor` 0x68 is read as `lwz` particle index (`TSmallEnemy::decHpByWater`, `TBossManta::receiveMessage`) and as `lha`/`sth` signed countdown (boss). Current source: anonymous union `int mParticleIndex` / `s16 mWaterHitCounter`. The original declaration is not established; do not split into a field at 0x6A.
- **Reuse existing types** when constructor stores and a caller's method call match: boss parts 0x110 is `TNpcInbetween*` (44 bytes), not a private struct.
- **Move a class to a shared header only for real cross-unit use** (`TFootHitActor` into `BossHanachan.hpp` for offset 0x6C `mJointMtx`), not to force inlining.
- Resolved (`MapObjCorona` batch): `TBathtubData::unk18` is `TRotation3<TMatrix33<SMatrix33R<f32>>>` (row-major). Evidence: the map's weak 4-byte `SMatrix33R<f>` constructor for that TU, and only the row-major `ref()` reproduces `TMatrix33::identity()`'s store order in `TBathtub::load` (0x1a4, 0x1a0, 0x19c, 0x194, 0x190, 0x18c). The bathtub's three axes live in the matrix's rows, so `setXDir`/`getXDir`/`getYDir`/`getZDir` are what the callers read. Other consumers were kept byte-identical with index swaps (`getPos`, two `at(1,n)` -> `at(n,1)` sites in `BathWaterManager.cpp`); `throwMario`'s `mult33` is really the transposed multiply and is the one residue. Retail probably reaches the constructor one level deeper still (`TPosition3<...>`): open.

## UNUSED functions

- Size agreement with the map is necessary, not proof: the body was dead-stripped.
- UNUSED bodies go in the `.cpp`. Explicit `inline` on a cpp helper dropped its UNUSED definition.
- Moving a header-defined constructor to its map-ordered cpp position emits a missing UNUSED body without breaking inlined callers (`TMapCollisionBase`).
- Restore UNUSED helpers from repeated branches: `InitChangeOneColor_Base`/`TwoColor_Base` (48/80 bytes), `initObjArray(int)` (60), `loadSaveParams_` (128), `checkJumpingThrowStart` (92).
- A map pass with four-byte stub bodies is not completion.
- An UNUSED helper can also be genuinely dead, and the caller's codegen decides. `TBombHei::bombIn` is size-exact, but *calling* it from `behaveToRelease` pushes the inlined `theNerve()` constructor a level too deep and emits a `bl TNerveBase::TNerveBase()` the ROM lacks; spelling the push out and leaving the helper uncalled matches. `UNUSED` cannot distinguish "inlined everywhere" from "never called".
- Second instance: `TFruitsBoat::rowToCurPathNode` (0x17c) appears twice verbatim in the GraphWander nerve; calling it gives 41.7%, pasting it 99.9%, and the out-of-line copy is 0x154, so the dead body is not what the nerve pastes (same shape as `TYumbo::lookatMario`).
- `@NNNN` literal numbers are assigned at deferred code generation, not at parse: a params constructor defined first still gets the highest `@NNNN`, so literal order is not evidence against reverse emission order.
- An UNUSED helper can exist *for* the extra inline level. `TMapObjWave::movement` (UNUSED, 0xd8) must be called from `perform`: with its body pasted in, `updateTime` sits at depth 1 and expands (0%); behind `movement()` it is at depth 2 and stays a `bl` as in the ROM. The out-of-line `movement` copy inlines `updateTime`, which is what makes it 0xd8.

## Return types

Inspect all callers before changing one; identical mangled names do not establish return types.
`TBathtub::getNumGripsDead` returns `int` (not `u8`); the grip constructor's `u16` store is that caller's own conversion.
`TSmallEnemyParams::getSL{Attack,Damage}{Radius,Height}()` return `f32` (`TSmallEnemy::moveObject` 83.5 -> 96.8).
A nested enum constant such as `TSmallEnemy::LIVE_FLAG_MELT_ON_DEATH` must be qualified outside the class; grep before inventing a name.

## Structural batch 75 (2026-09-18): symbol-order oracles

- **The map position of a local/weak template instantiation names the first function, in emission order, that calls it out of line.** `AnimalBase.o` agrees with the map for `MsWrap<f>` (after `execWalk`); `Bird.o` disagrees (after `doLanding` instead of `execute__WalkOnGround`), and that one disagreement produces one MISSING and one ORDER error at once. An instantiation's address is a precise oracle for where a refused inline must be (caller-size family; three inlines flip together at one site).
- **Local-static numbering proves nerve grouping**: consecutive `instance$N` with stride 6 and no gap means the `theNerve()` bodies are adjacent in source (bossManta: 2918..2942, then `@2983` upward), which `DEFINE_NERVE` cannot produce; a file-local `DEFINE_NERVE_INSTANCE` block ahead of plain `execute` definitions fixed the whole-TU ORDER failure with byte-identical output.
- **A weak, out-of-line ~1 KB member is not reproduced by `inline`, with or without `#pragma dont_inline`** (`TPauseMenu2::appearWindow/disappearWindow`, `TBathWaterMeshRenderer::makeHeightMap/makeNormalMap/calcCoord` all collapse into the caller); the smallest sibling (`clearHeightMap`, 0x1bc) survives because its caller's expansion budget is already spent. The lever is the caller's budget, not the callee's declaration.
- **Search the disassembly for `bl <symbol>` before removing an `inline`**: zero occurrences (`TMario::braking`) means retail inlined it everywhere and the keyword masks an under-decomposed body (statement count above 14), not a linkage mistake.
- **Sibling size deltas identify literal folding**: MWCC does not fold a multiply by the literal `1.0f`, so `setStickAgainstMario` (0x74) vs `setStickToAngle` (0x78) means the original spelled the stores out with the factor removed. Trimming a reconstructed UNUSED body one statement at a time against the map size is cheap and decisive (`TWaterGun::setEmitPt` 0xa4 = the emitter loop alone).
- `TNozzleTurbo`/`TNozzleButton` add no virtuals (UNUSED vtables 0x28 = `TNozzleBase`'s ten slots); the shipped turbo nozzle is a `TNozzleTrigger` with `NozzleTrgTurbo.prm`. `MARIO_STATUS_HIP_DROP_END` is not recoverable from the ROM. `YoshiHeadCtrl` is `static`.
- **Two class templates over the same `<T, I>` in one TU's UNUSED list are a derivation, and the larger dtor is the derived one**: `JGadget::TSingleLink<T, I> : TSingleLinkList<T, I>` (0x5c vs 0x58) retired the fabricated `TPerformLinkList` and linked MarDirector (header round 12).

## Missing-bodies batch 100 (2026-09-18): matched data 87.9 -> 90.2%, seven units to 100% data

- **A constructor the whole map has no `__ct__` symbol for is in-class**, and a factory branch proves it instruction by instruction (`li r3, N; bl __ct__<Base>; lis/addi __vt__<Derived>; stw 0(obj)` plus the second vptr at 0x20). Test: `grep -c "__ct__[0-9]*<Class>F" marioUS.MAP` == 0. Seventeen such classes in `MarNameRefGen_MapObj` (four also had wrong out-of-line definitions in a .cpp); `getNameRef_MapObj` 88.9 -> 96.9, data 11 -> 100. **The stores after the derived vptr are the derived ctor's member initialisers** and the only evidence for them. An in-class ctor with an empty body that should call its base is invisible in its own unit but kills the caller's whole `.rodata` (`TPoiHanaCollision`, poihana data 67 -> 100).
- **A compare tree with a pivot plus two bounds (`cmpwi 0x12 / 0x10 / 0x14` with a trailing `b`) is a `switch`, not an `if` chain**, even over a `u32` (signed `cmpwi` for switch range tests, `cmplwi` for plain equality); `TMovieDirector::rsetup`'s two ladders rewritten as `switch` restored two missing string literals. A `case` whose body is only `break;` still costs its own compare. A jump table's `.rel` list gives the case grouping (`initUnique`: seven entries alternating two targets = `case 0: case 2: case 4: case 6:`).
- **`u16 a = f(); a += 8;` and `u16 a = f() + 8;` differ**: arithmetic in the initialiser truncates at the assignment; split, MWCC keeps the value raw and normalises only for the comparison (`TEndingString::perform` 96.5 -> 99.9). `TEndingString` (whole class, ctor 0xe8 and dtor 0x74 exact, `startFadeIn` 0xe0 from its inlined copy in `direct()`).
- **A string literal's `@NNNN` is allocated when its function is code-generated, and a call site can pull that forward**: `new TEndingString(...)` makes MWCC generate the ctor body for inline analysis at that point and reject it, so `"ending_1.blo"` lands between `rsetup`'s literals instead of last; moving the definition only moves the generation. Open: what makes a callee a non-candidate before generation.
- **"Retail keeps a weak in-class body out of line and calls it", two more instances**: `TTelesaSlot::TTelesaSlot` (0x98) and `TSunGlass::TSunGlass` (0xb0) at factory sites, plus `TVec3::scale(f)` at four `TTamaNoko::landEffect` sites and `std::sqrtf` in `MSoundSE::startSoundActorWithInfo`; the shared property is a generated body of ~25-45 instructions — not statement count, depth or caller size. Same family as `std::fmodf`; research item.
- **`JALSystem::append` rows are machine-checkable** (resolve `@NNNN` and `addi r4, r31, off` to values and Shift-JIS names, print `(ModType, name, id, f1..f7, sign, u8)` per `bl`): six wrong values and one missing row in `MSoundSE::construct` (98.0 -> 99.98); `CS_NEGATIVE_CURVE/POSITIVE_CURVE/UNKNOWN_2` = 0/1/2. **The US build has English debug strings where the JP source has Japanese** (all 21 `TMenuDirector::setFixedStageValue` names, `/card/mariobnr.bti`, six disc-error messages with a raw `\x99`) — see region-us.md. **`(a == b) == c` on floats is a real original construct** (poihana's zero-vector guard; an unexplained `0x4330000000000000` in `.sdata2` means a bool or int reaches a float comparison). `KoopaNeckCallBack`'s threshold is a `double` (`lfd`). `TItemNozzle::calcRootMatrix` (0x108, all 66 instructions, frame 0x28 vs 0x38). Open: Application's `@2485` (two `J2DPrint` colours as one 8-byte object copied by value), limitkoopa's two missing local statics.

## Structural batch 141 (2026-09-18): implicit instantiation order

- **MWCC emits implicit template instantiations at the end of the TU in reverse order of first use.** Verified on J3DAnmLoader.cpp (all thirteen `JSUConvertOffsetToPtr<T>` fall that way, `<s>` pinned by its first use inside `readAnmTransform_Full`) and used predictively to reconstruct `J3DClusterLoader_v15::readCluster`'s statement order (name tables, block arrays, vertex arrays, u16 fix-up loops): the object reproduced the map's order on the first build, symbol order FAIL (9 MISSING + 3 BINDING) -> PASS. Corollary: the map prints no TU header for a section with nothing live, so a missing `.data` entry is not evidence that a class has no vtable. `J3DAnmLoader.cpp` is the sibling for the loader hierarchy shape (abstract base with pure `load` + in-class virtual dtor, `_v15` derived). Open sizes: `J3DClusterLoaderDataBase::load` 0x84 vs 0xa0, `readCluster` 0x27c vs 0x2bc (trial tables in the file).
- Stale-queue lesson: `TEndingString` and MarNameRefGen_MapObj's factory branches were already complete (the factory is 98.9%, its residue the `TTelesaSlot(const char*)` weak-plus-`bl` refusal); verify a queue item against the current tree before spawning it.
