# The lever card

The thirty levers that close most near-exact functions, condensed from `RULES.md`.
Read this card; grep `RULES.md` (518 one-line rules) only for a tell this card does not cover, and open a topic file only at the section a rule names.
Run `tools/lever-search.py -u <unit> -f <symbol>` before trying anything by hand: it already tries levers 8-17 mechanically.
Prices are per site unless stated; always measure, since every price below has exceptions.

## Diagnose first

1. Read `decomp-diff.py --clusters`: `markers` (`~` changed, `|` register, `<`/`>` missing/extra) and `frame: target / ours`.
   `changes_all` is not a frame probe; a frame move at equal instructions reports nothing.
2. Fix control flow and call order first, load/store order second, registers and frame last.
3. Classify a frame gap by inter-slot deltas: uniform shift = low region (inline temporaries, pool levers work); a hole inside the named block = a missing or misplaced declaration.
   Validate with a temporary `volatile char trash[N]` declared last in the caller's own body: 100% with no instruction diff means only the frame is wrong. Never commit it.
4. Skip a function whose TODO already records a deep search, and any function whose residue is a known-open class (bottom of this card).

## Control flow and order

5. MWCC lays out `return` blocks and switch arms in source order; the pivot tree is value-sorted. Decode case labels from branch destinations, a jump table's entry order, or `.rel` offsets.
6. A redundant `bcc next; b epilogue` pair is two `if (c) return;` that retail spelled as one `if (a || b) return;`; a trailing statement that belongs one brace level out is the commonest branch tell.
7. Call arguments evaluate right to left, named locals left to right; a store cannot cross a call, so a store after a `bl` dates its declaration after it.

## Accessor and raw forks (frame, pool)

8. Every `const T&`-returning accessor (`getPosition()`, `getRotation()`, `TParamT::get()`) reserves an 8-byte reference temporary per site; the raw member is -8. Too long by 8/0x10 with a param read: use raw `.value`.
9. A raw member read through its existing header accessor is +8 per read (`getSpine()`, `getMActor()`, `getActorType()`); a global through its inline accessor (`SMSGetCamera()`, `SMS_GetMarioPos()`) is +4 low pool; dropping it is the negative rung.
10. `!p` vs `p == nullptr` on an unnamed call result is a 4-byte pool lever either way; `int` vs `s32`/`u32` for a popped value can remove a 4-byte named slot.
11. Accessor prices saturate and are not monotone: try site subsets (2 of 3 sites), and try pairs of +0 levers, which can be +8 together.

## Binders and forks (TU-local `static inline`)

12. Binder `static inline T* f(P* p) { T* x = p->m; return x; }` is +8 per expansion; the direct-return fork `{ return p->m; }` is +0; a binder over the header accessor instead of the raw member is +0x10.
13. A direct-return fork nested inside a binder is the +4 rung; a by-value scalar fork over an accessor (`u16 f(J2DPane* p) { return p->getAlpha(); }`) is +4.
14. A binder returning a member address is +0x10, a member pointer +8; a two-local binder is +0x10.
15. Binder sites are tunable one at a time; price a binder in every function that inlines its host, because a shared helper moves all its callers.
16. Naming the receiver (`T* p = helper(); p->call();`) is a separate lever from the chained call; pick the right-hand side (accessor, fork, binder) per site by measurement.

## Named locals and declarations

17. Named locals fill the frame from the top, first-declared highest; inline temporaries fill up from 0xc in expansion order. C-style top declarations fix named-block order; declare an uninitialised aggregate early and assign later to move it.
18. An inlined callee's class-object locals form their own block above the caller's named locals; the only knob is which body declares the object.
19. Declare a named local after the expression that first produces its value (earlier costs an `fmr`); reuse an existing local instead of naming a second one to drop 8 bytes and an `fmr`.
20. Uninitialised locals retail declared reserve slots with no code (`Mtx` 48, `TVec3` 12): name them only when the binary shows the slot is used; a dead local added only to fill frame is refused.

## Registers

21. Callee-saved GPRs go out r31 downward: pool/base temps, then locals, then parameters, `this` last. Declaration order is a knob only among function-scope named locals.
22. Callee-saved FPRs: an inlined callee's temps first, then the function's own named `f32` locals in forward declaration order. Only a named scalar of the function's own body gets one; members reload per use.
23. Volatile FPRs trade in blocks; the knob is naming values (`f32 a = v; a *= k;` puts the value first, `v * k` the constant first; `x /= k` avoids the `fmr`).
24. `f(getPosition())` materialises `&mPosition` into a saved register before the receiver call, raw `mPosition` computes it after; a spine or params accessor spelled differently at one site renumbers volatiles at zero frame.

## Inlining

25. The budget is statement cost at the reached depth: 14 / 9 / 6 / 2 / never at depths 1-5; `inline` buys only depth 1. A named local counts one statement at zero codegen.
26. A callee `bl`ed at some sites and expanded at others is a missing helper level above those sites: wrap the calling site in a one-line `static inline`.
27. An UNUSED helper whose map size matches a big `<` cluster is a missing call site retail inlined; write the body as that helper and call it.
28. Replace `#pragma dont_inline` by counting missing statements with `(void)0;` fillers, then spelling them as real code (early-return guards, named call results, `else if`).

## Float and pool

29. `fp_contract` fuses only products of locals, plus `a += b*c` and an expression's last product; a TU-local inline returning the product unfuses it. `.sdata2` is reverse source order.
30. `x / 2.0f` keeps the dividend first, `x * 0.5f` the constant first; `setLength(1.0f)` vs `normalize()` is -4 per site.

## Known-open classes: stop, leave a TODO

- The `a = b - c` allocation order (retail 4/12 geometry); a header migration, not a per-site lever.
- The JGadget iterator block stride (8 retail, 12 ours): per-site receiver work only.
- The factory constructors retail calls (`TTelesaSlot`, `TFence`, `TSunGlass`); the `JUTColor` temp stride.
- Zero-frame `this`-vs-pool-base swaps with the named-scalar knob exhausted; per-expansion `theNerve()` splits.
- A dead low region with no legal carrier (no UNUSED callee, no empty-ctor class local).
- Frame deficits of 0x20 or more with every instruction right usually need a missing inline level or helper, not a lever: park them for a structural task.

## Non-negotiable

No padding, `volatile`, `#pragma dont_inline`, reinterpret casts, or nonsensical temporaries (a `self = p` alias, a two-step cast, a dead local).
Generated names from lever-search (`fabsf2`, `sMSGetMarDirector`, `colorChan6`) are renamed before commit.
Verify every change with `ninja changes_all` (no regressions) and the DOL SHA-1 `a6782903ef79...`.
Never use `git stash` in a worktree: the stash list is shared across all worktrees and a pop can take another agent's entry; copy files to scratch instead.
