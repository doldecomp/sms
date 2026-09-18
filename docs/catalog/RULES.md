# The rules card

One rule per line, with its price and a pointer to the section holding the evidence.
Read this instead of the catalog; open a topic file where a rule points.
Per site unless stated; a "not:" clause is disproved — do not retry it.

## Structure: calls, control flow, load/store order

- Match control flow and call order first, load/store order second (frame-gaps.md: "Diagnosing").
- `if (a || b) return;` leaves the last operand unfused (`beq +8; b end`); two `if`s fuse both (codegen-tells.md: "Control flow").
- A redundant `bcc next; b epilogue` pair means that test is one term of a wider `||` (codegen-tells.md: "batch 65").
- Switch arms are emitted in **source** order, the pivot tree is value-sorted; decode destinations, not constants (codegen-tells.md: "batch 122").
- Sequential `cmpwi` = `if`/`else if`; signed `cmpwi` on a `u32` = `switch`; empty cases move the pivot (codegen-tells.md: "`tinkoopa`").
- Materialised bool = inlined bool helper, direct branch = inline compare; both occur in one TU (codegen-tells.md: "Booleans and predicates").
- `return x==N` is branchless, `return h()` adds `clrlwi`, `?TRUE:FALSE` a second branch, `bool` branches to `li 1/0` (codegen-tells.md: "Booleans and predicates").
- A two-`return` body plus one level is refused right of `&&`/`||`: spell `if (c) return TRUE; return FALSE;` (codegen-tells.md: "header round 9").
- MWCC always CSEs two loads of a member across a short-circuit branch: a reload proves a `bl` or store between (codegen-tells.md: "batch 60").
- A `const` accessor restores per-use re-reads and can also *cause* a CSE; decide per statement (codegen-tells.md: "header round 7").
- A store cannot cross a call, so a store after a `bl` dates that declaration after it (codegen-tells.md: "Batch 59").
- Argument `.get()`s defer loads to the call; arguments evaluate right to left, named locals left to right (codegen-tells.md: "Load and store order").
- `f32 x = a - b;` subtracts into `b`'s register, `x = a; x -= b;` into `a`'s (codegen-tells.md: "batch 80").

## Inlining decisions

- The budget is a statement **cost at the reached depth**, never bytes: plain 14 / 9 / 6 / 2 / never at depths 1-5, `inline` unlimited at depth 1 (codegen-tells.md: "Inlining").
- The `inline` keyword buys **only depth 1**: from depth 2 down a weak header body and a plain function share 9 / 6 / 2 / never, and in-class members never hit the 15-statement floor (codegen-tells.md: "batch 146").
- Only recursion, mutual recursion, `...` varargs and `inline asm` are true inline refusals; a statement-level `asm {}` block still inlines, and plain `asm` functions are global, not weak (codegen-tells.md: "batch 146").
- Cost: 1 per expression statement or initialised declaration, 1 per `else`, a loop 1 + init + increment + 1 (codegen-tells.md: "`bosseel`/`fireWanwan`").
- Free: uninitialised declaration, `;`, `{}`, `return <local>;`, splitting `f32 x = e;`, statements from a callee's own inlines (codegen-tells.md: "Inlining").
- Depth counts from the innermost expression; an inlined call's argument is a level, an out-of-line call's is not (codegen-tells.md: "sweep 106").
- Any **stored** value accepts a level; a `return *this` forwarder is not a level, but still binds frame (codegen-tells.md: "sweep 106").
- not: definition order, "deferred inlines only later-defined callees", caller size, site count, declaration form — all inert (codegen-tells.md: "Inlining").
- Retail calling a plain method at depth 1 floors it at 15 statements; inlining over the allowance means `inline` (codegen-tells.md: "Inlining").
- One statement flips it both ways; a ternary cuts cost with identical codegen (codegen-tells.md: "`MapObjDolpic`").
- Codegen-neutral levels: a one-line forwarding overload, a **defaulted argument**, `v *= k`, a params wrapper, `TPosition3f` over `Mtx`, a TU-local `static inline` (codegen-tells.md: "header round 19").
- A class-template member defined out of class without `inline` never inlines: the only source of "weak plus `bl` at depth 1" (codegen-tells.md: "batch 104").
- `TVec3<f>` members are in-class: 3-statement (`scale`, `sub`, `add`, `set`) call at depth 4, 1-statement (`dot`, `operator=`) at 5. not: making them never-inline, 217 functions lost (codegen-tells.md: "batch 104").
- Spellings reaching them: `a = b - c`, `a = a + b` over `+=`, a product in an inlined argument, one thin length level (codegen-tells.md: "sweep 106").
- `TUtil<f32>::sqrt`/`inv_sqrt` are 3 statements: inline 1-3, `bl` at 4, so calling one and inlining the other measures extra levels (codegen-tells.md: "batch 71").
- `std::fmodf`/`TUtil<f32>::mod` have a real 0x5c body retail never expands; two-level wrap helpers stand in (codegen-tells.md: "header round 13").
- Steer a tiny accessor with one relative level (owning-class forwarder, or const/non-const pair), never a header-wide change (codegen-tells.md: "Inlining").
- Pasted-UNUSED: a size-exact UNUSED helper is often written out at its call sites; conversely one can exist only to *be* a level (codegen-tells.md: "`bosswanwan`").

