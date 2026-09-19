# The rules card

One rule per line, with its price and a pointer to the section holding the evidence.
Read this instead of the catalog; open a topic file where a rule points.
Per site unless stated; a "not:" clause is disproved — do not retry it.

## Structure: calls, control flow, load/store order

- Match control flow and call order first, load/store order second (frame-gaps.md: "Diagnosing").
- `if (a || b) return;` leaves the last operand unfused (`beq +8; b end`); two `if`s fuse both (codegen-tells.md: "Control flow").
- A redundant `bcc next; b epilogue` pair means that test is one term of a wider `||` (codegen-tells.md: "batch 65").
- In practice that pair is always two separate `if (c) return;` statements retail spells as one `if (a || b) return;`; read the compare's operand order off the `cmplw` (codegen-tells.md: "Structural pass 173").
- Consecutive `if (c) return;` whose earlier tests branch straight to the exit and whose last is unfused are one `if (a || b || c) return;`; the `return true` mirror shares one `li r3, 1` (codegen-tells.md: "Structural pass VI").
- `f32 x = e; x /= k;` lands the quotient in the local's FPR without an `fmr`; `x = !x` on a `u8` is `cntlzw`/`srwi`, `x = (x == 0)` adds a `neg` (codegen-tells.md: "Structural pass VI").
- `subfic A; addi B` for a folded `A - x + B` means the subtraction was its own inline; `case N: break;` on an empty first arm leaves the pivot tree, `case N: return;` keeps it; a receiver retail loads before the argument arithmetic is a named local (codegen-tells.md: "Structural pass IX").
- A lone `addi rD, rBase, off` between two member loads in a tiny accessor is a reference-typed local; store a helper's result in its exact return type (`s16` for `SMS_getShineID`) (codegen-tells.md: "header round 35").
- not: making an explicitly specialised class template's empty ctor a weak `bl` (`SMatrix34C<f32>`, -35 exact, DOL broken); the out-of-class forms emit a global (codegen-tells.md: "header round 35").
- A vtable slot past the declared base's last slot means the receiver is a **derived** class, not a wrong index (codegen-tells.md: "Structural pass 173").
- A stack or member address used at two call sites is bound once into a callee-saved register; per-site `addi` is one instruction short each time (codegen-tells.md: "Structural pass 173").
- An `int` local feeding an `int` parameter gives `mr`, a `u8` one `addi rD, rS, 0` (codegen-tells.md: "Structural pass 173").
- A dead `b <default>` in a switch is one more empty case; `if (c) return f(v); return v;` leaves a dead `b epilogue` where `if (c) v op= k; return v;` shares the exit; a surviving impossible compare is `MsClamp<T>` with a parameter bound (codegen-tells.md: "Structural pass VIII").
- `TVec3` values built only for `const TVec3&` arguments are call temporaries (right-to-left, ascending slots, reversed stores), never block locals; a pointer local that only feeds a call costs an `mr`: delete it and re-call the accessor (codegen-tells.md: "Structural pass X").
- Declare a named local **after** the expression that first materialises its value to share the load (before: an early load plus `fmr`); `MTXCopy(call(), member)` evaluates the destination first, so bind the member address before the call (codegen-tells.md: "Structural pass VIII").
- not: `TNerveBase<T>::TNerveBase()` out of class (-3.15 matched, DOL broken); retail's `bl` at `theNerve()` statics is per site like `TVec3::set<f>` (codegen-tells.md: "Structural pass VIII").
- Switch arms are emitted in **source** order, the pivot tree is value-sorted; decode destinations, not constants (codegen-tells.md: "batch 122").
- Range grouping of an `x == c` chain over contiguous constants (one range test vs one test plus a range) is emergent per expansion: retail emits both from one source; never reorder the `||` terms for it (codegen-tells.md: "Header round 32").
- Count a header helper's real call sites with `objdump -dr` and `R_PPC_REL24` rows; plain `objdump -d` shows an unrelocated `bl` as a self-branch (codegen-tells.md: "Header round 32").
- Sequential `cmpwi` = `if`/`else if`; signed `cmpwi` on a `u32` = `switch`; empty cases move the pivot (codegen-tells.md: "`tinkoopa`").
- Materialised bool = inlined bool helper, direct branch = inline compare; both occur in one TU (codegen-tells.md: "Booleans and predicates").
- `return x==N` is branchless, `return h()` adds `clrlwi`, `?TRUE:FALSE` a second branch, `bool` branches to `li 1/0` (codegen-tells.md: "Booleans and predicates").
- A two-`return` body plus one level is refused right of `&&`/`||`: spell `if (c) return TRUE; return FALSE;` (codegen-tells.md: "header round 9").
- MWCC always CSEs two loads of a member across a short-circuit branch: a reload proves a `bl` or store between (codegen-tells.md: "batch 60").
- A `const` accessor restores per-use re-reads and can also *cause* a CSE; decide per statement (codegen-tells.md: "header round 7").
- A store cannot cross a call, so a store after a `bl` dates that declaration after it (codegen-tells.md: "Batch 59").
- `a = b = c = v;` stores right to left; a C89 aggregate initialiser is emitted at its declaration, so a mid-function init means a nested block; a `u8` tested with signed `cmpwi` and no `extsb` was read as `int` (codegen-tells.md: "Structural pass VII").
- A raw `TVec3` build: interleaved load/store pairs ascending = three assignments, all loads first descending = a 3-argument `set`/ctor (codegen-tells.md: "Structural pass VII").
- Argument `.get()`s defer loads to the call; arguments evaluate right to left, named locals left to right (codegen-tells.md: "Load and store order").
- A global read through its inline accessor (`&SMS_GetMarioPos()` for `gpMarioPos`) is a free evaluation-order knob: as an argument it loads ahead of the receiver, as the receiver ahead of the arguments (codegen-tells.md: "Reloc-target pass IV").
- `a * b * c` association reads off the first `fmuls`; a retail `bne <true-arm>` with the false value in the fallthrough means the source negated the ternary's test (codegen-tells.md: "Reloc-target pass IV").
- `f32 x = a - b;` subtracts into `b`'s register, `x = a; x -= b;` into `a`'s (codegen-tells.md: "batch 80").

