# MWCC Matching Tips for Agents

This document collects practical knowledge about how the Metrowerks CodeWarrior C++ compiler (MWCC GC/1.2.5) generates PowerPC code. These tips help when iterating on source code to achieve a byte-identical match against the original binary.

Read this document before attempting to match functions. The patterns described here are recurring and will save significant trial-and-error time. Expand the document as needed. Ask for human review whenever the contents of the document disagree with observed reality.

## What NOT to focus on

There are some things that might seem like low hanging fruit at first sight but really are not. They are:

- function stack frame size
- prologue and epilogue shape
- wrong registers being used

These things are usually the hardest ones to get right and so should be left for the very end of matching an entire file, when nothing else remains to be done.
Instead of these, first and foremost wrong instructions being used and ordering of these instructions should be resolved.

## Function definition order matters

Functions with non-weak symbols must be defined in code in the exact order they appear in the target binary.
MWCC never reorders symbols, so this is a rule that must never be broken.

## Do not use reinterpret casts to access fields

Never use raw byte offsets and reinterpret casts to access fields -- the codegen from that almost always differs from using proper fields.
Instead, introduce the relevant fields and use them, or rather even getters/setters if already available.

## MWCC dislikes reordering

The compiler never reorders memory stores, loads and function calls relative to one another.
They always should be performed in source code in the exact order they appear in target assembly.

## MWCC can eliminate redundant reads, but not writes

All writes to memory that happen in code will be reproduced in assembly, and vice-versa, when reconstructing code from assembly, you MUST do repeated redundant stores into memory -- that's just the way the original code was written.

Redundant reads, on the other hand, can be eliminated by MWCC, but not always, e.g. function inlining might inhibit this.

## MWCC is very reluctant to optimize anything that uses stores ints to memory

If a Vec struct that contains 3 floats is copied via it's compiler-generated copy ctor or assignment operator -- it will be compiled to integer loads/stores. This makes the compiler unable to keep the values used in registers and forces it to spill them to stack, even if the surrounding code clearly allows for them to stay in floating point registers.

Same logic applies to a Color struct that contains four 8-bit ints: if the ints are initialized one by one, then the compiler will not be able to optimize it to simple bit manipulations in integral registers and will keep the struct on the stack.

## MWCC 1.2.5 stack padding bugs

Our version of MWCC has a bug where the backend allocates more stack than necessary.

Most commonly this happens when functions were inlined: inlined function calls often inflate the stack frame size. To correctly match a function with stack frame size issues, UNUSED inlines from the MAP need to be reconstructed based on their size, and sometimes new inlines need to be fabricated based on own's best judgement.

Another instance of it is using a ternary operator sometimes taking up more stack than using ifs.

Next, local variables can expand the stack even if they are always stored in a register and never actually spilled to the stack.

When no obviously correct way to make stack frame size match exists, a trick should be used to correctly match the function's context: a temporary char array of required size to inflate the stack. Such hacks however should be removed or commented out after the function is matching to allow for a possible proper solution in the future.

## Ifs

Ifs are always compiled to very simple code:
- compare (`cmpwi`/`cmplwi`/etc, or arithmetic instruction with a dot)
- conditional branch (`beq`/`bne`/`ble`/etc)
- the true block
- unconditional branch to end of false block (`b`)
- the false block

The compiler **NEVER** swaps the order of the true block and false block.
It is also very reluctant in changing the control flow, so C++ control flow usually corresponds to assembly one to one.

Ternary operator is compiled similarly, but it is the one exception to control flow  being the same. In the following case MWCC might initialize the variable's register with the "otherwise" value (zero) instead of doing so in the false branch, which eliminates the false branch entirely.
```
int b = thing == nullptr ? thing->field : 0;
```

## Sequential integer comparisons in a disjunction

Whe MWCC sees code like `if (a == 8 || a == 9 || a == 10)` it can optimize it to be `if (a - 8 <= 2)` sometimes. When the latter pattern is encountered with enums -- it should be reversed into multiple disjuncted equality comparisons.

## Switches

MWCC can compile switches in one of two ways: jump table or branching.
Jump tables are easily identifiable via `mtctr` and `bctr` instructions being used.
Switches that became branches usually have control flow that doesn't look like an if: multiple conditional branch instructions follow a single comparison instruction. E.g.
```
cmpwi r0, 0x1
beq ...
bge ...
cmpwi r0, 0x0
bge ...
b   ...
... the code block inside the switch ...
```