## Frame-size gaps

- Validate with `volatile char trash[N]`: 100% with no instruction diff means only the frame is wrong. Never commit padding (frame-gaps.md: "Diagnosing").
- Locals start at `0xc(r1)`, `frame = align8(0xc + locals + 1)`; an outgoing area costs 8 bytes per `f32` argument (frame-gaps.md: "Diagnosing").
- Inline temporaries grow up from 0xc in expansion order; named locals down from the top, first declared highest (frame-gaps.md: "Two regions").
- Classify by inter-slot deltas: uniform = low region (levers work), a hole in the named block = a missing declaration, located with `trash[N]` declared **last** (frame-gaps.md: "batch 120").
- Accessor steps are 8 bytes, never 4, on member chains; a global fork is +4 per read; an indexed accessor on structs +16 per use (codegen-tells.md: "batch 65").
- Accessor levers are a **signed** knob, saturating per member and per block; a fourth rung needs another class (frame-gaps.md: "batch 103").
- Two +0 levers can be +8 as a **pair** (six confirmations): try pairs before calling a residue unnameable (codegen-tells.md: "batches 91 and 94").
- Best lever: a TU-local `static inline` taking the receiver by pointer that **binds and returns** a value, +8 (sometimes +16) per expansion; returning directly, or a level above a real `bl`, is +0 (frame-gaps.md: "batch 110").
- Binding works over an accessor, a raw member read, or a struct/array member's **address**, which copies nothing; nested levels stack, two bindings in one level do not (frame-gaps.md: "sweep 127").
- An address binding at a call site is +8 for the pointer **and** the reference form, and can buy a whole register cluster: `J3DModel::entryModelData` (frame-gaps.md: "re-pass 162").
- Address binding is a frame lever only (0 keeps from 335 frame-exact functions). not: the reference form, 0 keeps in 1,243 trials (frame-gaps.md: "batch 130").
- A constant argument binds nothing, `this` only as a fresh load, a binding must stay live; a member accessor caps at +8, a by-pointer free function reaches +16 (frame-gaps.md: "batch 82").
- The two-argument `MSound::startSoundActor(id, pos)` binding overload is +8 per expansion, per site: multi-site branches overshoot (frame-gaps.md: "batch 82").
- A member read through an inlined accessor leaves a dead 4-byte temporary; inline temp cost is per parameter (one +8, two +16) (frame-gaps.md: "What causes a gap").
- Uninitialised locals the original declared reserve a slot with no code (`Mtx` 48, `TVec3` 12, `TQuat4` 16): name them or leave the gap (frame-gaps.md: "What causes a gap").
- A raw read where `offset - 0x10` is a 0x14-aligned slot is a `TParamRT`: declare the params class, call `.get()` (frame-gaps.md: "What causes a gap").
- Last 8 bytes, every displacement 8 higher: a two-word **aggregate** declared last, or an 8-byte **non-trivial** local of an inlined callee. not `f64` (frame-gaps.md: "The last 8 bytes").
- Dead low region: an uninitialised **non-trivial class** local of an inlined callee is the only zero-instruction lever; `sizeof` rounded to 8 (4/8 B +8, 12/16 +16, 20/24 +24) per expansion (frame-gaps.md: "The dead low region").
- Triviality is the switch (user ctor, dtor, copy ctor or virtual): trivial PODs are +0 in a callee, while in the caller's body only size counts (frame-gaps.md: "The dead low region").
- A carrier must be a callee with **no matching out-of-line copy**: an UNUSED function, or a header inline with no map symbol (frame-gaps.md: "The carrier has to be a callee").
- A dead named local in the caller adds its size **above** the pool (`TVec3` 12, `u8[4]` 4, `f32` 0); a trailing `u8[1..4]` is free (frame-gaps.md: "batch 119").
- It must be **last-declared** (declared first it lands the frame but scrambles the used locals) and its constructor must be **empty**: `TVec2<f32>` is 8, `TRect` is not a carrier at all (13 instructions), and a dead `TVec3` can be worth 0 (frame-gaps.md: "re-pass 162").
- One inlined callee's expansions **share** one temp block, so a big dead low region is never one pool per expansion (frame-gaps.md: "re-pass 162").
- Worth nothing: a scalar or trivial POD local of an inlined callee, a 4-byte local anywhere, a dead int-to-float conversion, `sqrtf`, `* 1.0f` (frame-gaps.md: "What is worth nothing").
- Each chained `>>` continuation is 8 bytes of low region (`continuations = reads - statements`) (frame-gaps.md: "batch 81").
- An **implicit** derived-from-base conversion on a `return` reserves 16 bytes (pool 20 low / 4 high): write `return iterator(Base::insert(...));`. Retail's list walks are `++it` (frame-gaps.md: "batch 133").
- not: `TSolidStack::push` by value (65 regressions), a local before `pushAfterCurrent`, nerve pushes as a gap size, an argument-area explanation (frame-gaps.md: "Ruled out").
- Seven units' scores rest on fabricated padding, so they are not evidence; do not link MarioGamePad (frame-gaps.md: "Pre-existing padding").
- `TVec3`'s `operator*` return type is a **consumption** split, not an overload one: a destination copy (`x = a*k`, `TVec3 v = a*k`) wants the reference return, a consumed temporary (argument, `+=`, `+`) the by-value one. not: a by-value `operator=` or implicit copy ctor (map-refuted), direct-init, the parameter form (frame-gaps.md: "Research batch 159").