## Inlining decisions

- The budget is a statement **cost at the reached depth**, never bytes: plain 14 / 9 / 6 / 2 / never at depths 1-5, `inline` unlimited at depth 1 (codegen-tells.md: "Inlining").
- The `inline` keyword buys **only depth 1**: from depth 2 down a weak header body and a plain function share 9 / 6 / 2 / never, and in-class members never hit the 15-statement floor (codegen-tells.md: "batch 146").
- Only recursion, mutual recursion, `...` varargs and `inline asm` are true inline refusals; a statement-level `asm {}` block still inlines, and plain `asm` functions are global, not weak (codegen-tells.md: "batch 146").
- Counted at zero codegen: `a = a;`, `(void)0;`, `a += 0;`, `if (0) {}`, `switch (x) {}`, `while (0);`, `goto`, a statement after `return`, an unused `register`/block-scope init decl, and **a named local** (`int t = e; x = t;`, the only plausible one); free: `;`, `{}`, uninit decl, `do {} while (0)`, `asm {}`, a folded `const` local, `enum`/`typedef`, a label. `a && b` costs the same as nested `if`s; the caller's context and `this->`/qualified calls are inert (codegen-tells.md: "Research batch 186").
- Cost: 1 per expression statement or initialised declaration, 1 per `else`, a loop 1 + init + increment + 1 (codegen-tells.md: "`bosseel`/`fireWanwan`").
- Free: uninitialised declaration, `;`, `{}`, `return <local>;`, splitting `f32 x = e;`, statements from a callee's own inlines (codegen-tells.md: "Inlining").
- An inlined one-liner's **argument** is not a level, only an enclosing inlined body is: `f(g(x))` flips at the same cost as `t = g(x)` (codegen-tells.md: "Batch 190").
- Depth counts from the innermost expression; an inlined call's argument is a level, an out-of-line call's is not (codegen-tells.md: "sweep 106").
- Any **stored** value accepts a level; a `return *this` forwarder is not a level, but still binds frame (codegen-tells.md: "sweep 106").
- not: definition order, "deferred inlines only later-defined callees", caller size, site count, declaration form — all inert (codegen-tells.md: "Inlining").
- Retail calling a plain method at depth 1 floors it at 15 statements; inlining over the allowance means `inline` (codegen-tells.md: "Inlining").
- One statement flips it both ways; a ternary cuts cost with identical codegen (codegen-tells.md: "`MapObjDolpic`").
- The named-local +1 is chosen by the diff: a callee-saved FPR holding a literal across a loop names the literal, an `addi` base reloaded through at every use names the **address** (`T** slot = &arr[i]`), a load feeding only a compare names the tested value; a local used twice collapses reloads, one feeding a call costs an `mr`, a loop bound hoists (codegen-tells.md: "Batch 189").
- Folding nested `if`/`else` returns into ternaries can flip a header inline back under budget so its weak copy vanishes as retail's did (`getSightDirection`); a TU-local forwarder around an implicit member-wise copy keeps `TVec3::operator=` out of line (codegen-tells.md: "Batch 189").
- MWCC never calls an in-class copy ctor for a by-value class parameter of an inlined function (value category, defaults, depth all inert); a one-line forwarding overload on the owning class sinks a *constructor body* one level tree-wide (`TGameSequence::set(u8, u8)`, +36 on `TApplication::TApplication`); `TVec3::set(x, y, z)` is one level below the caller with right-to-left evaluation (codegen-tells.md: "Header round 33").
- A defaulted **class-type** argument that is never read is instruction-neutral but reserves `align8(sizeof)` (+8 per site at <= 4 bytes, +16 at 8, +32 at 16) at every call site of the inline: the JALModSe 0x68 lands with it but the type is unnamed (frame-gaps.md: "Research batch 209").
- Codegen-neutral levels: a one-line forwarding overload, a **defaulted argument**, `v *= k`, a params wrapper, `TPosition3f` over `Mtx`, a TU-local `static inline` (codegen-tells.md: "header round 19").
- A class-template member defined out of class without `inline` never inlines: the only source of "weak plus `bl` at depth 1" (codegen-tells.md: "batch 104").
- `TVec3<f>` members are in-class: 3-statement (`scale`, `sub`, `add`, `set`) call at depth 4, 1-statement (`dot`, `operator=`) at 5. not: making them never-inline, 217 functions lost (codegen-tells.md: "batch 104").
- Spellings reaching them: `a = b - c`, `a = a + b` over `+=`, a product in an inlined argument, one thin length level (codegen-tells.md: "sweep 106").
- `TUtil<f32>::sqrt`/`inv_sqrt` are 3 statements: inline 1-3, `bl` at 4, so calling one and inlining the other measures extra levels (codegen-tells.md: "batch 71").
- `std::fmodf`/`TUtil<f32>::mod` have a real 0x5c body retail never expands; two-level wrap helpers stand in (codegen-tells.md: "header round 13").
- `addi rD, rS, 0; addi rD, rD, off` on a receiver is a **setter inlined at the call site** (`getModel()->setBaseTRMtx(m)` for `MTXCopy(m, getBaseTRMtx())`); a level over a pure `return member` decays to nothing (codegen-tells.md: "Header round 37").
- Of two test shapes on one inlined `bool` accessor only the `clrlwi.` site is the type tell; a `cmpwi` site is not a `BOOL` return (codegen-tells.md: "Header round 37").
- Steer a tiny accessor with one relative level (owning-class forwarder, or const/non-const pair), never a header-wide change (codegen-tells.md: "Inlining").
- Pasted-UNUSED: a size-exact UNUSED helper is often written out at its call sites; conversely one can exist only to *be* a level (codegen-tells.md: "`bosswanwan`").