### Case ordering is strict

MWCC **never reorders cases in a switch statement** during compilation. The order of `case` labels in the source code must match exactly the order they appear in the assembled jump table or branch chain. If a function exhibits a non-matching switch and you've verified the case logic is correct, inspect the target assembly to determine the actual case sequence and reorder the source cases accordingly. Even if semantically equivalent, the compiled output will differ until case order matches.

## Nonsensical control flow

As MWCC inlines functions, sometimes nonsensical control flow will be encountered in the assembly, one that doesn't correspond to any structured control flow constructions like switches, ifs or loops. Such cases are usually explained by **function inlining** rather than gotos. The place where a goto was supposedly used would actually correspond to a return statement, and the place where it points to would be the boundary of the inlined call.

## Keep track of known relevant inlines

Reconstructing correct inline calls is crucial in matching code correctly. When a similar block of code reoccurs -- always consider the possibility that it's an inline, but never disregard the possibility that the original authors simply copy-pasted it. When starting on a new function, explore the inlines already available in the different classes that it uses, as well as in the current translation unit.

UNUSED functions from the MAP must often be reconstructed even when they do not exist as standalone code in the final binary: their inlined bodies still determine caller codegen (register allocation, load offsets, branch layout). Treat their MAP signature and size as constraints, recover a plausible body from repeated callsite patterns, and validate by diffing the caller(s) after each inline-shape change rather than expecting a direct symbol-level match for the UNUSED function itself.

## Reference Locals Affect Register Allocation

Introducing a reference local before accessing struct members can change how the compiler allocates registers:

```cpp
// Direct access — compiler may reload base pointer each time:
unk0[i].mPos.set(...);
unk0[i].mVel.set(0.0f, 0.0f, 0.0f);

// Reference local — compiler keeps base in a register:
Node& node = unk0[i];
node.mPos.set(...);
node.mVel.set(0.0f, 0.0f, 0.0f);
```

Check whether the target reloads the base address or reuses a register. If it reuses, a reference local is likely in the original code.

## Inlined Functions Only Load `this` Once

Whenever a simple implementation loads a pointer into a register multiple times but the original did it once — the original might have used a function that got inlined.

```cpp
// `field` pointer could get loaded twice because
// the compiler assumes function calls can modify any memory
field->nonInlinedFuncCall1();
field->nonInlinedFuncCall2();

// `field` pointer loaded as `this` once
field->inlineFuncThatCallsBoth();

...

class FieldClass {
...
  void inlineFuncThatCallsBoth() {
    nonInlinedFuncCall1();
    nonInlinedFuncCall2();
  }
...
};
```

This is sometimes indistinguishable from a local reference being used.

## Intermediate Variables Heavily Influence Register Allocation

Intermediate variables are not inherently bad. They are a strong codegen lever in MWCC and can change register pressure, load/store timing, and expression scheduling.

Start with the simplest hypothesis, then iterate:
1. First try a fully inlined arithmetic form (no intermediate variables).
2. If that does not match, try decomposed forms with temporaries.
3. If still nonmatching, vary grouping/order (including compound assignments).

Examples (all semantically equivalent, all can compile differently):

```cpp
// Inline form
f32 c0 = (z1 - z0) * (x2 - x1) - (x1 - x0) * (z2 - z1);

// Decomposed form A
f32 dx = x1 - x0;
f32 dz = z1 - z0;
f32 c0 = dz * (x2 - x1) - dx * (z2 - z1);

// Decomposed form B
f32 c0 = (z1 - z0) * (x2 - x1);
c0 -= (x1 - x0) * (z2 - z1);
```

Practical rule: do not treat temporaries as "wrong" or inlining as "always right". Treat decomposition style as an iterative search dimension and test multiple shapes until one matches. When trying different approaches, always try things that look reasonable to have been written by a human first, and only if nothing sensible works start trying crazy and weird approaches.

## Avoid Reference Aliases as a Workaround

When trying to match complex struct access patterns, DO NOT use reference aliases as a general strategy to "improve" codegen. While they can sometimes help with register reuse (see "Reference Locals Affect Register Allocation" above), they frequently harm matching by changing pointer offset calculations and register assignments in ways that deviate from the original.