## Register and scheduling residues

- A callee-saved FPR goes only to a **named scalar local of the function's own body**; members and an inlined callee's locals reload per use (codegen-tells.md: "batch 83").
- So "x reloaded, y/z preserved" means retail named exactly those two components, at zero frame cost (codegen-tells.md: "batch 83").
- **First**-declared local gets f31 (batch 90's "last" was a slip); declare it at the statement that first reads it or it loads early and costs an `fmr` (codegen-tells.md: "batch 90", frame-gaps.md: "Research batch 171").
- A non-void return with no `return` reserves r3 for the whole body (frame-gaps.md: "batch 120").
- Callee-saved GPRs go out r31 down in reverse introduction order: pool/base temps, then locals, then parameters, `this` last, inner-block locals after `this`; use counts, first-use order and liveness are inert (frame-gaps.md: "Research batch 144", "batch 145").
- How many **named scalar locals** the frame holds decides whether the pool base outranks `this`; grouping four of them into an array closed `TSunMgr::load` at zero frame cost (frame-gaps.md: "Research batch 144").
- Declaration order is a knob **only** among function-scope named locals in callee-saved registers; it is inert on `this`, parameters, pool/constant temps and inner-block locals, and block scope is inert everywhere (frame-gaps.md: "batch 145").
- Callee-saved FPRs go out f31 down: an **inlined callee's** temps first (reverse declaration order inside it), then the function's own named `f32` locals in **forward** declaration order, then parameters in reverse order; an anonymous literal/member/global read never gets one (frame-gaps.md: "Research batch 171").
- Volatile FPRs (f0-f13) trade in blocks and ignore source order; the only knob is **naming** two values as locals, which sets which loads first (raw arguments evaluate right to left) and carries its register (frame-gaps.md: "Research batch 171").
- An inlined callee's **parameter modified in place** (`amp *= scale`) loads straight into the result's register; a fresh named local loads to a scratch first, and it costs a statement plus 8 bytes of shared temp block (frame-gaps.md: "FPR re-pass 172").
- Known-open: the `this`-vs-pool-base callee-saved swap and zero-frame rotations; `M3UMtxCalcBlendAux` moves with declaration order but never lands, and `TRKSuppAccessFile` (all 120 orders) plus `emitParticle_`, `checkNextFrameSe`, `partsPerform` (fully inert) are exhausted (frame-gaps.md: "batch 145").

## Float and pool

- `fp_contract` fuses only products of **locals**, except `a += b*c` and a single expression's last product (codegen-tells.md: "Floating point").
- `x / 2.0f` and `x * 0.5f` both give `fmuls x, 0.5`, but division keeps the dividend left, a literal the constant (codegen-tells.md: "batch 101").
- A surviving `* 0.0f`/`* 1.0f` proves an inline boundary or an accessor returning the constant; `TUtil<f32>::one()` is the +4 lever (codegen-tells.md: "Batch 58").
- `.sdata2` is reverse source order, so a late literal belongs near the top of the file and a shared slot fixes its value (codegen-tells.md: "Batch 59").
- `@NNNN` ids: aggregates and local statics numbered at **parse** (monotonic in source line), strings at deferred codegen; `.rodata` follows id order (tu-reconstruction.md: "Literal numbering").
- A missing function **permutes** the pool rather than shifting it: diff the two `.rodata` blobs before chasing immediates (codegen-tells.md: "`EventWatcher`").
- `RAD_TO_DEG` = `180.0f / M_PI` (`0x42652ee0`); `DEG_TO_RAD` is the single-precision `0.017453294f`, not the double fold; the integer 182 factor is real (linking.md: "Pool-order batch 96").
- Count `frsqrte` refinements: none = `x * __frsqrte(x)`, one double step = `MsSqrtf`, one single step with `cror` = `TUtil<f32>::sqrt`, three = `std::sqrtf` (codegen-tells.md: "`bosswanwan`").

## Data and layout

- A rogue-include header that defines string literals must be included where retail's `.rodata` puts them, not spelled out in the `.cpp`: `DummyStrings.hpp` above `SunModel.hpp` linked sunmgr (linking.md: "batch 145").
- Rogue-include prefixes: 12 zero bytes + a 20-byte Shift-JIS string = `DummyStrings.hpp`; + four `MActorMtxCalcType_*` = `InfectiousStrings.hpp`; fifteen 12-byte `.bss` + a 764-byte `__sinit` = the MSound pair (linking.md: "Data sweep batch 93").
- `MapCollisionEntry.hpp` adds a 12-byte zero and one vector and must come **after** `InfectiousStrings.hpp`; offsets 0x18 low = missing it (linking.md: "Data sweep batch 93").
- `__sinit`'s JAL list is reverse include order: read it off the target before ordering `MSSetSound.hpp` against `MSoundBGM.hpp` (codegen-tells.md: "`MapObjCorona`").
- Dump the whole vtable and declare virtuals in slot order; null tail slots are pure virtuals (tu-reconstruction.md: "Virtual order from the vtable").
- A changed return type, covariant or not, appends a vtable slot instead of overriding, as does a fabricated declaration-only virtual (tu-reconstruction.md: "Override return types").
- dtk infers false relocations in `.data` and `.text`; block with `block_relocations` (`target:`/`end:` for `.text`). Exactly 21 `.text` cases, all `MSD_BGM_*` (linking.md: "Structural batch 132").

## Linking: why a 100/100 object changes the DOL

- Promotion needs the DOL SHA-1, not `changes_all`: identical caller instructions do not prove identical whole-file emission (linking.md: "Other notes").
- Weak emission order is most-derived-class-first then declaration order in each class, so an override's declaration position is a free lever (linking.md: "Header round 14").
- A weak map symbol nothing defines is a missing header inline that silently externalises the class's vtable: fatal, invisible to objdiff. `check-weak-defined.py` lists them, NOBODY first (linking.md: "Header round 16").
- Re-link to diagnose: a shrunken section names the object starting early; identical section tables mean **content**, so `cmp -l` the DOLs (linking.md: "Structural batch 97").
- Before reading a section table, discount dtk's `gap_NN_*` fillers, dropped weak duplicates (the map's `>>>` lines) and dead-strippable `.sdata`/`.sbss`: only `.rodata` is unstrippable, so its order is what blocks (linking.md: "batch 145").
- A unit that matches but shortens `.sdata2` has a pool-order problem, and pool order is evidence about UNUSED stub bodies (linking.md: "Batch 72").
- Run `validate-symbol-order.py` before proposing a link: it catches missing UNUSED symbols and BINDING errors objdiff cannot see (linking.md: "Batch 72").
- Editing a shared header linked units depend on (`PollutionPos.hpp`, `MActor::getModel()`) breaks the DOL even when scores improve (frame-gaps.md: "batch 110").