## Frame-size gaps

- Validate with `volatile char trash[N]`: 100% with no instruction diff means only the frame is wrong. Never commit padding (frame-gaps.md: "Diagnosing").
- Locals start at `0xc(r1)`, `frame = align8(0xc + locals + 1)`; an outgoing area costs 8 bytes per `f32` argument (frame-gaps.md: "Diagnosing").
- An inlined callee's **class-object** locals form their own block that always ranks **above** the caller's own named locals; blocks stack downward in expansion order, later-declared higher inside a block; the only knob is which body declares the object, caller-side scope/position and triviality are inert (frame-gaps.md: "Research batch 208").
- Inline temporaries grow up from 0xc in expansion order; named locals down from the top, first declared highest (frame-gaps.md: "Two regions").
- Classify by inter-slot deltas: uniform = low region (levers work), a hole in the named block = a missing declaration, located with `trash[N]` declared **last** (frame-gaps.md: "batch 120").
- Accessor steps are 8 bytes, never 4, on member chains; a global fork is +4 per read; an indexed accessor on structs +16 per use (codegen-tells.md: "batch 65").
- Accessor levers are a **signed** knob, saturating per member and per block; a fourth rung needs another class (frame-gaps.md: "batch 103").
- Two +0 levers can be +8 as a **pair** (six confirmations): try pairs before calling a residue unnameable (codegen-tells.md: "batches 91 and 94").
- Best lever: a TU-local `static inline` taking the receiver by pointer that **binds and returns** a value, +8 (sometimes +16) per expansion; returning directly, or a level above a real `bl`, is +0 (frame-gaps.md: "batch 110").
- A parameterless binder over a real `bl` is +4, not +0, when the binder also does the call site's arithmetic (`CLBGameRenderWidthMax()`); with the arithmetic left outside it is +0 and reorders the neighbouring literal load (frame-gaps.md: "re-pass II 178").
- Binding works over an accessor, a raw member read, or a struct/array member's **address**, which copies nothing; nested levels stack, two bindings in one level do not (frame-gaps.md: "sweep 127").
- The +4 rung is a direct-return fork **nested inside a binder**, over a global, a file-local static *or a plain member read*; at the call site it is +0 and two identical stacked forks collapse (frame-gaps.md: "re-pass 172").
- A function 4 off an 8-byte binder grid wants the global-fork rung (`static inline THitActor* X() { return (THitActor*)gpMarioAddress; }` feeding the call); a binder over a large class member is priced by the object's size (+16 on a 0x18 `TVector`), not its return type (frame-gaps.md: "Closure 219").
- A fork over a global must be used at **every** site of its wrapper family or it rotates the first expansion's register coalescing (frame-gaps.md: "re-pass 172").
- A consumed reference binding is +4 out of line but **+8** in an inlined expansion of the same body, so price it across the whole family (frame-gaps.md: "re-pass 172").
- When the low region's *total* is pinned (16-aligned named block) the residue is an allocation order difference and the whole lever ladder collapses to 0/8/16 (frame-gaps.md: "re-pass 172").
- An address binding at a call site is +8 for the pointer **and** the reference form, and can buy a whole register cluster: `J3DModel::entryModelData` (frame-gaps.md: "re-pass 162").
- A +8 binding can be paid by dropping a neighbouring named pointer local its chain already reloads (`entryModelData`); `ptr + index` in one expression is `add rD, rIndex, rPtr` while `p += index` is `add rD, rPtr, rIndex`, and a constant outside the index puts `addi` first (frame-gaps.md: "Research batch 215"); a binder scoped to one function prices a lever a shared accessor would break (frame-gaps.md: "Library closure 202").
- Address binding is a frame lever only (0 keeps from 335 frame-exact functions). not: the reference form, 0 keeps in 1,243 trials (frame-gaps.md: "batch 130").
- A constant argument binds nothing, `this` only as a fresh load, a binding must stay live; a member accessor caps at +8, a by-pointer free function reaches +16 (frame-gaps.md: "batch 82").
- The two-argument `MSound::startSoundActor(id, pos)` binding overload is +8 per expansion, per site: multi-site branches overshoot (frame-gaps.md: "batch 82").
- A member read through an inlined accessor leaves a dead 4-byte temporary; inline temp cost is per parameter (one +8, two +16) (frame-gaps.md: "What causes a gap").
- Uninitialised locals the original declared reserve a slot with no code (`Mtx` 48, `TVec3` 12, `TQuat4` 16): name them or leave the gap (frame-gaps.md: "What causes a gap").
- A raw read where `offset - 0x10` is a 0x14-aligned slot is a `TParamRT`: declare the params class, call `.get()` (frame-gaps.md: "What causes a gap").
- Last 8 bytes, every displacement 8 higher: a two-word **aggregate** declared last, or an 8-byte **non-trivial** local of an inlined callee. not `f64` (frame-gaps.md: "The last 8 bytes").
- A fabricated flag-test gate whose call sites all pass constants is usually the TU's **real** gate function constant-folded; check the fold before treating the test as a carrier (frame-gaps.md: "Research batch 193").
- Dead low region: an uninitialised **non-trivial class** local of an inlined callee is the only zero-instruction lever; `sizeof` rounded to 8 (4/8 B +8, 12/16 +16, 20/24 +24) per expansion (frame-gaps.md: "The dead low region").
- Triviality is the switch (user ctor, dtor, copy ctor or virtual): trivial PODs are +0 in a callee, while in the caller's body only size counts (frame-gaps.md: "The dead low region").
- A carrier must be a callee with **no matching out-of-line copy**: an UNUSED function, or a header inline with no map symbol (frame-gaps.md: "The carrier has to be a callee").
- A dead named local in the caller adds its size **above** the pool (`TVec3` 12, `u8[4]` 4, `f32` 0); a trailing `u8[1..4]` is free (frame-gaps.md: "batch 119").
- It must be **last-declared** (declared first it lands the frame but scrambles the used locals) and its constructor must be **empty**: `TVec2<f32>` is 8, `TRect` is not a carrier at all (13 instructions), and a dead `TVec3` can be worth 0 (frame-gaps.md: "re-pass 162").
- One inlined callee's expansions **share** one temp block, so a big dead low region is never one pool per expansion (frame-gaps.md: "re-pass 162").
- Worth nothing: a scalar or trivial POD local of an inlined callee, a 4-byte local anywhere, a dead int-to-float conversion, `sqrtf`, `* 1.0f` (frame-gaps.md: "What is worth nothing").
- Each chained `>>` continuation is 8 bytes of low region (`continuations = reads - statements`) (frame-gaps.md: "batch 81").
- The JGadget pool pads one word per expansion when the `getChildren().push_back()` receiver is an unnamed expression and packs when it is a **named local pointer**; name the searched object, not the list, and never at a site that already wraps `push_back` (frame-gaps.md: "Research batch 211").
- JGadget list temps split into a derived-iterator block and a base-iterator block; the dead-word position is set by inline nesting depth, not by any structural property of `std-list.hpp` (inheritance vs containment, access, operators, the batch-133 conversion are all byte-identical); its `begin`/`end`/`insert`/`push_back` bodies are swept: do not re-sweep them (frame-gaps.md: "Research batch 184").
- An **implicit** derived-from-base conversion on a `return` reserves 16 bytes (pool 20 low / 4 high): write `return iterator(Base::insert(...));`. Retail's list walks are `++it` (frame-gaps.md: "batch 133").
- not: `TSolidStack::push` by value (65 regressions), a local before `pushAfterCurrent`, nerve pushes as a gap size, an argument-area explanation (frame-gaps.md: "Ruled out").
- Seven units' scores rest on fabricated padding, so they are not evidence; do not link MarioGamePad (frame-gaps.md: "Pre-existing padding").
- A TU-local `static inline` returning a **reference** to a member, or an existing reference-returning accessor, binds the member's address (`addi rD, rBase, off`) at zero frame cost; a defaulted constructor argument is one inline level (codegen-tells.md: "Structural pass V").
- `TVec3`'s `operator*` return type is a **consumption** split, not an overload one: a destination copy (`x = a*k`, `TVec3 v = a*k`) wants the reference return, a consumed temporary (argument, `+=`, `+`) the by-value one. not: a by-value `operator=` or implicit copy ctor (map-refuted), direct-init, the parameter form, V2/V3 header-wide (Tongue's sites are three-object V3, everything else two-object; closed, frame-gaps.md: "Research batch 159", "Header round 31").

