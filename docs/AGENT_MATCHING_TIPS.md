# MWCC Matching Tips for Agents

This document collects practical knowledge about how the Metrowerks CodeWarrior C++ compiler (MWCC GC/1.2.5) generates PowerPC code. These tips help when iterating on source code to achieve a byte-identical match against the original binary.

Read this document before attempting to match functions. The patterns described here are recurring and will save significant trial-and-error time. Expand the document as needed. Ask for human review whenever the contents of the document disagree with observed reality.

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

## Constant Hoisting and Loop Codegen

The compiler hoists constant loads (`lfs`, `lfd` from SDA/SDA2) before loops. The exact set of constants hoisted depends on:
- Which expressions appear in the loop body
- Which inline functions are called (they may reference additional constants)
- The order of operations within the loop

If the target hoists a constant (e.g., `lfd f28, @5181@sda21`) before a loop but our build does not, it means the compiler sees a different code structure. This is usually a symptom of a deeper structural mismatch in the loop body or inlined functions, not fixable by just moving the load.

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