## TU reconstruction

- Inventory the whole map first: every class touching the TU, every method wherever emitted, every free function including UNUSED (tu-reconstruction.md: "UNUSED functions").
- `-inline deferred` emission is strict reverse **definition** order, UNUSED included; reorder from the `.fn` lines of the dtk asm (tu-reconstruction.md: "Reordering a TU mechanically").
- A local or weak template instantiation is emitted next to its **last source-order user**, so symbol order can fail on placement alone (tu-reconstruction.md: "Structural batch 75").
- Factories give class shape: `li r3,N; bl __nw__` = `sizeof`, `bl __ct__X` = an out-of-line ctor, a following `__vt__Y` store = Y's ctor in-class, and the stores after it are its initialisers; no `__ct__` in the map means in-class (tu-reconstruction.md: "Factories are a complete oracle").
- `PARAM_INIT` stringifies the member name, so names and defaults read off `.rodata`/`.sdata2` interleaved in id order; integer params are `TParamRT<s32>` (tu-reconstruction.md: "Names and defaults").
- UNUSED bodies go in the `.cpp`, never `inline` or in-class; the size is necessary but not proof, and prunes contents statement by statement (tu-reconstruction.md: "UNUSED functions").
- Nerve tells: `nerve$localstatic0$theNerve__X` = an inline header `theNerve()`; `instance$N` at stride 6 = a file-local `DEFINE_NERVE_INSTANCE` block; `{0,0,dtor,0}` = an abstract nerve (tu-reconstruction.md: "Nerve shapes from the map").