## Register and scheduling residues

- A callee-saved FPR goes only to a **named scalar local of the function's own body**; members and an inlined callee's locals reload per use (codegen-tells.md: "batch 83").
- So "x reloaded, y/z preserved" means retail named exactly those two components, at zero frame cost (codegen-tells.md: "batch 83").
- **First**-declared local gets f31 (batch 90's "last" was a slip); declare it at the statement that first reads it or it loads early and costs an `fmr` (codegen-tells.md: "batch 90", frame-gaps.md: "Research batch 171").
- A non-void return with no `return` reserves r3 for the whole body (frame-gaps.md: "batch 120").
- Callee-saved ladder, six rungs: inlined-callee parameters > caller locals > caller parameters > `this` > pool/base temps > inner-block locals; an inlined member call costs +8 frame per parameter (frame-gaps.md: "Rotation-rule application 212").
- A zero-frame rotation of two locals: make them one inlined call's `this` and parameter (the parameter outranks `this`); grep the class's headers for an existing method with that body before fabricating a level; which operand of an indexed address is the named local picks the volatile register (frame-gaps.md: "Research batch 210").
- Callee-saved GPRs go out r31 down in reverse introduction order: pool/base temps, then locals, then parameters, `this` last, inner-block locals after `this`; use counts, first-use order and liveness are inert (frame-gaps.md: "Research batch 144", "batch 145").
- How many **named scalar locals** the frame holds decides whether the pool base outranks `this`; grouping four of them into an array closed `TSunMgr::load` at zero frame cost (frame-gaps.md: "Research batch 144").
- Declaration order is a knob **only** among function-scope named locals in callee-saved registers; it is inert on `this`, parameters, pool/constant temps and inner-block locals, and block scope is inert everywhere (frame-gaps.md: "batch 145").
- Callee-saved FPRs go out f31 down: an **inlined callee's** temps first (reverse declaration order inside it), then the function's own named `f32` locals in **forward** declaration order, then parameters in reverse order; an anonymous literal/member/global read never gets one (frame-gaps.md: "Research batch 171").
- A variable negated in two arms with the volatile-FPR pair swapped: name it in **only the earlier arm** (`matan`); the compound-assign lever swaps the int-to-float conversion buffers (frame-gaps.md: "Closure re-passes 206 and 207").
- Volatile FPRs (f0-f13) trade in blocks and ignore source order; the only knob is **naming** two values as locals, which sets which loads first (raw arguments evaluate right to left) and carries its register (frame-gaps.md: "Research batch 171").
- An inlined callee's **parameter modified in place** (`amp *= scale`) loads straight into the result's register; a fresh named local loads to a scratch first, and it costs a statement plus 8 bytes of shared temp block (frame-gaps.md: "FPR re-pass 172").
- Volatile GPRs: a pointer step's second SSA value dies at once and takes the **lowest free** volatile while the long-lived one takes the first register free across its range, so retail's one-register chain needs one value, not two; `p += n` keeps one register but MWCC flattens a following `p += const` into the uses' displacements, and only a basic-block boundary stops it (frame-gaps.md: "Research batch 219").
- Known-open: the `this`-vs-pool-base callee-saved swap and zero-frame rotations; `M3UMtxCalcBlendAux` moves with declaration order but never lands, and `TRKSuppAccessFile` (all 120 orders) plus `emitParticle_`, `checkNextFrameSe`, `partsPerform` (fully inert) are exhausted (frame-gaps.md: "batch 145").

## Float and pool

- `fp_contract` fuses only products of **locals**, except `a += b*c` and a single expression's last product (codegen-tells.md: "Floating point").
- `x / 2.0f` and `x * 0.5f` both give `fmuls x, 0.5`, but division keeps the dividend left, a literal the constant (codegen-tells.md: "batch 101").
- A surviving `* 0.0f`/`* 1.0f` proves an inline boundary or an accessor returning the constant; `TUtil<f32>::one()` is the +4 lever (codegen-tells.md: "Batch 58").
- `TVec3::normalize()` is `setLength(*this, TUtil<f32>::one())`, so `setLength(1.0f)` at a call site is that rung spent: -4 per site, instruction-identical, and per site (frame-gaps.md: "re-pass II 178").
- A rogue include can belong **mid-file** (igaiga's pair lands only with `MapCollisionManager.hpp` just before `perform`); two identical `const T[]` locals in one function are one TU-local inline expanded twice; sibling block-scope locals do not share a slot, one function-scope local does (linking.md: "Data pass III").
- Sibling switch arms declaring the same pointer twice (inner shadowing outer) cost a callee-saved register: retail's `addi; mr` on one register is one variable reassigned; a named pointer local fed by an inlined accessor over a real `bl` and used once is an honest +8 (frame-gaps.md: "JGadget application 213").
- The +4 rung of a member-read binder is +4 bottom **and** +4 top (+8 frame), never +4 alone (frame-gaps.md: "JGadget application 213").
- A `.data` pointer table with matching bytes but every relocation 24 low is a missing rogue include (`MapCollisionEntry.hpp`'s zero/(1,1,1) pair after `InfectiousStrings.hpp`); a POD copied through two stack temps is a named local plus a by-value parameter level (linking.md: "Data pass II").
- `matched_data` counts a section only at 100%: triage data by sub-100 sections in `report.json`, a unit at 27% is often one byte away; a jump table (`.rel <fn>`) is never a data fix (linking.md: "Data pass 197").
- A float literal in `.sdata` instead of `.sdata2` was bound to a `const T&` parameter (`std::min`/`max`), ids paired with one skipped (linking.md: "Data pass 197").
- `.sdata2` is reverse source order, so a late literal belongs near the top of the file and a shared slot fixes its value (codegen-tells.md: "Batch 59").
- A `name$NNNN` function-local static's map section tells its constness: `.rodata` = `static const`, `.data` = `static`; a wrong `const` shifts every later `.rodata` string by the array size (codegen-tells.md: "Header round 34").
- An implicit copy ctor word-copies an **array** member but `lfs/stfs`-copies through a class member with no user copy ctor: a word-copied float pair is `f32[2]`, not `TVec2`; never give `TVec2` a copy ctor (codegen-tells.md: "Header round 34").
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
- not: a defaulted **class** argument on `TVec3<f32>`'s copy constructor, `sub`, or any operator, to buy the `a = b - c` gap's missing 8 bytes above the temp: it lands retail's exact (4, 12) at 55 instructions and +9 functions, but MWCC mangles the parameter into the signature and the map fixes every candidate's arity (`__ct__...FRCQ29JGeometry8TVec3<f>` goes MISSING); the `operator-` half alone is +0/-2 and blocked only by the weak `__ami__` retail emits from two inline-refused sites in `TYoshiTongue::movement` (frame-gaps.md: "Research batch 214").
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