Only introduce reference aliases when:
1. The original code clearly had them (evidenced by a single load of a base pointer that's reused)
2. Direct member access results in repeated redundant loads that don't appear in the target
3. You are confident from diff analysis that a reference was intended

Otherwise, use direct member access or direct getter calls inline in expressions and let the common subexpression elimination & redundant load optimizations do the rest.

## Reconstructing initializer-heavy functions (init/ctors) from raw asm

When a function body is mostly field assignments (typical for `init()` / constructors), the fastest path is to decode the raw `.s` file directly rather than relying on `m2c` or guessing.

Workflow:

1. Find the function in `build/GMSJ01/asm/<path>.s` by mangled name (`grep -n` for `.fn <mangled>`).
2. Walk the prologue forward and map every `stb`/`sth`/`stw`/`stfs` to a class field using the `/* 0xNN */` offsets in the header. Each store = one source statement.
3. Float and integer constants live at the bottom of the same `.s` file as `.obj "@NNNN"` blocks with `.float`/`.4byte` directives. The assembly references them as `lfs f0, "@NNNN"@sda21(r0)`.
4. String literals (joint names, file paths) are in `.rodata` / `.sdata2` and referenced as `addi r4, r30, 0xNNN` where `r30` is the loaded address of `@1490` (the TU's local rodata base).
5. Cross-reference against the existing partial source — anything in the asm but not in the source is missing.

Compare offsets one-for-one. If the asm writes to offsets `0xFC, 0x100, 0x104` and the source only zeroes `0x108..0x110`, you're missing a `unkFC.zero();` before `unk108.zero();`.

## Recognising common MWCC store idioms

**`u32 → f32` cast** (typical for `(f32)someByte` from a packed struct like `GXColor`):
```
lbz   r0, OFF(rBase)              ; load unsigned byte
lis   r4, 0x4330                  ; high half of magic constant
lfd   f1, "@magic"@sda21(r0)      ; magic = 4503599627370496.0 (0x4330000000000000)
stw   r0, X(r1)                   ; store low half on stack
stw   r4, X-4(r1)                 ; store high half on stack
lfd   f0, X-4(r1)                 ; load as double
fsubs f0, f0, f1                  ; subtract magic → real float value
stfs  f0, FIELD(rThis)
```
Source: `unk84.x = (f32)bodyColor[0].r;` — straight cast of a `u8` field to `f32`.

**`(u8)` cast on an `int`-returning call before storing to a `u16` field**:
```
bl    JUTNameTab::getIndex(...)
clrlwi r0, r3, 24                 ; mask to low 8 bits
sth    r0, FIELD(rThis)           ; store as halfword
```
Source: `mField = (u8)getIndex(...);` even though `mField` is declared `u16`. Without the `(u8)` cast, MWCC emits `sth r3, FIELD(rThis)` directly with no `clrlwi`. This pattern appears repeatedly for joint-index assignments.

## Constant Hoisting and Loop Codegen

The compiler hoists constant loads (`lfs`, `lfd` from SDA/SDA2) before loops. The exact set of constants hoisted depends on:
- Which expressions appear in the loop body
- Which inline functions are called (they may reference additional constants)
- The order of operations within the loop

If the target hoists a constant (e.g., `lfd f28, @5181@sda21`) before a loop but our build does not, it means the compiler sees a different code structure. This is usually a symptom of a deeper structural mismatch in the loop body or inlined functions, not fixable by just moving the load.

## Function-local statics with non-trivial initializers: `init$NNN` is the guard

When a function-local `static` has a non-trivial initializer that has to run at first call — typically a constructor call like `static JGeometry::TVec3<f32> pos(1815.0f, 1500.0f, 1550.0f);` — MWCC emits **two symbols in `.sbss`**:

- The variable itself (e.g. `pos$NNN`), sized for the type (a Vec is 12 bytes).
- A 1-byte construction guard named `init$NNN` (note the literal name `init`, regardless of what your source variable is called).

The function-entry codegen is:

```asm
lbz   r0, init$NNN@sda21       ; load the guard byte
extsb. r0, r0                  ; sign-extend with dot (sets CR)
bne   .L_skip_init             ; already constructed → skip
... initializer code, writes to pos$NNN ...
li    r0, 1
stb   r0, init$NNN@sda21       ; mark constructed
.L_skip_init:
```

So when the target shows an `init$NNN` byte that is loaded with `lbz` + `extsb.` and tested, **don't** model it as a plain `static bool foo;` in your source — model it as the *guard* for some other static local with a real constructor. Pick the source statement that produces the matching initializer body (commonly a `JGeometry::TVec3<f32>(x, y, z)` if the init code writes three floats).

Aggregate-initialized PODs (`static Vec pos = { 1.0f, 2.0f, 3.0f };`) and zero-initialized statics (`static Vec pos;`) do NOT produce a guard — they sit in `.data` / `.bss` with no first-call check. The `init$NNN` pattern only appears when MWCC needs to run constructor code at first entry.

## Default arguments vs. spelling them out changes inlining

When a base/member constructor is invoked with a value that happens to be that
constructor's own default argument, prefer the default-arg call form over
repeating the literal. It is not just cosmetic: it can change MWCC's inline
depth decisions.

Concrete case (`BathWaterManager.cpp`): `JDrama::TViewObj`'s ctor defaults its
name to `"<TViewObj>"`. Writing a member's base as `JDrama::TViewObj("<TViewObj>")`
inlined the nested `TNameRef`/`TFlagT` ctors one level too deep; writing it as
`JDrama::TViewObj()` (letting the default supply the identical string) kept those
nested ctors as real `bl` calls, exactly like the target. This took the
enclosing constructor from 90.6% to 100%. If a nested ctor inlines when the
target keeps it as a call (or vice-versa), check whether the original used a
default argument.

## Unsized arrays go to `.data`, sized ones may go to `.sdata`

An array declared with an explicit size that is small enough (≤ the `-sdata`
threshold, default 8 bytes) can be placed in `.sdata` and addressed GP-relative
(`@sda21`, giving an indexed `lwzx` load). An array declared **unsized** — e.g.
`static const char* fileNames[]` with the size deduced from the initializer —
is always placed in `.data` instead, and addressed absolutely (`lis/addi @ha/@l`
+ `add`/`lwz`).

So if a small global array should live in `.data` (target uses `lis/addi`) but
yours lands in `.sdata` (`@sda21`), drop the explicit array bound and let the
initializer size it. Concrete case (`BathWaterManager.cpp`): `fileNames[2]` →
`.sdata` (nonmatching); `fileNames[]` → `.data` (match).

## Local Symbol Mangling: `@unnamed@` vs `static`

MWCC mangles symbols inside anonymous namespaces with an `@unnamed@` prefix.

- If a local symbol's mangled name includes `@unnamed@`, model it as being in an anonymous namespace.
- If the symbol is local but does not include `@unnamed@`, prefer a plain `static` function/variable instead.

## Symbol Order with `-inline deferred`

When a TU is compiled with `-inline deferred` (see TU-specific flags in `configure.py`), define symbols in reverse order relative to the map/symbol listing for that TU.

- In practice, function-definition order in the `.cpp` should be reversed for those TUs.
- If order-sensitive matching drifts for an `-inline deferred` TU, verify definition order before attempting smaller codegen tweaks.

## TVec3 / Vector Codegen Patterns

`JGeometry::TVec3<f32>` is a 12-byte struct with `x`, `y`, `z` float members. How you read/write it drastically affects code generation.

### Construction: component-by-component vs constructor

```cpp
// Constructor form — compiler batches all loads, then all stores:
//   lfs f0, ...; lfs f1, ...; lfs f2, ...
//   stfs f0, 0(rN); stfs f1, 4(rN); stfs f2, 8(rN)
JGeometry::TVec3<f32> pos(x, y, z);

// Component-by-component — compiler interleaves load/store pairs:
//   lfs f0, ...; stfs f0, 0(rN)
//   lfs f0, ...; stfs f0, 4(rN)
//   lfs f0, ...; stfs f0, 8(rN)
JGeometry::TVec3<f32> pos;
pos.x = x;
pos.y = y;
pos.z = z;
```

Check the target assembly to see which pattern (batched vs interleaved) is used, and write the source accordingly.

### `Vec3::zero()` vs `Vec3::set(0,0,0)`

`.zero()` emits stores in **reverse component order** (`z`, `y`, `x` → offsets `+8, +4, +0`). `.set(0,0,0)` typically emits forward order. Match the asm order to pick the right call.

### Assignment: `operator=` vs `.set()`

```cpp
// operator= (struct copy) — generates lwz/stw (word load/store):
//   lwz r0, 0(rSrc); stw r0, 0(rDst)
//   lwz r0, 4(rSrc); stw r0, 4(rDst)
//   lwz r0, 8(rSrc); stw r0, 8(rDst)
node.mPos = param_1;

// .set(vec) (float copy) — generates lfs/stfs (float load/store):
//   lfs f0, 0(rSrc); stfs f0, 0(rDst)
//   lfs f0, 4(rSrc); stfs f0, 4(rDst)
//   lfs f0, 8(rSrc); stfs f0, 8(rDst)
node.mPos.set(param_1);

// .set(x, y, z) (3-arg form) — generates lfs/stfs like component assignment
node.mPos.set(expr_x, expr_y, expr_z);
```

The target assembly will clearly show `lwz`/`stw` (integer move) vs `lfs`/`stfs` (float move). Choose the source pattern that matches.

### Constructor with a zero component + `+=` vs direct component init

When building a vector where some components are `0.0f + <expr>` and one is a
real value, the target may show *literal* `0.0` adds on the zero components:
```
lfs f4, @zero ; fadds f4, f4, f0   ; component = 0.0 + src.x
```
This is the signature of the original writing a base vector with literal-zero
components and then adding another vector, e.g.
```cpp
// point2 = (0, -unkC.y, 0) then add unk58 -> emits the "0.0 + unk58.x" adds
JGeometry::TVec3<f32> point2(0.0f, -1.0f * unkC.y, 0.0f);
point2 += data.unk58;
```
Writing the fully-fused form `TVec3(unk58.x, -unkC.y + unk58.y, unk58.z)`
instead reads the components directly with no zero-adds and does not match.
Concrete case (`BathWaterManager.cpp` `calcBathtub` inner-band branch): the
`+=` form recovered the `0.0 + x`/`0.0 + z` adds; see next tip for the `-1.0f *`.

### Negation as `-1.0f * x` (fmuls) vs `-x` (fneg)

`-x` compiles to `fneg`. But the target sometimes shows a standalone
`fmuls f_, f(-1.0), f(x)` (multiply by the `-1.0` SDA constant) where you'd
expect a negation — and crucially kept as a *separate* fmuls, not fused into a
following `fmadds`/`fnmsubs`. This happens when the original literally wrote
`-1.0f * x` (MWCC in does not algebraically fold
`-1.0f * x` -> `fneg x`). Prefer `-1.0f * x` over `-x` when the target shows the
extra `-1.0` load + `fmuls`. Concrete case (`BathWaterManager.cpp`
`calcBathtub`): `-1.0f * unkC.y` matched; `-unkC.y` gave `fneg` and did not.

## Reading a source array through `void*` defeats CSE (controls loop unroll)

When a loop copies the same source element to several destinations, e.g.
```cpp
TVec3<s16>* src = (TVec3<s16>*)getVtxPosArray();
for (...) { a[i] = src[i]; b[i] = src[i]; c[i] = src[i]; }
```
MWCC's alias analysis can prove `src` (a clean typed pointer) doesn't alias the
`new`-allocated destinations and **CSEs the `src[i]` load** — reading it once and
reusing it for all three stores. That shrinks the loop body, and MWCC's `-O4`
unroller then unrolls it *more* (e.g. ×8). The target often reloads `src[i]` for
every store (no CSE) and unrolls only ×2.

To reproduce the reloads (and the smaller unroll factor), read the source
through a `void*` and cast at each use, which blocks the alias analysis:
```cpp
void* src = getVtxPosArray();
for (...) {
    a[i] = ((TVec3<s16>*)src)[i];
    b[i] = ((TVec3<s16>*)src)[i];
    c[i] = ((TVec3<s16>*)src)[i];
}
```
Concrete case (`DrawUtil.cpp` `TTrembleModelEffect::init`): the typed-`src` form
CSE'd + unrolled ×8 (54%); the `void*`-cast form reloaded + unrolled ×2 (100%).