## Known-open residue classes (stop spending budget)

- The `a = b - c` / `bl TVec3::sub` residue is an intra-statement allocation **order** difference, not a by-value return slot; by-value still costs +6 (frame-gaps.md: "batch 142").
- Inline-temp price by return type: reference 8, pointer 4, void 0, additive over the levels; a *consumed* binding is 4 below the temp, a *dead* one 4 above (frame-gaps.md: "batch 142").
- Retail's (4, 12) needs two dead bindings after the subtraction, worth +9 exact functions but dropping `__ami__` to MISSING: bounty, not lever (frame-gaps.md: "batch 142").
- not: declaration order inside the class, the entire caller side, `operator=`'s return type, `sub` out of class with `inline` — inert for the temp (frame-gaps.md: "batch 142").
- not: a user `~TVec3() {}` to collapse the prefix — prefix 0 but total fuzzy 97.36 -> 95.87 (frame-gaps.md: "batch 119").
- Per-call-site inline splits with no lever: the `MapObjBall`/`amiNoko` `sqrt` sites, Koopa's five identical blocks (codegen-tells.md: "Inlining").
- `theNerve()` expansion is emergent per function from identical source: a 54-79% residual with every instruction matching (codegen-tells.md: "`BathtubKiller`").
- "Weak plus `bl` at depth 1" is now only the three factory ctors (`TTelesaSlot`, `TFence`, `TSunGlass`); `std::sqrtf` was a depth-3 measurement and closed, and `__construct_array`, inline asm and a per-caller budget are all refuted (codegen-tells.md: "batch 146").
- The JGadget iterator temp-pool **grouping** on `TSeal::init`, `TMirrorActor::init`, `TPerformList::perform` is not the conversion lever (frame-gaps.md: "batch 133").
- The JGadget grouping is a per-expansion block **stride** (8 retail, 12 ours), differs per site and ignores batch 142's return-type ladder; `operator++`/`operator*`/`operator->` return types are inert and the rest of the chain is pinned byte-exact. not: a named `TSingleNodeLinkList::begin()`/`end()` — lands `perform`'s 0xe8 but costs both `TPerformList::push_back` (frame-gaps.md: "Research batch 161").

## US-region differences

- Guard differences with `#if defined(VERSION_GMSE01)`, keep Japanese names, annotate the shift (region-us.md: preamble).
- `MSound`: 0x98 `u8 mWaterFilterOverride`, 0x9A `u16 mTimerParameter`, then 0x9C/0xA0/0xAC/0xB0/0xC8/0xCC/0xD1/0xD2; no per-caller casts (region-us.md: "`MSound`").
- `GCConsole2`: the US ctor writes a `u16` at 0x3AE and `mAppearFromDemo` at 0x3B0, pointers from 0x3B4; `checkDolpic8()` and its 47 tables are US-only (region-us.md: "`GCConsole2`").
- GC2D: check `new` sizes first — `TOptionSubtitleUnit` exists, `TBalloonControl` does not, `scScenarioNameTable` skips ids 8/9, the title has 18 panes (region-us.md: "`Option`, `ConsoleStr`, `StageUtil`").
- US debug strings are English (21 stage names, six disc-error messages, `/card/mariobnr.bti`); `HelpActor`'s id base is 0x33 (region-us.md: "Strings and IDs").
- Application's US disc-error strings are 121/106 bytes, not 124/109: seventeen later strings shift 4 and the next `.rodata` boundary moves 8 (linking.md: "US `.rodata` boundaries").
